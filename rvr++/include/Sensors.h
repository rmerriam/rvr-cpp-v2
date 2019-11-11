#ifndef Sensors_H_
#define Sensors_H_
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

 */

namespace rvr {

    class Sensors : protected CommandBase {

    public:
        enum VoltageType : uint8_t {
            CalibratedFiltered = 0, CalibratedUnfiltered = 1, UncalibratedUnfiltered = 2,
        };
        enum MotorSide : uint8_t {
            left = 0, right = 1
        };
        Sensors(Request& req) :
            CommandBase { Devices::sensors, req, microcontroller } {
        }

        Sensors(const Sensors& other) = delete;
        Sensors(Sensors&& other) = delete;
        Sensors& operator=(const Sensors& other) = delete;

        //----------------------------------------------------------------------------------------------------------------------
        void enableGyroMaxNotify() {
            cmd_enable(enable_gyro_max_notify, true, true);
        }
        //----------------------------------------------------------------------------------------------------------------------
        void disableGyroMaxNotify() {
            cmd_enable(enable_gyro_max_notify, false, true);
        }
        //
        //----------------------------------------------------------------------------------------------------------------------
        void configureStreaming(const uint8_t token, const MsgArray& cfg, const uint8_t proc, const bool get_response = true) {
            MsgArray msg { buildFlags(get_response), proc, mDevice, configure_streaming_service, mRequest.sequence(), };
            msg.insert(msg.end(), cfg.begin(), cfg.end());

            mRequest.send(msg);

//            cmd_byte(configure_streaming_service, token, true);
        }

        //----------------------------------------------------------------------------------------------------------------------
        void enableStreaming(const uint16_t millis) {
            cmd_int(start_streaming_service, millis, true);
        }
        //----------------------------------------------------------------------------------------------------------------------
        void disableStreaming() {
            cmd_basic(stop_streaming_service, true);
        }
        //----------------------------------------------------------------------------------------------------------------------
        void clearStreaming() {
            cmd_basic(clear_streaming_service, true);
        }
    private:
        enum MotorIndexes : uint8_t {
            left_motor_index = 0, right_motor_index = 1,
        };

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
            enable_gyro_max_notify = 0x0F, gyro_max_notify = 0x10,
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
            get_motor_temperature = 0x42,
            get_motor_thermal_protection_status = 0x4B,
            enable_motor_thermal_protection_status_notify = 0x4C,
            motor_thermal_protection_status_notify = 0x4D,
        };

    };

//    //----------------------------------------------------------------------------------------------------------------------
//    inline void Sensors::awake() {
//        cmd_basic(wake, true);
//    }
//    //----------------------------------------------------------------------------------------------------------------------
//    inline void Sensors::sleep() {
//        cmd_basic(snooze, true);
//    }
//    //----------------------------------------------------------------------------------------------------------------------
//    inline void Sensors::battery_precentage() {
//        cmd_basic(get_battery_percentage, true);
//    }
//    //----------------------------------------------------------------------------------------------------------------------
//    inline void Sensors::battery_voltage_state() {
//        cmd_basic(get_battery_voltage_state, true);
//    }
//    //----------------------------------------------------------------------------------------------------------------------
//    inline void Sensors::battery_voltage(const VoltageType vt) {
//        cmd_byte(get_battery_voltage_in_volts, vt, true);
//    }
//    //----------------------------------------------------------------------------------------------------------------------
//    inline void Sensors::battery_volt_thresholds() {
//        cmd_basic(get_battery_voltage_state_thresholds, true);
//    }
//    //----------------------------------------------------------------------------------------------------------------------
//    inline void Sensors::battery_current(const MotorSide ms) {
//        cmd_byte_alt(get_current_sense_amplifier_current, ms, true);
//    }

} /* namespace rvr */

#endif
