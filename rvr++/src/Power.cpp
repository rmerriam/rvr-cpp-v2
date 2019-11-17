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
        std::any value { bb::entryValue(Devices::power, get_battery_percentage) };
        return (value.has_value()) ? std::any_cast<int64_t>(value) : -1;
    }
    //----------------------------------------------------------------------------------------------------------------------
    float Power::motorCurrent() {
        std::any value { bb::entryValue(Devices::power, get_current_sense_amplifier_current) };
        return (value.has_value()) ? std::any_cast<float>(value) : -1;
    }
    //----------------------------------------------------------------------------------------------------------------------
    float Power::voltsCalibratedFiltered() {
        std::any value { bb::entryValueId(Devices::power, get_battery_voltage_in_volts, CalibratedFiltered) };
        return (value.has_value()) ? std::any_cast<float>(value) : NaN;
    }
    //----------------------------------------------------------------------------------------------------------------------
    float Power::voltsCalibratedUnfiltered() {
        std::any value { bb::entryValueId(Devices::power, get_battery_voltage_in_volts, CalibratedUnfiltered) };
        return (value.has_value()) ? std::any_cast<float>(value) : NaN;
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::string Power::voltState() {
        static char_ptr state[4] { "unknown", "ok", "low", "critical" };
        std::any value { bb::entryValue(Devices::power, get_battery_voltage_state) };
        const int64_t st { (value.has_value()) ? std::any_cast<int64_t>(value) : unknown };
        return state[st];
    }
    //----------------------------------------------------------------------------------------------------------------------
    float Power::voltThresholdCritical() {
        std::any value { bb::entryValue(Devices::power, get_battery_voltage_state_thresholds) };
        return (value.has_value()) ? std::any_cast<float>(value) : NaN;
    }
    float Power::voltThresholdLow() {
        std::any value { bb::entryValueId(Devices::power, get_battery_voltage_state_thresholds, 1) };
        return (value.has_value()) ? std::any_cast<float>(value) : NaN;
    }
    float Power::voltThresholdHysteresis() {
        std::any value { bb::entryValueId(Devices::power, get_battery_voltage_state_thresholds, 2) };
        return (value.has_value()) ? std::any_cast<float>(value) : NaN;
    }
    //----------------------------------------------------------------------------------------------------------------------
    float Power::voltsUncalibratedUnfiltered() {
        std::any value { bb::entryValueId(Devices::power, get_battery_voltage_in_volts, UncalibratedUnfiltered) };
        return (value.has_value()) ? std::any_cast<float>(value) : NaN;
    }
    //======================================================================================================================
    // Deserialization methods
    void Power::rxBatVoltageInVolts(const bb::key_t key, MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        Blackboard::entryValue(key) = bb::float_convert(begin, end);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Power::rxBatteryPercentage(const bb::key_t key, MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        Blackboard::entryValue(key) = (bb::int_convert(begin, end) * 100.0f) / 100.0f;
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Power::rxBatteryThresholds(const bb::key_t key, MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        Blackboard::entryValue(key) = bb::float_convert(begin, begin + 4);
        Blackboard::entryValue(static_cast<bb::key_t>((1 << 16) | key)) = bb::float_convert(begin + 4, begin + 8);
        Blackboard::entryValue(static_cast<bb::key_t>((2 << 16) | key)) = bb::float_convert(begin + 8, end);
    }
}

