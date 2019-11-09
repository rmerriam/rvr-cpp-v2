#ifndef DRIVE_H_
#define DRIVE_H_
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
#include "Request.h"
#include "CommandBase.h"

namespace rvr {

    class Drive : protected CommandBase {
    public:
        Drive(Request& req) :
            CommandBase { Devices::drive, req, microcontroller } {

        }
        Drive(Drive&& other) = delete;
        Drive(const Drive& other) = delete;
        Drive& operator=(const Drive& other) = delete;

        void drive(const double& left, const double& right, const bool get_response = false);
        void stop(const int& heading, const bool get_response = false);
        void spin_drive(const double& speed, const int& heading, const bool get_response = false);

        void getMotorFault();

        void enableMotorFaultNotify();
        void disableMotorFaultNotify();
        void enableMotorStallNotify();
        void disableMotorStallNotify();

        void fixHeading(const bool get_response = false);

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

        const std::tuple<uint8_t, uint8_t> speed_mode(const double& speed) const;
    };
    //----------------------------------------------------------------------------------------------------------------------
    inline void Drive::stop(const int& heading, const bool get_response) {
        spin_drive(0, heading, get_response);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Drive::getMotorFault() {
        do_request(get_motor_fault_state, true);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Drive::enableMotorFaultNotify() {
        enable_request(enable_motor_fault_notify, true, true);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Drive::disableMotorFaultNotify() {
        enable_request(enable_motor_fault_notify, false, true);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Drive::enableMotorStallNotify() {
        enable_request(enable_motor_stall_notify, true, true);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Drive::disableMotorStallNotify() {
        enable_request(enable_motor_stall_notify, false, true);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Drive::fixHeading(const bool get_response) {
        do_request(reset_yaw, true);
    }
}
/* namespace rvr */

#endif /* DRIVE_H_ */
