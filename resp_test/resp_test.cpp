//============================================================================
// Name        : rvr_serial.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <chrono>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>
#include <thread>
#include <unordered_map>
using namespace std;

#include "trace.h"
#include "Response.h"

#include "ApiShell.h"
#include "Connection.h"
#include "Drive.h"
#include "Power.h"
#include "SystemInfo.h"

using char_ptr = const char *;
const char_ptr port_name { "/dev/rvr" };
namespace rvr {
    //----------------------------------------------------------------------------------------------------------------------
    void decode_flags(const uint8_t f) {
        using RFlags = Request::flags;

        for (auto mask { 0x01 }; mask != 0; mask <<= 1) {
            switch (mask & f) {
                case RFlags::response:
                    mys::trace("response | ");
                    break;
                case RFlags::request_response:
                    mys::trace("request_response | ");
                    break;
                case RFlags::request_error_response:
                    mys::trace("request_error_response | ");
                    break;
                case RFlags::activity:
                    mys::trace("activity | ");
                    break;
                case RFlags::has_target:
                    mys::trace("has_target | ");
                    break;
                case RFlags::has_source:
                    mys::trace("has_source | ");
                    break;
                case RFlags::has_more_flags:
                    mys::trace("has_more_flags | ");
                    break;
            }
        }
        mys::trace() << mys::nl;
    }

    enum Devices : uint8_t {
        api_and_shell = 0x10,    //
        system = 0x11,   //
        power = 0x13,   //
        drive = 0x16,   //
        sensors = 0x18,   //
        connection = 0x19,   //
        io_led = 0x1A,   //
    };

    using DeviceDecoder = unordered_map <uint8_t, string>;
    DeviceDecoder device_names { //
    { 0x10, "api_and_shell" }, //
        { 0x11, "system" }, //
        { 0x13, "power" }, //
        { 0x16, "drive" }, //
        { 0x18, "sensors" }, //
        { 0x19, "connection" }, //
        { 0x1A, "io_led" }, //
    };

    //----------------------------------------------------------------------------------------------------------------------
    void raw_data(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        std::copy(begin, end, std::ostream_iterator<int>(cerr, " "));
    }
    //----------------------------------------------------------------------------------------------------------------------
    void string_data(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        std::copy(begin, end, std::ostream_iterator<uint8_t>(cerr));
    }
    //----------------------------------------------------------------------------------------------------------------------
    void int_data(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        long long value { };
        for (auto it { begin }; it != end; ++it) {
            const uint8_t& v { *it };

            value <<= 8;
            value += v;
        }
        std::cerr << dec;
        mys::trace(value);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void float_data(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        uint16_t value { };

        for (auto it { begin }; it != end; ++it) {
            const uint8_t& v { *it };
            value <<= 8;
            value += v;
            std::cerr << dec;
        }
        mys::trace( *reinterpret_cast<float*>( &value));
    }
    //----------------------------------------------------------------------------------------------------------------------
    void tri_float_data(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        float_data(begin, begin + 3);
        float_data(begin + 4, begin + 7);
        float_data(begin + 8, end);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void status_data(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        char_ptr status[] { "okay", "fail" };
        mys::trace(status[begin[ -1]]);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void motor_fault_data(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        // ******** NOT SURE THIS IS CORRECT **** RETURN FROM ENABLE DOESN"T HAVE SEQUENCE #
        std::cerr << std::boolalpha;
        mys::trace(begin[ -1] == 0);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void motor_stall_data(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        // ******** NOT SURE THIS IS CORRECT **** RETURN FROM ENABLE DOESN"T HAVE SEQUENCE #
        char_ptr motor[] { "left", "right" };

        mys::trace(motor[begin[0]]);
        std::cerr << std::boolalpha;
        mys::trace(begin[1] == 0);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void prrcentage_data(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        double value { float( *begin) / 100.0 };
        std::cerr << dec;
        mys::trace(value);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void version_data(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        std::cerr << dec;
        mys::trace(begin[0] << 8 | begin[1]);
        mys::trace(".");
        mys::trace(begin[2] << 8 | begin[3]);
        mys::trace(".");
        mys::trace(begin[4] << 8 | begin[5]);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void mac_addr(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        std::cerr << dec;
        for (auto it = begin; it < end - 2; ++it) {
            mys::trace( *it++);
            mys::trace( *it);
            mys::trace(":");
        }
        mys::trace( *(end - 2));
        mys::trace( *(end - 1));
    }
    //----------------------------------------------------------------------------------------------------------------------
    void battery_state_data(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        using char_ptr = const char *;
        char_ptr state[4] { "unknown", "ok", "low", "critical" };

        std::cerr << dec;
        mys::trace(state[ *begin]);
    }

    //----------------------------------------------------------------------------------------------------------------------
    using FuncPtr = void (*)(MsgArray::const_iterator , MsgArray::const_iterator );

    struct RespDecoder {
        string name;
        FuncPtr func { raw_data };
    };

    using DecoderMap = unordered_map <uint16_t, RespDecoder>;
    //----------------------------------------------------------------------------------------------------------------------
    DecoderMap decoder_map { //
    { api_and_shell << 8 | 0x00, RespDecoder { "echo", raw_data } }, //
        //
        { connection << 8 | 0x05, RespDecoder { "get_bluetooth_advertising_name", string_data } }, //
        //
        { power << 8 | 0x01, RespDecoder { "snooze" } }, //
        { power << 8 | 0x0D, RespDecoder { "wake", status_data } }, //
        { power << 8 | 0x10, RespDecoder { "get_battery_percentage", prrcentage_data } }, //
        { power << 8 | 0x17, RespDecoder { "get_battery_voltage_state", battery_state_data } }, //
        { power << 8 | 0x19, RespDecoder { "will_sleep_notify" } }, //
        { power << 8 | 0x1A, RespDecoder { "did_sleep_notify" } }, //
        { power << 8 | 0x1B, RespDecoder { "enable_battery_voltage_state_change_notify" } }, //
        { power << 8 | 0x1C, RespDecoder { "battery_voltage_state_change_notify" } }, //
        { power << 8 | 0x25, RespDecoder { "get_battery_voltage_in_volts", int_data } }, //
        { power << 8 | 0x26, RespDecoder { "get_battery_vupdatoltage_state_thresholds" } }, //
        { power << 8 | 0x27, RespDecoder { "get_current_sense_amplifier_current" } }, //
        //
        { system << 8 | 0x00, RespDecoder { "get_main_application_version", version_data } }, //
        { system << 8 | 0x01, RespDecoder { "get_bootloader_version", version_data } }, //
        { system << 8 | 0x03, RespDecoder { "get_board_revision", int_data } }, //
        { system << 8 | 0x06, RespDecoder { "get_mac_address", mac_addr } }, //
        { system << 8 | 0x13, RespDecoder { "get_stats_id", int_data } }, //
        { system << 8 | 0x1F, RespDecoder { "get_processor_name", string_data } }, //
        { system << 8 | 0x38, RespDecoder { "get_sku", string_data } }, //
        { system << 8 | 0x39, RespDecoder { "get_core_up_time_in_milliseconds", int_data } }, //
//
        { drive << 8 | 0x01, RespDecoder { "raw_motors", status_data } }, //
        { drive << 8 | 0x06, RespDecoder { "reset_yaw", status_data } }, //
        { drive << 8 | 0x07, RespDecoder { "drive_with_heading", status_data } }, //
        { drive << 8 | 0x25, RespDecoder { "enable_motor_stall_notify", status_data } }, //
        { drive << 8 | 0x26, RespDecoder { "motor_stall_notify", motor_stall_data } }, //
        { drive << 8 | 0x27, RespDecoder { "enable_motor_fault_notify", status_data } }, //
        { drive << 8 | 0x28, RespDecoder { "motor_fault_notify", motor_fault_data } }, //
        { drive << 8 | 0x29, RespDecoder { "get_motor_fault_state", motor_fault_data } },
    //
    };
    //----------------------------------------------------------------------------------------------------------------------
    void decode_error(auto err_byte) {
        switch (err_byte) {
            case 1: {
                mys::trace("bad_did");
                break;
            }
            case 2: {
                mys::trace("bad_cid");
                break;
            }
            case 3: {
                mys::trace("not_yes_implemented");
                break;
            }
            case 4: {
                mys::trace("restricted");
                break;
            }
            case 5: {
                mys::trace("bad_data_length");
                break;
            }
            case 6: {
                mys::trace("failed");
                break;
            }
            case 7: {
                mys::trace("bad bad_data_value");
                break;
            }
            case 8: {
                mys::trace("busy");
                break;
            }
            case 9: {
                mys::trace("bad_tid");
                break;
            }
            case 0xA: {
                mys::trace("target_unavailable");
                break;
            }
        }
    }
//----------------------------------------------------------------------------------------------------------------------
    void decode(rvr::MsgArray packet) {
        enum BytePositions : uint8_t {
            flags = 0x01,   //
//            targ = 0x02,   //
            src = 0x02,   //
            dev = 0x03,   //
            cmd = 0x04,   //
            seq = 0x05,   //
            status = 0x06,   //
            data = 0x07,   //
        };

        Packet::unescape_msg(packet);
// if there is a target byte then increment byte position by 1
        uint8_t offset = (packet[flags] & RFlags::has_target) ? 1 : 0;
        decode_flags(packet[flags]);

        string device = device_names[packet[dev + offset]];

        const uint16_t key = packet[dev + offset] << 8 | packet[cmd + offset];
        string command { decoder_map[key].name };

        mys::trace(device);
        mys::trace(command) << mys::nl;

        if (packet[status + offset]) {
            auto err_byte { packet[status + offset] };
            mys::trace("ERROR: ");
            decode_error(err_byte);
        }
        else {
            FuncPtr decode_func { decoder_map[key].func };
            decode_func(packet.begin() + data + offset, packet.end() - 2);
        }
        mys::trace() << mys::nl << mys::nl << hex;

    }
//----------------------------------------------------------------------------------------------------------------------
    void Response::readx() {
        rvr::MsgArray in;
        in.reserve(40);

        const uint8_t EopSop[] { 0xD8, 0x8D };

        for (auto i { 0 }; i < 25; ++i) {

            std::cerr << hex;

            if (mSerialPort.count() != 0) {
                uint8_t r[in.capacity()];
                int cnt = mSerialPort.read(r, in.capacity());

                in.insert(in.end(), r, &r[cnt]);

                mys::trace(in) << mys::nl;
            }

            while (in.size() >= 10) {
                auto pos = std::search(in.begin(), in.end(), EopSop, &EopSop[1]);

                if (pos != in.end()) {
                    rvr::MsgArray packet { in.begin(), pos + 1 };
                    mys::trace(packet);
                    decode(packet);
                    in.erase(in.begin(), pos + 1);
                }
                else {
                    break;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    }
}
//----------------------------------------------------------------------------------------------------------------------
int main() {

    cerr << std::hex << setfill('0') << std::uppercase;

    SerialPort serial { port_name, 115200 };
    rvr::Request req { serial };
    rvr::Response resp { serial };

    rvr::ApiShell api(req);
    rvr::Connection cmd(req);
    rvr::Drive drive(req);
    rvr::SystemInfo sys(req);

#if 1

    drive.fixHeading(true);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    drive.stop(90, true);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    drive.drive(25, 25, true);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    drive.spin_drive(0, 20, true);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    drive.fixHeading(true);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    drive.enableMotorStallNotify();
    drive.enableMotorFaultNotify();

    drive.disableMotorStallNotify();
    drive.disableMotorFaultNotify();

    drive.getMotorFault();
    drive.getMotorFault();

#elif 0
    rvr::MsgArray dead { 0xDE, 0xAD };
    api.echo(dead, true);

    cmd.bluetoothName();

    sys.getMainAppVersion();
    sys.getBootloaderVersion();
    sys.getBoardRevision();
    sys.getMacId();
    sys.getStatsId();
    sys.getUpTime();
    sys.getProcessorName();
    sys.getSku();
    sys.getMainAppVersion();
#elif 0
    rvr::Power pow(req);

    pow.awake();
    pow.battery_precentage();
    pow.battery_voltage_state();
    pow.battery_voltage();
    pow.battery_volt_thresholds();
    pow.battery_current();
    pow.awake();

    //    pow.sleep();

#endif
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cerr << "===================\n";
    resp.readx();
}
