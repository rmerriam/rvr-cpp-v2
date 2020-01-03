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
//     Created: Nov 8, 2019
//
//======================================================================================================================
#include <chrono>
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
//---------------------------------------------------------------------------------------------------------------------
mys::TraceStart terr { std::cerr };
mys::TraceStart tout { std::cout };
//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void opt_output(std::string const& text, std::optional<T> v) {
    tout << code_loc << text << mys::sp << (v ? v.value() : T { });
}
//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void opt_output_hex(std::string const& text, std::optional<T> v) {
    tout << code_loc << text << mys::sp << std::hex << (v ? v.value() : T { });
}
//---------------------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
//    mys::TraceOff terr_off { terr };
    tout << code_loc << " Opening serial " << argv[1] << std::setprecision(4) << std::fixed << std::boolalpha;

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

        sen_d.resetLocatorXY();
        sen_d.setLocatorFlags(true);

        //  turn off all streaming from previous possible aborted runs
        sen_s.disableStreaming();
        sen_s.clearStreaming();
        std::this_thread::sleep_for(50ms);

        // setup streaming for all pose information
//        sen_s.streamImuAccelGyro();
        sen_s.streamSpeedVelocityLocator();
//        sen_s.streamQuaternion();

        constexpr uint16_t stream_period { 30 };
        sen_s.enableStreaming(stream_period);

        auto [a_x, a_y, a_z] { sen_s.accelerometer().value_or(rvr::AccelData { }) };
        tout << code_loc << "accelerometer: " << a_x << mys::sp << a_y << mys::sp << a_z;

        auto [g_x, g_y, g_z] { sen_s.gyroscope().value_or(rvr::GyroData { }) };
        tout << code_loc << "gyroscope: " << g_x << mys::sp << g_y << mys::sp << g_z;

        auto [i_x, i_y, i_z] { sen_s.imu().value_or(rvr::ImuData { }) };
        tout << code_loc << "imu: " << i_x << mys::sp << i_y << mys::sp << i_z;

        rvr::LocatorData loc { sen_s.locator().value_or(rvr::LocatorData { }) };
        tout << code_loc << "locator: " << loc.x << mys::sp << loc.y;

        opt_output("Speed"s, sen_s.speed());

        auto [v_x, v_y] { sen_s.velocity().value_or(rvr::VelocityData { }) };
        tout << code_loc << "Velocity: " << v_x << mys::sp << v_y;

//        auto [q_w, q_x, q_y, q_z] { sen_s.quaternion().value_or(rvr::QuatData { }) };
//        tout << code_loc << "quaternion: " << q_w << mys::sp << q_x << mys::sp << q_y << mys::sp << q_z;

        drive.resetYaw();

        loc = sen_s.locator().value_or(rvr::LocatorData { });
        tout << code_loc << "locator: " << loc.x << mys::sp << loc.y;

        double sp { 15 };
        while (loc.y < 1.0) {
//            drive.drive(sp, sp);
            drive.driveWithHeading(sp, 0);
            std::this_thread::sleep_for(std::chrono::milliseconds(stream_period));
            loc = sen_s.locator().value_or(rvr::LocatorData { });
            tout << code_loc << "locator: " << loc.x << mys::sp << loc.y;
        }

        drive.stop(0);
        std::this_thread::sleep_for(300ms);

        loc = sen_s.locator().value_or(rvr::LocatorData { });
        tout << code_loc << "locator: " << loc.x << mys::sp << loc.y;
        tout << code_loc;
        tout << code_loc;
        while (loc.y > 0.0) {
//            drive.drive( -sp, -sp);
            drive.driveWithHeading( -sp, 0);
            std::this_thread::sleep_for(std::chrono::milliseconds(stream_period));
            loc = sen_s.locator().value_or(rvr::LocatorData { });
            tout << code_loc << "locator: " << loc.x << mys::sp << loc.y;
        }
        drive.stop(0);
//        std::this_thread::sleep_for(std::chrono::milliseconds(stream_period));
//
//        loc = sen_s.locator().value_or(rvr::LocatorData { });
//        tout << code_loc << "locator: " << loc.x << mys::sp << loc.y;

    }
    catch (std::exception& e) {
        tout << code_loc << e.what() << "=================================";
    }
    sen_s.disableStreaming();
    sen_s.clearStreaming();

    pow.sleep();
    tout << code_loc << "----------------" << mys::nl;

    std::this_thread::sleep_for(1s);
    end_tasks.set_value();
    resp_future.get();

//    bb.m_to_v();

    return 0;
}

