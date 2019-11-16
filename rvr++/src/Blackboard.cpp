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
     * epeated for the desired sensors. Sensors are grouped by the processor and token.

     * The responses are all 0x3D streaming service notify. The data is in the order and size specified in the streaming
     * ;request from (1) determined by processor and token.
     */
    Blackboard::Blackboard() {
    }

    //======================================================================================================================
    //----------------------------------------------------------------------------------------------------------------------
    void raw_data(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        std::cerr << std::hex;
        std::copy(begin, end, std::ostream_iterator<int>(std::cerr, " "));
    }
    //----------------------------------------------------------------------------------------------------------------------
    long long Blackboard::int_convert(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        long long value { };
        for (auto it { begin }; it != end; ++it) {
            const uint8_t& v { *it };
            value <<= 8;
            value += v;
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
    void string_data(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        std::copy(begin, end, std::ostream_iterator<uint8_t>(std::cerr));
    }
    //----------------------------------------------------------------------------------------------------------------------
    void float_data(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        terr << __func__ << mys::sp << Blackboard::float_convert(begin, end);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void int_data(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        long long value { Blackboard::int_convert(begin, end) };
        terr << __func__ << mys::sp << value;
    }
    //----------------------------------------------------------------------------------------------------------------------
    void tri_float_data(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        terr << __func__ << mys::sp << Blackboard::float_convert(begin, begin + 4);
        terr << __func__ << mys::sp << Blackboard::float_convert(begin + 4, begin + 8);
        terr << __func__ << mys::sp << Blackboard::float_convert(begin + 8, end);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void status_data(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        char_ptr status[] { "okay", "fail" };
        terr << __func__ << mys::sp << status[begin[ -1]];
    }
    //----------------------------------------------------------------------------------------------------------------------
    void notification_status(MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        terr << __func__ << mys::sp << "okay";
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
    using dev = Devices;
    Blackboard::DecoderMap Blackboard::decoder_map { //
    //
    { dev::api_and_shell << 8 | 0x00, RespDecoder { "echo", raw_data } }, //
    //
    { dev::connection << 8 | 0x05, RespDecoder { "get_bluetooth_advertising_name", string_data } }, //
    //
    { dev::drive << 8 | 0x01, RespDecoder { "raw_motors", status_data } }, //
    { dev::drive << 8 | 0x06, RespDecoder { "reset_yaw", status_data } }, //
    { dev::drive << 8 | 0x07, RespDecoder { "drive_with_heading", status_data } }, //
    { dev::drive << 8 | 0x25, RespDecoder { "enable_motor_stall_notify", status_data } }, //
    { dev::drive << 8 | 0x26, RespDecoder { "motor_stall_notify", motor_stall_data } }, //
    { dev::drive << 8 | 0x27, RespDecoder { "enable_motor_fault_notify", status_data } }, //
    { dev::drive << 8 | 0x28, RespDecoder { "motor_fault_notify", motor_fault_data } }, //
    { dev::drive << 8 | 0x29, RespDecoder { "get_motor_fault_state", motor_fault_data } },
    //
    { dev::io_led << 8 | 0x1A, RespDecoder { "set_all_leds", status_data } }, //
    { dev::io_led << 8 | 0x4E, RespDecoder { "release_led_requests", status_data } }, //
    //
    { dev::power << 8 | 0x01, RespDecoder { "snooze", raw_data } }, //
    { dev::power << 8 | 0x0D, RespDecoder { "wake", status_data } }, //
    { dev::power << 8 | 0x10, RespDecoder { "get_battery_percentage", percentage_data } }, //
    { dev::power << 8 | 0x11, RespDecoder { "system_awake_notification", notification_status } }, //
    { dev::power << 8 | 0x17, RespDecoder { "get_battery_voltage_state", battery_state_data } }, //
    { dev::power << 8 | 0x19, RespDecoder { "will_sleep_notify", raw_data } }, //
    { dev::power << 8 | 0x1A, RespDecoder { "did_sleep_notify", raw_data } }, //
    { dev::power << 8 | 0x1B, RespDecoder { "enable_battery_voltage_state_change_notify", raw_data } }, //
    { dev::power << 8 | 0x1C, RespDecoder { "battery_voltage_state_change_notify", raw_data } }, //
    { dev::power << 8 | 0x25, RespDecoder { "get_battery_voltage_in_volts", Power::rxBatVoltageInVolts } }, //
    { dev::power << 8 | 0x26, RespDecoder { "get_battery_voltage_state_thresholds", tri_float_data } }, //
    { dev::power << 8 | 0x27, RespDecoder { "get_current_sense_amplifier_current", float_data } }, //
    //
    { dev::sensors << 8 | 0x0F, RespDecoder { "enable_gyro_max_notify", raw_data } }, //
    { dev::sensors << 8 | 0x10, RespDecoder { "gyro_max_notify", raw_data } }, //
    { dev::sensors << 8 | 0x13, RespDecoder { "reset_locator_x_and_y", raw_data } }, //
    { dev::sensors << 8 | 0x17, RespDecoder { "set_locator_flags ", raw_data } }, //
    { dev::sensors << 8 | 0x22, RespDecoder { "get_bot_to_bot_infrared_readings ", raw_data } }, //
    { dev::sensors << 8 | 0x23, RespDecoder { "get_rgbc_senso", raw_data } }, //
    { dev::sensors << 8 | 0x27, RespDecoder { "start_robot_to_robot_infrared_broadcasting", raw_data } }, //
    { dev::sensors << 8 | 0x28, RespDecoder { "start_robot_to_robot_infrared_following", raw_data } }, //
    { dev::sensors << 8 | 0x29, RespDecoder { "stop_robot_to_robot_infrared_broadcasting", raw_data } }, //
    { dev::sensors << 8 | 0x2C, RespDecoder { "robot_to_robot_infrared_message_received_notify", raw_data } }, //
    { dev::sensors << 8 | 0x30, RespDecoder { "get_ambient_light_sensor_value", raw_data } }, //
    { dev::sensors << 8 | 0x32, RespDecoder { "stop_robot_to_robot_infrared_following", raw_data } }, //
    { dev::sensors << 8 | 0x33, RespDecoder { "start_robot_to_robot_infrared_evading", raw_data } }, //
    { dev::sensors << 8 | 0x34, RespDecoder { "stop_robot_to_robot_infrared_evading", raw_data } }, //
    { dev::sensors << 8 | 0x35, RespDecoder { "enable_color_detection_notify", raw_data } }, //
    { dev::sensors << 8 | 0x36, RespDecoder { "color_detection_notify", raw_data } }, //
    { dev::sensors << 8 | 0x37, RespDecoder { "get_current_detected_color_reading", raw_data } }, //
    { dev::sensors << 8 | 0x38, RespDecoder { "enable_color_detection", raw_data } }, //
    { dev::sensors << 8 | 0x39, RespDecoder { "configure_streaming_service", raw_data } }, //
    { dev::sensors << 8 | 0x3A, RespDecoder { "start_streaming_service", raw_data } }, //
    { dev::sensors << 8 | 0x3B, RespDecoder { "stop_streaming_service", raw_data } }, //
    { dev::sensors << 8 | 0x3C, RespDecoder { "clear_streaming_service", raw_data } }, //
    { dev::sensors << 8 | 0x3D, RespDecoder { "streaming_service_data_notify", notification_status } }, //
    { dev::sensors << 8 | 0x3E, RespDecoder { "enable_robot_infrared_message_notify", raw_data } }, //
    { dev::sensors << 8 | 0x3F, RespDecoder { "send_infrared_message", raw_data } }, //
    { dev::sensors << 8 | 0x42, RespDecoder { "get_motor_temperature", raw_data } }, //
    { dev::sensors << 8 | 0x4B, RespDecoder { "get_motor_thermal_protection_status", raw_data } }, //
    { dev::sensors << 8 | 0x4C, RespDecoder { "enable_motor_thermal_protection_status_notify", raw_data } }, //
    { dev::sensors << 8 | 0x4D, RespDecoder { "motor_thermal_protection_status_notify", raw_data } }, //
    //
    { dev::system << 8 | 0x00, RespDecoder { "get_main_application_version", version_data } }, //
    { dev::system << 8 | 0x01, RespDecoder { "get_bootloader_version", version_data } }, //
    { dev::system << 8 | 0x03, RespDecoder { "get_board_revision", int_data } }, //
    { dev::system << 8 | 0x06, RespDecoder { "get_mac_address", mac_addr } }, //
    { dev::system << 8 | 0x13, RespDecoder { "get_stats_id", int_data } }, //
    { dev::system << 8 | 0x1F, RespDecoder { "get_processor_name", string_data } }, //
    { dev::system << 8 | 0x38, RespDecoder { "get_sku", string_data } }, //
    { dev::system << 8 | 0x39, RespDecoder { "get_core_up_time_in_milliseconds", int_data } }, //
    };
} /* namespace rvr */
