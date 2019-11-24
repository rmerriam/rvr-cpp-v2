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
        std::any value { bb::entryValue(mTarget, Devices::power, get_battery_percentage) };
        return (value.has_value()) ? std::any_cast<int64_t>(value) : -1;
    }
    //----------------------------------------------------------------------------------------------------------------------
    float Power::motorCurrent(const MotorSide ms) {
        std::any value { bb::entryValueId(mAltTarget, Devices::power, get_current_sense_amplifier_current, ms) };
        return (value.has_value()) ? std::any_cast<float>(value) : -1;
    }
    //----------------------------------------------------------------------------------------------------------------------
    float Power::voltsCalibratedFiltered() {
        std::any value { bb::entryValueId(mTarget, Devices::power, get_battery_voltage_in_volts, CalibratedFiltered) };
        return (value.has_value()) ? std::any_cast<float>(value) : NaN;
    }
    //----------------------------------------------------------------------------------------------------------------------
    float Power::voltsCalibratedUnfiltered() {
        std::any value { bb::entryValueId(mTarget, Devices::power, get_battery_voltage_in_volts, CalibratedUnfiltered) };
        return (value.has_value()) ? std::any_cast<float>(value) : NaN;
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::string Power::voltState() {
        static char_ptr state[4] { "unknown", "ok", "low", "critical" };
        std::any value { bb::entryValue(mTarget, Devices::power, get_battery_voltage_state) };
        const int64_t st { (value.has_value()) ? std::any_cast<int64_t>(value) : unknown };
        return state[st];
    }
    //----------------------------------------------------------------------------------------------------------------------
    float Power::voltThresholdCritical() {
        std::any value { bb::entryValue(mTarget, Devices::power, get_battery_voltage_state_thresholds) };
        return (value.has_value()) ? std::any_cast<float>(value) : NaN;
    }
    float Power::voltThresholdLow() {
        std::any value { bb::entryValueId(mTarget, Devices::power, get_battery_voltage_state_thresholds, 1) };
        return (value.has_value()) ? std::any_cast<float>(value) : NaN;
    }
    float Power::voltThresholdHysteresis() {
        std::any value { bb::entryValueId(mTarget, Devices::power, get_battery_voltage_state_thresholds, 2) };
        return (value.has_value()) ? std::any_cast<float>(value) : NaN;
    }
    //----------------------------------------------------------------------------------------------------------------------
    float Power::voltsUncalibratedUnfiltered() {
        std::any value { bb::entryValueId(mTarget, Devices::power, get_battery_voltage_in_volts, UncalibratedUnfiltered) };
        return (value.has_value()) ? std::any_cast<float>(value) : NaN;
    }
//    //----------------------------------------------------------------------------------------------------------------------
    bool Power::checkBatteryStateChange() {
        std::any value { bb::entryValue(mTarget, Devices::power, enable_battery_voltage_state_change_notify) };
        return value.has_value();
//        return (value.has_value()) ? (std::any_cast<int64_t>(value)) == 0 : false;
    }
    //----------------------------------------------------------------------------------------------------------------------
    bool Power::isSleepNotify() {
        std::any value { bb::entryValue(mTarget, Devices::power, did_sleep_notify) };
        return value.has_value();
    }
    //----------------------------------------------------------------------------------------------------------------------
    bool Power::isWakeNotify() {
        std::any value { bb::entryValue(mTarget, Devices::power, system_awake_notify) };
        return value.has_value();
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Power::resetWakeNotify() {
//        std::any& value { bb::entryValue(mTarget, Devices::power, system_awake_notify) };
//        value.reset();
        resetNotify(system_awake_notify);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Power::resetNotify(const uint8_t cmd) const {
        std::any& value { bb::entryValue(mTarget, Devices::power, cmd) };
        value.reset();
    }    //======================================================================================================================
    // Deserialization methods
//    void Power::rxBatVoltageInVolts(const bb::key_t key, MsgArray::iterator begin, MsgArray::iterator end) {
//        Blackboard::entryValue(key) = bb::float_convert(begin, end);
//    }
//    //----------------------------------------------------------------------------------------------------------------------
    void Power::rxBatteryThresholds(const bb::key_t key, MsgArray::iterator begin, MsgArray::iterator end) {
        Blackboard::entryValue(key) = bb::float_convert(begin, begin + 4);
        bb::key_s id_key { key };
        id_key.id = 1;
        Blackboard::entryValue(id_key) = bb::float_convert(begin + 4, begin + 8);
        id_key.id = 2;
        Blackboard::entryValue(id_key) = bb::float_convert(begin + 8, end);
    }
}

