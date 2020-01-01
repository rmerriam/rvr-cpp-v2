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
#include "SensorsStream.h"

using stream = rvr::SensorsStream;

//---------------------------------------------------------------------------------------------------------------------
extern rvr::SendPacket* packet_send;
extern rvr::Blackboard* blackboard;

struct stream_test_data {
    rvr::Blackboard* bb;
    rvr::SensorsStream stream { *bb, *packet_send };
};
//=====================================================================================================================
namespace tut {

    using t_group = test_group<stream_test_data>;

    t_group stream("Sensors Stream Test");

    using stream_tests = t_group::object;
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void stream_tests::test<1>() {
        set_test_name("accelConfig");
//        mys::TraceOn t_on(terr);
        stream.clearStreaming();
        std::this_thread::sleep_for(50ms);

        stream.streamImuAccelGyro();

        stream.enableStreaming(50);
        std::this_thread::sleep_for(100ms);

        stream.disableStreaming();

        auto [x, y, z] = stream.accelerometer();
        ensure_distance("Value bad SensorsStream::accelConfig x", x, 0.0f, 16.0f);
        ensure_distance("Value bad SensorsStream::accelConfig y", y, 0.0f, 16.0f);
        ensure_distance("Value bad SensorsStream::accelConfig z", z, 0.0f, 16.0f);
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void stream_tests::test<2>() {
        set_test_name("ambientConfig");
//        mys::TraceOn t_on(terr);
        stream.clearStreaming();
        std::this_thread::sleep_for(50ms);

        stream.streamAmbient();

        stream.enableStreaming(50);
        std::this_thread::sleep_for(100ms);

        stream.disableStreaming();

        ensure_equals("Value bad SensorsStream::ambientConfig", stream.ambient(), 12000.0 / 2, 120000.0 / 2);
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void stream_tests::test<3>() {
        set_test_name("gyroConfig");
//        mys::TraceOn t_on(terr);
        stream.clearStreaming();
        std::this_thread::sleep_for(50ms);

        stream.streamImuAccelGyro();

        stream.enableStreaming(50);
        std::this_thread::sleep_for(100ms);

        stream.disableStreaming();

        auto [x, y, z] = stream.gyroscope();
        ensure_distance("Value bad SensorsStream::gyroConfig x", x, 0.0f, 2000.0f);
        ensure_distance("Value bad SensorsStream::gyroConfig y", y, 0.0f, 2000.0f);
        ensure_distance("Value bad SensorsStream::gyroConfig z", z, 0.0f, 2000.0f);
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void stream_tests::test<4>() {
        set_test_name("imuConfig");
        //        mys::TraceOn t_on(terr);
        stream.clearStreaming();
        std::this_thread::sleep_for(50ms);

        stream.streamImuAccelGyro();

        stream.enableStreaming(50);
        std::this_thread::sleep_for(100ms);

        stream.disableStreaming();

        auto [pitch, roll, yaw] = stream.imu();
        ensure_distance("Value bad SensorsStream::imuConfig pitch", pitch, 0.0f, 180.0f);
        ensure_distance("Value bad SensorsStream::imuConfig roll", roll, 0.0f, 90.0f);
        ensure_distance("Value bad SensorsStream::imuConfig yaw", yaw, 0.0f, 180.0f);
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void stream_tests::test<5>() {
        set_test_name("quaternionConfig");
        //        mys::TraceOn t_on(terr);
        stream.clearStreaming();
        std::this_thread::sleep_for(50ms);

        stream.streamQuaternion();

        stream.enableStreaming(50);
        std::this_thread::sleep_for(100ms);

        stream.disableStreaming();

        auto [w, x, y, z] = stream.quaternion();
        ensure_distance("Value bad SensorsStream::quaternionConfig w", w, 0.0f, 1.0f);
        ensure_distance("Value bad SensorsStream::quaternionConfig x", x, 0.0f, 1.0f);
        ensure_distance("Value bad SensorsStream::quaternionConfig y", y, 0.0f, 1.0f);
        ensure_distance("Value bad SensorsStream::quaternionConfig z", z, 0.0f, 1.0f);
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void stream_tests::test<6>() {
        set_test_name("locatorConfig");
//        mys::TraceOn t_on(terr);
        stream.clearStreaming();
        std::this_thread::sleep_for(50ms);

        stream.streamSpeedVelocityLocator();

        stream.enableStreaming(50);
        std::this_thread::sleep_for(100ms);

        stream.disableStreaming();

        auto [x, y] = stream.locator();
        ensure_distance("Value bad SensorsStream::locatorConfig x", x, 0.0f, 16000.0f);
        ensure_distance("Value bad SensorsStream::locatorConfig y", y, 0.0f, 16000.0f);
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void stream_tests::test<7>() {
        set_test_name("speedConfig");
//        mys::TraceOn t_on(terr);
        stream.clearStreaming();
        std::this_thread::sleep_for(50ms);

        stream.streamSpeedVelocityLocator();

        stream.startStreamingNordic(50);
        std::this_thread::sleep_for(100ms);

        stream.disableStreamingBT();

        ensure_distance("Value bad SensorsStream::speedConfig", stream.speed(), 5.0f / 2, 5.0f);
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void stream_tests::test<8>() {
        set_test_name("velocityConfig");
//        mys::TraceOn t_on(terr);
        stream.clearStreaming();
        std::this_thread::sleep_for(50ms);

        stream.streamSpeedVelocityLocator();

        stream.startStreamingBT(50);
        std::this_thread::sleep_for(100ms);

        stream.disableStreamingBT();

        auto [x, y] = stream.velocity();
        ensure_distance("Value bad SensorsStream::velocityConfig x", x, 0.0f, 5.0f);
        ensure_distance("Value bad SensorsStream::velocityConfig y", y, 0.0f, 5.0f);
    }
}   // namespace end
