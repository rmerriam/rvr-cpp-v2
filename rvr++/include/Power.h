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
#include "Request.h"
#include "CommandBase.h"

namespace rvr {

    class Power : protected CommandBase {

    public:
        enum VoltageType : uint8_t {
            CalibratedFiltered = 0, CalibratedUnfiltered = 1, UncalibratedUnfiltered = 2,
        };
        enum MotorSide : uint8_t {
            left = 0, right = 1
        };
        Power(Request& req) :
            CommandBase { Devices::power, req, bluetoothSOC } {
        }

        Power(const Power& other) = delete;
        Power(Power&& other) = delete;
        Power& operator=(const Power& other) = delete;

        void awake();
        void sleep();

        void battery_precentage();
        void battery_voltage_state();
        void battery_volt_thresholds();
        void battery_current(const MotorSide ms);

        void battery_voltage(const VoltageType vt);

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
    };
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::awake() {
        cmd_basic(wake, true);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::sleep() {
        cmd_basic(snooze, true);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::battery_precentage() {
        cmd_basic(get_battery_percentage, true);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::battery_voltage_state() {
        cmd_basic(get_battery_voltage_state, true);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::battery_voltage(const VoltageType vt) {
        cmd_byte(get_battery_voltage_in_volts, vt, true);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::battery_volt_thresholds() {
        cmd_basic(get_battery_voltage_state_thresholds, true);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Power::battery_current(const MotorSide ms) {
        cmd_byte_alt(get_current_sense_amplifier_current, ms, true);
    }

} /* namespace rvr */

#endif
