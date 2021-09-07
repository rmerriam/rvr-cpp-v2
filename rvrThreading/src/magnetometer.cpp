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
#include <future>
#include <thread>

#include <rvr++.h>
//---------------------------------------------------------------------------------------------------------------------
void magnetometer(rvr::SensorsDirect& sen_d) {
<<<<<<< HEAD
    mys::TraceOn tinfo_ctrl { mys::tout };
    mys::tout << code_line;
=======
   mys::TraceOn tinfo_ctrl { mys::tout };
   mys::tout << code_line;
>>>>>>> branch 'master' of https://rmerriam@bitbucket.org/rmerriam/rvr-cpp-v2.git

<<<<<<< HEAD
    {
        auto mag_done { sen_d.isMagnetometerCalibrationDone().valid() };
        mys::tout << code_line << "mag cal done: " << mag_done;
=======
   {
      auto mag_done { sen_d.isMagnetometerCalibrationDone().valid() };
      mys::tout << code_line << "mag cal done: " << mag_done;
>>>>>>> branch 'master' of https://rmerriam@bitbucket.org/rmerriam/rvr-cpp-v2.git

<<<<<<< HEAD
        auto mag_data { sen_d.magnetometerData() };
        mys::tout << code_line << "mag data valid: " << mag_data.valid();
=======
      auto mag_data { sen_d.magnetometerData() };
      mys::tout << code_line << "mag data valid: " << mag_data.valid();
>>>>>>> branch 'master' of https://rmerriam@bitbucket.org/rmerriam/rvr-cpp-v2.git

<<<<<<< HEAD
//        auto [x, y, z] { mag_data.get_or() };
//        mys::tout << code_line << "mag data: " << x << mys::sp << y << mys::sp << z;
//        mys::tout << code_line << mys::nl;
=======
      //        auto [x, y, z] { mag_data.get() };
      //        mys::tout << code_line << "mag data: " << x << mys::sp << y << mys::sp << z;
      //        mys::tout << code_line << mys::nl;
>>>>>>> branch 'master' of https://rmerriam@bitbucket.org/rmerriam/rvr-cpp-v2.git

<<<<<<< HEAD
        mys::tout << code_line << mys::nl;
//        return;
    }
    {
        sen_d.calibrateMagnetometer(rvr::CommandResponse::resp_yes);
        while (sen_d.isMagnetometerCalibrationDone().invalid()) {
            std::this_thread::sleep_for(50ms);
        };
=======
      mys::tout << code_line << mys::nl;
      //        return;
   }
   {
      sen_d.calibrateMagnetometer(rvr::CommandResponse::resp_yes);
      while(sen_d.isMagnetometerCalibrationDone().invalid()) {
         std::this_thread::sleep_for(50ms);
      };
>>>>>>> branch 'master' of https://rmerriam@bitbucket.org/rmerriam/rvr-cpp-v2.git

      sen_d.getMagnetometerData();

<<<<<<< HEAD
        auto mag_done { sen_d.isMagnetometerCalibrationDone() };
        mys::tout << code_line << "mag cal done: " << mag_done.valid();
=======
      auto mag_done { sen_d.isMagnetometerCalibrationDone() };
      mys::tout << code_line << "mag cal done: " << mag_done.valid();
>>>>>>> branch 'master' of https://rmerriam@bitbucket.org/rmerriam/rvr-cpp-v2.git

<<<<<<< HEAD
        std::this_thread::sleep_for(50ms);
        mys::tout << code_line << "mag offset: " << sen_d.magnetometerCalibrationYaw().get_or();
=======
      std::this_thread::sleep_for(50ms);
      mys::tout << code_line << "mag offset: " << sen_d.magnetometerCalibrationYaw().get();
>>>>>>> branch 'master' of https://rmerriam@bitbucket.org/rmerriam/rvr-cpp-v2.git

<<<<<<< HEAD
        auto mag_data { sen_d.magnetometerData() };
        auto const [xx, yy, zz] { mag_data.get_or() };
        mys::tout << code_line << "mag data: " << xx << mys::sp << yy << mys::sp << zz;
=======
      auto mag_data { sen_d.magnetometerData() };
      auto const [xx, yy, zz] { mag_data.get() };
      mys::tout << code_line << "mag data: " << xx << mys::sp << yy << mys::sp << zz;
>>>>>>> branch 'master' of https://rmerriam@bitbucket.org/rmerriam/rvr-cpp-v2.git

<<<<<<< HEAD
        sen_d.resetMagnetometerCalibration();
    }
    {
        auto mag_done { sen_d.isMagnetometerCalibrationDone() };
        mys::tout << code_line << "mag cal done: " << mag_done.valid();
        mys::tout << code_line << mys::nl;
    }
=======
      sen_d.resetMagnetometerCalibration();
   }
   {
      auto mag_done { sen_d.isMagnetometerCalibrationDone() };
      mys::tout << code_line << "mag cal done: " << mag_done.valid();
      mys::tout << code_line << mys::nl;
   }
>>>>>>> branch 'master' of https://rmerriam@bitbucket.org/rmerriam/rvr-cpp-v2.git

<<<<<<< HEAD
    mys::tout << code_line << mys::nl;
    mys::tout << code_line << mys::nl;
=======
   mys::tout << code_line << mys::nl;
   mys::tout << code_line << mys::nl;
>>>>>>> branch 'master' of https://rmerriam@bitbucket.org/rmerriam/rvr-cpp-v2.git
}
