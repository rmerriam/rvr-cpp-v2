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
#include "SensorsDirect.h"

namespace rvr {

    //----------------------------------------------------------------------------------------------------------------------
    std::optional<ColorData> SensorsDirect::currentRGBValues() {
        auto const msg { mBlackboard.msgValue(mAltTarget, mDevice, get_rgbc_sensor_values) };
        if (msg) {

            return ColorData { mBlackboard.uintValue(mAltTarget, mDevice, get_rgbc_sensor_values, 0).value(), //
            mBlackboard.uintValue(mAltTarget, mDevice, get_rgbc_sensor_values, 1).value(), //
            mBlackboard.uintValue(mAltTarget, mDevice, get_rgbc_sensor_values, 2).value(), //
            mBlackboard.uintValue(mAltTarget, mDevice, get_rgbc_sensor_values, 2.5).value() //
            };
        }
        return {};
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<ColorDetection> SensorsDirect::colorDetectionValues() {
        auto const msg { mBlackboard.msgValue(mAltTarget, mDevice, color_detection_notify) };
        if (msg) {
            return ColorDetection { //
            static_cast<uint8_t>(msg.value()[1]), //
            static_cast<uint8_t>(msg.value()[2]), //
            static_cast<uint8_t>(msg.value()[3]), //
            static_cast<uint8_t>(msg.value()[4]), //
            static_cast<uint8_t>(msg.value()[5]) //
            };
        }
        return {};
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<ThermalProtection> SensorsDirect::thermalProtectionValues() {
        auto const& msg { mBlackboard.msgValue(mTarget, mDevice, get_motor_thermal_protection_status) };
        if (msg) {
            float lm { mBlackboard.floatValue(mTarget, mDevice, get_motor_thermal_protection_status, 0).value() };
            float rm { mBlackboard.floatValue(mTarget, mDevice, get_motor_thermal_protection_status, 1.25).value() };
            return ThermalProtection { lm, msg.value()[4],  //
                                       rm, msg.value()[9]   //
            };
        }
        return {};
    }
}
