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
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <future>
#include <version>

using namespace std::literals;

#include "Trace.h"
#include "Blackboard.h"
#include "Response.h"

#include "ApiShell.h"
#include "Connection.h"
#include "Drive.h"
#include "IoLed.h"
#include "Power.h"
#include "SensorsDirect.h"
#include "SensorsStream.h"
#include "SystemInfo.h"

#include "RosSensors.h"
//---------------------------------------------------------------------------------------------------------------------
mys::TraceStart terr { std::cerr };
mys::TraceStart tout { std::cout };
//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void opt_output(std::string const& text, std::optional<T> v) {
    terr << code_loc << text << mys::sp << (v ? v.value() : T { });
}
//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void opt_output_hex(std::string const& text, std::optional<T> v) {
    terr << code_loc << text << mys::sp << std::hex << (v ? v.value() : T { });
}

//---------------------------------------------------------------------------------------------------------------------
rvr::TripleFloat quat2angles(rvr::QuatData q) {
    float x { -q.x };
    float y { -q.y };
    float z { -q.z };
    float w { q.w };
    float x_angle = std::atan2( +2.0 * (y * z - w * x), 1.0 - 2.0 * (x * x + y * y)) * 180 / M_PI;
    float y_angle = std::atan2( -2.0 * (x * z + w * y), 1.0 - 2.0 * (x * x + y * y)) * 180 / M_PI;
    float z_angle = std::atan2( -2.0 * (x * y + w * z), 1.0 - 2.0 * (x * x + z * z)) * 180 / M_PI;
    return {x_angle, y_angle, z_angle};
}
//---------------------------------------------------------------------------------------------------------------------

void twist(float const linear, float angular, rvr::Drive& drive, rvr::SensorsStream& sen_s) {
    rvr::ImuData imu { sen_s.imu().value_or(rvr::ImuData { }) };

    static float rot { };
    static int cnt { };
    if (abs(imu.yaw) < angular) {
        rot = -imu.yaw + (angular / 20);
        drive.driveWithHeading(linear, rot);
    }
    else {
        drive.driveWithHeading(linear, angular);
    }
//    cnt = (cnt + 1) % 3;
    terr << code_loc << "Rotation: " << cnt << mys::sp << rot << mys::sp << angular;
}
//---------------------------------------------------------------------------------------------------------------------
void twist2(float const linear, float angular, rvr::Drive& drive) {
//    constexpr float wr { .0035 };
//    constexpr float ws { .018 };
//    angular = (angular * ws / 2.0) / wr;
    angular *= 15.8;

    float const vel_left = linear - angular;
    float const vel_right = linear + angular;
    drive.drive(vel_left, vel_right);
    terr << code_loc << "Velocity: " << angular << mys::sp << vel_left << mys::sp << vel_right;
}
//---------------------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
//    mys::TraceOff terr_off { terr };
    terr << code_loc << " Opening serial " << argv[1] << std::setprecision(4) << std::fixed << std::boolalpha;

    SerialPort serial { argv[1], 115200 };
    rvr::SendPacket req { serial };
    rvr::ReadPacket in_packet { serial };
    rvr::Blackboard bb;

//---------------------------------------------------------------------------------------------------------------------
//  Setup the thread to read responses
    std::promise<void> end_tasks;
    std::shared_future<void> end_future(end_tasks.get_future());
    rvr::Response resp { in_packet, bb, end_future };

    auto resp_future = std::async(std::launch::async, std::ref(resp));

    rvr::Power pow(bb, req);

    pow.awake();
    std::this_thread::sleep_for(500ms);
//---------------------------------------------------------------------------------------------------------------------
    rvr::SensorsStream sen_s(bb, req);
    try {
        //---------------------------------------------------------------------------------------------------------------------
        //  Setup the LED handling
        rvr::Drive drive(bb, req);
        rvr::IoLed led(bb, req);
        rvr::SensorsDirect sen_d(bb, req);

        led.ledsOff();

#if 1
        drive.resetYaw();

        sen_d.resetLocatorXY();
        sen_d.setLocatorFlags(true);

        //  turn off all streaming from previous possible aborted runs
        sen_s.disableStreaming();
        sen_s.clearStreaming();
        std::this_thread::sleep_for(50ms);

        // setup streaming for all pose information
        sen_s.streamImuAccelGyro();
        sen_s.streamSpeedVelocityLocator();
        sen_s.streamQuaternion();

        constexpr uint16_t stream_period { 50 };
        sen_s.enableStreaming(stream_period);

        std::this_thread::sleep_for(std::chrono::milliseconds(stream_period * 2));

        rvr::AccelData a { sen_s.accelerometer().value_or(rvr::AccelData { }) };
        terr << code_loc << "accelerometer: " << a.x << mys::sp << a.y << mys::sp << a.z;

        rvr::GyroData g { sen_s.gyroscope().value_or(rvr::GyroData { }) };
        terr << code_loc << "gyroscope: " << g.x << mys::sp << g.y << mys::sp << g.z;

        rvr::ImuData imu { sen_s.imu().value_or(rvr::ImuData { }) };
        terr << code_loc << "imu: " << imu.pitch << mys::sp << imu.roll << mys::sp << imu.yaw;

        rvr::LocatorData loc { sen_s.locator().value_or(rvr::LocatorData { }) };
        terr << code_loc << "locator: " << loc.x << mys::sp << loc.y;

        opt_output("Speed"s, sen_s.speed());

        rvr::VelocityData v { sen_s.velocity().value_or(rvr::VelocityData { }) };
        terr << code_loc << "Velocity: " << v.x << mys::sp << v.y;

        rvr::QuatData q { sen_s.quaternion().value_or(rvr::QuatData { }) };
        terr << code_loc << "quaternion: " << q.w << mys::sp << q.x << mys::sp << q.y << mys::sp << q.z;

        loc = sen_s.locator().value_or(rvr::LocatorData { });
        terr << code_loc << "locator: " << loc.x << mys::sp << loc.y;
#endif

//        sen_d.calibrateMagnetometer();
//        std::this_thread::sleep_for(3000ms);
//        sen_d.getMagnetometerData();
//        std::optional<bool> msg_cal_done = sen_d.isMagnetometerCalibrationDone();
//        terr << code_loc << "mag calibrate " << msg_cal_done.value();

#if 1
//        float prev_yaw { };
        double sp { 50 }; // 49.6
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
//        while (loc.y < .5)
//        while (abs(imu.yaw) < 90)
//        while (loc.y < 1.0)
        {
//            drive.drive(sp, sp);
//            drive.driveWithHeading(sp, 0);
//            twist(sp, 0, drive);
//            twist(sp, 90, drive, sen_s);
            std::this_thread::sleep_for(std::chrono::milliseconds(stream_period));

            loc = sen_s.locator().value_or(rvr::LocatorData { });
            imu = { sen_s.imu().value_or(rvr::ImuData { }) };
            a = sen_s.accelerometer().value_or(rvr::AccelData { });
            v = sen_s.velocity().value_or(rvr::VelocityData { });
            g = sen_s.gyroscope().value_or(rvr::GyroData { });
            q = sen_s.quaternion().value_or(rvr::QuatData { });

//            terr << code_loc << "locator: " << loc.x << mys::sp << loc.y;
            terr << code_loc << "Velocity: " << v.x << mys::sp << v.y << mys::sp << imu.yaw;
            terr << code_loc << "accelerometer: " << a.x << mys::sp << a.y << mys::sp << a.z;
            terr << code_loc << " imu: " << imu.pitch << mys::sp << imu.roll << mys::sp << imu.yaw;
//            terr << code_loc << "gyroscope: " << angles.x << mys::sp << angles.y << mys::sp << angles.z;
            terr << code_loc << "yaw: " << imu.yaw << mys::sp << loc.y;
            terr << code_loc << "quat: " << q.w << mys::sp << q.x << mys::sp << q.y << mys::sp << q.z;
            opt_output("Speed"s, sen_s.speed());

//            rvr::TripleFloat angles { quat2angles(q) };
//            terr << code_loc << " ang: " << angles.x << mys::sp << angles.y << mys::sp << angles.z;
        }
//        std::this_thread::sleep_for(300ms);
//        bb.m_to_v();
#endif

        drive.stop(0);
        std::this_thread::sleep_for(300ms);

//        loc = sen_s.locator().value_or(rvr::LocatorData { });
//        terr << code_loc << "locator: " << loc.x << mys::sp << loc.y;

//        terr << code_loc;
//        terr << code_loc;
//        while (loc.y > 0.0) {
////            drive.drive( -sp, -sp);
//            drive.driveWithHeading( -sp, 0);
//            std::this_thread::sleep_for(std::chrono::milliseconds(stream_period));
//            loc = sen_s.locator().value_or(rvr::LocatorData { });
////            terr << code_loc << "locator: " << loc.x << mys::sp << loc.y;
//        }
//        drive.stop(0);
////        std::this_thread::sleep_for(std::chrono::milliseconds(stream_period));
////
//        loc = sen_s.locator().value_or(rvr::LocatorData { });
//        terr << code_loc << "locator: " << loc.x << mys::sp << loc.y;

    }
    catch (std::exception& e) {
        terr << code_loc << e.what() << "=================================";
    }
    sen_s.disableStreaming();
    sen_s.clearStreaming();

    pow.sleep();
    terr << code_loc << "----------------" << mys::nl;

    std::this_thread::sleep_for(1s);
    end_tasks.set_value();
    resp_future.get();

    bb.m_to_v();

    return 0;
}

