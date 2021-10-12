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
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//======================================================================================================================
//
//     Author: rmerriam
//
//     Created: May 29, 2021
//
//======================================================================================================================
#include "SensorsDirect.h"

namespace rvr {
    //----------------------------------------------------------------------------------------------------------------------
    Result<ColorData> SensorsDirect::currentRGBValues() const noexcept {
        auto const msg { mBlackboard.entryValue(mAltTarget, mDevice, get_rgbc_sensor_values) };
        Result<ColorData> res;

        if ( !msg.empty()) {
            PayloadDecode<uint16_t, uint16_t, uint16_t, uint16_t> payload(msg);

            res = ColorData { //
            payload.get<0>(), //
                payload.get<1>(), //
                payload.get<2>(), //
                payload.get<3>(), //
            };
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    Result<ColorDetection> SensorsDirect::colorDetectionValues() const noexcept {
        auto const& msg { mBlackboard.entryValue(mAltTarget, mDevice, color_detection_notify) };
        Result<ColorDetection> res;

        if ( !msg.empty()) {
            PayloadDecode<uint8_t, uint8_t, uint8_t, uint8_t, uint8_t> payload(msg);

            res = ColorDetection { //
            payload.get<0>(), //
                payload.get<1>(), //
                payload.get<2>(), //
                payload.get<3>(), //
                payload.get<4>(), //
            };
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    ResultBool SensorsDirect::isColorDetectionEnabled() const noexcept {
        auto const msg { mBlackboard.entryValue(mAltTarget, mDevice, enable_color_detection) };
        return decode_type<bool>(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    ResultBool SensorsDirect::isColorDetectionNotifyEnabled() const noexcept {
        auto const msg { mBlackboard.entryValue(mAltTarget, mDevice, enable_color_detection_notify) };
        return decode_type<bool>(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    ResultBool SensorsDirect::isGyroMaxNotifyEnabled() const noexcept {
        RvrMsgView msg { mBlackboard.entryValue(mTarget, mDevice, enable_gyro_max_notify) };
        return decode_type<bool>(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    ResultBool SensorsDirect::isMagnetometerCalibrationDone() const noexcept {
        auto const msg { mBlackboard.entryValue(mTarget, mDevice, magnetometer_calibration_complete_notify) };
        return decode_type<bool>(msg);
    }
//----------------------------------------------------------------------------------------------------------------------
    ResultInt16 SensorsDirect::magnetometerCalibrationYaw() const noexcept {
        auto const msg { mBlackboard.entryValue(mTarget, mDevice, magnetometer_calibration_complete_notify) };
        return decode_type<int16_t>(msg);
    }
//----------------------------------------------------------------------------------------------------------------------
    Result<MagnetometerData> SensorsDirect::magnetometerData() const noexcept {
        auto const& msg { mBlackboard.entryValue(mTarget, mDevice, get_magnetometer_reading) };
        Result<MagnetometerData> res;

        if ( !msg.empty()) {
            PayloadDecode<float, float, float> payload(msg);
            res = MagnetometerData { //
            payload.get<0>(), //
                payload.get<1>(), //
                payload.get<2>(), //

            };
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    Result<ThermalProtection> SensorsDirect::thermalProtectionValues() const noexcept {
        auto const& msg { mBlackboard.entryValue(mTarget, mDevice, get_motor_thermal_protection_status) };
        Result<ThermalProtection> res;

        if ( !msg.empty()) {
            PayloadDecode<float, uint8_t, float, uint8_t> payload(msg);

            res = ThermalProtection { //
            payload.get<0>(), //
                static_cast<rvr::ThermalStatus>(payload.get<1>()), //
                payload.get<2>(), //
                static_cast<rvr::ThermalStatus>(payload.get<3>()), //
            };
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    ResultBool SensorsDirect::isThermalProtectionNotifyEnabled() const noexcept {
        RvrMsgView msg { mBlackboard.entryValue(mTarget, mDevice, enable_motor_thermal_protection_status_notify) };
        return decode_type<bool>(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    ResultFloat SensorsDirect::ambientLight() const noexcept {
        auto const& msg { mBlackboard.entryValue(mAltTarget, mDevice, get_ambient_light_sensor_value) };
        return decode_type<bool>(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    rvr::Result<EncoderData> SensorsDirect::encoderCounts() const noexcept {
        auto const& msg { mBlackboard.entryValue(mTarget, mDevice, get_encoder_counts) };
        Result<EncoderData> res;

        if ( !msg.empty()) {
            PayloadDecode<uint32_t, uint32_t> payload(msg);
            res = EncoderData { //
            payload.get<0>(), //
                payload.get<1>(), //
            };
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    ResultFloat SensorsDirect::leftMotorTemp() const noexcept {
        auto const& msg { mBlackboard.entryValue(mTarget, mDevice, get_temperature,
                                                 (uint8_t)TemperatureIndexes::left_motor_temperature) };
        return decode_type<float>(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    ResultFloat SensorsDirect::rightMotorTemp() const noexcept {
        auto const& msg { mBlackboard.entryValue(mTarget, mDevice, get_temperature,
                                                 (uint8_t)TemperatureIndexes::right_motor_temperature) };
        return decode_type<float>(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    ResultFloat SensorsDirect::nordicTemp() const noexcept {
        auto const& msg { mBlackboard.entryValue(mTarget, mDevice, get_temperature,
                                                 (uint8_t)TemperatureIndexes::nordic_die_temperature) };
        return decode_type<float>(msg);
    }

}   // end of rvr
