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

namespace rvr {
    /*
     * Send to a processor a stream: <token><high id><low id><size>...<high id><low id><size> where the id and size are r
     * repeated for the desired sensors. Sensors are grouped by the processor and token.

     * The responses are all 0x3D streaming service notify. The data is in the order and size specified in the streaming
     * ;request from (1) determined by processor and token.
     */
    Blackboard::Blackboard() {
    }
    using bb = Blackboard;

    //======================================================================================================================
    int32_t Blackboard::int_convert(MsgArray::iterator begin, MsgArray::iterator end) {
        int64_t value { };
        for (auto it { begin }; it != end; ++it) {
            const uint8_t v { *it };
            value <<= 8;
            value |= v;
        }
        return value;
    }
//----------------------------------------------------------------------------------------------------------------------
    float Blackboard::float_convert(MsgArray::iterator begin, MsgArray::iterator end) {
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
    void raw_data(const bb::key_t key, MsgArray::iterator begin, MsgArray::iterator end) {
        std::cerr << std::hex;
        Blackboard::entryValue(key) = MsgArray(begin, end);
        terr << code_loc << std::hex << key << mys::sp;
        std::copy(begin, end, std::ostream_iterator<int>(std::cerr, " "));

    }
    //----------------------------------------------------------------------------------------------------------------------
    void string_data(const bb::key_t key, MsgArray::iterator begin, MsgArray::iterator end) {
        Blackboard::entryValue(key) = std::string(begin, end);
        terr << code_loc << key << mys::sp << std::any_cast<std::string>(Blackboard::entryValue(key));
    }
    //----------------------------------------------------------------------------------------------------------------------
    void float_data(const bb::key_t key, MsgArray::iterator begin, MsgArray::iterator end) {
        Blackboard::entryValue(key) = bb::float_convert(begin, end);
        terr << code_loc << std::hex << key << mys::sp << std::any_cast<float>(Blackboard::entryValue(key));
    }
    //----------------------------------------------------------------------------------------------------------------------
    void int_data(const bb::key_t key, MsgArray::iterator begin, MsgArray::iterator end) {
        Blackboard::entryValue(key) = bb::int_convert(begin, end);
        terr << code_loc << std::hex << key << mys::sp << std::any_cast<int64_t>(Blackboard::entryValue(key));
    }
    //----------------------------------------------------------------------------------------------------------------------
    void status_data(const bb::key_t key, MsgArray::iterator begin, MsgArray::iterator end) {
        Blackboard::entryValue(key) = *(begin - 1);
        terr << code_loc << "status: " << (bool) *(begin - 1);
    }
    //----------------------------------------------------------------------------------------------------------------------
    template <typename T>
    float normalize(const T value, const float out_min, const float out_max) {
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
    void notification_data(const bb::key_t key, MsgArray::iterator begin, MsgArray::iterator end) {
        terr << code_loc << "notification: " << std::hex << key << mys::sp;
        std::copy(begin, end, std::ostream_iterator<int>(std::cerr, " "));
        terr << code_loc << bb::int_convert(begin + 1, begin + 1 + 4); // << mys::tab << bb::int_convert(begin + 1 + 4, end);

        uint32_t value { bb::int_convert(begin + 1, begin + 1 + 4) };

        float res = normalize(value, -16000.0f, 16000.f);

        terr << code_loc << value << mys::tab << res;
    }
    //----------------------------------------------------------------------------------------------------------------------
//    void status_notification(const bb::key_t key, MsgArray::iterator seq, MsgArray::iterator end) {
//        Blackboard::entryValue(key) = *seq;
//    }
//----------------------------------------------------------------------------------------------------------------------
    void motor_fault_data(const bb::key_t key, MsgArray::iterator begin, MsgArray::iterator end) {
// ******** NOT SURE THIS IS CORRECT **** RETURN FROM ENABLE DOESN"T HAVE SEQUENCE #
        terr << code_loc << std::boolalpha << (begin[ -1] == 0);
    }
//----------------------------------------------------------------------------------------------------------------------
    void motor_stall_data(const bb::key_t key, MsgArray::iterator begin, MsgArray::iterator end) {
// ******** NOT SURE THIS IS CORRECT **** RETURN FROM ENABLE DOESN"T HAVE SEQUENCE #
        char_ptr motor[] { "left", "right" };
        terr << code_loc << motor[begin[0]] << std::boolalpha << (begin[1] == 0);
    }
//----------------------------------------------------------------------------------------------------------------------
    void mac_addr(const bb::key_t key, MsgArray::iterator begin, MsgArray::iterator end) {
        for (auto it = begin; it < end - 2; ++it) {
            terr << code_loc << *it++ << *it << ":";
        }
        terr << code_loc << *(end - 2) << *(end - 1);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void msg_array(const bb::key_t key, MsgArray::iterator begin, MsgArray::iterator end) {
//        auto m_key = static_cast<bb::key_t>(key | (( *begin - 4)));

        MsgArray msg { begin, end };
        Blackboard::entryValue(key) = msg;
        terr << code_loc << std::hex << key << mys::sp << std::any_cast<MsgArray>(Blackboard::entryValue(key));
    }
//----------------------------------------------------------------------------------------------------------------------
    using dev = Devices;
    constexpr CommandBase::TargetPort nordic = CommandBase::nordic;
    constexpr CommandBase::TargetPort btc = CommandBase::bluetoothSOC;

    bb::BBDictionary bb::mDictionary { //
//
    { entryKey(nordic, dev::api_and_shell, 0x00), BlackboardEntry { "echo", raw_data } }, //
//
    { entryKey(btc, dev::connection, 0x05), BlackboardEntry { "get_bluetooth_advertising_name", string_data } }, //
#if 1
//
    { entryKey(nordic, dev::drive, 0x01), BlackboardEntry { "raw_motors", status_data } }, //
    { entryKey(nordic, dev::drive, 0x06), BlackboardEntry { "reset_yaw", status_data } }, //
    { entryKey(nordic, dev::drive, 0x07), BlackboardEntry { "drive_with_heading", status_data } }, //
    { entryKey(nordic, dev::drive, 0x25), BlackboardEntry { "enable_motor_stall_notify", status_data } }, //
    { entryKey(nordic, dev::drive, 0x26), BlackboardEntry { "motor_stall_notify", notification_data } }, //
    { entryKey(nordic, dev::drive, 0x27), BlackboardEntry { "enable_motor_fault_notify", status_data } }, //
    { entryKey(nordic, dev::drive, 0x28), BlackboardEntry { "motor_fault_notify", notification_data } }, //
    { entryKey(nordic, dev::drive, 0x29), BlackboardEntry { "get_motor_fault_state", motor_fault_data } },
//
    { entryKey(btc, dev::io_led, 0x1A), BlackboardEntry { "set_all_leds", status_data } }, //
    { entryKey(btc, dev::io_led, 0x4E), BlackboardEntry { "release_led_requests", status_data } }, //
//
    { entryKey(btc, dev::power, 0x01), BlackboardEntry { "snooze", status_data } }, //
    { entryKey(btc, dev::power, 0x0D), BlackboardEntry { "wake", status_data } }, //
    { entryKey(btc, dev::power, 0x10), BlackboardEntry { "get_battery_percentage", int_data } }, //
    { entryKey(btc, dev::power, 0x11), BlackboardEntry { "system_awake_notification", notification_data } }, //
    { entryKey(btc, dev::power, 0x17), BlackboardEntry { "get_battery_voltage_state", int_data } }, //
    { entryKey(btc, dev::power, 0x19), BlackboardEntry { "will_sleep_notify", notification_data } }, //
    { entryKey(btc, dev::power, 0x1A), BlackboardEntry { "did_sleep_notify", notification_data } }, //
    { entryKey(btc, dev::power, 0x1B), BlackboardEntry { "enable_battery_voltage_state_change_notify", int_data } }, //
    { entryKey(btc, dev::power, 0x1C), BlackboardEntry { "battery_voltage_state_change_notify", notification_data } }, //
    { entryKey(btc, dev::power, 0x25, Power::CalibratedFiltered), BlackboardEntry { "get_battery_voltage_in_volts", float_data } }, //
    { entryKey(btc, dev::power, 0x25, Power::CalibratedUnfiltered), BlackboardEntry { "get_battery_voltage_in_volts", float_data } }, //
    { entryKey(btc, dev::power, 0x25, Power::UncalibratedUnfiltered), BlackboardEntry { "get_battery_voltage_in_volts", float_data } }, //
    { entryKey(btc, dev::power, 0x26), BlackboardEntry { "get_battery_voltage_state_thresholds", Power::rxBatteryThresholds } }, //
    { entryKey(btc, dev::power, 0x26, 1), BlackboardEntry { "get_battery_voltage_state_thresholds", Power::rxBatteryThresholds } }, //
    { entryKey(btc, dev::power, 0x26, 2), BlackboardEntry { "get_battery_voltage_state_thresholds", Power::rxBatteryThresholds } }, //
    { entryKey(nordic, dev::power, 0x27), BlackboardEntry { "get_current_sense_amplifier_current", float_data } }, //
    { entryKey(nordic, dev::power, 0x27, 1), BlackboardEntry { "get_current_sense_amplifier_current", float_data } }, //
//
    { entryKey(nordic, dev::sensors, 0x0F), BlackboardEntry { "enable_gyro_max_notify", status_data } }, //
    { entryKey(nordic, dev::sensors, 0x10), BlackboardEntry { "gyro_max_notify", notification_data } }, //
    { entryKey(nordic, dev::sensors, 0x13), BlackboardEntry { "reset_locator_x_and_y", raw_data } }, //
    { entryKey(nordic, dev::sensors, 0x17), BlackboardEntry { "set_locator_flags ", raw_data } }, //
    { entryKey(nordic, dev::sensors, 0x22), BlackboardEntry { "get_bot_to_bot_infrared_readings ", raw_data } }, //
    { entryKey(nordic, dev::sensors, 0x23), BlackboardEntry { "get_rgbc_senso", raw_data } }, //
    { entryKey(nordic, dev::sensors, 0x27), BlackboardEntry { "start_robot_to_robot_infrared_broadcasting", raw_data } }, //
    { entryKey(nordic, dev::sensors, 0x28), BlackboardEntry { "start_robot_to_robot_infrared_following", raw_data } }, //
    { entryKey(nordic, dev::sensors, 0x29), BlackboardEntry { "stop_robot_to_robot_infrared_broadcasting", raw_data } }, //
    { entryKey(nordic, dev::sensors, 0x2C), BlackboardEntry { "robot_to_robot_infrared_message_received_notify", notification_data } }, //
    { entryKey(btc, dev::sensors, 0x30), BlackboardEntry { "get_ambient_light_sensor_value", float_data } }, //
    { entryKey(nordic, dev::sensors, 0x32), BlackboardEntry { "stop_robot_to_robot_infrared_following", raw_data } }, //
    { entryKey(nordic, dev::sensors, 0x33), BlackboardEntry { "start_robot_to_robot_infrared_evading", raw_data } }, //
    { entryKey(nordic, dev::sensors, 0x34), BlackboardEntry { "stop_robot_to_robot_infrared_evading", raw_data } }, //

    { entryKey(btc, dev::sensors, 0x35), BlackboardEntry { "enable_color_detection_notify", status_data } }, //
    { entryKey(btc, dev::sensors, 0x36), BlackboardEntry { "color_detection_notify", SensorsDirect::rxColotNotify } }, //
    { entryKey(btc, dev::sensors, 0x37), BlackboardEntry { "get_current_detected_color_reading", status_data } }, //
    { entryKey(btc, dev::sensors, 0x38), BlackboardEntry { "enable_color_detection", status_data } }, //

    { entryKey(nordic, dev::sensors, 0x39), BlackboardEntry { "configure_streaming_service", status_data } }, //
    { entryKey(btc, dev::sensors, 0x39), BlackboardEntry { "configure_streaming_service", status_data } }, //
    { entryKey(nordic, dev::sensors, 0x3A), BlackboardEntry { "start_streaming_service", status_data } }, //
    { entryKey(btc, dev::sensors, 0x3A), BlackboardEntry { "start_streaming_service", status_data } }, //
    { entryKey(nordic, dev::sensors, 0x3B), BlackboardEntry { "stop_streaming_service", status_data } }, //
    { entryKey(btc, dev::sensors, 0x3B), BlackboardEntry { "stop_streaming_service", status_data } }, //
    { entryKey(nordic, dev::sensors, 0x3C), BlackboardEntry { "clear_streaming_service", status_data } }, //
    { entryKey(btc, dev::sensors, 0x3C), BlackboardEntry { "clear_streaming_service", status_data } }, //
    { entryKey(nordic, dev::sensors, 0x3D), BlackboardEntry { "streaming_service_data_notify", notification_data } }, //
    { entryKey(btc, dev::sensors, 0x3D), BlackboardEntry { "streaming_service_data_notify", notification_data } }, //
    { entryKey(nordic, dev::sensors, 0x3E), BlackboardEntry { "enable_robot_infrared_message_notify", status_data } }, //
    { entryKey(nordic, dev::sensors, 0x3F), BlackboardEntry { "send_infrared_message", raw_data } }, //
    //
//    { entryKey(nordic, dev::sensors, 0x4A, 0), BlackboardEntry { "left_motor_temperature", SensorsDirect::motorTemperature } }, // left
    { entryKey(nordic, dev::sensors, 0x4A, 0), BlackboardEntry { "left_motor_temperature", msg_array } }, // left
    { entryKey(nordic, dev::sensors, 0x4A, 1), BlackboardEntry { "right_motor_temperature", SensorsDirect::motorTemperature } }, // right
    { entryKey(nordic, dev::sensors, 0x4B), BlackboardEntry { "get_motor_thermal_protection_status", raw_data } }, //
    { entryKey(nordic, dev::sensors, 0x4C), BlackboardEntry { "enable_motor_thermal_protection_status_notify", status_data } }, //
    { entryKey(nordic, dev::sensors, 0x4D), BlackboardEntry { "motor_thermal_protection_status_notify", raw_data } }, //
//
    { entryKey(nordic, dev::system, 0x00), BlackboardEntry { "nordic_main_application_version", msg_array } }, //
    { entryKey(btc, dev::system, 0x00), BlackboardEntry { "bt_main_application_version", msg_array } }, //
    { entryKey(nordic, dev::system, 0x01), BlackboardEntry { "nordic_bootloader_version", msg_array } }, //
    { entryKey(btc, dev::system, 0x01), BlackboardEntry { "bt_bootloader_version", msg_array } }, //
    { entryKey(btc, dev::system, 0x03), BlackboardEntry { "board_revision", msg_array } }, //
    { entryKey(btc, dev::system, 0x06), BlackboardEntry { "mac_address", msg_array } }, //
    { entryKey(btc, dev::system, 0x13), BlackboardEntry { "stats_id", msg_array } }, //
    { entryKey(nordic, dev::system, 0x1F), BlackboardEntry { "nordic_processor_name", msg_array } }, //
    { entryKey(btc, dev::system, 0x1F), BlackboardEntry { "bt_processor_name", msg_array } }, //
    { entryKey(btc, dev::system, 0x38), BlackboardEntry { "get_sku", msg_array } }, //
    { entryKey(nordic, dev::system, 0x39), BlackboardEntry { "get_core_up_time_in_milliseconds", msg_array } }, //
#endif
    };
    MsgArray empty;

    //----------------------------------------------------------------------------------------------------------------------
    uint8_t Blackboard::byteValue(const uint8_t cmd, const CommandBase::TargetPort target, const Devices dev) {
        std::any value { bb::entryValue(target, dev, cmd) };
        return (value.has_value()) ? std::any_cast<MsgArray>(value)[0] : 0;
    }
    //----------------------------------------------------------------------------------------------------------------------
    uint16_t Blackboard::uintValue(const uint8_t cmd, const CommandBase::TargetPort target, const Devices dev) {
        std::any value { bb::entryValue(target, dev, cmd) };
        MsgArray msg { (value.has_value()) ? std::any_cast<MsgArray>(value) : empty };
        int16_t res { };
        if (msg.size() >= sizeof(int16_t)) {
            res = uintConvert(msg.begin(), sizeof(int16_t));
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    int16_t Blackboard::intValue(const uint8_t cmd, const CommandBase::TargetPort target, const Devices dev) {
        return static_cast<int16_t>(uintValue(cmd, target, dev));
    }
    //----------------------------------------------------------------------------------------------------------------------
    uint64_t Blackboard::uintConvert(rvr::MsgArray::const_iterator begin, const uint8_t n) {
        uint64_t res { };
        for (auto it { begin }; it != begin + n; ++it) {
            res <<= 8;
            res |= *it;
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    uint64_t Blackboard::uint64Value(const uint8_t cmd, const CommandBase::TargetPort target, const Devices dev) {
        std::any value { bb::entryValue(target, dev, cmd) };
        MsgArray msg { (value.has_value()) ? std::any_cast<MsgArray>(value) : empty };
        uint64_t res { };

        if (msg.size() >= sizeof(uint64_t)) {
            res = uintConvert(msg.begin(), sizeof(uint64_t));
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::string Blackboard::stringValue(const uint8_t cmd, const CommandBase::TargetPort target, const Devices dev) {
        std::any value { entryValue(target, dev, cmd) };
        MsgArray msg { (value.has_value()) ? std::any_cast<MsgArray>(value) : empty };
        return std::string { msg.begin(), msg.end() };
    }

}
/* namespace rvr */
