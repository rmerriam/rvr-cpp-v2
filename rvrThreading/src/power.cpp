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
//		 File: direct.cpp
//
//     Author: rmerriam
//
//    Created: Jun 10, 2021
//
//======================================================================================================================
#include <thread>
#include <Trace.h>
#include <rvr++.h>
//---------------------------------------------------------------------------------------------------------------------
void power(rvr::Power& pow) {
   pow.batteryPercentage();
   pow.batteryVoltageState();

   pow.batteryVoltage(rvr::Power::VoltageType::CalibratedFiltered);
   pow.batteryVoltage(rvr::Power::VoltageType::CalibratedUnfiltered);
   pow.batteryVoltage(rvr::Power::VoltageType::UncalibratedUnfiltered);

   pow.enableBatteryStateChange();

   pow.batteryVoltThresholds();
   pow.batteryMotorCurrent(rvr::Power::MotorSide::left);
   pow.batteryMotorCurrent(rvr::Power::MotorSide::right);

   std::this_thread::sleep_for(1s);

   mys::tout << code_line << mys::nl;
   mys::tout << code_line << "Power";

   mys::tout << code_line << "VPercent: " << pow.batteryPercent().get();

   mys::tout << code_line << "Sleep Notify: " << pow.isDidSleepNotify().get();
   mys::tout << code_line << "State: " << (int)pow.voltState().get();

   mys::tout << code_line << "Wake Notify: " << pow.isWakeNotify().get();

   mys::tout << code_line << "VoltageCF: " << pow.voltsCalibratedFiltered().get();
   mys::tout << code_line << "VoltageCUf: " << pow.voltsCalibratedUnfiltered().get();
   mys::tout << code_line << "VoltageUcUf: " << pow.voltsUncalibratedUnfiltered().get();
   mys::tout << code_line << "L Motor Current: " << pow.motorCurrent(rvr::Power::MotorSide::left).get();
   mys::tout << code_line << "R Motor Current: " << pow.motorCurrent(rvr::Power::MotorSide::right).get();
   mys::tout << code_line << "Critical Threshold: " << pow.voltThresholdCritical().get();
   mys::tout << code_line << "Low Threshold: " << pow.voltThresholdLow().get();
   mys::tout << code_line << "Hysteresis Threshold: " << pow.voltThresholdHysteresis().get();

   mys::tout << code_line << "Wake Notify Set?: " << pow.isWakeNotify().get();

   mys::tout << code_line << "Wake Notify Cleared?: " << pow.isWakeNotify().get();

   mys::tout << code_line << "Set State Change Enabled: " << pow.isBatteryStateChangeEnabled().get();

   mys::tout << code_line << mys::nl;
   mys::tout << code_line << "disableBatteryStateChange";

   pow.disableBatteryStateChange();
   std::this_thread::sleep_for(50ms);

   mys::tout << code_line << "Set State Change Enabled: " << pow.isBatteryStateChangeEnabled().get();

   mys::tout << code_line << mys::nl;
#if 0
 pow.sleep().get();

 std::this_thread::sleep_for(5000ms);    // have to wait for notification
 mys:;tinfo<< code_line  << "Did Sleep Notify: " << pow.isDidSleepNotify().get();
 mys:;tinfo<< code_line << mys::nl;
#endif
}
