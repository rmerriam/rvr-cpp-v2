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

#include <rvr++.h>
//---------------------------------------------------------------------------------------------------------------------
void magnetometer(rvr::SensorsDirect& sen_d) {
    mys::TraceOn tinfo_ctrl { mys::tinfo };
    mys::tinfo << code_line;

    {
//    auto mag_done { sen_d.isMagnetometerCalibrationDone().value() };
//    mys::tinfo << code_line << "mag cal done: " << mag_done;
        mys::tinfo << code_line << mys::nl;
    }
    {
        sen_d.calibrateMagnetometer(rvr::CommandResponse::resp_yes);
        std::this_thread::sleep_for(5000ms);

        sen_d.getMagnetometerData();

        auto mag_done { sen_d.isMagnetometerCalibrationDone().value() };
        mys::tinfo << code_line << "mag cal done: " << mag_done;

        std::this_thread::sleep_for(50ms);
        mys::tinfo << code_line << "mag offset: " << sen_d.magnetometerCalibration().value();

        auto [x, y, z] { sen_d.magnetometerData().value() };
        mys::tinfo << code_line << "mag data: " << x << mys::sp << y << mys::sp << z;

        mys::tinfo << code_line << mys::nl;
    }

    mys::tinfo << code_line << mys::nl;
}

