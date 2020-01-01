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

#include <Request.h>
#include <SendPacket.h>
#include "Blackboard.h"
//----------------------------------------------------------------------------------------------------------------------
namespace rvr {
    class Blackboard;

    class Power : protected Request {

    public:
        Power(Blackboard& bb, SendPacket& req) :
            Request { bb, Devices::power, req, nordic } {
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

        void powerOff(uint8_t const secs, CommandResponse const want_resp = resp_on_error) const;
        void sleep(CommandResponse const want_resp = resp_on_error) const;
        void awake(CommandResponse const want_resp = resp_on_error) const;

        void batteryPercentage(CommandResponse const want_resp = resp_yes) const;
        void batteryVoltageState(CommandResponse const want_resp = resp_yes) const;

        //  Will Sleep Notify
        //  Did Sleep Notify

        void enableBatteryStateChange(CommandResponse const want_resp = resp_yes) const;
        void disableBatteryStateChange(CommandResponse const want_resp = resp_yes) const;

        void batteryVoltage(VoltageType const vt, CommandResponse const want_resp = resp_yes) const;
        void batteryVoltThresholds(CommandResponse const want_resp = resp_yes) const;
        void batteryMotorCurrent(MotorSide const ms, CommandResponse const want_resp = resp_yes) const;

        //----------------------------------------------------------------------------------------------------------------------
        // Data access methods

        std::optional<int> batteryPercent();

        enum BatteryVoltState : uint8_t {
            unknown, ok, low, critical,
        };
        std::optional<BatteryVoltState> voltState();
        std::optional<std::string> voltStateText();

        // batteryVoltage
        std::optional<float> voltsCalibratedFiltered();
        std::optional<float> voltsCalibratedUnfiltered();
        std::optional<float> voltsUncalibratedUnfiltered();

        // batteryVoltThresholds
        std::optional<float> voltThresholdCritical();
        std::optional<float> voltThresholdLow();
        std::optional<float> voltThresholdHysteresis();

        // batteryMotorCurrent
        std::optional<float> motorCurrent(MotorSide const ms);

        std::optional<bool> isWakeNotify();
        void resetWakeNotify();

        std::optional<bool> isBatteryStateChangeEnabled();
        std::optional<bool> isDidSleepNotify();

    private:

        enum Cmd : uint8_t {
            power_off = 0x00,        //
            snooze = 0x01, //
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
        basic(wake, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::powerOff(uint8_t const secs, CommandResponse const want_resp) const {
        reqByte(power_off, secs, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::sleep(CommandResponse const want_resp) const {
        basic(snooze, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::batteryPercentage(CommandResponse const want_resp) const {
        basic(get_battery_percentage, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::batteryVoltageState(CommandResponse const want_resp) const {
        basic(get_battery_voltage_state, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::batteryVoltage(VoltageType const vt, CommandResponse const want_resp) const {
        byteId(get_battery_voltage_in_volts, vt, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::batteryVoltThresholds(CommandResponse const want_resp) const {
        basic(get_battery_voltage_state_thresholds, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::batteryMotorCurrent(MotorSide const ms, CommandResponse const want_resp) const {
        byteAltId(get_current_sense_amplifier_current, ms, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::enableBatteryStateChange(CommandResponse const want_resp) const {
        cmdEnable(enable_battery_voltage_state_change_notify, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::disableBatteryStateChange(CommandResponse const want_resp) const {
        cmdDisable(enable_battery_voltage_state_change_notify, want_resp);
    }

} /* namespace rvr */

#endif
