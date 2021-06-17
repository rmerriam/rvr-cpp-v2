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
//		 File: streaming.cpp
//
//     Author: rmerriam
//
//    Created: Jun 16, 2021
//
//======================================================================================================================
#include <Trace.h>
#include <rvr++.h>

#include "opt_output.h"
//---------------------------------------------------------------------------------------------------------------------
void nordic_ambient(rvr::SensorsStream& sen_s) {
    std::cout << code_loc << __func__ << mys::nl;

    sen_s.disableStreamingNordic();
    std::this_thread::sleep_for(50ms);

    sen_s.streamAmbient(rvr::CommandResponse::resp_yes);

    sen_s.startStreamingNordic(50, rvr::CommandResponse::resp_yes);
    std::this_thread::sleep_for(100ms);

    sen_s.disableStreamingNordic();

    opt_output("Ambient"s, sen_s.ambient());
    std::cout << code_loc << mys::nl;
}
//---------------------------------------------------------------------------------------------------------------------
void nordic_color(rvr::SensorsStream& sen_s) {
    std::cout << code_loc << mys::nl;

    sen_s.disableStreamingNordic();
    sen_s.enableColorDetection(rvr::CommandResponse::resp_yes);
    std::this_thread::sleep_for(500ms);

    sen_s.streamColor(rvr::CommandResponse::resp_yes);

    sen_s.startStreamingNordic(50, rvr::CommandResponse::resp_yes);
    std::this_thread::sleep_for(100ms);

    sen_s.disableStreamingNordic();
    sen_s.disableColorDetection(rvr::CommandResponse::resp_yes);
    std::this_thread::sleep_for(100ms);

    std::cout << code_loc << mys::nl;
    auto [d_r, d_g, d_b, index, conf] { sen_s.colors().value() };
    std::cout << code_loc << "streaming colors: " << std::hex << (int)(d_r) << mys::sp << (int)(d_g) << mys::sp << (int)(d_b)
              << mys::sp << (int)(index) << mys::sp << conf << mys::nl;
    std::cout << std::dec << code_loc << mys::nl;
}
//---------------------------------------------------------------------------------------------------------------------
void nordic_core_time(rvr::SensorsStream& sen_s) {
    std::cout << code_loc << mys::nl;

    sen_s.disableStreamingNordic();
    std::this_thread::sleep_for(50ms);

    sen_s.streamNordicTime(rvr::CommandResponse::resp_yes);

    sen_s.startStreamingNordic(50, rvr::CommandResponse::resp_yes);
    std::this_thread::sleep_for(100ms);

    sen_s.disableStreamingNordic();

    auto ct { sen_s.nordicTime() };
    std::cout << code_loc << mys::nl;
    std::cout << code_loc << "Nordic Core Time: "s << ct << mys::nl;
    std::cout << code_loc << mys::nl;

}
//---------------------------------------------------------------------------------------------------------------------
void bt_core_time(rvr::SensorsStream& sen_s) {
    std::cout << code_loc << __func__ << mys::nl;

    sen_s.disableStreamingNordic();
    std::this_thread::sleep_for(50ms);

    sen_s.streamBtTime(rvr::CommandResponse::resp_yes);

    sen_s.startStreamingBT(50, rvr::CommandResponse::resp_yes);
    std::this_thread::sleep_for(100ms);

    std::this_thread::sleep_for(100ms);
    sen_s.disableStreamingBT();

    auto ct { sen_s.btTime() };
    std::cout << code_loc << mys::nl;
    std::cout << code_loc << "BT Core Time: "s << ct << mys::nl;
    std::cout << code_loc << mys::nl;
}
//---------------------------------------------------------------------------------------------------------------------
void bt_imu_accel_gyro(rvr::SensorsStream& sen_s) {
    std::cout << code_loc << __func__ << mys::nl;

    sen_s.disableStreamingBT();
    std::this_thread::sleep_for(50ms);

    sen_s.streamImuAccelGyro(rvr::CommandResponse::resp_yes);

    sen_s.startStreamingBT(50, rvr::CommandResponse::resp_yes);
    std::this_thread::sleep_for(100ms);

    sen_s.disableStreamingBT();

    std::this_thread::sleep_for(100ms);
    std::cout << code_loc << mys::nl;

    auto [a_x, a_y, a_z] { sen_s.accelerometer().value_or(rvr::AccelData { }) };
    std::cout << code_loc << "accelerometer: " << a_x << mys::sp << a_y << mys::sp << a_z << mys::nl;

    auto [g_x, g_y, g_z] { sen_s.gyroscope().value_or(rvr::GyroData { }) };
    std::cout << code_loc << "gyroscope: " << g_x << mys::sp << g_y << mys::sp << g_z << mys::nl;

    auto [i_x, i_y, i_z] { sen_s.imu().value_or(rvr::ImuData { }) };
    std::cout << code_loc << "imu: " << i_x << mys::sp << i_y << mys::sp << i_z << mys::nl;
    std::cout << code_loc << mys::nl;
}
//---------------------------------------------------------------------------------------------------------------------
void bt_speed_vel_loc(rvr::SensorsStream& sen_s) {
    std::cout << code_loc << __func__ << mys::nl;

    sen_s.disableStreamingBT();
    std::this_thread::sleep_for(50ms);

    sen_s.streamSpeedVelocityLocator(rvr::CommandResponse::resp_yes);

    sen_s.startStreamingBT(50, rvr::CommandResponse::resp_yes);
    std::this_thread::sleep_for(100ms);

    sen_s.disableStreamingBT();

    std::this_thread::sleep_for(100ms);

    std::cout << code_loc << mys::nl;
    auto [l_x, l_y] { sen_s.locator().value_or(rvr::LocatorData { }) };
    std::cout << code_loc << "locator: " << l_x << mys::sp << l_y << mys::nl;

    opt_output("Speed"s, sen_s.speed());

    auto [v_x, v_y] { sen_s.velocity().value_or(rvr::VelocityData { }) };
    std::cout << code_loc << "Velocity: " << v_x << mys::sp << v_y << mys::nl;

    std::cout << code_loc << mys::nl;
}
//---------------------------------------------------------------------------------------------------------------------
void bt_quaternion(rvr::SensorsStream& sen_s) {
    std::cout << code_loc << __func__ << mys::nl;

    sen_s.disableStreamingBT();
    std::this_thread::sleep_for(50ms);

    sen_s.streamQuaternion(rvr::CommandResponse::resp_yes);

    sen_s.startStreamingBT(50, rvr::CommandResponse::resp_yes);
    std::this_thread::sleep_for(100ms);

    sen_s.disableStreamingBT();

    std::this_thread::sleep_for(100ms);

    auto [q_w, q_x, q_y, q_z] { sen_s.quaternion().value_or(rvr::QuatData { }) };
    std::this_thread::sleep_for(50ms);

    std::cout << code_loc << mys::nl;
    std::cout << code_loc << "quaternion: " << q_w << mys::sp << q_x << mys::sp << q_y << mys::sp << q_z << mys::nl;
    std::cout << code_loc << mys::nl;
}
//---------------------------------------------------------------------------------------------------------------------
void streaming(rvr::SensorsStream& sen_s) {
    std::cout << code_loc << mys::nl;
    std::cout << code_loc << __func__ << mys::nl;

    sen_s.disableAllStreaming();
//    nordic_ambient(sen_s);
    nordic_color(sen_s);
//    nordic_core_time(sen_s);

//    bt_imu_accel_gyro(sen_s);
//    bt_speed_vel_loc(sen_s);
//    bt_quaternion(sen_s);
//    bt_core_time(sen_s);

    std::cout << code_loc << mys::nl;

    sen_s.disableAllStreaming();
    sen_s.clearAllStreaming();
    std::this_thread::sleep_for(50ms);

}

