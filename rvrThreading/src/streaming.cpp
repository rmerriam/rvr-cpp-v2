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
#include <thread>

#include <rvr++.h>
//---------------------------------------------------------------------------------------------------------------------
void nordic_ambient_light(rvr::SensorsStream& sen_s) {
<<<<<<< HEAD
    mys::tout << code_line;
=======
   mys::tout << code_line;
>>>>>>> branch 'master' of https://rmerriam@bitbucket.org/rmerriam/rvr-cpp-v2.git

   sen_s.disableStreamingNordic();
   std::this_thread::sleep_for(50ms);

   sen_s.streamAmbient(rvr::CommandResponse::resp_yes);

   sen_s.startStreamingNordic(50, rvr::CommandResponse::resp_yes);
   std::this_thread::sleep_for(100ms);

   sen_s.disableStreamingNordic();

<<<<<<< HEAD
    mys::tout << code_line << "ambient: "s << sen_s.ambient().get_or();
    mys::tout << code_line;
=======
   mys::tout << code_line << "ambient: "s << sen_s.ambient().get();
   mys::tout << code_line;
>>>>>>> branch 'master' of https://rmerriam@bitbucket.org/rmerriam/rvr-cpp-v2.git
}
//---------------------------------------------------------------------------------------------------------------------
void nordic_color(rvr::SensorsStream& sen_s) {
<<<<<<< HEAD
    mys::tout << code_line;
=======
   mys::tout << code_line;
>>>>>>> branch 'master' of https://rmerriam@bitbucket.org/rmerriam/rvr-cpp-v2.git

   sen_s.disableStreamingNordic();
   sen_s.enableColorDetection(rvr::CommandResponse::resp_yes);
   std::this_thread::sleep_for(500ms);

   sen_s.streamColor(rvr::CommandResponse::resp_yes);

   sen_s.startStreamingNordic(50, rvr::CommandResponse::resp_yes);
   std::this_thread::sleep_for(100ms);

   sen_s.disableStreamingNordic();
   sen_s.disableColorDetection(rvr::CommandResponse::resp_yes);
   std::this_thread::sleep_for(100ms);

<<<<<<< HEAD
    auto [d_r, d_g, d_b, index, conf] { sen_s.colors().get_or() };
    mys::tout << code_line << "streaming colors: " << std::hex << std::showbase << (int)(d_r) << mys::sp << (int)(d_g)
               << mys::sp << (int)(d_b) << mys::sp << (int)(index) << std::noshowbase << mys::sp << conf;
    mys::tout << code_line;
=======
   auto [d_r, d_g, d_b, index, conf] { sen_s.colors().get() };
   mys::tout << code_line << "streaming colors: " << std::hex << std::showbase << (int)(d_r) << mys::sp << (int)(d_g) << mys::sp
              << (int)(d_b) << mys::sp << (int)(index) << std::noshowbase << mys::sp << conf;
   mys::tout << code_line;
>>>>>>> branch 'master' of https://rmerriam@bitbucket.org/rmerriam/rvr-cpp-v2.git
}
//---------------------------------------------------------------------------------------------------------------------
void nordic_core_time(rvr::SensorsStream& sen_s) {
<<<<<<< HEAD
    mys::tout << code_line;
=======
   mys::tout << code_line;
>>>>>>> branch 'master' of https://rmerriam@bitbucket.org/rmerriam/rvr-cpp-v2.git

   sen_s.disableStreamingNordic();
   std::this_thread::sleep_for(50ms);

   sen_s.streamNordicTime(rvr::CommandResponse::resp_yes);

   sen_s.startStreamingNordic(50, rvr::CommandResponse::resp_yes);
   std::this_thread::sleep_for(100ms);

   sen_s.disableStreamingNordic();

<<<<<<< HEAD
    auto ct { sen_s.nordicTime().get_or() };
=======
   auto ct { sen_s.nordicTime().get() };
>>>>>>> branch 'master' of https://rmerriam@bitbucket.org/rmerriam/rvr-cpp-v2.git

<<<<<<< HEAD
    mys::tout << code_line << "Nordic Core Time: "s << std::dec << ct;
    mys::tout << code_line;
=======
   mys::tout << code_line << "Nordic Core Time: "s << std::dec << ct;
   mys::tout << code_line;
>>>>>>> branch 'master' of https://rmerriam@bitbucket.org/rmerriam/rvr-cpp-v2.git
}
//---------------------------------------------------------------------------------------------------------------------
void bt_core_time(rvr::SensorsStream& sen_s) {
<<<<<<< HEAD
    mys::tout << code_line;
=======
   mys::tout << code_line;
>>>>>>> branch 'master' of https://rmerriam@bitbucket.org/rmerriam/rvr-cpp-v2.git

   sen_s.disableStreamingNordic();
   std::this_thread::sleep_for(50ms);

   sen_s.streamBtTime(rvr::CommandResponse::resp_yes);

   sen_s.startStreamingBT(50, rvr::CommandResponse::resp_yes);
   std::this_thread::sleep_for(100ms);

   std::this_thread::sleep_for(100ms);
   sen_s.disableStreamingBT();

<<<<<<< HEAD
    auto ct { sen_s.btTime().get_or() };
    mys::tout << code_line << "BT Core Time: "s << ct;
    mys::tout << code_line;
=======
   auto ct { sen_s.btTime().get() };
   mys::tout << code_line << "BT Core Time: "s << ct;
   mys::tout << code_line;
>>>>>>> branch 'master' of https://rmerriam@bitbucket.org/rmerriam/rvr-cpp-v2.git
}
//---------------------------------------------------------------------------------------------------------------------
void bt_imu_accel_gyro(rvr::SensorsStream& sen_s) {
<<<<<<< HEAD
    mys::tout << code_line;
=======
   mys::tout << code_line;
>>>>>>> branch 'master' of https://rmerriam@bitbucket.org/rmerriam/rvr-cpp-v2.git

   sen_s.disableStreamingBT();
   std::this_thread::sleep_for(50ms);

   sen_s.streamImuAccelGyro(rvr::CommandResponse::resp_yes);

   sen_s.startStreamingBT(50, rvr::CommandResponse::resp_yes);
   std::this_thread::sleep_for(100ms);

   sen_s.disableStreamingBT();

   std::this_thread::sleep_for(100ms);

<<<<<<< HEAD
    auto [a_x, a_y, a_z] { sen_s.accelerometer().get_or() };
    mys::tout << code_line << "accelerometer: " << a_x << mys::sp << a_y << mys::sp << a_z;
=======
   auto [a_x, a_y, a_z] { sen_s.accelerometer().get() };
   mys::tout << code_line << "accelerometer: " << a_x << mys::sp << a_y << mys::sp << a_z;
>>>>>>> branch 'master' of https://rmerriam@bitbucket.org/rmerriam/rvr-cpp-v2.git

<<<<<<< HEAD
    auto [g_x, g_y, g_z] { sen_s.gyroscope().get_or() };
    mys::tout << code_line << "gyroscope: " << g_x << mys::sp << g_y << mys::sp << g_z;
=======
   auto [g_x, g_y, g_z] { sen_s.gyroscope().get() };
   mys::tout << code_line << "gyroscope: " << g_x << mys::sp << g_y << mys::sp << g_z;
>>>>>>> branch 'master' of https://rmerriam@bitbucket.org/rmerriam/rvr-cpp-v2.git

<<<<<<< HEAD
    auto [i_x, i_y, i_z] { sen_s.imu().get_or() };
    mys::tout << code_line << "imu: " << i_x << mys::sp << i_y << mys::sp << i_z;
    mys::tout << code_line;
=======
   auto [i_x, i_y, i_z] { sen_s.imu().get() };
   mys::tout << code_line << "imu: " << i_x << mys::sp << i_y << mys::sp << i_z;
   mys::tout << code_line;
>>>>>>> branch 'master' of https://rmerriam@bitbucket.org/rmerriam/rvr-cpp-v2.git
}
//---------------------------------------------------------------------------------------------------------------------
void bt_speed_vel_loc(rvr::SensorsStream& sen_s) {
<<<<<<< HEAD
    mys::tout << code_line;
=======
   mys::tout << code_line;
>>>>>>> branch 'master' of https://rmerriam@bitbucket.org/rmerriam/rvr-cpp-v2.git

   sen_s.disableStreamingBT();
   std::this_thread::sleep_for(50ms);

   sen_s.streamSpeedVelocityLocator(rvr::CommandResponse::resp_yes);

   sen_s.startStreamingBT(50, rvr::CommandResponse::resp_yes);
   std::this_thread::sleep_for(100ms);

   sen_s.disableStreamingBT();

   std::this_thread::sleep_for(100ms);

<<<<<<< HEAD
    auto [l_x, l_y] { sen_s.locator().get_or() };
    mys::tout << code_line << "locator: " << l_x << mys::sp << l_y;
=======
   auto [l_x, l_y] { sen_s.locator().get() };
   mys::tout << code_line << "locator: " << l_x << mys::sp << l_y;
>>>>>>> branch 'master' of https://rmerriam@bitbucket.org/rmerriam/rvr-cpp-v2.git

<<<<<<< HEAD
    mys::tout << code_line << "speed: "s << sen_s.speed().get_or();
=======
   mys::tout << code_line << "speed: "s << sen_s.speed().get();
>>>>>>> branch 'master' of https://rmerriam@bitbucket.org/rmerriam/rvr-cpp-v2.git

<<<<<<< HEAD
    auto [v_x, v_y] { sen_s.velocity().get_or() };
    mys::tout << code_line << "velocity: " << v_x << mys::sp << v_y;
    mys::tout << code_line;
=======
   auto [v_x, v_y] { sen_s.velocity().get() };
   mys::tout << code_line << "velocity: " << v_x << mys::sp << v_y;
   mys::tout << code_line;
>>>>>>> branch 'master' of https://rmerriam@bitbucket.org/rmerriam/rvr-cpp-v2.git
}
//---------------------------------------------------------------------------------------------------------------------
void bt_quaternion(rvr::SensorsStream& sen_s) {
<<<<<<< HEAD
    mys::tout << code_line;
=======
   mys::tout << code_line;
>>>>>>> branch 'master' of https://rmerriam@bitbucket.org/rmerriam/rvr-cpp-v2.git

   sen_s.disableStreamingBT();
   std::this_thread::sleep_for(50ms);

   sen_s.streamQuaternion(rvr::CommandResponse::resp_yes);

   sen_s.startStreamingBT(50, rvr::CommandResponse::resp_yes);
   std::this_thread::sleep_for(100ms);

   sen_s.disableStreamingBT();

   //    std::this_thread::sleep_for(100ms);

<<<<<<< HEAD
    auto [q_w, q_x, q_y, q_z] { sen_s.quaternion().get_or() };
//    std::this_thread::sleep_for(50ms);
=======
   auto [q_w, q_x, q_y, q_z] { sen_s.quaternion().get() };
   //    std::this_thread::sleep_for(50ms);
>>>>>>> branch 'master' of https://rmerriam@bitbucket.org/rmerriam/rvr-cpp-v2.git

<<<<<<< HEAD
    mys::tout << code_line << "quaternion: " << q_w << mys::sp << q_x << mys::sp << q_y << mys::sp << q_z;
    mys::tout << code_line;
=======
   mys::tout << code_line << "quaternion: " << q_w << mys::sp << q_x << mys::sp << q_y << mys::sp << q_z;
   mys::tout << code_line;
>>>>>>> branch 'master' of https://rmerriam@bitbucket.org/rmerriam/rvr-cpp-v2.git
}
//---------------------------------------------------------------------------------------------------------------------
void streaming(rvr::SensorsStream& sen_s) {
<<<<<<< HEAD
    mys::tout << code_line;
=======
   mys::tout << code_line;
>>>>>>> branch 'master' of https://rmerriam@bitbucket.org/rmerriam/rvr-cpp-v2.git

   sen_s.disableAllStreaming();
   sen_s.clearAllStreaming();

   nordic_ambient_light(sen_s);
   nordic_color(sen_s);
   nordic_core_time(sen_s);

   bt_imu_accel_gyro(sen_s);
   bt_speed_vel_loc(sen_s);
   bt_quaternion(sen_s);
   bt_core_time(sen_s);

   sen_s.disableAllStreaming();
   sen_s.clearAllStreaming();
   std::this_thread::sleep_for(50ms);
}
