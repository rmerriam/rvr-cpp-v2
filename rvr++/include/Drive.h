#ifndef DRIVE_H_
#define DRIVE_H_
#include <Request.h>
#include <SendPacket.h>
#include <tuple>

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
//     Created: Oct 22, 2019
//
//======================================================================================================================
#include "Blackboard.h"

namespace rvr {

    class Drive : protected Request {

    public:
        Drive(Blackboard& bb, SendPacket& req) :
            Request { bb, Devices::drive, req, bluetoothSOC } {
        }
        Drive(Drive&& other) = delete;
        Drive(Drive const& other) = delete;
        Drive& operator=(Drive const& other) = delete;

        void drive(double const& left, double const& right, CommandResponse const want_resp = resp_on_error) const;
        void stop(int const& heading, CommandResponse const want_resp = resp_on_error) const;

        void resetYaw(CommandResponse const want_resp = resp_on_error) const;
        void driveWithHeading(double const& speed, int const& heading, CommandResponse const want_resp = resp_on_error) const;

        void enableMotorStallNotify(CommandResponse const want_resp = resp_on_error) const;
        void disableMotorStallNotify(CommandResponse const want_resp = resp_on_error) const;

        void enableMotorFaultNotify(CommandResponse const want_resp = resp_on_error) const;
        void disableMotorFaultNotify(CommandResponse const want_resp = resp_on_error) const;

        void getMotorFault(CommandResponse const want_resp = resp_on_error) const;

        //======================================================================================================================
        // Access data
        std::optional<bool> motorFaultState() const {
            return mBlackboard.boolValue(mTarget, mDevice, get_motor_fault_state);
        }
        //----------------------------------------------------------------------------------------------------------------------
        std::optional<bool> motorFaultNotifySet() const {
            return mBlackboard.notifyState(mTarget, mDevice, enable_motor_fault_notify);
        }
        //----------------------------------------------------------------------------------------------------------------------
        std::optional<bool> motorStallNotifySet() const {
            return mBlackboard.notifyState(mTarget, mDevice, enable_motor_stall_notify);
        }

    private:

        // drive raw modes
        enum Mode : uint8_t {
            off, forward, reverse,
        };

        // drive commands
        enum Cmd : uint8_t {
            raw_motors = 0x01,
            reset_yaw = 0x06,
            drive_with_heading = 0x07,
            enable_motor_stall_notify = 0x25,
            motor_stall_notify = 0x26,
            enable_motor_fault_notify = 0x27,
            motor_fault_notify = 0x28,
            get_motor_fault_state = 0x29,
        };

        std::tuple<uint8_t, uint8_t> const speed_mode(double const& speed) const;
    };

}
/* namespace rvr */

#endif /* DRIVE_H_ */
