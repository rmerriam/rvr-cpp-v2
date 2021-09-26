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
//		 File: color.cpp
//
//     Author: rmerriam
//
//    Created: Jun 10, 2021
//
//======================================================================================================================
#include <thread>

#include <Trace.h>
#include <rvr++.h>
//---------------------------------------------------------------------------------------------------------------------
void color(rvr::SensorsDirect& sen_d) {
    mys::tout << code_line;

    sen_d.enableColorDetection(); // must preceed color detection to turn on bottom LEDs
    sen_d.enableColorDetectionNotify(true, 50, 0);
    std::this_thread::sleep_for(500ms);

    sen_d.getRgbcSensorValue();
    sen_d.getCurrentDectectedColor(); // triggers a notification
    std::this_thread::sleep_for(100ms);

    auto [c_r, c_g, c_b, c_c] { sen_d.currentRGBValues().get_or() };
    auto [d_r, d_g, d_b, conf, classification] { sen_d.colorDetectionValues().get_or() };

    mys::tout << code_line << "isColorDetectionEnabled: " << sen_d.isColorDetectionEnabled().get_or();
    mys::tout << code_line << "isColorDetectionNotifyEnabled: " << sen_d.isColorDetectionNotifyEnabled().get_or();

    mys::tout << code_line << "currentRGBValues: " << c_r << mys::sp << c_g << mys::sp << c_b << mys::sp << c_c;
    mys::tout << code_line << "colorDetectionValues: " << (int)(d_r) << mys::sp << (int)(d_g) << mys::sp << (int)(d_b)
        << mys::sp << (int)(conf) << mys::sp << (int)(classification);

    mys::tout << code_line;

    sen_d.enableColorDetectionNotify(false, 500, 0);
    sen_d.disableColorDetection(); // turns off bottom LEDs
    std::this_thread::sleep_for(50ms);

    mys::tout << code_line;
    mys::tout << code_line << "isColorDetectionEnabled: " << sen_d.isColorDetectionEnabled().get_or();
    mys::tout << code_line << "isColorDetectionNotifyEnabled: " << sen_d.isColorDetectionNotifyEnabled().get_or();
    mys::tout << code_line << mys::nl;
}
