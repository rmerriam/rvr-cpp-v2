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
#include "Power.h"
namespace rvr {

    //======================================================================================================================
    // data access methods
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<int> Power::batteryPercent() {
        return mBlackboard.byteValue(mTarget, mDevice, get_battery_percentage);
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<float> Power::motorCurrent(MotorSide const ms) {
        return mBlackboard.floatValue(mAltTarget, mDevice, get_current_sense_amplifier_current, 0, ms);
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<float> Power::voltsCalibratedFiltered() {
        return mBlackboard.floatValue(mTarget, mDevice, get_battery_voltage_in_volts, 0, CalibratedFiltered);
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<float> Power::voltsCalibratedUnfiltered() {
        return mBlackboard.floatValue(mTarget, mDevice, get_battery_voltage_in_volts, 0, CalibratedUnfiltered);
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<float> Power::voltsUncalibratedUnfiltered() {
        return mBlackboard.floatValue(mTarget, mDevice, get_battery_voltage_in_volts, 0, UncalibratedUnfiltered);
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<Power::BatteryVoltState> Power::voltState() {
        return static_cast<BatteryVoltState>(mBlackboard.byteValue(mTarget, mDevice, get_battery_voltage_state).value());
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<std::string> Power::voltStateText() {
        static char_ptr state[4] { "unknown", "ok", "low", "critical" };
        return state[mBlackboard.byteValue(mTarget, mDevice, get_battery_voltage_state).value()];
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<float> Power::voltThresholdCritical() {
        return mBlackboard.floatValue(mTarget, mDevice, get_battery_voltage_state_thresholds);
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<float> Power::voltThresholdLow() {
        return mBlackboard.floatValue(mTarget, mDevice, get_battery_voltage_state_thresholds, 1);
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<float> Power::voltThresholdHysteresis() {
        return mBlackboard.floatValue(mTarget, mDevice, get_battery_voltage_state_thresholds, 2);
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<bool> Power::isBatteryStateChangeEnabled() {
        return mBlackboard.getNotify(mTarget, mDevice, enable_battery_voltage_state_change_notify);
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<bool> Power::isDidSleepNotify() {
        return mBlackboard.notifyState(mTarget, mDevice, did_sleep_notify);
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<bool> Power::isWakeNotify() {
        return mBlackboard.notifyState(mTarget, mDevice, system_awake_notify);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Power::resetWakeNotify() {
        mBlackboard.resetNotify(mTarget, mDevice, system_awake_notify);
    }
}

