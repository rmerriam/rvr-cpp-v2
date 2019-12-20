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
    ColorData SensorsDirect::currentRGBValues() {

        return { //
            mBlackboard.uintValue(mAltTarget, mDevice, get_rgbc_sensor_values, 0),//
            mBlackboard.uintValue(mAltTarget, mDevice, get_rgbc_sensor_values, 1),//
            mBlackboard.uintValue(mAltTarget, mDevice, get_rgbc_sensor_values, 2),//
            mBlackboard.uintValue(mAltTarget, mDevice, get_rgbc_sensor_values, 2.5)//
        };
    }

    //----------------------------------------------------------------------------------------------------------------------
    ColorDetection SensorsDirect::colorDetectionValues() {
        rvr::RvrMsg const msg { mBlackboard.msgValue(mTarget, mDevice, color_detection_notify) };
        return { //
//            static_cast<uint8_t>(msg[1]),//
            msg[1],//
            static_cast<uint8_t>(msg[2]),//
            static_cast<uint8_t>(msg[3]),//
            static_cast<uint8_t>(msg[4]),//
            static_cast<uint8_t>(msg[5])//
        };
    }

    //----------------------------------------------------------------------------------------------------------------------
    ThermalProtection SensorsDirect::thermalProtectionValues() {
        rvr::RvrMsg const& msg { mBlackboard.msgValue(mTarget, mDevice, get_motor_thermal_protection_status) };
        return { //
            mBlackboard.floatValue(mTarget, mDevice, get_motor_thermal_protection_status, 0),//
            msg[4],
            mBlackboard.floatValue(mTarget, mDevice, get_motor_thermal_protection_status, 1.25),
            msg[9]
        };
    }

}
