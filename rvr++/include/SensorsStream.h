#ifndef SensorsNordic_H_
#define SensorsNordic_H_
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
//     Created: Oct 26, 2019
//
//======================================================================================================================
#include <array>

#include "Request.h"
#include "CommandBase.h"

/*
 streaming service configuration for RVR:
 | Id     | Processor          | Token | Service            | Attributes                 |
 | ------ | ------------- -----| ----- | ------------------ | -------------------------- |
 | 0x0003 | Nordic (1)         | 1     | ColorDetection     | R, G, B, Index, Confidence |
 | 0x000A | Nordic (1)         | 2     | AmbientLight       | Light                      |
 -----------------------------------------------------------------------------------------
 | 0x0000 | ST (2)             | 1     | Quaternion         | W, X, Y, Z                 |
 | 0x0001 | ST (2)             | 1     | IMU                | Pitch, Roll, Yaw           |
 | 0x0002 | ST (2)             | 1     | Accelerometer      | X, Y, Z                    |
 | 0x0004 | ST (2)             | 1     | Gyroscope          | X, Y, Z                    |
 | 0x0006 | ST (2)             | 2     | Locator            | X, Y                       |
 | 0x0007 | ST (2)             | 2     | Velocity           | X, Y                       |
 | 0x0008 | ST (2)             | 2     | Speed              | Speed                      |
 -----------------------------------------------------------------------------------------
 | 0x0009 | Nordic (1), ST (2) | 3     | CoreTime           | TimeUpper, TimeLower       |
 -----------------------------------------------------------------------------------------
 Quaternion -1.0 - 1.0
 IMU -180 - 180
 Accelerometer -16.0 - 16.0
 Gyroscope -2000.0 - 2000.0

 Normalize: (value) / Max * (rangeMax - rangeMin) + rangeMin

 Accelerometer returns X = 7F B5 AF 00 = 2142613248
 Normalize = 2142613248/4294967295 * (16 - -16) + -16
 Normalize = .498866 * 32 - 16
 Normalize = -0.036288

 Thermal Protection
 0x00 = OK
 0x01 = WARN
 0x02 = CRITICAL_FORCED_COOL_DOWN
 */

namespace rvr {

    class SensorsStream : protected CommandBase {

    public:
        enum VoltageType : uint8_t {
            CalibratedFiltered = 0, CalibratedUnfiltered = 1, UncalibratedUnfiltered = 2,
        };

        SensorsStream(Request& req) :
            CommandBase { Devices::sensors, req, nordic } {
        }

        SensorsStream(const SensorsStream& other) = delete;
        SensorsStream(SensorsStream&& other) = delete;
        SensorsStream& operator=(const SensorsStream& other) = delete;

        void configureStreaming(const MsgArray& cfg, const CommandResponse want_resp = resp_on_error);
        void enableStreaming(const uint16_t millis, const CommandResponse want_resp = resp_on_error);
        void disableStreaming(const CommandResponse want_resp = resp_on_error);
        void clearStreaming(const CommandResponse want_resp = resp_on_error);

        //======================================================================================================================
        // data access methods
        //----------------------------------------------------------------------------------------------------------------------
        //======================================================================================================================
        // data conversion methods
//        int batteryPercent() {
//            std::any value { bb::entryValue(mTarget, Devices::power, get_battery_percentage) };
//            return (value.has_value()) ? std::any_cast<int64_t>(value) : -1;
//        }

    private:

        enum ThermalProtectionStatusEnum : uint8_t {
            ok = 0, warn = 1, critical = 2,
        };

        enum StreamingDataSizesEnum : uint8_t {
            eight_bit = 0x00, sixteen_bit = 0x01, thirty_two_bit = 0x02,
        };

        enum struct GyroMaxFlagsBitmask : uint8_t {
            none = 0, max_plus_x = 1, max_minus_x = 2, max_plus_y = 4, max_minus_y = 8, max_plus_z = 16, max_minus_z = 32,
        };

        enum struct LocatorFlagsBitmask : uint8_t {
            none = 0, auto_calibrate = 1,
        };

        enum struct IrLocationMake : uint32_t {
            none = 0, front_left = 0x000000FF, front_right = 0x0000FF00, back_right = 0x00FF0000, back_left = 0xFF000000,
        };

        enum Cmd : uint8_t {
            enable_gyro_max_notify = 0x0F, //
            gyro_max_notify = 0x10,
            //
            reset_locator_x_and_y = 0x13,
            set_locator_flags = 0x17,
            //
            get_bot_to_bot_infrared_readings = 0x22,
            //
            get_rgbc_sensor_values = 0x23,
            //
            start_robot_to_robot_infrared_broadcasting = 0x27,
            start_robot_to_robot_infrared_following = 0x28,
            stop_robot_to_robot_infrared_broadcasting = 0x29,
            robot_to_robot_infrared_message_received_notify = 0x2C,
            get_ambient_light_sensor_value = 0x30,
            stop_robot_to_robot_infrared_following = 0x32,
            start_robot_to_robot_infrared_evading = 0x33,
            stop_robot_to_robot_infrared_evading = 0x34,
            //
            enable_color_detection_notify = 0x35,
            color_detection_notify = 0x36,
            get_current_detected_color_reading = 0x37,
            enable_color_detection = 0x38,
            //
            configure_streaming_service = 0x39,
            start_streaming_service = 0x3A,
            stop_streaming_service = 0x3B,
            clear_streaming_service = 0x3C,
            streaming_service_data_notify = 0x3D,
            //
            enable_robot_infrared_message_notify = 0x3E,
            send_infrared_message = 0x3F,
            //
            get_motor_temperature = 0x4A,
            get_motor_thermal_protection_status = 0x4B,
            enable_motor_thermal_protection_status_notify = 0x4C,
            motor_thermal_protection_status_notify = 0x4D,
        };

    };

    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsStream::configureStreaming(const MsgArray& cfg, const CommandResponse want_resp) {
        cmd_data(configure_streaming_service, cfg, want_resp);
        cmd_data_alt(configure_streaming_service, cfg, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsStream::enableStreaming(const uint16_t millis, const CommandResponse want_resp) {
        cmd_int(start_streaming_service, millis, want_resp);
        cmd_int_alt(start_streaming_service, millis, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsStream::disableStreaming(const CommandResponse want_resp) {
        cmd_basic(stop_streaming_service, want_resp);
        cmd_basic_alt(stop_streaming_service, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsStream::clearStreaming(const CommandResponse want_resp) {
        cmd_basic(clear_streaming_service, want_resp);
        cmd_basic_alt(clear_streaming_service, want_resp);
    }

} /* namespace rvr */

#endif