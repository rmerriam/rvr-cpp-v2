#ifndef POWER_H_
#define POWER_H_
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

#include "Blackboard.h"
#include "Request.h"
#include "CommandBase.h"

namespace rvr {

    class Power : protected CommandBase {

    public:
        enum VoltageType : uint8_t {
            CalibratedFiltered = 0, //
            CalibratedUnfiltered = 1, //
            UncalibratedUnfiltered = 2,
        };
        enum MotorSide : uint8_t {
            left = 0, //
            right = 1
        };

        Power(Request& req) :
            CommandBase { Devices::power, req, bluetoothSOC } {
        }

        Power(const Power& other) = delete;
        Power(Power&& other) = delete;
        Power& operator=(const Power& other) = delete;

        // requests to RVR
        void awake(const CommandResponse want_resp = resp_yes);
        void sleep(const CommandResponse want_resp = resp_yes);

        void batteryPercentage(const CommandResponse want_resp = resp_yes);
        void batteryVoltageState(const CommandResponse want_resp = resp_yes);
        void batteryVoltThresholds(const CommandResponse want_resp = resp_yes);
        void batteryMotorCurrent(const MotorSide ms, const CommandResponse want_resp = resp_yes);
        void batteryVoltage(const VoltageType vt, const CommandResponse want_resp = resp_yes);

        using bb= Blackboard;

        // Data access methods
        float batteryVoltsCalibratedFiltered();
        float batteryVoltsCalibratedUnfiltered();
        float batteryVoltsUncalibratedUnfiltered();

        int batteryPercent();
        std::string batteryVoltageState();
        float batteryVoltThresholds();
        float batteryMotorCurrent();

        // De-serialization handlers
        static void rxBatVoltageInVolts(const bb::key_t key, MsgArray::const_iterator begin, MsgArray::const_iterator end);
        static void rxBatteryPercentage(const bb::key_t key, MsgArray::const_iterator begin, MsgArray::const_iterator end);

    private:
        enum BatteryVoltageStates : uint8_t {
            unknown, ok, low, critical,
        };
        enum Cmd : uint8_t {
            snooze = 0x01, //
            wake = 0x0D, //
            get_battery_percentage = 0x10, //
            activity_ack = 0x11, //
            get_battery_voltage_state = 0x17, //
            will_sleep_notify = 0x19, //
            did_sleep_notify = 0x1A, //
            enable_battery_voltage_state_change_notify = 0x1B, //
            battery_voltage_state_change_notify = 0x1C, //
            get_battery_voltage_in_volts = 0x25, //
            get_battery_voltage_state_thresholds = 0x26, //
            get_current_sense_amplifier_current = 0x27, //
        };

        inline static const float NaN { (0.0f / 0.0f) };    // something to return when there is no value for float
    };
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::awake(const CommandResponse want_resp) {
        cmd_basic(wake, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::sleep(const CommandResponse want_resp) {
        cmd_basic(snooze, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::batteryPercentage(const CommandResponse want_resp) {
        cmd_basic(get_battery_percentage, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::batteryVoltageState(const CommandResponse want_resp) {
        cmd_basic(get_battery_voltage_state, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::batteryVoltage(const VoltageType vt, const CommandResponse want_resp) {
        cmd_byte(get_battery_voltage_in_volts, vt, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::batteryVoltThresholds(const CommandResponse want_resp) {
        cmd_basic(get_battery_voltage_state_thresholds, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::batteryMotorCurrent(const MotorSide ms, const CommandResponse want_resp) {
        cmd_byte_alt(get_current_sense_amplifier_current, ms, want_resp);
    }
    //======================================================================================================================
    // data access methods
    inline float Power::batteryVoltsCalibratedFiltered() {
        std::any value { bb::entryValue(Devices::power, get_battery_voltage_in_volts) };
        return (value.has_value()) ? std::any_cast<float>(value) : NaN;
    }
    inline float Power::batteryVoltsCalibratedUnfiltered() {
        std::any value { bb::entryValue(Devices::power, get_battery_voltage_in_volts) };
        return (value.has_value()) ? std::any_cast<float>(value) : NaN;
    }
    inline float Power::batteryVoltsUncalibratedUnfiltered() {
        std::any value { bb::entryValue(Devices::power, get_battery_voltage_in_volts) };
        return (value.has_value()) ? std::any_cast<float>(value) : NaN;
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline int Power::batteryPercent() {
        std::any value { bb::entryValue(Devices::power, get_battery_percentage) };
        return (value.has_value()) ? std::any_cast<float>(value) : -1;
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline std::string Power::batteryVoltageState() {
        using namespace std::literals;
        std::any value { bb::entryValue(Devices::power, get_battery_voltage_state) };
        return (value.has_value()) ? std::any_cast<std::string>(value) : ""s;
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline float Power::batteryVoltThresholds() {
        std::any value { bb::entryValue(Devices::power, get_battery_voltage_state_thresholds) };
        return (value.has_value()) ? std::any_cast<float>(value) : NaN;
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline float Power::batteryMotorCurrent() {
        std::any value { bb::entryValue(Devices::power, get_current_sense_amplifier_current) };
        return (value.has_value()) ? std::any_cast<float>(value) : -1;
    }
    //======================================================================================================================
    // Deserialization methods
    inline void Power::rxBatVoltageInVolts(const bb::key_t key, MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        Blackboard::entryValue(key) = Blackboard::float_convert(begin, end);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::rxBatteryPercentage(const bb::key_t key, MsgArray::const_iterator begin, MsgArray::const_iterator end) {
        Blackboard::entryValue(key) = (Blackboard::int_convert(begin, end) * 100.0f) / 100.0f;
    }
} /* namespace rvr */

#endif
