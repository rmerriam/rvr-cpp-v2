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
#include <rvr++.h>
//---------------------------------------------------------------------------------------------------------------------
void nordic_ambient_light(rvr::SensorsStream& sen_s) {
    mys::tinfo << code_loc;

    sen_s.disableStreamingNordic();
    std::this_thread::sleep_for(50ms);

    sen_s.streamAmbient(rvr::CommandResponse::resp_yes);

    sen_s.startStreamingNordic(50, rvr::CommandResponse::resp_yes);
    std::this_thread::sleep_for(100ms);

    sen_s.disableStreamingNordic();

    mys::tinfo << code_loc << "ambient: "s << sen_s.ambient().value();
    mys::tinfo << code_loc << mys::nl;
}
//---------------------------------------------------------------------------------------------------------------------
void nordic_color(rvr::SensorsStream& sen_s) {
    mys::tdbg << code_entry;
    mys::tinfo << code_loc;

    sen_s.disableStreamingNordic();
    sen_s.enableColorDetection(rvr::CommandResponse::resp_yes);
    std::this_thread::sleep_for(500ms);

    sen_s.streamColor(rvr::CommandResponse::resp_yes);

    sen_s.startStreamingNordic(50, rvr::CommandResponse::resp_yes);
    std::this_thread::sleep_for(100ms);

    sen_s.disableStreamingNordic();
    sen_s.disableColorDetection(rvr::CommandResponse::resp_yes);
    std::this_thread::sleep_for(100ms);

    auto [d_r, d_g, d_b, index, conf] { sen_s.colors().value() };
    mys::tinfo << code_loc << "streaming colors: " << std::hex << (int)(d_r) << mys::sp << (int)(d_g) << mys::sp
               << (int)(d_b) << mys::sp << (int)(index) << mys::sp << conf;
    mys::tinfo << code_loc << mys::nl;
}
//---------------------------------------------------------------------------------------------------------------------
void nordic_core_time(rvr::SensorsStream& sen_s) {
    mys::tdbg << code_entry;
    mys::tinfo << code_loc;

    sen_s.disableStreamingNordic();
    std::this_thread::sleep_for(50ms);

    sen_s.streamNordicTime(rvr::CommandResponse::resp_yes);

    sen_s.startStreamingNordic(50, rvr::CommandResponse::resp_yes);
    std::this_thread::sleep_for(100ms);

    sen_s.disableStreamingNordic();

    auto ct { sen_s.nordicTime() };
    mys::tdbg << code_loc;
    mys::tinfo << code_loc << "Nordic Core Time: "s << ct;
    mys::tinfo << code_loc << mys::nl;
}
//---------------------------------------------------------------------------------------------------------------------
void bt_core_time(rvr::SensorsStream& sen_s) {
    mys::tdbg << code_entry;
    mys::tinfo << code_loc;

    sen_s.disableStreamingNordic();
    std::this_thread::sleep_for(50ms);

    sen_s.streamBtTime(rvr::CommandResponse::resp_yes);

    sen_s.startStreamingBT(50, rvr::CommandResponse::resp_yes);
    std::this_thread::sleep_for(100ms);

    std::this_thread::sleep_for(100ms);
    sen_s.disableStreamingBT();

    auto ct { sen_s.btTime() };
    mys::tinfo << code_loc << "BT Core Time: "s << ct;
    mys::tinfo << code_loc << mys::nl;
}
//---------------------------------------------------------------------------------------------------------------------
void bt_imu_accel_gyro(rvr::SensorsStream& sen_s) {
    mys::tdbg << code_entry;
    mys::tinfo << code_loc;

    sen_s.disableStreamingBT();
    std::this_thread::sleep_for(50ms);

    sen_s.streamImuAccelGyro(rvr::CommandResponse::resp_yes);

    sen_s.startStreamingBT(50, rvr::CommandResponse::resp_yes);
    std::this_thread::sleep_for(100ms);

    sen_s.disableStreamingBT();

    std::this_thread::sleep_for(100ms);
    mys::tdbg << code_entry;

    auto [a_x, a_y, a_z] { sen_s.accelerometer().value_or(rvr::AccelData { }) };
    mys::tinfo << code_loc << "accelerometer: " << a_x << mys::sp << a_y << mys::sp << a_z;

    auto [g_x, g_y, g_z] { sen_s.gyroscope().value_or(rvr::GyroData { }) };
    mys::tinfo << code_loc << "gyroscope: " << g_x << mys::sp << g_y << mys::sp << g_z;

    auto [i_x, i_y, i_z] { sen_s.imu().value_or(rvr::ImuData { }) };
    mys::tinfo << code_loc << "imu: " << i_x << mys::sp << i_y << mys::sp << i_z;
    mys::tinfo << code_loc << mys::nl;
}
//---------------------------------------------------------------------------------------------------------------------
void bt_speed_vel_loc(rvr::SensorsStream& sen_s) {
    mys::tdbg << code_entry;
    mys::tinfo << code_loc;

    sen_s.disableStreamingBT();
    std::this_thread::sleep_for(50ms);

    sen_s.streamSpeedVelocityLocator(rvr::CommandResponse::resp_yes);

    sen_s.startStreamingBT(50, rvr::CommandResponse::resp_yes);
    std::this_thread::sleep_for(100ms);

    sen_s.disableStreamingBT();

    std::this_thread::sleep_for(100ms);

    auto [l_x, l_y] { sen_s.locator().value_or(rvr::LocatorData { }) };
    mys::tinfo << code_loc << "locator: " << l_x << mys::sp << l_y;

    mys::tinfo << code_loc << "tspeed: "s << sen_s.speed().value();

    auto [v_x, v_y] { sen_s.velocity().value_or(rvr::VelocityData { }) };
    mys::tinfo << code_loc << "Velocity: " << v_x << mys::sp << v_y;
    mys::tinfo << code_loc << mys::nl;
}
//---------------------------------------------------------------------------------------------------------------------
void bt_quaternion(rvr::SensorsStream& sen_s) {
    mys::tdbg << code_entry;
    mys::tinfo << code_loc;

    sen_s.disableStreamingBT();
    std::this_thread::sleep_for(50ms);

    sen_s.streamQuaternion(rvr::CommandResponse::resp_yes);

    sen_s.startStreamingBT(50, rvr::CommandResponse::resp_yes);
    std::this_thread::sleep_for(100ms);

    sen_s.disableStreamingBT();

//    std::this_thread::sleep_for(100ms);

    auto [q_w, q_x, q_y, q_z] { sen_s.quaternion().value_or(rvr::QuatData { }) };
//    std::this_thread::sleep_for(50ms);

    mys::tdbg << code_loc;
    mys::tinfo << code_loc << "quaternion: " << q_w << mys::sp << q_x << mys::sp << q_y << mys::sp << q_z;
    mys::tinfo << code_loc << mys::nl;
}
//---------------------------------------------------------------------------------------------------------------------
void streaming(rvr::SensorsStream& sen_s) {
    mys::tdbg << code_entry;
    mys::tinfo << code_loc;

    sen_s.disableAllStreaming();
    sen_s.clearAllStreaming();

    nordic_ambient_light(sen_s);
//    nordic_color(sen_s);
//    nordic_core_time(sen_s);

//    bt_imu_accel_gyro(sen_s);
//    bt_speed_vel_loc(sen_s);
    bt_quaternion(sen_s);
//    bt_core_time(sen_s);

    sen_s.disableAllStreaming();
    sen_s.clearAllStreaming();
    std::this_thread::sleep_for(50ms);
}

