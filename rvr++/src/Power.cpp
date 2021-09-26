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
#include "Power.h"
namespace rvr {

    //======================================================================================================================
    // data access methods
    //----------------------------------------------------------------------------------------------------------------------
    ResultUInt16 Power::batteryPercent() const noexcept {
        return ResultUInt16 { static_cast<uint16_t>(mBlackboard.byteValue(mTarget, mDevice, get_battery_percentage).get_or()) };
    }
//----------------------------------------------------------------------------------------------------------------------
    ResultFloat Power::motorCurrent(MotorSide const ms) const noexcept {
        return mBlackboard.floatValue(mAltTarget, mDevice, get_current_sense_amplifier_current, 0, ms);
    }
//----------------------------------------------------------------------------------------------------------------------
    ResultFloat Power::voltsCalibratedFiltered() const noexcept {
        return mBlackboard.floatValue(mTarget, mDevice, get_battery_voltage_in_volts, 0, CalibratedFiltered);
    }
//----------------------------------------------------------------------------------------------------------------------
    ResultFloat Power::voltsCalibratedUnfiltered() const noexcept {
        return mBlackboard.floatValue(mTarget, mDevice, get_battery_voltage_in_volts, 0, CalibratedUnfiltered);
    }
//----------------------------------------------------------------------------------------------------------------------
    ResultFloat Power::voltsUncalibratedUnfiltered() const noexcept {
        return mBlackboard.floatValue(mTarget, mDevice, get_battery_voltage_in_volts, 0, UncalibratedUnfiltered);
    }
//----------------------------------------------------------------------------------------------------------------------
    Result<Power::BatteryVoltState> Power::voltState() const noexcept {
        return Result<Power::BatteryVoltState> { //
        static_cast<BatteryVoltState>(mBlackboard.byteValue(mTarget, mDevice, get_battery_voltage_state).get_or()) };
    }
//----------------------------------------------------------------------------------------------------------------------
    ResultString Power::voltStateText() const noexcept {
        static char_ptr state[4] { "unknown", "ok", "low", "critical" };
        return ResultString { state[mBlackboard.byteValue(mTarget, mDevice, get_battery_voltage_state).get_or()] };
    }
//----------------------------------------------------------------------------------------------------------------------
    ResultFloat Power::voltThresholdCritical() const noexcept {
        return mBlackboard.floatValue(mTarget, mDevice, get_battery_voltage_state_thresholds);
    }
//----------------------------------------------------------------------------------------------------------------------
    ResultFloat Power::voltThresholdLow() const noexcept {
        return mBlackboard.floatValue(mTarget, mDevice, get_battery_voltage_state_thresholds, 1);
    }
//----------------------------------------------------------------------------------------------------------------------
    ResultFloat Power::voltThresholdHysteresis() const noexcept {
        return mBlackboard.floatValue(mTarget, mDevice, get_battery_voltage_state_thresholds, 2);
    }
//----------------------------------------------------------------------------------------------------------------------
    ResultBool Power::isBatteryStateChangeEnabled() const noexcept {
        return mBlackboard.getNotify(mTarget, mDevice, enable_battery_voltage_state_change_notify);
    }
//----------------------------------------------------------------------------------------------------------------------
    ResultBool Power::isDidSleepNotify() const noexcept {
        return mBlackboard.notifyState(mTarget, mDevice, did_sleep_notify);
    }
//----------------------------------------------------------------------------------------------------------------------
    ResultBool Power::isWakeNotify() const noexcept {
        return mBlackboard.notifyState(mTarget, mDevice, system_awake_notify);
    }
//----------------------------------------------------------------------------------------------------------------------
    void Power::resetWakeNotify() const noexcept {
        mBlackboard.resetNotify(mTarget, mDevice, system_awake_notify);
    }
}

