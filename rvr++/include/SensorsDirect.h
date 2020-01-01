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
#include <Request.h>
#include <SendPacket.h>
#include "enum.h"
#include "Blackboard.h"
//----------------------------------------------------------------------------------------------------------------------
/*
 * For sensor ranges see: ~/devr/nodejs/src/modules/controls/v1.0/sensor-control.ts
 *                      https://sdk.sphero.com/docs/general_documentation/sensors/
 */
//----------------------------------------------------------------------------------------------------------------------
namespace rvr {

    class Blackboard;

    class SensorsDirect : protected Request {

    public:
        SensorsDirect(Blackboard& bb, SendPacket& req) :
            Request { bb, Devices::sensors, req, bluetoothSOC } {
        }

        SensorsDirect(SensorsDirect const& other) = delete;
        SensorsDirect(SensorsDirect&& other) = delete;
        SensorsDirect& operator=(SensorsDirect const& other) = delete;

        void enableGyroMaxNotify(CommandResponse const want_resp = resp_yes) const;
        void disableGyroMaxNotify(CommandResponse const want_resp = resp_yes) const;
        // Gyro Max Notify
        void resetMaxGyroNotify() const;    // not part of API

        void resetLocatorXY(CommandResponse const want_resp = resp_on_error) const;
        void setLocatorFlags(bool const flag, CommandResponse const want_resp = resp_on_error) const;

        //  Get Bot To Bot Infrared Readings

        void getRgbcSensorValue(CommandResponse const want_resp = resp_yes) const;

        //  Start Robot To Robot Infrared Broadcasting
        //  Start Robot To Robot Infrared Following
        //  Stop Robot To Robot Infrared Broadcasting
        //  Robot To Robot Infrared Message Received Notify

        void getAmbienLightSensorValue(CommandResponse const want_resp = resp_yes) const;

        //  Stop Robot To Robot Infrared Following
        //  Start Robot To Robot Infrared Evading
        //  Stop Robot To Robot Infrared Evading

        void enableColorDetectionNotify(bool const enable, uint16_t const timer, uint8_t const confidence,
            CommandResponse const want_resp = resp_yes) const;
        //  Color Detection Notify
        void resetColorDetectionNotify() const;     // not part of API

        void getCurrentDectectedColor(CommandResponse const want_resp = resp_yes) const;
        void enableColorDetection(CommandResponse const want_resp = resp_yes) const;
        void disableColorDetection(CommandResponse const want_resp = resp_yes) const;

        //* implemented in SensorsStream class
        //*     Configure Streaming Service
        //*     Start Streaming Service
        //*     Stop Streaming Service
        //*     Clear Streaming Service
        //*     Streaming Service Data Notify

        //  Enable Robot Infrared Message Notify
        //  Send Infrared Message

        void getRightMotorTemp(CommandResponse const want_resp = resp_yes) const;
        void getLeftMotorTemp(CommandResponse const want_resp = resp_yes) const;
        void getThermalProtectionStatus(CommandResponse const want_resp = resp_yes) const;
        void enableThermalProtectionNotify(CommandResponse const want_resp = resp_yes) const;
        void disableThermalProtectionNotify(CommandResponse const want_resp = resp_yes) const;

        //  Methods to access data

        std::optional<bool> isGyroMaxNotifyEnabled() const;
        std::optional<bool> isThermalProtectionNotifyEnabled() const;
        std::optional<bool> isColorDetectionEnabled() const;
        std::optional<bool> isColorDetectionNotifyEnabled() const;

        std::optional<float> ambient() const;
        std::optional<float> leftMotorTemp() const;
        std::optional<float> rightMotorTemp() const;
        std::optional<ColorData> currentRGBValues();
        std::optional<ColorDetection> colorDetectionValues();
        std::optional<ThermalProtection> thermalProtectionValues();

    private:
        //----------------------------------------------------------------------------------------------------------------------

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
            get_temperature = 0x4A,
            get_motor_thermal_protection_status = 0x4B,
            enable_motor_thermal_protection_status_notify = 0x4C,
            motor_thermal_protection_status_notify = 0x4D,
        };
    };
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::setLocatorFlags(bool const flag, CommandResponse const want_resp) const {
        reqByte(set_locator_flags, flag, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::resetLocatorXY(CommandResponse const want_resp) const {
        basic(reset_locator_x_and_y, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::enableGyroMaxNotify(CommandResponse const want_resp) const {
        cmdEnable(enable_gyro_max_notify, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::disableGyroMaxNotify(CommandResponse const want_resp) const {
        cmdDisable(enable_gyro_max_notify, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::getLeftMotorTemp(CommandResponse const want_resp) const {
        byteId(get_temperature, 0x04, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::getRightMotorTemp(CommandResponse const want_resp) const {
        byteId(get_temperature, 0x05, want_resp);
    }
    inline void SensorsDirect::getRgbcSensorValue(CommandResponse const want_resp) const {
        basicAlt(get_rgbc_sensor_values, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::getThermalProtectionStatus(CommandResponse const want_resp) const {
        basic(get_motor_thermal_protection_status, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::enableThermalProtectionNotify(CommandResponse const want_resp) const {
        cmdEnable(enable_motor_thermal_protection_status_notify, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::disableThermalProtectionNotify(CommandResponse const want_resp) const {
        cmdDisable(enable_motor_thermal_protection_status_notify, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::getAmbienLightSensorValue(CommandResponse const want_resp) const {
        basicAlt(get_ambient_light_sensor_value, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::getCurrentDectectedColor(CommandResponse const want_resp) const {
        basicAlt(get_current_detected_color_reading, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::enableColorDetection(CommandResponse const want_resp) const {
        cmdEnableAlt(enable_color_detection, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::disableColorDetection(CommandResponse const want_resp) const {
        cmdDisableAlt(enable_color_detection, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::enableColorDetectionNotify(bool const enable, uint16_t const timer, uint8_t const confidence,
        CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mAltTarget, mDevice,
                     enable_color_detection_notify, //
                     static_cast<uint8_t>(enable ? 0x20 : 0x21), enable, static_cast<uint8_t>(timer >> 8),
                     static_cast<uint8_t>(timer & 0xFF), confidence };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline std::optional<bool> SensorsDirect::isGyroMaxNotifyEnabled() const {
        return mBlackboard.getNotify(mTarget, mDevice, enable_gyro_max_notify);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline std::optional<bool> SensorsDirect::isThermalProtectionNotifyEnabled() const {
        return mBlackboard.getNotify(mTarget, mDevice, enable_motor_thermal_protection_status_notify);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline std::optional<bool> SensorsDirect::isColorDetectionEnabled() const {
        return mBlackboard.getNotify(mAltTarget, mDevice, enable_color_detection);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline std::optional<bool> SensorsDirect::isColorDetectionNotifyEnabled() const {
        return mBlackboard.getNotify(mAltTarget, mDevice, enable_color_detection_notify);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline std::optional<float> SensorsDirect::ambient() const {
        return mBlackboard.floatValue(mAltTarget, mDevice, get_ambient_light_sensor_value);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline std::optional<float> SensorsDirect::leftMotorTemp() const {
        return mBlackboard.floatValue(mTarget, mDevice, get_temperature, 0, 4);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline std::optional<float> SensorsDirect::rightMotorTemp() const {
        return mBlackboard.floatValue(mTarget, mDevice, get_temperature, 0, 5);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::resetMaxGyroNotify() const {
        mBlackboard.resetNotify(mAltTarget, mDevice, gyro_max_notify);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::resetColorDetectionNotify() const {
        mBlackboard.resetNotify(mTarget, mDevice, color_detection_notify);
    }
}

/* namespace rvr */

#endif
