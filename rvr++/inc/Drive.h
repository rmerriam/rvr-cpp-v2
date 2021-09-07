#ifndef DRIVE_H_
#define DRIVE_H_
#include <Request.h>
#include <SendPacket.h>
#include <tuple>

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
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//======================================================================================================================
//
//     Author: rmerriam
//
//     Created: May 29, 2021
//
//======================================================================================================================
#include "Blackboard.h"

namespace rvr {

class Drive : protected Request {

   public:
   Drive(Blackboard& bb, SendPacket& req)
       : Request { bb, Devices::drive, req, bluetoothSOC } {
   }
   Drive(Drive&& other) = delete;
   Drive(Drive const& other) = delete;
   Drive& operator=(Drive const& other) = delete;

   void drive(double const& left, double const& right, CommandResponse const want_resp = CommandResponse::resp_on_error) const;
   void stop(int const& heading, CommandResponse const want_resp = CommandResponse::resp_on_error) const;

   void tank_normalized(double const& left, double const& right, CommandResponse const want_resp = CommandResponse::resp_on_error) const;

   void resetYaw(CommandResponse const want_resp = CommandResponse::resp_on_error) const;
   void driveWithHeading(double const& speed, int const& heading, CommandResponse const want_resp = CommandResponse::resp_on_error) const;

   void enableMotorStallNotify(CommandResponse const want_resp = CommandResponse::resp_on_error) const;
   void disableMotorStallNotify(CommandResponse const want_resp = CommandResponse::resp_on_error) const;

   void enableMotorFaultNotify(CommandResponse const want_resp = CommandResponse::resp_on_error) const;
   void disableMotorFaultNotify(CommandResponse const want_resp = CommandResponse::resp_on_error) const;

   void getMotorFault(CommandResponse const want_resp = CommandResponse::resp_on_error) const;

   //======================================================================================================================
   // Access data
   ResultBool motorFaultState() const {
      return mBlackboard.boolValue(mTarget, mDevice, get_motor_fault_state);
   }
   //----------------------------------------------------------------------------------------------------------------------
   ResultBool motorFaultNotifySet() const {
      return mBlackboard.notifyState(mTarget, mDevice, enable_motor_fault_notify);
   }
   //----------------------------------------------------------------------------------------------------------------------
   ResultBool motorStallNotifySet() const {
      return mBlackboard.notifyState(mTarget, mDevice, enable_motor_stall_notify);
   }

   private:
   // drive raw modes
   enum Mode : uint8_t {
      off,
      forward,
      reverse,
   };

   // drive commands
   //        enum Cmd : uint8_t {
   //            raw_motors = 0x01,
   //            reset_yaw = 0x06,
   //            drive_with_heading = 0x07,
   //            enable_motor_stall_notify = 0x25,
   //            motor_stall_notify = 0x26,
   //            enable_motor_fault_notify = 0x27,
   //            motor_fault_notify = 0x28,
   //            get_motor_fault_state = 0x29,
   //        };

   enum Cmd : uint8_t {
      raw_motors = 0x01, //  v1
      reset_yaw = 0x06,  //  v1

      drive_with_heading = 0x07, //  v1

      set_default_control_system_for_type = 0x0E, //
      set_custom_control_system_timeout = 0x22,   //

      enable_motor_stall_notify = 0x25, //  v1
      motor_stall_notify = 0x26,        //  v1
      enable_motor_fault_notify = 0x27, //  v1
      motor_fault_notify = 0x28,        //  v1
      get_motor_fault_state = 0x29,     //  v1

      drive_tank_si_units = 0x32,   //
      drive_tank_normalized = 0x33, //

      drive_rc_si_units = 0x34,   //
      drive_rc_normalized = 0x35, //

      drive_with_yaw_si = 0x36,         //
      drive_with_yaw_normalized = 0x37, //

      drive_to_position_si = 0x38,            //
      drive_to_position_normalized = 0x39,    //
      xy_position_drive_result_notify = 0x3A, //

      set_drive_target_slew_parameters = 0x3C, //
      get_drive_target_slew_parameters = 0x3D, //

      drive_stop_custom_decel = 0x3E,                      //
      robot_has_stopped_notify = 0x3F,                     //
      restore_default_drive_target_slew_parameters = 0x40, //
      get_stop_controller_state = 0x41,                    //
      drive_stop = 0x42,                                   //
      restore_default_control_system_timeout = 0x43,       //
      get_active_control_system_id = 0x44,                 //
      restore_initial_default_control_systems = 0x45,      //
      get_default_control_system_for_type = 0x46           //
   };

   std::tuple<uint8_t, uint8_t> const speed_mode(double const& speed) const;
};

} // namespace rvr
/* namespace rvr */

#endif /* DRIVE_H_ */
