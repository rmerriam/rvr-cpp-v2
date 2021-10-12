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

    mys::tout << code_line << "power voltsCalibratedFiltered valid? " << pow.voltsCalibratedFiltered().valid();
    mys::tout << code_line << "VoltageCF: " << pow.voltsCalibratedFiltered().get_or();
    mys::tout << code_line << "power voltsCalibratedFiltered valid? " << pow.voltsCalibratedFiltered().valid();

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

    mys::tout << code_line << "Power";

    mys::tout << code_line << "VPercent: " << pow.batteryPercent().get_or();

    pow.resetSleepNotify();

    mys::tout << code_line << "Sleep Notify: " << pow.isDidSleepNotify().get_or();
    mys::tout << code_line << "Wake Notify: " << pow.isWakeNotify().get_or();
    mys::tout << code_line << mys::nl;

    mys::tout << code_line << "Voltage State: " << (int)pow.voltState().get_or() << mys::sp << pow.voltStateText().get_or();
    mys::tout << code_line << mys::nl;

    mys::tout << code_line << "VoltageCF: " << pow.voltsCalibratedFiltered().get_or();
    mys::tout << code_line << "VoltageCUf: " << pow.voltsCalibratedUnfiltered().get_or();
    mys::tout << code_line << "VoltageUcUf: " << pow.voltsUncalibratedUnfiltered().get_or();
    mys::tout << code_line << mys::nl;

    mys::tout << code_line << "L Motor Current: " << pow.motorCurrent(rvr::Power::MotorSide::left).get_or();
    mys::tout << code_line << "R Motor Current: " << pow.motorCurrent(rvr::Power::MotorSide::right).get_or();
    mys::tout << code_line << mys::nl;

    auto [critical, low, hysteresis] { pow.voltageThresholds().get_or() };
    mys::tout << code_line << "Critical Threshold: " << critical;
    mys::tout << code_line << "Low Threshold: " << low;
    mys::tout << code_line << "Hysteresis Threshold: " << hysteresis;
    mys::tout << code_line << mys::nl;

    mys::tout << code_line << "Wake Notify Set?: " << pow.isWakeNotify().get_or();
    pow.resetWakeNotify();
    mys::tout << code_line << "Wake Notify Cleared?: " << pow.isWakeNotify().get_or();
    mys::tout << code_line << mys::nl;

    mys::tout << code_line << "Set State Change Enabled: " << pow.isBatteryStateChangeEnabled().get_or();
    mys::tout << code_line << "disableBatteryStateChange";

    pow.disableBatteryStateChange();
    std::this_thread::sleep_for(50ms);

    mys::tout << code_line << "Set State Change Enabled: " << pow.isBatteryStateChangeEnabled().get_or();

    mys::tout << code_line << mys::nl;

#if 1
    pow.resetSleepNotify();
    pow.sleep();
    mys::tout << code_line << "Sleep Notify: " << pow.isDidSleepNotify().get_or();
    mys::tout << code_line << "wait for sleep notification";

    while (pow.isDidSleepNotify().invalid()) {
        std::this_thread::sleep_for(500ms);    // have to wait for notification
    }

    mys::tout << code_line << "Recieve Sleep Notify: " << pow.isDidSleepNotify().get_or();
    mys::tout << code_line << mys::nl;

//    pow.resetSleepNotify();

#endif
}
