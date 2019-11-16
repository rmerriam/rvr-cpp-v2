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
    long long Blackboard::int_convert(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        long long value { };
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
        std::copy(begin, end, std::ostream_iterator<int>(std::cerr, " "));
    }
    //----------------------------------------------------------------------------------------------------------------------
    void string_data(const bb::key_t key, MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        std::copy(begin, end, std::ostream_iterator<uint8_t>(std::cerr));
    }
    //----------------------------------------------------------------------------------------------------------------------
    void float_data(const bb::key_t key, MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        terr << __func__ << mys::sp << Blackboard::float_convert(begin, end);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void int_data(const bb::key_t key, MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        long long value { Blackboard::int_convert(begin, end) };
        terr << __func__ << mys::sp << value;
    }
    //----------------------------------------------------------------------------------------------------------------------
    void tri_float_data(const bb::key_t key, MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        terr << __func__ << mys::sp << Blackboard::float_convert(begin, begin + 4);
        terr << __func__ << mys::sp << Blackboard::float_convert(begin + 4, begin + 8);
        terr << __func__ << mys::sp << Blackboard::float_convert(begin + 8, end);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void status_data(const bb::key_t key, MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        char_ptr status[] { "okay", "fail" };
        terr << __func__ << mys::sp << status[begin[ -1]];
    }
    //----------------------------------------------------------------------------------------------------------------------
    void notification_status(const bb::key_t key, MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        terr << __func__ << mys::sp << "okay";
    }
//----------------------------------------------------------------------------------------------------------------------
    void motor_fault_data(const bb::key_t key, MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        // ******** NOT SURE THIS IS CORRECT **** RETURN FROM ENABLE DOESN"T HAVE SEQUENCE #
        terr << __func__ << mys::sp << std::boolalpha << (begin[ -1] == 0);
    }
//----------------------------------------------------------------------------------------------------------------------
    void motor_stall_data(const bb::key_t key, MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        // ******** NOT SURE THIS IS CORRECT **** RETURN FROM ENABLE DOESN"T HAVE SEQUENCE #
        char_ptr motor[] { "left", "right" };
        terr << __func__ << mys::sp << motor[begin[0]] << std::boolalpha << (begin[1] == 0);
    }
//----------------------------------------------------------------------------------------------------------------------
    void percentage_data(const bb::key_t key, MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        double value { float( *begin) / 100.0 };
        terr << __func__ << mys::sp << value;
    }
//----------------------------------------------------------------------------------------------------------------------
    void version_data(const bb::key_t key, MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        terr << __func__ << mys::sp << mys::sp << ((begin[0] << 8) | begin[1]) << "." << ((begin[2] << 8) | begin[3]) << "."
             << ((begin[4] << 8) | begin[5]);
    }
//----------------------------------------------------------------------------------------------------------------------
    void mac_addr(const bb::key_t key, MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        for (auto it = begin; it < end - 2; ++it) {
            terr << __func__ << mys::sp << *it++ << *it << ":";
        }
        terr << __func__ << mys::sp << *(end - 2) << *(end - 1);
    }
//----------------------------------------------------------------------------------------------------------------------
    void battery_state_data(const bb::key_t key, MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        using char_ptr = const char *;
        char_ptr state[4] { "unknown", "ok", "low", "critical" };
        terr << __func__ << mys::sp << state[ *begin];
    }
    //----------------------------------------------------------------------------------------------------------------------
    using dev = Devices;
    bb::BBDictionary bb::mDictionary { //
    //
//        { entryKey(dev::api_and_shell, 0x00)), BlackboardEntry { "echo", raw_data } }, //
    { key_t(dev::api_and_shell << 8 | 0x00), BlackboardEntry { "echo", raw_data } }, //
    //
#if 1
    { key_t(dev::connection << 8 | 0x05), BlackboardEntry { "get_bluetooth_advertising_name", string_data } }, //
    //
    { key_t(dev::drive << 8 | 0x01), BlackboardEntry { "raw_motors", status_data } }, //
    { key_t(dev::drive << 8 | 0x06), BlackboardEntry { "reset_yaw", status_data } }, //
    { key_t(dev::drive << 8 | 0x07), BlackboardEntry { "drive_with_heading", status_data } }, //
    { key_t(dev::drive << 8 | 0x25), BlackboardEntry { "enable_motor_stall_notify", status_data } }, //
    { key_t(dev::drive << 8 | 0x26), BlackboardEntry { "motor_stall_notify", motor_stall_data } }, //
    { key_t(dev::drive << 8 | 0x27), BlackboardEntry { "enable_motor_fault_notify", status_data } }, //
    { key_t(dev::drive << 8 | 0x28), BlackboardEntry { "motor_fault_notify", motor_fault_data } }, //
    { key_t(dev::drive << 8 | 0x29), BlackboardEntry { "get_motor_fault_state", motor_fault_data } },
    //
    { key_t(dev::io_led << 8 | 0x1A), BlackboardEntry { "set_all_leds", status_data } }, //
    { key_t(dev::io_led << 8 | 0x4E), BlackboardEntry { "release_led_requests", status_data } }, //
    //
    { key_t(dev::power << 8 | 0x01), BlackboardEntry { "snooze", raw_data } }, //
    { key_t(dev::power << 8 | 0x0D), BlackboardEntry { "wake", status_data } }, //
    { key_t(dev::power << 8 | 0x10), BlackboardEntry { "get_battery_percentage", Power::rxBatteryPercentage } }, //
    { key_t(dev::power << 8 | 0x11), BlackboardEntry { "system_awake_notification", notification_status } }, //
    { key_t(dev::power << 8 | 0x17), BlackboardEntry { "get_battery_voltage_state", battery_state_data } }, //
    { key_t(dev::power << 8 | 0x19), BlackboardEntry { "will_sleep_notify", raw_data } }, //
    { key_t(dev::power << 8 | 0x1A), BlackboardEntry { "did_sleep_notify", raw_data } }, //
    { key_t(dev::power << 8 | 0x1B), BlackboardEntry { "enable_battery_voltage_state_change_notify", raw_data } }, //
    { key_t(dev::power << 8 | 0x1C), BlackboardEntry { "battery_voltage_state_change_notify", raw_data } }, //
    { key_t(dev::power << 8 | 0x25), BlackboardEntry { "get_battery_voltage_in_volts", Power::rxBatVoltageInVolts } }, //
    { key_t(dev::power << 8 | 0x26), BlackboardEntry { "get_battery_voltage_state_thresholds", tri_float_data } }, //
    { key_t(dev::power << 8 | 0x27), BlackboardEntry { "get_current_sense_amplifier_current", float_data } }, //
    //
    { key_t(dev::sensors << 8 | 0x0F), BlackboardEntry { "enable_gyro_max_notify", raw_data } }, //
    { key_t(dev::sensors << 8 | 0x10), BlackboardEntry { "gyro_max_notify", raw_data } }, //
    { key_t(dev::sensors << 8 | 0x13), BlackboardEntry { "reset_locator_x_and_y", raw_data } }, //
    { key_t(dev::sensors << 8 | 0x17), BlackboardEntry { "set_locator_flags ", raw_data } }, //
    { key_t(dev::sensors << 8 | 0x22), BlackboardEntry { "get_bot_to_bot_infrared_readings ", raw_data } }, //
    { key_t(dev::sensors << 8 | 0x23), BlackboardEntry { "get_rgbc_senso", raw_data } }, //
    { key_t(dev::sensors << 8 | 0x27), BlackboardEntry { "start_robot_to_robot_infrared_broadcasting", raw_data } }, //
    { key_t(dev::sensors << 8 | 0x28), BlackboardEntry { "start_robot_to_robot_infrared_following", raw_data } }, //
    { key_t(dev::sensors << 8 | 0x29), BlackboardEntry { "stop_robot_to_robot_infrared_broadcasting", raw_data } }, //
    { key_t(dev::sensors << 8 | 0x2C), BlackboardEntry { "robot_to_robot_infrared_message_received_notify", raw_data } }, //
    { key_t(dev::sensors << 8 | 0x30), BlackboardEntry { "get_ambient_light_sensor_value", raw_data } }, //
    { key_t(dev::sensors << 8 | 0x32), BlackboardEntry { "stop_robot_to_robot_infrared_following", raw_data } }, //
    { key_t(dev::sensors << 8 | 0x33), BlackboardEntry { "start_robot_to_robot_infrared_evading", raw_data } }, //
    { key_t(dev::sensors << 8 | 0x34), BlackboardEntry { "stop_robot_to_robot_infrared_evading", raw_data } }, //
    { key_t(dev::sensors << 8 | 0x35), BlackboardEntry { "enable_color_detection_notify", raw_data } }, //
    { key_t(dev::sensors << 8 | 0x36), BlackboardEntry { "color_detection_notify", raw_data } }, //
    { key_t(dev::sensors << 8 | 0x37), BlackboardEntry { "get_current_detected_color_reading", raw_data } }, //
    { key_t(dev::sensors << 8 | 0x38), BlackboardEntry { "enable_color_detection", raw_data } }, //
    { key_t(dev::sensors << 8 | 0x39), BlackboardEntry { "configure_streaming_service", raw_data } }, //
    { key_t(dev::sensors << 8 | 0x3A), BlackboardEntry { "start_streaming_service", raw_data } }, //
    { key_t(dev::sensors << 8 | 0x3B), BlackboardEntry { "stop_streaming_service", raw_data } }, //
    { key_t(dev::sensors << 8 | 0x3C), BlackboardEntry { "clear_streaming_service", raw_data } }, //
    { key_t(dev::sensors << 8 | 0x3D), BlackboardEntry { "streaming_service_data_notify", notification_status } }, //
    { key_t(dev::sensors << 8 | 0x3E), BlackboardEntry { "enable_robot_infrared_message_notify", raw_data } }, //
    { key_t(dev::sensors << 8 | 0x3F), BlackboardEntry { "send_infrared_message", raw_data } }, //
    { key_t(dev::sensors << 8 | 0x42), BlackboardEntry { "get_motor_temperature", raw_data } }, //
    { key_t(dev::sensors << 8 | 0x4B), BlackboardEntry { "get_motor_thermal_protection_status", raw_data } }, //
    { key_t(dev::sensors << 8 | 0x4C), BlackboardEntry { "enable_motor_thermal_protection_status_notify", raw_data } }, //
    { key_t(dev::sensors << 8 | 0x4D), BlackboardEntry { "motor_thermal_protection_status_notify", raw_data } }, //
    //
    { key_t(dev::system << 8 | 0x00), BlackboardEntry { "get_main_application_version", version_data } }, //
    { key_t(dev::system << 8 | 0x01), BlackboardEntry { "get_bootloader_version", version_data } }, //
    { key_t(dev::system << 8 | 0x03), BlackboardEntry { "get_board_revision", int_data } }, //
    { key_t(dev::system << 8 | 0x06), BlackboardEntry { "get_mac_address", mac_addr } }, //
    { key_t(dev::system << 8 | 0x13), BlackboardEntry { "get_stats_id", int_data } }, //
    { key_t(dev::system << 8 | 0x1F), BlackboardEntry { "get_processor_name", string_data } }, //
    { key_t(dev::system << 8 | 0x38), BlackboardEntry { "get_sku", string_data } }, //
    { key_t(dev::system << 8 | 0x39), BlackboardEntry { "get_core_up_time_in_milliseconds", int_data } }, //
#endif
    };
}
/* namespace rvr */
