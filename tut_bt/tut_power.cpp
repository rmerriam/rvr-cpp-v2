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

#include <chrono>
#include <string>
#include <thread>
using namespace std::literals;

#include <tut/tut.hpp>

#include "Trace.h"

#include "Blackboard.h"
#include "Power.h"

using Pow = rvr::Power;
//---------------------------------------------------------------------------------------------------------------------
extern rvr::SendPacket* packet_send;
extern rvr::Blackboard* blackboard;

struct pow_test_data {
    rvr::Blackboard* bb;
    rvr::Power pow { *bb, *packet_send };
};
//=====================================================================================================================
namespace tut {

    using t_group = test_group<pow_test_data>;

    t_group pow("Power Test");

    using pow_tests = t_group::object;
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void pow_tests::test<1>() {
        set_test_name("batteryPercentage");

        rvr::RvrMsg text { 0xDE, 0xAD, 0xFE, 0xED };
        pow.batteryPercentage();
        std::this_thread::sleep_for(50ms);

        ensure_equals("Value bad Power::batteryPercentage", pow.batteryPercent(), 50.0, 49.0);
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void pow_tests::test<2>() {
        set_test_name("batteryVoltageState");

        pow.batteryVoltageState();
        std::this_thread::sleep_for(50ms);

        Pow::BatteryVoltState bvs { pow.voltState() };
        ensure("Value bad Power::batteryVoltageState", (bvs >= pow.unknown) & (bvs <= Pow::critical));
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void pow_tests::test<3>() {
        set_test_name("voltsCalibratedFiltered");

        pow.batteryVoltage(Pow::CalibratedFiltered);
        std::this_thread::sleep_for(50ms);

        ensure_equals("Value bad Power::voltsCalibratedFiltered", pow.voltsCalibratedFiltered(), 7.5, 2.0);
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void pow_tests::test<4>() {
        set_test_name("voltsCalibratedUnfiltered");

        pow.batteryVoltage(Pow::CalibratedUnfiltered);
        std::this_thread::sleep_for(50ms);

        ensure_equals("Value bad Power::voltsCalibratedUnfiltered", pow.voltsCalibratedUnfiltered(), 7.5, 2.0);
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void pow_tests::test<5>() {
        set_test_name("voltsUncalibratedUnfiltered");

        pow.batteryVoltage(Pow::UncalibratedUnfiltered);
        std::this_thread::sleep_for(50ms);

        ensure_equals("Value bad Power::voltsUncalibratedUnfiltered", pow.voltsUncalibratedUnfiltered(), 7.5, 2.0);
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void pow_tests::test<6>() {
        set_test_name("batteryVoltThresholds Critical");

        pow.batteryVoltThresholds();
        std::this_thread::sleep_for(50ms);

        ensure_equals("Value bad Power::batteryVoltThresholds Critical", pow.voltThresholdCritical(), 6.5, 0.005);
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void pow_tests::test<7>() {
        set_test_name("batteryVoltThresholds Low");

        pow.batteryVoltThresholds();
        std::this_thread::sleep_for(50ms);

        ensure_equals("Value bad Power::batteryVoltThresholds Low", pow.voltThresholdLow(), 7.0, 0.005);
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void pow_tests::test<8>() {
        set_test_name("batteryVoltThresholds Hysteresis");

        pow.batteryVoltThresholds();
        std::this_thread::sleep_for(50ms);

        ensure_equals("Value bad Power::batteryVoltThresholds Hysteresis", pow.voltThresholdHysteresis(), 0.2, 0.005);
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void pow_tests::test<9>() {
        set_test_name("batteryMotorCurrent Left");

        pow.batteryMotorCurrent(Pow::left);
        std::this_thread::sleep_for(50ms);

        ensure_equals("Value bad Power::batteryMotorCurrent Left", pow.motorCurrent(Pow::left), 0.0, 0.005);
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void pow_tests::test<10>() {
        set_test_name("batteryMotorCurrent Right");

        pow.batteryMotorCurrent(Pow::right);
        std::this_thread::sleep_for(50ms);

        ensure_equals("Value bad Power::batteryMotorCurrent Right", pow.motorCurrent(Pow::right), 0.0, 0.005);
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void pow_tests::test<11>() {
        set_test_name("enableBatteryStateChange");
//        mys::TraceOn t_on(terr);

        pow.enableBatteryStateChange();
        std::this_thread::sleep_for(50ms);

        ensure("Value bad Power::enableBatteryStateChange", pow.isBatteryStateChangeEnabled());
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void pow_tests::test<12>() {
        set_test_name("disableBatteryStateChange");
//        mys::TraceOn t_on(terr);

        pow.disableBatteryStateChange();
        std::this_thread::sleep_for(50ms);

        ensure("Value bad Power::disableBatteryStateChange", !pow.isBatteryStateChangeEnabled());
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void pow_tests::test<14>() {
        set_test_name("isDidSleepNotify");
//        mys::TraceOn t_on(terr);

        pow.sleep();
        std::this_thread::sleep_for(4000ms);
        ensure("Value bad Power::isDidSleepNotify", pow.isDidSleepNotify());
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void pow_tests::test<15>() {
        set_test_name("isWakeNotify");
//        mys::TraceOn t_on(terr);

        pow.awake();
        std::this_thread::sleep_for(100ms);
        ensure("Value bad Power::isWakeNotify", pow.isWakeNotify());
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void pow_tests::test<16>() {
        set_test_name("resetWakeNotify");
//        mys::TraceOn t_on(terr);

        pow.resetWakeNotify();
        ensure("Value bad Power::resetWakeNotify", !pow.isWakeNotify());
    }

}	// namespace end
