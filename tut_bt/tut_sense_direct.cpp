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
#include "SensorsDirect.h"

using Sense = rvr::SensorsDirect;
//---------------------------------------------------------------------------------------------------------------------
extern rvr::Request* request;

struct sense_test_data {
    rvr::Blackboard* bb;
    rvr::SensorsDirect sense { *bb, *request };
};
//=====================================================================================================================
namespace tut {

    using t_group = test_group<sense_test_data>;

    t_group sense("Sensors Direct Test");

    using sense_tests = t_group::object;
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void sense_tests::test<1>() {
        set_test_name("enableGyroMaxNotify");
//        mys::TraceOn t_on(terr);

        sense.enableGyroMaxNotify();
        std::this_thread::sleep_for(50ms);

        ensure("Value bad SensorsDirect::enableGyroMaxNotify", sense.isGyroMaxNotifyEnabled());
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void sense_tests::test<2>() {
        set_test_name("disableGyroMaxNotify");
//        mys::TraceOn t_on(terr);

        sense.disableGyroMaxNotify();
        std::this_thread::sleep_for(50ms);

        ensure("Value bad SensorsDirect::disableGyroMaxNotify", !sense.isGyroMaxNotifyEnabled());
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void sense_tests::test<3>() {
        set_test_name("enableColorDetection");
        //        mys::TraceOn t_on(terr);
        // this has to run early because color detection needs to be enabled
        sense.enableColorDetection();
        std::this_thread::sleep_for(50ms);

        ensure("Value bad SensorsDirect::enableColorDetection", sense.isColorDetectionEnabled());
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void sense_tests::test<4>() {
        set_test_name("getRgbcSensorValue");
//        mys::TraceOn t_on(terr);

        sense.getRgbcSensorValue();
        std::this_thread::sleep_for(1000ms);
        auto [r, g, b, c] = sense.currentRGBValues();

        ensure_equals("Value bad SensorsDirect::getRgbcSensorValue r", r, 0, 2000);
        ensure_equals("Value bad SensorsDirect::getRgbcSensorValue g", g, 0, 2000);
        ensure_equals("Value bad SensorsDirect::getRgbcSensorValue b", b, 0, 2000);
        ensure_equals("Value bad SensorsDirect::getRgbcSensorValue c", c, 0, 6000);
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void sense_tests::test<5>() {
        set_test_name("getAmbienLightSensorValue");
        //        mys::TraceOn t_on(terr);

        sense.getAmbienLightSensorValue();
        std::this_thread::sleep_for(50ms);

        ensure_equals("Value bad SensorsDirect::getAmbienLightSensorValue", sense.ambient(), 0.0, 200000);
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void sense_tests::test<6>() {
        set_test_name("enableColorDetectionNotify");
        //        mys::TraceOn t_on(terr);
        // this has to run early because color detection needs to be enabled
        sense.enableColorDetectionNotify(true, 100, 0);
        std::this_thread::sleep_for(50ms);

        ensure("Value bad SensorsDirect::enableColorDetectionNotify", sense.isColorDetectionNotifyEnabled());
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void sense_tests::test<49>() {  // setup to run later but here to be with enable
        set_test_name("disableColorDetection");
//        mys::TraceOn t_on(terr);
        // this has to run early because color detection needs to be enabled
        sense.disableColorDetection();
        std::this_thread::sleep_for(50ms);

        ensure("Value bad SensorsDirect::disableColorDetection", !sense.isColorDetectionEnabled());
    }
}   // namespace end
