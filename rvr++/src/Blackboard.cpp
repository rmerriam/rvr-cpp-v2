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
//     Created: Nov 10, 2019
//
//======================================================================================================================
#include <limits>
#include "Trace.h"

#include "Blackboard.h"

#include "CommandBase.h"
#include "ApiShell.h"
#include "Power.h"
#include "SensorsDirect.h"
#include "SensorsStream.h"
#include <string_view>

namespace rvr {

    using bb = Blackboard;
    //----------------------------------------------------------------------------------------------------------------------
    Blackboard::Blackboard() {
    }
    //----------------------------------------------------------------------------------------------------------------------
    Blackboard::key_s::key_s(CommandBase::TargetPort const proc, Devices const dev, uint8_t const cmd, uint8_t const id) :
        id(id), cmd(cmd), dev(dev), proc(proc) {
    }
    //----------------------------------------------------------------------------------------------------------------------
    Blackboard::key_s::key_s(key_t const k) :
        id { static_cast<uint8_t>(k & 0xFF) }, //
        cmd { static_cast<uint8_t>((k >> 8) & 0xFF) }, //
        dev { static_cast<uint8_t>((k >> 16) & 0xFF) }, //
        proc { static_cast<uint8_t>((k >> 24) & 0xFF) } {
    }

    //----------------------------------------------------------------------------------------------------------------------
    std::string Blackboard::entryName(key_t const key) {
        return mDictionary[key].name;
    }
    //----------------------------------------------------------------------------------------------------------------------
    RvrMsg& Blackboard::entryValue(CommandBase::TargetPort const proc, Devices const dev, uint8_t const cmd, uint8_t const id) {
        return entryValue(entryKey(proc, dev, cmd, id));
    }
    //----------------------------------------------------------------------------------------------------------------------
    RvrMsg& Blackboard::entryValue(key_t const key) {
        return mDictionary[key].value;
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline Blackboard::key_s::operator key_t() {
        return key_t(proc << 24 | (dev << 16) | (cmd << 8) | id);
    }
    //======================================================================================================================
    int32_t Blackboard::int_convert(RvrMsg::iterator begin, RvrMsg::iterator end) {
        int64_t value { };
        for (auto it { begin }; it != end; ++it) {
            uint8_t const v { *it };
            value <<= 8;
            value |= v;
        }
        return value;
    }
    //----------------------------------------------------------------------------------------------------------------------
    void bb::rawData(bb::key_t const key, RvrMsg::iterator begin, RvrMsg::iterator end) {
        std::cerr << std::hex;
        Blackboard::entryValue(key) = RvrMsg(begin, end);
        terr << code_loc << std::hex << key << mys::sp;
        std::copy(begin, end, std::ostream_iterator<int>(std::cerr, " "));
    }
    //----------------------------------------------------------------------------------------------------------------------
    template <typename T>
    float normalize(T const value, float const out_min, float const out_max) {
        T min { 0 }; //{ std::numeric_limits<T>::min() };
        T max { std::numeric_limits<T>::max() };
//        terr << float(value) << mys::tab << (float(value) - min) //
//            << mys::tab << (max - min) //
//            << mys::tab << (float(value) - min) / (max - min) //
//            << mys::tab << ((float(value) - min) / (max - min)) + out_min; //
        return (    //
        ((float(value) - min) / (max - min)) //
        * (out_max - out_min)) + out_min;
    }
    //----------------------------------------------------------------------------------------------------------------------
    // Notifications don't have sequence numbers so 'data' starts at what is usually the sequence
    void bb::notification_data(bb::key_t const key, RvrMsg::iterator begin, RvrMsg::iterator end) {
        terr << code_loc << "notification: " << std::hex << key << mys::sp;
        std::copy(begin, end, std::ostream_iterator<int>(std::cerr, " "));
        terr << code_loc << bb::int_convert(begin, begin + 4); // << mys::tab << bb::int_convert(begin + 1 + 4, end);

        uint32_t value { bb::int_convert(begin, begin + 4) };

        float res = normalize(value, -16000.0f, 16000.f);

        terr << code_loc << value << mys::tab << res;
    }
    //----------------------------------------------------------------------------------------------------------------------
//    void status_notification(bb::key_t const key, MsgArray::iterator seq, MsgArray::iterator end) {
//        Blackboard::entryValue(key) = *seq;
//    }
////----------------------------------------------------------------------------------------------------------------------
//    void motor_fault_data(bb::key_t const key, MsgArray::iterator begin, MsgArray::iterator end) {
//// ******** NOT SURE THIS IS CORRECT **** RETURN FROM ENABLE DOESN"T HAVE SEQUENCE #
//        terr << code_loc << std::boolalpha << (begin[ -1] == 0);
//    }
////----------------------------------------------------------------------------------------------------------------------
//    void motor_stall_data(bb::key_t const key, MsgArray::iterator begin, MsgArray::iterator end) {
//// ******** NOT SURE THIS IS CORRECT **** RETURN FROM ENABLE DOESN"T HAVE SEQUENCE #
//        char_ptr motor[] { "left", "right" };
//        terr << code_loc << motor[begin[0]] << std::boolalpha << (begin[1] == 0);
//    }
////----------------------------------------------------------------------------------------------------------------------
//    void mac_addr(bb::key_t const key, MsgArray::iterator begin, MsgArray::iterator end) {
//        for (auto it = begin; it < end - 2; ++it) {
//            terr << code_loc << *it++ << *it << ":";
//        }
//        terr << code_loc << *(end - 2) << *(end - 1);
//    }
//    //----------------------------------------------------------------------------------------------------------------------
//    void msg_array(const bb::key_t key, MsgArray::iterator begin, MsgArray::iterator end) {
//        Blackboard::entryValue(key) = *(begin - 1);
//        terr << code_loc << "status: " << (bool) *(begin - 1);
//    }
    //----------------------------------------------------------------------------------------------------------------------
    void Blackboard::msgArray(bb::key_t const key, RvrMsg::iterator begin, RvrMsg::iterator end) {
        RvrMsg msg { begin, end };
        if (msg.empty()) {
            msg.push_back(0xFF);
        }
        entryValue(key) = msg;
        terr << code_loc << std::hex << key << mys::sp << msg;
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Blackboard::msgArrayWithId(bb::key_t const key, RvrMsg::iterator begin, RvrMsg::iterator end) {
        msgArray(key, begin + 1, end);
    }
    //----------------------------------------------------------------------------------------------------------------------
    using dev = Devices;
    constexpr CommandBase::TargetPort nordic = CommandBase::nordic;
    constexpr CommandBase::TargetPort btc = CommandBase::bluetoothSOC;

    bb::BBDictionary bb::mDictionary { //
//
    { entryKey(nordic, dev::api_and_shell, 0x00), BlackboardEntry { "echo" } }, //
//
        { entryKey(btc, dev::connection, 0x05), BlackboardEntry { "get_bluetooth_advertising_name" } }, //
#if 1
//
        { entryKey(nordic, dev::drive, 0x01), BlackboardEntry { "raw_motors" } }, //
        { entryKey(nordic, dev::drive, 0x06), BlackboardEntry { "reset_yaw" } }, //
        { entryKey(nordic, dev::drive, 0x07), BlackboardEntry { "drive_with_heading" } }, //
        { entryKey(nordic, dev::drive, 0x25), BlackboardEntry { "enable_motor_stall_notify" } }, //
        { entryKey(nordic, dev::drive, 0x26), BlackboardEntry { "motor_stall_notify" } }, //
        { entryKey(nordic, dev::drive, 0x27), BlackboardEntry { "enable_motor_fault_notify" } }, //
        { entryKey(nordic, dev::drive, 0x28), BlackboardEntry { "motor_fault_notify" } }, //
        { entryKey(nordic, dev::drive, 0x29), BlackboardEntry { "get_motor_fault_state" } },
//
        { entryKey(btc, dev::io_led, 0x1A), BlackboardEntry { "set_all_leds" } }, //
        { entryKey(btc, dev::io_led, 0x4E), BlackboardEntry { "release_led_requests" } }, //
//
        { entryKey(btc, dev::power, 0x00), BlackboardEntry { "power off" } }, //
        { entryKey(btc, dev::power, 0x01), BlackboardEntry { "snooze" } }, //
        { entryKey(btc, dev::power, 0x0D), BlackboardEntry { "wake" } }, //
        { entryKey(btc, dev::power, 0x10), BlackboardEntry { "get_battery_percentage" } }, //
        { entryKey(btc, dev::power, 0x11), BlackboardEntry { "system_awake_notification" } }, //
        { entryKey(btc, dev::power, 0x17), BlackboardEntry { "get_battery_voltage_state" } }, //
        { entryKey(btc, dev::power, 0x19), BlackboardEntry { "will_sleep_notify" } }, //
        { entryKey(btc, dev::power, 0x1A), BlackboardEntry { "did_sleep_notify" } }, //
        { entryKey(btc, dev::power, 0x1B), BlackboardEntry { "enable_battery_voltage_state_change_notify" } }, //
        { entryKey(btc, dev::power, 0x1C), BlackboardEntry { "battery_voltage_state_change_notify" } }, //
        { entryKey(btc, dev::power, 0x25, Power::CalibratedFiltered), BlackboardEntry { "get_battery_voltage_in_volts" } }, //
        { entryKey(btc, dev::power, 0x25, Power::CalibratedUnfiltered), BlackboardEntry { "get_battery_voltage_in_volts" } }, //
        { entryKey(btc, dev::power, 0x25, Power::UncalibratedUnfiltered), BlackboardEntry { "get_battery_voltage_in_volts" } }, //
        { entryKey(btc, dev::power, 0x26), BlackboardEntry { "get_battery_voltage_state_thresholds" } }, //
        { entryKey(btc, dev::power, 0x26, 1), BlackboardEntry { "get_battery_voltage_state_thresholds" } }, //
        { entryKey(btc, dev::power, 0x26, 2), BlackboardEntry { "get_battery_voltage_state_thresholds" } }, //
        { entryKey(nordic, dev::power, 0x27), BlackboardEntry { "get_current_sense_amplifier_current" } }, //
        { entryKey(nordic, dev::power, 0x27, 1), BlackboardEntry { "get_current_sense_amplifier_current" } }, //
//
        { entryKey(nordic, dev::sensors, 0x0F), BlackboardEntry { "enable_gyro_max_notify" } }, //
        { entryKey(nordic, dev::sensors, 0x10), BlackboardEntry { "gyro_max_notify" } }, //
        { entryKey(nordic, dev::sensors, 0x13), BlackboardEntry { "reset_locator_x_and_y" } }, //
        { entryKey(nordic, dev::sensors, 0x17), BlackboardEntry { "set_locator_flags " } }, //
        { entryKey(nordic, dev::sensors, 0x22), BlackboardEntry { "get_bot_to_bot_infrared_readings " } }, //
        { entryKey(nordic, dev::sensors, 0x23), BlackboardEntry { "get_rgbc_senso" } }, //
        { entryKey(nordic, dev::sensors, 0x27), BlackboardEntry { "start_robot_to_robot_infrared_broadcasting" } }, //
        { entryKey(nordic, dev::sensors, 0x28), BlackboardEntry { "start_robot_to_robot_infrared_following" } }, //
        { entryKey(nordic, dev::sensors, 0x29), BlackboardEntry { "stop_robot_to_robot_infrared_broadcasting" } }, //
        { entryKey(nordic, dev::sensors, 0x2C), BlackboardEntry { "robot_to_robot_infrared_message_received_notify" } }, //
        { entryKey(btc, dev::sensors, 0x30), BlackboardEntry { "get_ambient_light_sensor_value" } }, //
        { entryKey(nordic, dev::sensors, 0x32), BlackboardEntry { "stop_robot_to_robot_infrared_following" } }, //
        { entryKey(nordic, dev::sensors, 0x33), BlackboardEntry { "start_robot_to_robot_infrared_evading" } }, //
        { entryKey(nordic, dev::sensors, 0x34), BlackboardEntry { "stop_robot_to_robot_infrared_evading" } }, //

        { entryKey(btc, dev::sensors, 0x35), BlackboardEntry { "enable_color_detection_notify" } }, //
        { entryKey(btc, dev::sensors, 0x36), BlackboardEntry { "color_detection_notify" } }, //
        { entryKey(btc, dev::sensors, 0x37), BlackboardEntry { "get_current_detected_color_reading" } }, //
        { entryKey(btc, dev::sensors, 0x38), BlackboardEntry { "enable_color_detection" } }, //

        { entryKey(nordic, dev::sensors, 0x39), BlackboardEntry { "configure_streaming_service" } }, //
        { entryKey(btc, dev::sensors, 0x39), BlackboardEntry { "configure_streaming_service" } }, //
        { entryKey(nordic, dev::sensors, 0x3A), BlackboardEntry { "start_streaming_service" } }, //
        { entryKey(btc, dev::sensors, 0x3A), BlackboardEntry { "start_streaming_service" } }, //
        { entryKey(nordic, dev::sensors, 0x3B), BlackboardEntry { "stop_streaming_service" } }, //
        { entryKey(btc, dev::sensors, 0x3B), BlackboardEntry { "stop_streaming_service" } }, //
        { entryKey(nordic, dev::sensors, 0x3C), BlackboardEntry { "clear_streaming_service" } }, //
        { entryKey(btc, dev::sensors, 0x3C), BlackboardEntry { "clear_streaming_service" } }, //
        { entryKey(nordic, dev::sensors, 0x3D), BlackboardEntry { "streaming_service_data_notify" } }, //
        { entryKey(btc, dev::sensors, 0x3D), BlackboardEntry { "streaming_service_data_notify" } }, //
        { entryKey(nordic, dev::sensors, 0x3E), BlackboardEntry { "enable_robot_infrared_message_notify" } }, //
        { entryKey(nordic, dev::sensors, 0x3F), BlackboardEntry { "send_infrared_message" } }, //
        //
//    { entryKey(nordic, dev::sensors, 0x4A, 0), BlackboardEntry { "left_motor_temperature", SensorsDirect::motorTemperature } }, // left
        { entryKey(nordic, dev::sensors, 0x4A, 4), BlackboardEntry { "left_motor_temperature" } }, // left
        { entryKey(nordic, dev::sensors, 0x4A, 5), BlackboardEntry { "right_motor_temperature" } }, // right
        { entryKey(nordic, dev::sensors, 0x4B), BlackboardEntry { "get_motor_thermal_protection_status" } }, //
        { entryKey(nordic, dev::sensors, 0x4C), BlackboardEntry { "enable_motor_thermal_protection_status_notify" } }, //
        { entryKey(nordic, dev::sensors, 0x4D), BlackboardEntry { "motor_thermal_protection_status_notify" } }, //
//
        { entryKey(nordic, dev::system, 0x00), BlackboardEntry { "nordic_main_application_version" } }, //
        { entryKey(btc, dev::system, 0x00), BlackboardEntry { "bt_main_application_version" } }, //
        { entryKey(nordic, dev::system, 0x01), BlackboardEntry { "nordic_bootloader_version" } }, //
        { entryKey(btc, dev::system, 0x01), BlackboardEntry { "bt_bootloader_version" } }, //
        { entryKey(btc, dev::system, 0x03), BlackboardEntry { "board_revision" } }, //
        { entryKey(btc, dev::system, 0x06), BlackboardEntry { "mac_address" } }, //
        { entryKey(btc, dev::system, 0x13), BlackboardEntry { "stats_id" } }, //
        { entryKey(nordic, dev::system, 0x1F), BlackboardEntry { "nordic_processor_name" } }, //
        { entryKey(btc, dev::system, 0x1F), BlackboardEntry { "bt_processor_name" } }, //
        { entryKey(btc, dev::system, 0x38), BlackboardEntry { "get_sku" } }, //
        { entryKey(nordic, dev::system, 0x39), BlackboardEntry { "get_core_up_time_in_milliseconds" } }, //
#endif
    };
    //----------------------------------------------------------------------------------------------------------------------
    RvrMsg empty;
    constexpr uint8_t two_byte_size { sizeof(uint16_t) };
    //----------------------------------------------------------------------------------------------------------------------
    Blackboard::key_t Blackboard::entryKey(CommandBase::TargetPort const proc, Devices const dev, uint8_t const cmd, uint8_t const id) {
        return static_cast<key_t>(key_s(proc, dev, cmd, id));
    }
    //----------------------------------------------------------------------------------------------------------------------
    uint64_t Blackboard::uintConvert(RvrMsg::const_iterator begin, uint8_t const n) {
        uint64_t res { };
        for (auto it { begin }; it != begin + n; ++it) {
            res <<= 8;
            res |= *it;
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    bool Blackboard::boolValue(CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd) {
        RvrMsg& msg { entryValue(target, dev, cmd) };
        bool res { };
        if ( !msg.empty()) {
            res = msg[0] != 0;
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    uint8_t Blackboard::byteValue(CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd) {
        RvrMsg& msg { bb::entryValue(target, dev, cmd) };
        uint8_t res { };
        if ( !msg.empty()) {
            res = msg[2];
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    uint16_t Blackboard::uintValue(CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd) {
        RvrMsg msg { bb::entryValue(target, dev, cmd) };
        uint16_t res { };
        if (msg.size() >= two_byte_size) {
            res = uintConvert(msg.begin(), two_byte_size);
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    int16_t Blackboard::intValue(CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd) {
        return static_cast<int16_t>(uintValue(target, dev, cmd));
    }
    //----------------------------------------------------------------------------------------------------------------------
    uint64_t Blackboard::uint64Value(CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd) {
        RvrMsg msg { bb::entryValue(target, dev, cmd) };
        uint64_t res { };

        if (msg.size() >= sizeof(uint64_t)) {
            res = uintConvert(msg.begin(), sizeof(uint64_t));
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    float Blackboard::floatValue(CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd, uint8_t pos,
        uint8_t const id) {
        RvrMsg& msg { entryValue(target, dev, cmd, id) };
        union {
            uint8_t buf[4];
            float result { };
        };

        if (msg.size() >= static_cast<uint8_t>((pos * 4) + 2)) {

            std::u8string_view sv(msg.c_str(), msg.size());
            std::u8string_view sub_sv(sv.substr((pos * 4) + 2, 4));

//            uint8_t offset { static_cast<uint8_t>(pos * 4) };
            buf[0] = sub_sv[2];
            buf[1] = sub_sv[3];
            buf[2] = sub_sv[1];
            buf[3] = sub_sv[0];
        }
        return result;
    }
//----------------------------------------------------------------------------------------------------------------------
    void Blackboard::resetNotify(CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd) {
        RvrMsg& msg { entryValue(target, dev, cmd) };
        if (msg.empty()) {
            msg.push_back(0);
        }
        msg[0] = 0;
    }
//----------------------------------------------------------------------------------------------------------------------
    std::string Blackboard::stringValue(CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd) {
        RvrMsg msg { entryValue(target, dev, cmd) };
        return std::string { msg.begin(), msg.end() };
    }
//----------------------------------------------------------------------------------------------------------------------
    RvrMsg
    const& Blackboard::msgValue(CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd) {
        return {entryValue(target, dev, cmd)};
    }
//======================================================================================================================
    std::ostream& operator <<(std::ostream& os, Blackboard::key_s const& k) {
        os << k.proc << mys::sp << k.dev << mys::sp << k.cmd << mys::sp << k.id;
        return os;
    }
//----------------------------------------------------------------------------------------------------------------------
    void Blackboard::m_to_v() {
        struct v_map {
            Blackboard::key_t key;
            BlackboardEntry be;
        };
        std::vector<v_map> vec;
        for (auto b : rvr::Blackboard::mDictionary) {
            v_map v { b.first, b.second };
            vec.push_back(v);
        }
        std::sort(vec.begin(), vec.end(), //
        [](v_map const& a, v_map const& b) {
            return a.key < b.key;
        }
        );
        for (auto& i : vec) {
            terr << std::hex << std::uppercase << i.key << mys::tab << std::setw(45) << std::setfill(' ') << std::left << i.be.name << //
                mys::tab << mys::tab << i.be.value;
        }
    }

}
/* namespace rvr */
