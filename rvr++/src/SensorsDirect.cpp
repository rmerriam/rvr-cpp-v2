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
    std::optional<ColorData> SensorsDirect::currentRGBValues() {
        auto msg { mBlackboard.msgValue(mAltTarget, mDevice, get_rgbc_sensor_values) };

        if (msg) {
            return ColorData { static_cast<uint16_t>(mBlackboard.uintConvert(msg.value().begin(), 2)), //
                static_cast<uint16_t>(mBlackboard.uintConvert(msg.value().begin() + 2, 2)),  //
                static_cast<uint16_t>(mBlackboard.uintConvert(msg.value().begin() + 4, 2)),  //

                static_cast<uint16_t>(mBlackboard.uintConvert(msg.value().begin() + 6, 2)),  //
            };
        }
        return {};
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<ColorDetection> SensorsDirect::colorDetectionValues() {
        auto const msg { mBlackboard.msgValue(mAltTarget, mDevice, color_detection_notify) };
        if (msg) {
            return ColorDetection { //
            static_cast<uint8_t>(msg.value()[0]), //
                static_cast<uint8_t>(msg.value()[1]), //
                static_cast<uint8_t>(msg.value()[2]), //
                static_cast<uint8_t>(msg.value()[3]), //
                static_cast<uint8_t>(msg.value()[4]), //
            };
        }
        return {};
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<ThermalProtection> SensorsDirect::thermalProtectionValues() {
        auto const& msg { mBlackboard.msgValue(mTarget, mDevice, get_motor_thermal_protection_status) };
        if (msg) {
            auto const& begin { msg.value().begin() };
            auto lm { mBlackboard.floatConvert(begin) };
            auto rm { mBlackboard.floatConvert(begin + 5) };
            return ThermalProtection { lm, (rvr::ThermalStatus)msg.value()[4], rm, (rvr::ThermalStatus)msg.value()[9] };
        }
        return {};
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<bool> SensorsDirect::isMagnetometerCalibrationDone() const {
        auto const msg { mBlackboard.msgValue(mTarget, mDevice, magnetometer_calibration_complete_notify) };
        if (msg) {
            return { //
                msg.value()[0],
            };
        }
        return {};
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<int16_t> SensorsDirect::magnetometerCalibration() const {
        auto const& msg { mBlackboard.msgValue(mTarget, mDevice, magnetometer_calibration_complete_notify) };
        if (msg) {
            return {
                static_cast<int16_t>(mBlackboard.uintConvert(msg.value().begin() + 1, 2))
            };
        }

        return {};
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<MagnetometerData> SensorsDirect::magnetometerData() {
        auto x { mBlackboard.floatValue(mTarget, mDevice, get_magnetometer_reading, 0) };
        if (x) {
            auto y { mBlackboard.floatValue(mTarget, mDevice, get_magnetometer_reading, 1) };
            auto z { mBlackboard.floatValue(mTarget, mDevice, get_magnetometer_reading, 2) };

            return MagnetometerData { x.value_or(0), y.value_or(0), z.value_or(0) };
        }
        return {};
    }
}   // end of rvr
