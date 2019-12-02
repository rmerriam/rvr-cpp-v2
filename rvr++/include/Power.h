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
        using bb = Blackboard;

    public:
        Power(Request& req) :
            CommandBase { Devices::power, req, bluetoothSOC } {
        }
        enum VoltageType : uint8_t {
            CalibratedFiltered = 0, //
            CalibratedUnfiltered = 1, //
            UncalibratedUnfiltered = 2,
        };
        enum MotorSide : uint8_t {
            left = 0, //
            right = 1
        };

        Power(Power const& other) = delete;
        Power(Power&& other) = delete;
        Power& operator=(Power const& other) = delete;

        // requests to RVR
        void awake(CommandResponse const want_resp = resp_on_error) const;
        void powerOff(uint8_t const secs, CommandResponse const want_resp = resp_on_error) const;
        void sleep(CommandResponse const want_resp = resp_on_error) const;

        void batteryMotorCurrent(MotorSide const ms, CommandResponse const want_resp = resp_on_error) const;
        void batteryPercentage(CommandResponse const want_resp = resp_on_error) const;
        void batteryVoltage(VoltageType const vt, CommandResponse const want_resp = resp_on_error) const;
        void batteryVoltageState(CommandResponse const want_resp = resp_on_error) const;
        void batteryVoltThresholds(CommandResponse const want_resp = resp_on_error) const;

        void enableBatteryStateChange(CommandResponse const want_resp = resp_on_error) const;
        void disableBatteryStateChange(CommandResponse const want_resp = resp_on_error) const;

        // Data access methods`
        float voltsCalibratedFiltered();
        float voltsCalibratedUnfiltered();
        float voltsUncalibratedUnfiltered();

        float voltThresholdCritical();
        float voltThresholdLow();
        float voltThresholdHysteresis();

        float motorCurrent(MotorSide const ms);
        int batteryPercent();
        std::string voltState();

        bool isWakeNotify();
        void resetWakeNotify();

        bool checkBatteryStateChange();
        bool isSleepNotify();

        // De-serialization handlers
//        static void rxBatteryThresholds(const bb::key_t key, MsgArray::iterator begin, MsgArray::iterator end);

    private:
        enum BatteryVoltageStates : uint8_t {
            unknown, ok, low, critical,
        };
        enum Cmd : uint8_t {
            power_off = 0x00, snooze = 0x01, //
            wake = 0x0D, //
            get_battery_percentage = 0x10, //
            system_awake_notify = 0x11, //
            get_battery_voltage_state = 0x17, //
            will_sleep_notify = 0x19, //
            did_sleep_notify = 0x1A, //
            enable_battery_voltage_state_change_notify = 0x1B, //
            battery_voltage_state_change_notify = 0x1C, //
            get_battery_voltage_in_volts = 0x25, //
            get_battery_voltage_state_thresholds = 0x26, //
            get_current_sense_amplifier_current = 0x27, //
        };

    };
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::awake(CommandResponse const want_resp) const {
        cmd_basic(wake, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::powerOff(uint8_t const secs, CommandResponse const want_resp) const {
        cmd_byte(power_off, secs, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::sleep(CommandResponse const want_resp) const {
        cmd_basic(snooze, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::batteryPercentage(CommandResponse const want_resp) const {
        cmd_basic(get_battery_percentage, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::batteryVoltageState(CommandResponse const want_resp) const {
        cmd_basic(get_battery_voltage_state, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::batteryVoltage(VoltageType const vt, CommandResponse const want_resp) const {
        cmd_byte_id(get_battery_voltage_in_volts, vt, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::batteryVoltThresholds(CommandResponse const want_resp) const {
        cmd_basic(get_battery_voltage_state_thresholds, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::batteryMotorCurrent(MotorSide const ms, CommandResponse const want_resp) const {
        cmd_byte_alt_id(get_current_sense_amplifier_current, ms, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::enableBatteryStateChange(CommandResponse const want_resp) const {
        cmd_enable(enable_battery_voltage_state_change_notify, true, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::disableBatteryStateChange(CommandResponse const want_resp) const {
        cmd_enable(enable_battery_voltage_state_change_notify, false, want_resp);
//        bb::resetNotify(enable_battery_voltage_state_change_notify);
        // cannot do this because the response puts a value back into the dictionary
    }

} /* namespace rvr */

#endif
