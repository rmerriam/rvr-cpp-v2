//======================================================================================================================
// 2021 Copyright Mystic Lake Software
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
// along with this program.  If not, see <http:             //www.gnu.org/licenses/>.
//======================================================================================================================
//
//     Author: rmerriam
//
//     Created: May 29, 2021
//
//======================================================================================================================
#include <PayloadDecode.h>
#include <Power.h>
namespace rvr {
    //----------------------------------------------------------------------------------------------------------------------
    ResultUInt8 Power::batteryPercent() const noexcept {
        auto const& msg { mBlackboard.entryValue(mTarget, mDevice, get_battery_percentage) };
//        return msg.empty() ? ResultUInt8 {} : decode_type<uint8_t>(msg);
        return decode_type<uint8_t>(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    ResultFloat Power::motorCurrent(MotorSide const ms) const noexcept {
        auto const& msg { mBlackboard.entryValue(mAltTarget, mDevice, get_current_sense_amplifier_current, ms) };
        return msg.empty() ? ResultFloat {} : decode_type<float>(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    ResultFloat Power::voltsCalibratedFiltered() const noexcept {
        auto const& msg { mBlackboard.entryValue(mTarget, mDevice, get_battery_voltage_in_volts, CalibratedFiltered) };
        return msg.empty() ? ResultFloat {} : decode_type<float>(msg);
    }
//----------------------------------------------------------------------------------------------------------------------
    ResultFloat Power::voltsCalibratedUnfiltered() const noexcept {
        auto const& msg { mBlackboard.entryValue(mTarget, mDevice, get_battery_voltage_in_volts, CalibratedUnfiltered) };
        return msg.empty() ? ResultFloat {} : decode_type<float>(msg);
    }
//----------------------------------------------------------------------------------------------------------------------
    ResultFloat Power::voltsUncalibratedUnfiltered() const noexcept {
        auto const& msg { mBlackboard.entryValue(mTarget, mDevice, get_battery_voltage_in_volts, UncalibratedUnfiltered) };
        return msg.empty() ? ResultFloat {} : decode_type<float>(msg);
    }
//----------------------------------------------------------------------------------------------------------------------
    Result<Power::BatteryVoltState> Power::voltState() const noexcept {
        auto const& msg { mBlackboard.entryValue(mTarget, mDevice, get_battery_voltage_state) };
        return Power::BatteryVoltState { decode_type<uint8_t>(msg) };
    }
//----------------------------------------------------------------------------------------------------------------------
    ResultString Power::voltStateText() const noexcept {
        static char_ptr state[4] { "unknown", "ok", "low", "critical" };
        return ResultString { state[voltState().get_or(BatteryVoltState::unknown)] };
    }
//----------------------------------------------------------------------------------------------------------------------
    Result<VoltageThresholds> Power::voltageThresholds() const noexcept {
        auto const& msg { mBlackboard.entryValue(mTarget, mDevice, get_battery_voltage_state_thresholds) };
        Result<VoltageThresholds> res {};

        if ( !msg.empty()) {
            PayloadDecode<float, float, float> payload(msg);

            res = VoltageThresholds {              //
            payload.get<0>(),              //
                payload.get<1>(),              //
                payload.get<2>(),              //
            };
        }
        return res;
    }
//----------------------------------------------------------------------------------------------------------------------
    ResultBool Power::isBatteryStateChangeEnabled() const noexcept {
        RvrMsgView msg { mBlackboard.entryValue(mTarget, mDevice, enable_battery_voltage_state_change_notify) };
        return msg.empty() ? bool {} : decode_type<bool>(msg);
    }
//----------------------------------------------------------------------------------------------------------------------
    ResultBool Power::isDidSleepNotify() const noexcept {
        RvrMsgView msg { mBlackboard.entryValue(mTarget, mDevice, did_sleep_notify) };
        return decode_type<bool>(msg);
    }
//----------------------------------------------------------------------------------------------------------------------
    void Power::resetSleepNotify() const noexcept {
        mBlackboard.resetNotify(mTarget, mDevice, did_sleep_notify);
    }
//----------------------------------------------------------------------------------------------------------------------
    ResultBool Power::isWakeNotify() const noexcept {
        RvrMsgView msg { mBlackboard.entryValue(mTarget, mDevice, system_awake_notify) };
        return decode_type<bool>(msg);
    }
//----------------------------------------------------------------------------------------------------------------------
    void Power::resetWakeNotify() const noexcept {
        mBlackboard.resetNotify(mTarget, mDevice, system_awake_notify);
    }
}

