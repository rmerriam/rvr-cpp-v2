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

#include "Blackboard.h"
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

    class SensorsDirect : protected CommandBase {

    public:
        enum VoltageType : uint8_t {
            CalibratedFiltered = 0, CalibratedUnfiltered = 1, UncalibratedUnfiltered = 2,
        };

        SensorsDirect(Request& req) :
            CommandBase { Devices::sensors, req, bluetoothSOC } {
        }

        SensorsDirect(SensorsDirect const& other) = delete;
        SensorsDirect(SensorsDirect&& other) = delete;
        SensorsDirect& operator=(SensorsDirect const& other) = delete;

        void setLocatorFlags(bool const flag, CommandResponse const want_resp = resp_on_error) const;
        void resetLocatorXY(CommandResponse const want_resp = resp_on_error) const;

        void enableGyroMaxNotify(CommandResponse const want_resp = resp_on_error) const;
        void disableGyroMaxNotify(CommandResponse const want_resp = resp_on_error) const;

        void getRightMotorTemp(CommandResponse const want_resp = resp_on_error) const;
        void getLeftMotorTemp(CommandResponse const want_resp = resp_on_error) const;

        void getThermalProtectionStatus(CommandResponse const want_resp = resp_on_error) const;
        void enableThermal(CommandResponse const want_resp = resp_on_error) const;
        void disableThermal(CommandResponse const want_resp = resp_on_error) const;

        void getAmbient(CommandResponse const want_resp = resp_on_error) const;

        void disableColorDetection(CommandResponse const want_resp = resp_on_error) const;
        void enabeColorDetectionNotify(bool const enable, uint16_t const timer, uint8_t const confidence, CommandResponse const want_resp =
            resp_on_error) const;
        void enableColorDetection(CommandResponse const want_resp = resp_on_error) const;
        void getCurrentColor(CommandResponse const want_resp = resp_on_error) const;

        float ambient() const;
        float leftMotorTemp() const;
        float rightMotorTemp() const;

        void resetMaxGyroNotifyNotify();
        void resetColorDetectionNotify();

    private:
        //----------------------------------------------------------------------------------------------------------------------
        struct color_note {
            uint8_t red;
            uint8_t green;
            uint8_t blue;
            uint8_t confidence;
            uint8_t classification;
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
    inline void SensorsDirect::setLocatorFlags(bool const flag, CommandResponse const want_resp) const {
        cmd_byte_alt(set_locator_flags, flag, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::resetLocatorXY(CommandResponse const want_resp) const {
        cmd_basic_alt(reset_locator_x_and_y, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::enableGyroMaxNotify(CommandResponse const want_resp) const {
        cmd_enable_alt(enable_gyro_max_notify, true, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::disableGyroMaxNotify(CommandResponse const want_resp) const {
        cmd_enable_alt(enable_gyro_max_notify, false, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::getRightMotorTemp(CommandResponse const want_resp) const {
        cmd_byte_alt_id(get_motor_temperature, 0x05, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::getLeftMotorTemp(CommandResponse const want_resp) const {
        cmd_byte_alt_id(get_motor_temperature, 0x04, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::getThermalProtectionStatus(CommandResponse const want_resp) const {
        cmd_basic_alt(get_motor_thermal_protection_status, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::enableThermal(CommandResponse const want_resp) const {
        cmd_enable_alt(enable_motor_thermal_protection_status_notify, true, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::disableThermal(CommandResponse const want_resp) const {
        cmd_enable_alt(enable_motor_thermal_protection_status_notify, false, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::getAmbient(CommandResponse const want_resp) const {
        cmd_basic(get_ambient_light_sensor_value, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::getCurrentColor(CommandResponse const want_resp) const {
        cmd_basic(get_current_detected_color_reading, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::enableColorDetection(CommandResponse const want_resp) const {
        cmd_enable(enable_color_detection, true, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::disableColorDetection(CommandResponse const want_resp) const {
        cmd_enable(enable_color_detection, false, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::enabeColorDetectionNotify(bool const enable, uint16_t const timer, uint8_t const confidence,
        CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mTarget, mDevice, enable_color_detection_notify, sequence(), //
            enable, static_cast<uint8_t>(timer >> 8), static_cast<uint8_t>(timer & 0xFF), confidence };
        mRequest.send(msg);
    }

//    inline void SensorsDirect::rxColotNotify(const bb::key_t key, MsgArray::iterator begin, MsgArray::iterator end) {
//        terr << code_loc << "notification: " << std::hex << key << mys::sp;
//        std::copy(begin, end, std::ostream_iterator<int>(std::cerr, " "));
//        const color_note &cn = *new ( &( *begin)) color_note;
//        terr << code_loc << std::hex << //
//             (int)cn.red << mys::sp //
//             << (int)cn.green << mys::sp //
//             << (int)cn.blue << mys::sp //
//             << (int)cn.confidence << mys::sp //
//             << (int)cn.classification;
//    }
    //----------------------------------------------------------------------------------------------------------------------
    inline float SensorsDirect::ambient() const {
        return bb::floatValue(mTarget, mDevice, get_ambient_light_sensor_value);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline float SensorsDirect::leftMotorTemp() const {
        return bb::floatValue(mAltTarget, mDevice, get_motor_temperature, 0, 4);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline float SensorsDirect::rightMotorTemp() const {
        return bb::floatValue(mAltTarget, mDevice, get_motor_temperature, 0, 5);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::resetMaxGyroNotifyNotify() {
        bb::resetNotify(mAltTarget, mDevice, gyro_max_notify);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::resetColorDetectionNotify() {
        bb::resetNotify(mTarget, mDevice, color_detection_notify);
    }

} /* namespace rvr */

#endif
