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
    int Power::batteryPercent() {
        return bb::byteValue(mTarget, mDevice, get_battery_percentage);
    }
    //----------------------------------------------------------------------------------------------------------------------
    float Power::motorCurrent(MotorSide const ms) {
        return bb::floatValue(mAltTarget, mDevice, get_current_sense_amplifier_current, ms);
    }
    //----------------------------------------------------------------------------------------------------------------------
    float Power::voltsCalibratedFiltered() {
        return bb::floatValue(mTarget, mDevice, get_battery_voltage_in_volts, 0, CalibratedFiltered);
    }
    //----------------------------------------------------------------------------------------------------------------------
    float Power::voltsCalibratedUnfiltered() {
        return bb::floatValue(mTarget, mDevice, get_battery_voltage_in_volts, 0, CalibratedUnfiltered);
    }
    //----------------------------------------------------------------------------------------------------------------------
    float Power::voltsUncalibratedUnfiltered() {
        return bb::floatValue(mTarget, mDevice, get_battery_voltage_in_volts, 0, UncalibratedUnfiltered);
    }
    //----------------------------------------------------------------------------------------------------------------------
    Power::BatteryVoltState Power::voltState() {
        return static_cast<BatteryVoltState>(bb::byteValue(mTarget, mDevice, get_battery_voltage_state));
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::string Power::voltStateText() {
        static char_ptr state[4] { "unknown", "ok", "low", "critical" };
        return state[bb::byteValue(mTarget, mDevice, get_battery_voltage_state)];
    }
    //----------------------------------------------------------------------------------------------------------------------
    float Power::voltThresholdCritical() {
        return bb::floatValue(mTarget, mDevice, get_battery_voltage_state_thresholds);
    }
    //----------------------------------------------------------------------------------------------------------------------
    float Power::voltThresholdLow() {
        return bb::floatValue(mTarget, mDevice, get_battery_voltage_state_thresholds, 1);
    }
    //----------------------------------------------------------------------------------------------------------------------
    float Power::voltThresholdHysteresis() {
        return bb::floatValue(mTarget, mDevice, get_battery_voltage_state_thresholds, 2);
    }
    //----------------------------------------------------------------------------------------------------------------------
    bool Power::isBatteryStateChangeEnabled() {
        return bb::getNotify(mTarget, mDevice, enable_battery_voltage_state_change_notify);
    }
    //----------------------------------------------------------------------------------------------------------------------
    bool Power::isDidSleepNotify() {
        return bb::notifyState(mTarget, mDevice, did_sleep_notify);
    }
    //----------------------------------------------------------------------------------------------------------------------
    bool Power::isWakeNotify() {
        return bb::notifyState(mTarget, mDevice, system_awake_notify);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Power::resetWakeNotify() {
        bb::resetNotify(mTarget, mDevice, system_awake_notify);
    }
}

