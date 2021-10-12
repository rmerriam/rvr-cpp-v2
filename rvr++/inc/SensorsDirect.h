#pragma once
//======================================================================================================================
// 2021 Copyright Mystic Lake Software
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
// along with this program.  If not, see <http:                //www.gnu.org/licenses/>.
//======================================================================================================================
//
//     Author: rmerriam
//
//     Created: May 29, 2021
//
//======================================================================================================================
#include <Blackboard.h>
#include <enum.h>
#include <PayloadDecode.h>
#include <Request.h>
#include <SendPacket.h>
//----------------------------------------------------------------------------------------------------------------------
/*
 * For sensor ranges see:
 * ~/devr/nodejs/src/modules/controls/v1.0/sensor-control.ts
 *                      https:                //sdk.sphero.com/docs/general_documentation/sensors/
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

        void enableGyroMaxNotify(CommandResponse const want_resp = CommandResponse::resp_yes) const;
        void disableGyroMaxNotify(CommandResponse const want_resp = CommandResponse::resp_yes) const;
        void resetMaxGyroNotify() const;                 // not part of API

        enum struct LocatorFlagsBitmask : uint8_t {
            none = 0, // reset on yaw reset
            auto_calibrate = 1, // retain locator from startup
        };

        void resetLocatorXY(CommandResponse const want_resp = CommandResponse::resp_on_error) const;
        void setLocatorFlags(LocatorFlagsBitmask const flag,
            CommandResponse const want_resp = CommandResponse::resp_on_error) const;

        void getRgbcSensorValue(CommandResponse const want_resp = CommandResponse::resp_yes) const;
        void getEncoderCounts(CommandResponse const want_resp = CommandResponse::resp_yes) const;

        //  Get Bot To Bot Infrared Readings
        //  Start Robot To Robot Infrared Broadcasting
        //  Start Robot To Robot Infrared Following
        //  Stop Robot To Robot Infrared Broadcasting
        //  Robot To Robot Infrared Message Received Notify

        void getAmbientLightSensorValue(CommandResponse const want_resp = CommandResponse::resp_yes) const;

        //  Stop Robot To Robot Infrared Following
        //  Start Robot To Robot Infrared Evading
        //  Stop Robot To Robot Infrared Evading

        //----------------------------------------------------------------------------------------------------------------------
        void enableColorDetectionNotify(bool const enable, uint16_t const timer, uint8_t const confidence,
            CommandResponse const want_resp = CommandResponse::resp_yes) const;
        //  Color Detection Notify
        void resetColorDetectionNotify() const;                 // not part of API

        void getCurrentDectectedColor(CommandResponse const want_resp = CommandResponse::resp_yes) const;
        void enableColorDetection(CommandResponse const want_resp = CommandResponse::resp_yes) const;
        void disableColorDetection(CommandResponse const want_resp = CommandResponse::resp_yes) const;

        //----------------------------------------------------------------------------------------------------------------------
        //* implemented in SensorsStream class
        //*     Configure Streaming Service
        //*     Start Streaming Service
        //*     Stop Streaming Service
        //*     Clear Streaming Service
        //*     Streaming Service Data Notify

        //  Enable Robot Infrared Message Notify
        //  Send Infrared Message

        //----------------------------------------------------------------------------------------------------------------------
        void getRightMotorTemp(CommandResponse const want_resp = CommandResponse::resp_yes) const;
        void getLeftMotorTemp(CommandResponse const want_resp = CommandResponse::resp_yes) const;
        void getNordicTemp(CommandResponse const want_resp = CommandResponse::resp_yes) const;
        void getThermalProtectionStatus(CommandResponse const want_resp = CommandResponse::resp_yes) const;
        void enableThermalProtectionNotify(CommandResponse const want_resp = CommandResponse::resp_yes) const;
        void disableThermalProtectionNotify(CommandResponse const want_resp = CommandResponse::resp_yes) const;

        void calibrateMagnetometer(CommandResponse const want_resp = CommandResponse::resp_yes) const;
        void getMagnetometerData(CommandResponse const want_resp = CommandResponse::resp_yes) const;
        void resetMagnetometerCalibration();

        //  Methods to access data

        [[nodiscard]] ResultBool isColorDetectionEnabled() const noexcept;
        [[nodiscard]] ResultBool isColorDetectionNotifyEnabled() const noexcept;
        [[nodiscard]] ResultBool isGyroMaxNotifyEnabled() const noexcept;
        [[nodiscard]] ResultBool isThermalProtectionNotifyEnabled() const noexcept;

        [[nodiscard]] ResultBool isMagnetometerCalibrationDone() const noexcept;

        [[nodiscard]] ResultFloat ambientLight() const noexcept;
        [[nodiscard]] Result<ColorDetection> colorDetectionValues() const noexcept;
        [[nodiscard]] Result<ColorData> currentRGBValues() const noexcept;
        [[nodiscard]] Result<EncoderData> encoderCounts() const noexcept;
        ;
        [[nodiscard]] ResultFloat leftMotorTemp() const noexcept;
        [[nodiscard]] ResultFloat rightMotorTemp() const noexcept;
        [[nodiscard]] Result<MagnetometerData> magnetometerData() const noexcept;
        [[nodiscard]] ResultInt16 magnetometerCalibrationYaw() const noexcept;
        [[nodiscard]] ResultFloat nordicTemp() const noexcept;
        [[nodiscard]] Result<ThermalProtection> thermalProtectionValues() const noexcept;

    private:
        //----------------------------------------------------------------------------------------------------------------------
        enum struct IrLocationMake : uint32_t {
            none = 0, front_left = 0x000000FF, front_right = 0x0000FF00, back_right = 0x00FF0000, back_left = 0xFF000000,
        };

        enum Cmd : uint8_t {
            enable_gyro_max_notify = 0x0F,                 //
            gyro_max_notify = 0x10,                        //
            //
            reset_locator_x_and_y = 0x13,                 //
            set_locator_flags = 0x17,                     //
            //
            get_bot_to_bot_infrared_readings = 0x22,                 //
            //
            get_rgbc_sensor_values = 0x23,                 //
            //
            magnetometer_calibrate_to_north = 0x25,                 //
            //
            start_robot_to_robot_infrared_broadcasting = 0x27,                      //
            start_robot_to_robot_infrared_following = 0x28,                         //
            stop_robot_to_robot_infrared_broadcasting = 0x29,                       //
            robot_to_robot_infrared_message_received_notify = 0x2C,                 //
            //
            get_ambient_light_sensor_value = 0x30,                 //
            //
            stop_robot_to_robot_infrared_following = 0x32,                 //
            start_robot_to_robot_infrared_evading = 0x33,                  //
            stop_robot_to_robot_infrared_evading = 0x34,                   //
            //
            enable_color_detection_notify = 0x35,                      //
            color_detection_notify = 0x36,                             //
            get_current_detected_color_reading = 0x37,                 //
            enable_color_detection = 0x38,                             //
            //
            configure_streaming_service = 0x39,                   //
            start_streaming_service = 0x3A,                       //
            stop_streaming_service = 0x3B,                        //
            clear_streaming_service = 0x3C,                       //
            streaming_service_data_notify = 0x3D,                 //
            //
            enable_robot_infrared_message_notify = 0x3E,                 //
            send_infrared_message = 0x3F,                                //
            //
            get_temperature = 0x4A,                                               //
            get_motor_thermal_protection_status = 0x4B,                           //
            enable_motor_thermal_protection_status_notify = 0x4C,                 //
            motor_thermal_protection_status_notify = 0x4D,                        //
            //
            magnetometer_calibration_complete_notify = 0x51,                 //
            get_magnetometer_reading = 0x52,                                 //
            //
            get_encoder_counts = 0x53,                                        //
            disable_notifications_and_active_commands = 0x54,                 //
        };
    };
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::setLocatorFlags(LocatorFlagsBitmask const flag, CommandResponse const want_resp) const {
        reqByte(set_locator_flags, static_cast<uint8_t>(flag), want_resp);
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
    inline void SensorsDirect::getRightMotorTemp(CommandResponse const want_resp) const {
        byteId(get_temperature, (uint8_t)TemperatureIndexes::right_motor_temperature, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::getLeftMotorTemp(CommandResponse const want_resp) const {
        byteId(get_temperature, (uint8_t)TemperatureIndexes::left_motor_temperature, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::getNordicTemp(CommandResponse const want_resp) const {
        byteId(get_temperature, (uint8_t)TemperatureIndexes::nordic_die_temperature, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::getRgbcSensorValue(CommandResponse const want_resp) const {
        basicAlt(get_rgbc_sensor_values, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::getEncoderCounts(CommandResponse const want_resp) const {
        basic(get_encoder_counts, want_resp);
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
    inline void SensorsDirect::getAmbientLightSensorValue(CommandResponse const want_resp) const {
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
        RvrMsg msg { buildFlags(want_resp), mAltTarget, mDevice, enable_color_detection_notify,                 //
        static_cast<uint8_t>(enable ? 0x20 : 0x21), enable, static_cast<uint8_t>(timer >> 8), static_cast<uint8_t>(timer
            & 0xFF), confidence };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::resetMaxGyroNotify() const {
        mBlackboard.resetNotify(mAltTarget, mDevice, gyro_max_notify);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::resetColorDetectionNotify() const {
        mBlackboard.resetNotify(mTarget, mDevice, color_detection_notify);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::calibrateMagnetometer(CommandResponse const want_resp) const {
        basic(magnetometer_calibrate_to_north, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::getMagnetometerData(CommandResponse const want_resp) const {
        basic(get_magnetometer_reading, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsDirect::resetMagnetometerCalibration() {
        mBlackboard.resetNotify(mTarget, mDevice, magnetometer_calibration_complete_notify);
    }

}                 // namespace rvr
