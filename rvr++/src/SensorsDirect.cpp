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
#include "SensorsDirect.h"

namespace rvr {

//----------------------------------------------------------------------------------------------------------------------
Result<ColorData> SensorsDirect::currentRGBValues() {
   auto const msg { mBlackboard.msgValue(mAltTarget, mDevice, get_rgbc_sensor_values) };
   Result<ColorData> res;

   if(!msg.empty()) {
      res = ColorData {
         static_cast<uint16_t>(mBlackboard.uintConvert(msg.begin(), 2)),     //
         static_cast<uint16_t>(mBlackboard.uintConvert(msg.begin() + 2, 2)), //
         static_cast<uint16_t>(mBlackboard.uintConvert(msg.begin() + 4, 2)), //

         static_cast<uint16_t>(mBlackboard.uintConvert(msg.begin() + 6, 2)), //
      };
   }
   return res;
}
//----------------------------------------------------------------------------------------------------------------------
Result<ColorDetection> SensorsDirect::colorDetectionValues() {
   auto const& msg { mBlackboard.msgValue(mAltTarget, mDevice, color_detection_notify) };
   Result<ColorDetection> res;

   if(!msg.empty()) {
      res = ColorDetection {
         //
         static_cast<uint8_t>(msg[0]), //
         static_cast<uint8_t>(msg[1]), //
         static_cast<uint8_t>(msg[2]), //
         static_cast<uint8_t>(msg[3]), //
         static_cast<uint8_t>(msg[4]), //
      };
   }
   return res;
}
//----------------------------------------------------------------------------------------------------------------------
Result<ThermalProtection> SensorsDirect::thermalProtectionValues() {
   auto const& msg { mBlackboard.msgValue(mTarget, mDevice, get_motor_thermal_protection_status) };
   Result<ThermalProtection> res;

   if(!msg.empty()) {
      auto const& begin { msg.begin() };
      auto lm { mBlackboard.floatConvert(begin) };
      auto rm { mBlackboard.floatConvert(begin + 5) };
      res = ThermalProtection { lm, (rvr::ThermalStatus)msg[4], rm, (rvr::ThermalStatus)msg[9] };
   }
   return res;
}
//----------------------------------------------------------------------------------------------------------------------
ResultBool SensorsDirect::isMagnetometerCalibrationDone() const {

   rvr::ResultBool res;

   auto const msg { mBlackboard.msgValue(mTarget, mDevice, magnetometer_calibration_complete_notify) };
   if(!msg.empty()) {
      res = ResultBool(static_cast<bool>(msg[0]));
   }
   return res;
}
//----------------------------------------------------------------------------------------------------------------------
ResultInt16 SensorsDirect::magnetometerCalibrationYaw() const {
   auto const msg { mBlackboard.msgValue(mTarget, mDevice, magnetometer_calibration_complete_notify) };
   ResultInt16 res;
   if(!msg.empty()) {
      res = ResultInt16(static_cast<int16_t>(mBlackboard.uintConvert(msg.begin() + 1, 2)));
   }

   return res;
}
//----------------------------------------------------------------------------------------------------------------------
Result<MagnetometerData> SensorsDirect::magnetometerData() const noexcept {
   Result<MagnetometerData> res;
   auto const& msg { mBlackboard.msgValue(mTarget, mDevice, get_magnetometer_reading) };

   if(!msg.empty()) {
      auto const& begin { msg.begin() };
      auto const x { mBlackboard.floatConvert(begin) };
      auto const y { mBlackboard.floatConvert(begin + 2) };
      auto const z { mBlackboard.floatConvert(begin + 4) };
      res = Result<MagnetometerData> { MagnetometerData { x, y, z } };
   }

   return res;
}
} // namespace rvr
