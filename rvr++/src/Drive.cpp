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
//     Created: Oct 27, 2019
//
//======================================================================================================================

#include "Drive.h"

namespace rvr {
    //----------------------------------------------------------------------------------------------------------------------
    void Drive::getMotorFault(CommandResponse const want_resp) const {
        basic(get_motor_fault_state, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Drive::enableMotorFaultNotify(CommandResponse const want_resp) const {
        cmdEnable(enable_motor_fault_notify, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Drive::disableMotorFaultNotify(CommandResponse const want_resp) const {
        cmdDisable(enable_motor_fault_notify, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Drive::enableMotorStallNotify(CommandResponse const want_resp) const {
        cmdEnable(enable_motor_stall_notify, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Drive::disableMotorStallNotify(CommandResponse const want_resp) const {
        cmdDisable(enable_motor_stall_notify, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Drive::resetYaw(CommandResponse const want_resp) const {
        basic(reset_yaw, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::tuple<unsigned char, unsigned char> const Drive::speed_mode(double const& speed) const {
        uint8_t s { static_cast<uint8_t>(abs(speed) / 100.0 * 256) };
        uint8_t m { static_cast<uint8_t>((speed > 0) ? forward : ((speed < 0) ? reverse : off)) };
        return std::tuple(s, m);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Drive::drive(double const& left, double const& right, CommandResponse const want_resp) const {
        auto [l_speed, l_mode] = speed_mode(left);
        auto [r_speed, r_mode] = speed_mode(right);

        RvrMsg msg { l_mode, l_speed, r_mode, r_speed };
        cmdData(raw_motors, msg, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Drive::stop(int const& heading, CommandResponse const want_resp) const {
        drive(0, 0, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Drive::driveWithHeading(double const& speed, int const& heading, CommandResponse const want_resp) const {
        auto [spd, mode] = speed_mode(abs(speed));
        RvrMsg msg { spd, static_cast<uint8_t>(heading >> 8), static_cast<uint8_t>(heading & 0xFF), static_cast<uint8_t>(
            (speed >= 0.0) ? 0 : 1) };
        cmdData(drive_with_heading, msg, want_resp);
    }
}
