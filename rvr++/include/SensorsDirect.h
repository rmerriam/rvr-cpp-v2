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

    class SensorsDirect : protected CommandBase {

    public:
        enum VoltageType : uint8_t {
            CalibratedFiltered = 0, CalibratedUnfiltered = 1, UncalibratedUnfiltered = 2,
        };

        SensorsDirect(Request& req) :
            CommandBase { Devices::sensors, req, bluetoothSOC } {
        }

        SensorsDirect(const SensorsDirect& other) = delete;
        SensorsDirect(SensorsDirect&& other) = delete;
        SensorsDirect& operator=(const SensorsDirect& other) = delete;

        void enableGyroMaxNotify(const CommandResponse want_resp = resp_on_error);
        void disableGyroMaxNotify(const CommandResponse want_resp = resp_on_error);

        void getRightMotorTemp(const CommandResponse want_resp = resp_on_error);
        void getLeftMotorTemp(const CommandResponse want_resp = resp_on_error);

        void getThermalProtectionStatus(const CommandResponse want_resp);
        void enableThermal(const CommandResponse want_resp = resp_on_error);
        void disableThermal(const CommandResponse want_resp = resp_on_error);

        void getAmbient(const CommandResponse want_resp = resp_on_error);

        void disableColorDetection(const CommandResponse want_resp);
        void enabeColorDetectionNotify(const bool enable, const uint16_t timer, const uint8_t confidence, const CommandResponse want_resp);
        void enableColorDetection(const CommandResponse want_resp);
        void getCurrentColor(const CommandResponse want_resp);

        //----------------------------------------------------------------------------------------------------------------------
        // 0x08 is the Nordic die temperature sensor

        static void motorTemperature(const bb::key_t key, MsgArray::iterator begin, MsgArray::iterator end) {
            auto m_key = static_cast<bb::key_t>(key | (( *begin - 4)));
            //            Blackboard::entryValue(m_key) = bb::float_convert(begin + 1, end);
            //            terr << code_loc << std::hex << m_key << mys::sp << MsgArray_cast<float>(Blackboard::entryValue(m_key));

            MsgArray msg { begin, end };
            Blackboard::entryValue(m_key) = msg;
            terr << code_loc << std::hex << m_key << mys::sp << msg;
        }
        //----------------------------------------------------------------------------------------------------------------------
        // Notification data converter
        static void rxColotNotify(const bb::key_t key, MsgArray::iterator begin, MsgArray::iterator end);
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
    inline void SensorsDirect::enableGyroMaxNotify(const CommandResponse want_resp) {
        cmd_enable_alt(enable_gyro_max_notify, true, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::disableGyroMaxNotify(const CommandResponse want_resp) {
        cmd_enable_alt(enable_gyro_max_notify, false, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::getRightMotorTemp(const CommandResponse want_resp) {
        MsgArray m { 4, 5 };
        cmd_data_alt(get_motor_temperature, m, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::getLeftMotorTemp(const CommandResponse want_resp) {
        cmd_byte_alt(get_motor_temperature, 0x04, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::getThermalProtectionStatus(const CommandResponse want_resp) {
        cmd_basic_alt(get_motor_thermal_protection_status, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::enableThermal(const CommandResponse want_resp) {
        cmd_enable_alt(enable_motor_thermal_protection_status_notify, true, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::disableThermal(const CommandResponse want_resp) {
        cmd_enable_alt(enable_motor_thermal_protection_status_notify, false, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::getAmbient(const CommandResponse want_resp) {
        cmd_basic(get_ambient_light_sensor_value, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::getCurrentColor(const CommandResponse want_resp) {
        cmd_basic(get_current_detected_color_reading, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::enableColorDetection(const CommandResponse want_resp) {
        cmd_enable(enable_color_detection, true, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::disableColorDetection(const CommandResponse want_resp) {
        cmd_enable(enable_color_detection, false, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::enabeColorDetectionNotify(const bool enable, const uint16_t timer, const uint8_t confidence,
        const CommandResponse want_resp) {
        MsgArray msg { buildFlags(want_resp), mTarget, mDevice, enable_color_detection_notify, sequence(), //
                       enable, static_cast<uint8_t>(timer >> 8), static_cast<uint8_t>(timer & 0xFF), confidence };
        mRequest.send(msg);
    }

    inline void SensorsDirect::rxColotNotify(const bb::key_t key, MsgArray::iterator begin, MsgArray::iterator end) {
        terr << code_loc << "notification: " << std::hex << key << mys::sp;
        std::copy(begin, end, std::ostream_iterator<int>(std::cerr, " "));
        const color_note &cn = *new ( &( *begin)) color_note;
        terr << code_loc << std::hex << //
             (int)cn.red << mys::sp //
             << (int)cn.green << mys::sp //
             << (int)cn.blue << mys::sp //
             << (int)cn.confidence << mys::sp //
             << (int)cn.classification;
    }

} /* namespace rvr */

#endif
