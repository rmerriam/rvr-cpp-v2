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
        Drive(Drive const& other) = delete;
        Drive& operator=(Drive const& other) = delete;

        void drive(double const& left, double const& right, bool const get_response = false);
        void stop(int const& heading, bool const get_response = false);
        void spin_drive(double const& speed, int const& heading, bool const get_response = false);

        void getMotorFault();
        void getMotorStall();

        void fixHeading();

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
    //----------------------------------------------------------------------------------------------------------------------
    inline void Drive::stop(int const& heading, bool const get_response) {
        spin_drive(0, heading, get_response);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Drive::getMotorFault() {
        do_request(get_motor_fault_state, true);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Drive::getMotorStall() {
        do_request(enable_motor_stall_notify, true);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Drive::fixHeading() {
        do_request(reset_yaw, true);
    }
}
/* namespace rvr */

#endif /* DRIVE_H_ */
