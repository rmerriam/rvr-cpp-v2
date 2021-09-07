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
#include <rvr++.h>
//---------------------------------------------------------------------------------------------------------------------
void notifications(rvr::SensorsDirect& sen_d) {
    mys::tout << code_line;

    sen_d.enableColorDetection(); // must preceed color detection to turn on bottom LEDs
    sen_d.enableColorDetectionNotify(true, 50, 0);
    std::this_thread::sleep_for(150ms);
    sen_d.disableColorDetection();

    auto [d_r, d_g, d_b, conf, classification] { sen_d.colorDetectionValues().get_or() };
    mys::tout << code_line << "colorDetectionValues: " << (int)(d_r) << mys::sp << (int)(d_g) << mys::sp << (int)(d_b)
        << mys::sp << (int)(conf) << mys::sp << (int)(classification);

    std::this_thread::sleep_for(50ms);

#if 0
    //  can't get these to trigger
    mys::tout << code_line;
    mys::tout << code_line;
    sen_d.enableGyroMaxNotify();
    std::this_thread::sleep_for(1500ms);

    mys::tout << code_line;
    mys::tout << code_line;
    sen_d.enableThermalProtectionNotify();  // responds when status changes
    std::this_thread::sleep_for(150ms);
    auto [left_temp, left_status, right_temp, right_status] { sen_d.thermalProtectionValues().get_or(
        rvr::ThermalProtection { }) };
    mys::tout << code_line << "thermalProtectionValues: " << left_temp << mys::sp << (int)left_status //
               << mys::sp << right_temp << mys::sp << (int)right_status;
    sen_d.disableThermalProtectionNotify();
#endif

    mys::tout << code_line;
}

