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
#include <Blackboard.h>
#include "Trace.h"

#include "CommandBase.h"
#include "ApiShell.h"
#include "Power.h"

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
    int64_t Blackboard::int_convert(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        int64_t value { };
        for (auto it { begin }; it != end; ++it) {
            const uint8_t v { *it };
            value <<= 8;
            value |= v;
        }
        return value;
    }
//----------------------------------------------------------------------------------------------------------------------
    float Blackboard::float_convert(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
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
    void raw_data(const bb::key_t key, MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        std::cerr << std::hex;
        Blackboard::entryValue(key) = MsgArray(begin, end);
        terr << code_loc << std::hex << key << mys::sp;
        std::copy(begin, end, std::ostream_iterator<int>(std::cerr, " "));

    }
    //----------------------------------------------------------------------------------------------------------------------
    void string_data(const bb::key_t key, MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        Blackboard::entryValue(key) = std::string(begin, end);
        terr << code_loc << key << mys::sp << std::any_cast<std::string>(Blackboard::entryValue(key));
    }
    //----------------------------------------------------------------------------------------------------------------------
    void float_data(const bb::key_t key, MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        Blackboard::entryValue(key) = bb::float_convert(begin, end);
        terr << code_loc << key << mys::sp << std::any_cast<float>(Blackboard::entryValue(key));
    }
    //----------------------------------------------------------------------------------------------------------------------
    void int_data(const bb::key_t key, MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        Blackboard::entryValue(key) = bb::int_convert(begin, end);
        terr << code_loc << key << mys::sp << std::any_cast<int64_t>(Blackboard::entryValue(key));
    }
    //----------------------------------------------------------------------------------------------------------------------
    void status_data(const bb::key_t key, MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        Blackboard::entryValue(key) = *(begin - 1);
        terr << code_loc << "status: " << (bool) *(begin - 1);
    }
    //----------------------------------------------------------------------------------------------------------------------
    // Notifications don't have sequence numbers so 'data' starts at what is usually the sequence
    // They are just flags
    void status_notification(const bb::key_t key, MsgArray::const_iterator seq, MsgArray::const_iterator end) {
        Blackboard::entryValue(key) = *seq;
    }
    //----------------------------------------------------------------------------------------------------------------------
//    void status_notification(const bb::key_t key, MsgArray::const_iterator seq, MsgArray::const_iterator end) {
//        Blackboard::entryValue(key) = *seq;
//    }
//----------------------------------------------------------------------------------------------------------------------
    void motor_fault_data(const bb::key_t key, MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        // ******** NOT SURE THIS IS CORRECT **** RETURN FROM ENABLE DOESN"T HAVE SEQUENCE #
        terr << code_loc << std::boolalpha << (begin[ -1] == 0);
    }
//----------------------------------------------------------------------------------------------------------------------
    void motor_stall_data(const bb::key_t key, MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        // ******** NOT SURE THIS IS CORRECT **** RETURN FROM ENABLE DOESN"T HAVE SEQUENCE #
        char_ptr motor[] { "left", "right" };
        terr << code_loc << motor[begin[0]] << std::boolalpha << (begin[1] == 0);
    }
//----------------------------------------------------------------------------------------------------------------------
    void mac_addr(const bb::key_t key, MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        for (auto it = begin; it < end - 2; ++it) {
            terr << code_loc << *it++ << *it << ":";
        }
        terr << code_loc << *(end - 2) << *(end - 1);
    }

//----------------------------------------------------------------------------------------------------------------------
    using dev = Devices;
    constexpr uint8_t micro = CommandBase::microcontroller;
    constexpr uint8_t btc = CommandBase::bluetoothSOC;

    bb::BBDictionary bb::mDictionary { //
//
    { entryKey(micro, dev::api_and_shell, ApiShell::echo_cmd), BlackboardEntry { "echo", raw_data } }, //
//
    { entryKey(btc, dev::connection, 0x05), BlackboardEntry { "get_bluetooth_advertising_name", string_data } }, //
#if 1
//
    { entryKey(micro, dev::drive, 0x01), BlackboardEntry { "raw_motors", status_data } }, //
    { entryKey(micro, dev::drive, 0x06), BlackboardEntry { "reset_yaw", status_data } }, //
    { entryKey(micro, dev::drive, 0x07), BlackboardEntry { "drive_with_heading", status_data } }, //
    { entryKey(micro, dev::drive, 0x25), BlackboardEntry { "enable_motor_stall_notify", status_data } }, //
    { entryKey(micro, dev::drive, 0x26), BlackboardEntry { "motor_stall_notify", status_notification } }, //
    { entryKey(micro, dev::drive, 0x27), BlackboardEntry { "enable_motor_fault_notify", status_data } }, //
    { entryKey(micro, dev::drive, 0x28), BlackboardEntry { "motor_fault_notify", status_notification } }, //
    { entryKey(micro, dev::drive, 0x29), BlackboardEntry { "get_motor_fault_state", motor_fault_data } },
//
    { entryKey(btc, dev::io_led, 0x1A), BlackboardEntry { "set_all_leds", status_data } }, //
    { entryKey(btc, dev::io_led, 0x4E), BlackboardEntry { "release_led_requests", status_data } }, //
//
    { entryKey(btc, dev::power, 0x01), BlackboardEntry { "snooze", raw_data } }, //
    { entryKey(btc, dev::power, 0x0D), BlackboardEntry { "wake", status_data } }, //
    { entryKey(btc, dev::power, 0x10), BlackboardEntry { "get_battery_percentage", int_data } }, //
    { entryKey(btc, dev::power, 0x11), BlackboardEntry { "system_awake_notification", status_notification } }, //
    { entryKey(btc, dev::power, 0x17), BlackboardEntry { "get_battery_voltage_state", int_data } }, //
    { entryKey(btc, dev::power, 0x19), BlackboardEntry { "will_sleep_notify", status_notification } }, //
    { entryKey(btc, dev::power, 0x1A), BlackboardEntry { "did_sleep_notify", status_notification } }, //
    { entryKey(btc, dev::power, 0x1B), BlackboardEntry { "enable_battery_voltage_state_change_notify", int_data } }, //
    { entryKey(btc, dev::power, 0x1C), BlackboardEntry { "battery_voltage_state_change_notify", status_notification } }, //
    { entryKey(btc, dev::power, 0x25, Power::CalibratedFiltered), BlackboardEntry { "get_battery_voltage_in_volts", float_data } }, //
    { entryKey(btc, dev::power, 0x25, Power::CalibratedUnfiltered), BlackboardEntry { "get_battery_voltage_in_volts", float_data } }, //
    { entryKey(btc, dev::power, 0x25, Power::UncalibratedUnfiltered), BlackboardEntry { "get_battery_voltage_in_volts", float_data } }, //
    { entryKey(btc, dev::power, 0x26), BlackboardEntry { "get_battery_voltage_state_thresholds", Power::rxBatteryThresholds } }, //
    { entryKey(btc, dev::power, 0x26, 1), BlackboardEntry { "get_battery_voltage_state_thresholds", Power::rxBatteryThresholds } }, //
    { entryKey(btc, dev::power, 0x26, 2), BlackboardEntry { "get_battery_voltage_state_thresholds", Power::rxBatteryThresholds } }, //
    { entryKey(micro, dev::power, 0x27), BlackboardEntry { "get_current_sense_amplifier_current", float_data } }, //
    { entryKey(micro, dev::power, 0x27, 1), BlackboardEntry { "get_current_sense_amplifier_current", float_data } }, //
//
    { entryKey(micro, dev::sensors, 0x0F), BlackboardEntry { "enable_gyro_max_notify", status_data } }, //
    { entryKey(micro, dev::sensors, 0x10), BlackboardEntry { "gyro_max_notify", status_notification } }, //
    { entryKey(micro, dev::sensors, 0x13), BlackboardEntry { "reset_locator_x_and_y", raw_data } }, //
    { entryKey(micro, dev::sensors, 0x17), BlackboardEntry { "set_locator_flags ", raw_data } }, //
    { entryKey(micro, dev::sensors, 0x22), BlackboardEntry { "get_bot_to_bot_infrared_readings ", raw_data } }, //
    { entryKey(micro, dev::sensors, 0x23), BlackboardEntry { "get_rgbc_senso", raw_data } }, //
    { entryKey(micro, dev::sensors, 0x27), BlackboardEntry { "start_robot_to_robot_infrared_broadcasting", raw_data } }, //
    { entryKey(micro, dev::sensors, 0x28), BlackboardEntry { "start_robot_to_robot_infrared_following", raw_data } }, //
    { entryKey(micro, dev::sensors, 0x29), BlackboardEntry { "stop_robot_to_robot_infrared_broadcasting", raw_data } }, //
    { entryKey(micro, dev::sensors, 0x2C), BlackboardEntry { "robot_to_robot_infrared_message_received_notify", status_notification } }, //
    { entryKey(btc, dev::sensors, 0x30), BlackboardEntry { "get_ambient_light_sensor_value", float_data } }, //
    { entryKey(micro, dev::sensors, 0x32), BlackboardEntry { "stop_robot_to_robot_infrared_following", raw_data } }, //
    { entryKey(micro, dev::sensors, 0x33), BlackboardEntry { "start_robot_to_robot_infrared_evading", raw_data } }, //
    { entryKey(micro, dev::sensors, 0x34), BlackboardEntry { "stop_robot_to_robot_infrared_evading", raw_data } }, //
    { entryKey(micro, dev::sensors, 0x35), BlackboardEntry { "enable_color_detection_notify", status_data } }, //
    { entryKey(micro, dev::sensors, 0x36), BlackboardEntry { "color_detection_notify", status_notification } }, //
    { entryKey(micro, dev::sensors, 0x37), BlackboardEntry { "get_current_detected_color_reading", raw_data } }, //
    { entryKey(micro, dev::sensors, 0x38), BlackboardEntry { "enable_color_detection", status_data } }, //
    { entryKey(micro, dev::sensors, 0x39), BlackboardEntry { "configure_streaming_service", status_data } }, //
    { entryKey(micro, dev::sensors, 0x3A), BlackboardEntry { "start_streaming_service", status_data } }, //
    { entryKey(micro, dev::sensors, 0x3B), BlackboardEntry { "stop_streaming_service", status_data } }, //
    { entryKey(micro, dev::sensors, 0x3C), BlackboardEntry { "clear_streaming_service", status_data } }, //
    { entryKey(micro, dev::sensors, 0x3D), BlackboardEntry { "streaming_service_data_notify", status_notification } }, //
    { entryKey(micro, dev::sensors, 0x3E), BlackboardEntry { "enable_robot_infrared_message_notify", status_data } }, //
    { entryKey(micro, dev::sensors, 0x3F), BlackboardEntry { "send_infrared_message", raw_data } }, //
    { entryKey(btc, dev::sensors, 0x42, 0), BlackboardEntry { "get_motor_temperature", raw_data } }, //
    { entryKey(btc, dev::sensors, 0x42, 1), BlackboardEntry { "get_motor_temperature", raw_data } }, //
    { entryKey(btc, dev::sensors, 0x4B), BlackboardEntry { "get_motor_thermal_protection_status", raw_data } }, //
    { entryKey(micro, dev::sensors, 0x4C), BlackboardEntry { "enable_motor_thermal_protection_status_notify", status_data } }, //
    { entryKey(micro, dev::sensors, 0x4D), BlackboardEntry { "motor_thermal_protection_status_notify", raw_data } }, //
//
    { entryKey(micro, dev::system, 0x00), BlackboardEntry { "get_main_application_version", raw_data } }, //
    { entryKey(btc, dev::system, 0x00), BlackboardEntry { "get_main_application_version", raw_data } }, //
    { entryKey(micro, dev::system, 0x01), BlackboardEntry { "get_bootloader_version", raw_data } }, //
    { entryKey(btc, dev::system, 0x01), BlackboardEntry { "get_bootloader_version", raw_data } }, //
    { entryKey(btc, dev::system, 0x03), BlackboardEntry { "get_board_revision", int_data } }, //
    { entryKey(btc, dev::system, 0x06), BlackboardEntry { "get_mac_address", string_data } }, //
    { entryKey(micro, dev::system, 0x13), BlackboardEntry { "get_stats_id", int_data } }, //
    { entryKey(micro, dev::system, 0x1F), BlackboardEntry { "get_processor_name", string_data } }, //
    { entryKey(btc, dev::system, 0x1F), BlackboardEntry { "get_processor_name", string_data } }, //
    { entryKey(btc, dev::system, 0x38), BlackboardEntry { "get_sku", string_data } }, //
    { entryKey(micro, dev::system, 0x39), BlackboardEntry { "get_core_up_time_in_milliseconds", int_data } }, //
#endif
    };
}
/* namespace rvr */
