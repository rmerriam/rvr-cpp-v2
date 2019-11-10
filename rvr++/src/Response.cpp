//======================================================================================================================
// 2019 Copyright Mystic Lake Software
//
// This is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//======================================================================================================================
//
//     Author: rmerriam
//
//     Created: Oct 25, 2019
//
//======================================================================================================================
#include <chrono>
#include <string>

#include "Trace.h"
#include "Packet.h"
#include "Response.h"

namespace rvr {
    using char_ptr = const char *;

    enum Devices : uint8_t {
        api_and_shell = 0x10,    //
        system = 0x11,   //
        power = 0x13,   //
        drive = 0x16,   //
        sensors = 0x18,   //
        connection = 0x19,   //
        io_led = 0x1A,   //
    };
    //----------------------------------------------------------------------------------------------------------------------
    void raw_data(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        std::cerr << std::hex;
        std::copy(begin, end, std::ostream_iterator<int>(std::cerr, " "));
    }
    //----------------------------------------------------------------------------------------------------------------------
    void string_data(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        std::copy(begin, end, std::ostream_iterator<uint8_t>(std::cerr));
    }
    //----------------------------------------------------------------------------------------------------------------------
    void int_data(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        long long value { };
        for (auto it { begin }; it != end; ++it) {
            const uint8_t& v { *it };

            value <<= 8;
            value += v;
        }
        terr << __func__ << mys::sp << value;
    }
//----------------------------------------------------------------------------------------------------------------------
    float float_convert(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        union {
            uint8_t buf[4];
            float result;
        };

        buf[0] = *(begin + 2);
        buf[1] = *(begin + 3);
        buf[2] = *(begin + 1);
        buf[3] = *(begin + 0);

        return result;
    }
    //----------------------------------------------------------------------------------------------------------------------
    void float_data(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        terr << __func__ << mys::sp << float_convert(begin, end);
    }
//----------------------------------------------------------------------------------------------------------------------
    void tri_float_data(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        terr << __func__ << mys::sp << float_convert(begin, begin + 4);
        terr << __func__ << mys::sp << float_convert(begin + 4, begin + 8);
        terr << __func__ << mys::sp << float_convert(begin + 8, end);
    }
//----------------------------------------------------------------------------------------------------------------------
    void status_data(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        char_ptr status[] { "okay", "fail" };
        terr << __func__ << mys::sp << status[begin[ -1]];
    }
//----------------------------------------------------------------------------------------------------------------------
    void motor_fault_data(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        // ******** NOT SURE THIS IS CORRECT **** RETURN FROM ENABLE DOESN"T HAVE SEQUENCE #
        terr << __func__ << mys::sp << std::boolalpha << (begin[ -1] == 0);
    }
//----------------------------------------------------------------------------------------------------------------------
    void motor_stall_data(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        // ******** NOT SURE THIS IS CORRECT **** RETURN FROM ENABLE DOESN"T HAVE SEQUENCE #
        char_ptr motor[] { "left", "right" };

        terr << __func__ << mys::sp << motor[begin[0]] << std::boolalpha << (begin[1] == 0);
    }
//----------------------------------------------------------------------------------------------------------------------
    void percentage_data(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        double value { float( *begin) / 100.0 };
        terr << __func__ << mys::sp << value;
    }
//----------------------------------------------------------------------------------------------------------------------
    void version_data(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        terr << __func__ << mys::sp << mys::sp << ((begin[0] << 8) | begin[1]) << "." << ((begin[2] << 8) | begin[3]) << "."
             << ((begin[4] << 8) | begin[5]);
    }
//----------------------------------------------------------------------------------------------------------------------
    void mac_addr(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        for (auto it = begin; it < end - 2; ++it) {
            terr << __func__ << mys::sp << *it++ << *it << ":";
        }
        terr << __func__ << mys::sp << *(end - 2) << *(end - 1);
    }
//----------------------------------------------------------------------------------------------------------------------
    void battery_state_data(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        using char_ptr = const char *;
        char_ptr state[4] { "unknown", "ok", "low", "critical" };
        terr << __func__ << mys::sp << state[ *begin];
    }
//----------------------------------------------------------------------------------------------------------------------
    Response::DecoderMap decoder_map { //
    { api_and_shell << 8 | 0x00, Response::RespDecoder { "echo", raw_data } }, //
//
    { connection << 8 | 0x05, Response::RespDecoder { "get_bluetooth_advertising_name", string_data } }, //
//
    { io_led << 8 | 0x1A, Response::RespDecoder { "set_all_leds", status_data } }, //
    { io_led << 8 | 0x4E, Response::RespDecoder { "release_led_requests", status_data } }, //
    //
    { power << 8 | 0x01, Response::RespDecoder { "snooze", raw_data } }, //
    { power << 8 | 0x0D, Response::RespDecoder { "wake", status_data } }, //
    { power << 8 | 0x10, Response::RespDecoder { "get_battery_percentage", percentage_data } }, //
    { power << 8 | 0x11, Response::RespDecoder { "activity_ack", raw_data } }, //
    { power << 8 | 0x17, Response::RespDecoder { "get_battery_voltage_state", battery_state_data } }, //
    { power << 8 | 0x19, Response::RespDecoder { "will_sleep_notify", raw_data } }, //
    { power << 8 | 0x1A, Response::RespDecoder { "did_sleep_notify", raw_data } }, //
    { power << 8 | 0x1B, Response::RespDecoder { "enable_battery_voltage_state_change_notify", raw_data } }, //
    { power << 8 | 0x1C, Response::RespDecoder { "battery_voltage_state_change_notify", raw_data } }, //
    { power << 8 | 0x25, Response::RespDecoder { "get_battery_voltage_in_volts", float_data } }, //
    { power << 8 | 0x26, Response::RespDecoder { "get_battery_voltage_state_thresholds", tri_float_data } }, //
    { power << 8 | 0x27, Response::RespDecoder { "get_current_sense_amplifier_current", float_data } }, //
//
    { system << 8 | 0x00, Response::RespDecoder { "get_main_application_version", version_data } }, //
    { system << 8 | 0x01, Response::RespDecoder { "get_bootloader_version", version_data } }, //
    { system << 8 | 0x03, Response::RespDecoder { "get_board_revision", int_data } }, //
    { system << 8 | 0x06, Response::RespDecoder { "get_mac_address", mac_addr } }, //
    { system << 8 | 0x13, Response::RespDecoder { "get_stats_id", int_data } }, //
    { system << 8 | 0x1F, Response::RespDecoder { "get_processor_name", string_data } }, //
    { system << 8 | 0x38, Response::RespDecoder { "get_sku", string_data } }, //
    { system << 8 | 0x39, Response::RespDecoder { "get_core_up_time_in_milliseconds", int_data } }, //
//
    { drive << 8 | 0x01, Response::RespDecoder { "raw_motors", status_data } }, //
    { drive << 8 | 0x06, Response::RespDecoder { "reset_yaw", status_data } }, //
    { drive << 8 | 0x07, Response::RespDecoder { "drive_with_heading", status_data } }, //
    { drive << 8 | 0x25, Response::RespDecoder { "enable_motor_stall_notify", status_data } }, //
    { drive << 8 | 0x26, Response::RespDecoder { "motor_stall_notify", motor_stall_data } }, //
    { drive << 8 | 0x27, Response::RespDecoder { "enable_motor_fault_notify", status_data } }, //
    { drive << 8 | 0x28, Response::RespDecoder { "motor_fault_notify", motor_fault_data } }, //
    { drive << 8 | 0x29, Response::RespDecoder { "get_motor_fault_state", motor_fault_data } },

    };

    using DeviceDecoder = std::unordered_map <uint8_t, std::string>;
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
    void Response::readx() {
        uint8_t resp[120];
        int cnt = mSerialPort.read(resp, 120);
        mMsg.assign(resp, &resp[cnt]);

        unescape_msg(mMsg);

        terr << __func__ << mys::sp << __func__ << mys::sp << std::hex << mMsg;
//        return cnt;
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Response::checkForData(rvr::MsgArray& in) {
        if (mSerialPort.count() != 0) {
            uint8_t r[in.capacity()];
            int cnt = mSerialPort.read(r, in.capacity());
            in.insert(in.end(), r, &r[cnt]);
//            terr << __func__ << mys::sp << " in: " << std::hex << in;
        }
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Response::processData(rvr::MsgArray& in) {
        constexpr uint8_t EopSop[] { EOP, SOP };
        while (in.size() > 8) {
            auto pos = std::search(in.begin(), in.end(), EopSop, &EopSop[1]);

            if (pos != in.end()) {
                rvr::MsgArray packet { in.begin(), pos + 1 };
                terr << __func__ << mys::sp << "pkt: " << std::hex << packet;
                decode(packet);
                in.erase(in.begin(), pos + 1);
            }
            else {
                break;
            }
        }
    }
    //----------------------------------------------------------------------------------------------------------------------
    bool Response::operator ()() {
        rvr::MsgArray in;
        in.reserve(80);

        while (mEnd.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) {
            read(in);
        }
        terr << __func__ << mys::sp << " exit";
        return true;
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Response::read(rvr::MsgArray& in) {
        processData(in);
        checkForData(in);
    }
//----------------------------------------------------------------------------------------------------------------------
    void Response::decode_flags(const uint8_t f) {
        std::string flags { };

        for (auto mask { 0x01 }; mask != 0; mask <<= 1) {
            switch (mask & f) {
                case RFlags::response:
                    flags += "response | ";
                    break;
                case RFlags::request_response:
                    flags += "request_response | ";
                    break;
                case RFlags::request_error_response:
                    flags += "request_error_response | ";
                    break;
                case RFlags::activity:
                    flags += "activity | ";
                    break;
                case RFlags::has_target:
                    flags += "has_target | ";
                    break;
                case RFlags::has_source:
                    flags += "has_source | ";
                    break;
                case RFlags::has_more_flags:
                    flags += "has_more_flags | ";
                    break;
            }
        }
        if ((f & RFlags::response) == 0) {
            flags += "no_response";
        }
        terr << __func__ << mys::sp << flags;
    }
//----------------------------------------------------------------------------------------------------------------------
    void Response::decode_error(auto err_byte) {
        switch (err_byte) {
            case 1: {
                terr << __func__ << mys::sp << "bad_did";
                break;
            }
            case 2: {
                terr << __func__ << mys::sp << "bad_cid";
                break;
            }
            case 3: {
                terr << __func__ << mys::sp << "not_yes_implemented";
                break;
            }
            case 4: {
                terr << __func__ << mys::sp << "restricted";
                break;
            }
            case 5: {
                terr << __func__ << mys::sp << "bad_data_length";
                break;
            }
            case 6: {
                terr << __func__ << mys::sp << "failed";
                break;
            }
            case 7: {
                terr << __func__ << mys::sp << "bad bad_data_value";
                break;
            }
            case 8: {
                terr << __func__ << mys::sp << "busy";
                break;
            }
            case 9: {
                terr << __func__ << mys::sp << "bad_tid";
                break;
            }
            case 0xA: {
                terr << __func__ << mys::sp << "target_unavailable";
                break;
            }
        }
    }
//----------------------------------------------------------------------------------------------------------------------
    void Response::decode(MsgArray packet) {
        enum BytePositions : uint8_t {
            flags = 0x01,   //
            //            targ = 0x02,   //
            src = 0x02,            //
            dev = 0x03,            //
            cmd = 0x04,            //
            seq = 0x05,            //
            status = 0x06,            //
            data = 0x07,            //
        };

        Packet::unescape_msg(packet);

        const bool is_resp { (packet[flags] & RFlags::response) == RFlags::response };   // versus notification

        // if there is a target byte then increment byte position by 1
        int8_t offset = (packet[flags] & RFlags::has_target) ? 1 : 0;
        decode_flags(packet[flags]);

        std::string device = device_names[packet[dev + offset]];

        const uint16_t key = packet[dev + offset] << 8 | packet[cmd + offset];
        std::string command { decoder_map[key].name };

        if (command.empty()) {
            terr << __func__ << " Command not in decode table " << device << mys::sp << std::hex << key;
        }
        else {
            terr << __func__ << mys::sp << device << mys::sp << command;

            if (is_resp) {
                if (packet[status + offset]) {
                    auto err_byte { packet[status + offset] };
                    terr << __func__ << mys::sp << "ERROR: " << (uint16_t)err_byte;
                    decode_error(err_byte);
                }
                else {
                    FuncPtr decode_func { decoder_map[key].func };
                    decode_func(packet.begin() + data + offset, packet.end() - 2);
                }
            }
            else {  // notification - no sequence number
                terr << __func__ << " notification " << std::hex << (int)packet[seq];
            }
        }
        terr << __func__ << " **************";
    }
}
