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
        using bb= Blackboard;

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

        void batteryMotorCurrent(const MotorSide ms, const CommandResponse want_resp = resp_yes);
        void batteryPercentage(const CommandResponse want_resp = resp_yes);
        void batteryVoltage(const VoltageType vt, const CommandResponse want_resp = resp_yes);
        void batteryVoltageState(const CommandResponse want_resp = resp_yes);
        void batteryVoltThresholds(const CommandResponse want_resp = resp_yes);

        // Data access methods`
        float voltsCalibratedFiltered();
        float voltsCalibratedUnfiltered();
        float voltsUncalibratedUnfiltered();

        float voltThresholdCritical();
        float voltThresholdLow();
        float voltThresholdHysteresis();

        float motorCurrent();
        int batteryPercent();
        std::string voltState();

        // De-serialization handlers
        static void rxBatteryPercentage(const bb::key_t key, MsgArray::const_iterator begin, MsgArray::const_iterator end);
        static void rxBatteryStateChange(const bb::key_t key, MsgArray::const_iterator begin, MsgArray::const_iterator end);
        static void rxBatVoltageInVolts(const bb::key_t key, MsgArray::const_iterator begin, MsgArray::const_iterator end);
        static void rxBatteryThresholds(const bb::key_t key, MsgArray::const_iterator begin, MsgArray::const_iterator end);

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
        cmd_byte_id(get_battery_voltage_in_volts, vt, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::batteryVoltThresholds(const CommandResponse want_resp) {
        cmd_basic(get_battery_voltage_state_thresholds, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::batteryMotorCurrent(const MotorSide ms, const CommandResponse want_resp) {
        cmd_byte_alt(get_current_sense_amplifier_current, ms, want_resp);
    }

} /* namespace rvr */

#endif
