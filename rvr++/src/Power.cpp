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
        return bb::byteValue(mTarget, Devices::power, get_battery_percentage);
    }
    //----------------------------------------------------------------------------------------------------------------------
    float Power::motorCurrent(MotorSide const ms) {
        return bb::floatValue(mTarget, Devices::power, get_current_sense_amplifier_current, ms);
    }
    //----------------------------------------------------------------------------------------------------------------------
    float Power::voltsCalibratedFiltered() {
        return bb::floatValue(mTarget, Devices::power, get_battery_voltage_in_volts, 0, CalibratedFiltered);
    }
    //----------------------------------------------------------------------------------------------------------------------
    float Power::voltsCalibratedUnfiltered() {
        return bb::floatValue(mTarget, Devices::power, get_battery_voltage_in_volts, 0, CalibratedUnfiltered);
    }
    //----------------------------------------------------------------------------------------------------------------------
    float Power::voltsUncalibratedUnfiltered() {
        return bb::floatValue(mTarget, Devices::power, get_battery_voltage_in_volts, 0, UncalibratedUnfiltered);
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::string Power::voltState() {
        static char_ptr state[4] { "unknown", "ok", "low", "critical" };
        return state[bb::byteValue(mTarget, Devices::power, get_battery_voltage_state)];
    }
    //----------------------------------------------------------------------------------------------------------------------
    float Power::voltThresholdCritical() {
        return bb::floatValue(mTarget, Devices::power, get_battery_voltage_state_thresholds);
    }
    //----------------------------------------------------------------------------------------------------------------------
    float Power::voltThresholdLow() {
        return bb::floatValue(mTarget, Devices::power, get_battery_voltage_state_thresholds, 1);
    }
    //----------------------------------------------------------------------------------------------------------------------
    float Power::voltThresholdHysteresis() {
        return bb::floatValue(mTarget, Devices::power, get_battery_voltage_state_thresholds, 2);
    }
    //----------------------------------------------------------------------------------------------------------------------
    bool Power::checkBatteryStateChange() {
        return bb::boolValue(mTarget, Devices::power, enable_battery_voltage_state_change_notify);
    }
    //----------------------------------------------------------------------------------------------------------------------
    bool Power::isSleepNotify() {
        return bb::boolValue(mTarget, Devices::power, did_sleep_notify);
    }
    //----------------------------------------------------------------------------------------------------------------------
    bool Power::isWakeNotify() {
        return bb::boolValue(mTarget, Devices::power, system_awake_notify);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Power::resetWakeNotify() {
        bb::resetNotify(mTarget, Devices::power, system_awake_notify);
    }

//======================================================================================================================
// Deserialization methods
//    void Power::rxBatVoltageInVolts(const bb::key_t key, MsgArray::iterator begin, MsgArray::iterator end) {
//        Blackboard::entryValue(key) = bb::float_convert(begin, end);
//    }
//    //----------------------------------------------------------------------------------------------------------------------
//    void Power::rxBatteryThresholds(const bb::key_t key, MsgArray::iterator begin, MsgArray::iterator end) {
//        Blackboard::entryValue(key) = bb::float_convert(begin, begin + 4);
//        bb::key_s id_key { key };
//        id_key.id = 1;
//        Blackboard::entryValue(id_key) = bb::float_convert(begin + 4, begin + 8);
//        id_key.id = 2;
//        Blackboard::entryValue(id_key) = bb::float_convert(begin + 8, end);
//    }
}

