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

#include "Trace.h"
#include "ResponseDecoder.h"

namespace rvr {

    ResponseDecoder::ResponseDecoder() {

    }
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
    using dev = Packet::Devices;

    ResponseDecoder::DecoderMap ResponseDecoder::decoder_map { //
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
    { dev::power << 8 | 0x11, RespDecoder { "activity_ack", raw_data } }, //
    { dev::power << 8 | 0x17, RespDecoder { "get_battery_voltage_state", battery_state_data } }, //
    { dev::power << 8 | 0x19, RespDecoder { "will_sleep_notify", raw_data } }, //
    { dev::power << 8 | 0x1A, RespDecoder { "did_sleep_notify", raw_data } }, //
    { dev::power << 8 | 0x1B, RespDecoder { "enable_battery_voltage_state_change_notify", raw_data } }, //
    { dev::power << 8 | 0x1C, RespDecoder { "battery_voltage_state_change_notify", raw_data } }, //
    { dev::power << 8 | 0x25, RespDecoder { "get_battery_voltage_in_volts", float_data } }, //
    { dev::power << 8 | 0x26, RespDecoder { "get_battery_voltage_state_thresholds", tri_float_data } }, //
    { dev::power << 8 | 0x27, RespDecoder { "get_current_sense_amplifier_current", float_data } }, //
    //
    { dev::sensors << 8 | 0x0F, RespDecoder { "enable_gyro_max_notify", raw_data } }, //
    { dev::sensors << 8 | 0x10, RespDecoder { "gyro_max_notify", raw_data } }, //
    { dev::sensors << 8 | 0x39, RespDecoder { "configure_streaming_service", raw_data } }, //
    { dev::sensors << 8 | 0x3A, RespDecoder { "start_streaming_service", raw_data } }, //
    { dev::sensors << 8 | 0x3B, RespDecoder { "stop_streaming_service", status_data } }, //
    { dev::sensors << 8 | 0x3C, RespDecoder { "clear_streaming_service", status_data } }, //

#if 0
    enable_gyro_max_notify = 0x0F,
    gyro_max_notify = 0x10,
    reset_locator_x_and_y = 0x13,
    set_locator_flags = 0x17,
    get_bot_to_bot_infrared_readings = 0x22,
    get_rgbc_sensor_values = 0x23,
    start_robot_to_robot_infrared_broadcasting = 0x27,
    start_robot_to_robot_infrared_following = 0x28,
    stop_robot_to_robot_infrared_broadcasting = 0x29,
    robot_to_robot_infrared_message_received_notify = 0x2C,
    get_ambient_light_sensor_value = 0x30,
    stop_robot_to_robot_infrared_following = 0x32,
    start_robot_to_robot_infrared_evading = 0x33,
    stop_robot_to_robot_infrared_evading = 0x34,
    enable_color_detection_notify = 0x35,
    color_detection_notify = 0x36,
    get_current_detected_color_reading = 0x37,
    enable_color_detection = 0x38,
    configure_streaming_service = 0x39,
    start_streaming_service = 0x3A,
    stop_streaming_service = 0x3B,
    clear_streaming_service = 0x3C,
    streaming_service_data_notify = 0x3D,
    enable_robot_infrared_message_notify = 0x3E,
    send_infrared_message = 0x3F,
    get_motor_temperature = 0x42,
    get_motor_thermal_protection_status = 0x4B,
    enable_motor_thermal_protection_status_notify = 0x4C,
    motor_thermal_protection_status_notify = 0x4D,

#endif

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
