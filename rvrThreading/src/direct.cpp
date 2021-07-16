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
void color(rvr::SensorsDirect& sen_d);
void magnetometer(rvr::SensorsDirect& sen_d);
//---------------------------------------------------------------------------------------------------------------------
void general(rvr::SensorsDirect& sen_d) {
    sen_d.enableGyroMaxNotify();

    sen_d.resetLocatorXY();
    sen_d.setLocatorFlags(true);

    sen_d.getAmbientLightSensorValue();

    sen_d.getLeftMotorTemp();
    sen_d.getRightMotorTemp();

    sen_d.getThermalProtectionStatus();
    sen_d.enableThermalProtectionNotify();  // responds when status changes

    std::this_thread::sleep_for(150ms);

    mys::tinfo << code_loc << "isGyroMaxNotifyEnabled: " << sen_d.isGyroMaxNotifyEnabled().get();
    mys::tinfo << code_loc << "isThermalProtectionNotifyEnabled: " << sen_d.isThermalProtectionNotifyEnabled().get();

    std::this_thread::sleep_for(50ms);

    mys::tinfo << code_loc << "ambient light: " << std::setprecision(2) << sen_d.ambientLight().get();

    auto [left_temp, left_status, right_temp, right_status] { sen_d.thermalProtectionValues().value_or(
        rvr::ThermalProtection { }) };
    mys::tinfo << code_loc << "thermalProtectionValues: " << left_temp << mys::sp << (int)left_status //
               << mys::sp << right_temp << mys::sp << (int)right_status;

//        opt_output("Ambient"s, sen_d.ambient(), -1.0f);
//        opt_output("Left Temp:"s, sen_d.leftMotorTemp(), -1.0f);
//        opt_output("Right Temp:"s, sen_d.rightMotorTemp(), -1.0f);

    mys::tinfo << code_loc << "Left Temp: " << sen_d.leftMotorTemp().get();
    mys::tinfo << code_loc << "Right Temp: " << sen_d.rightMotorTemp().get();

    sen_d.disableGyroMaxNotify();
    sen_d.disableThermalProtectionNotify();
    std::this_thread::sleep_for(50ms);

    mys::tinfo << code_loc << mys::nl;
    mys::tinfo << code_loc << "isGyroMaxNotifyEnabled: " << sen_d.isGyroMaxNotifyEnabled().get();
    mys::tinfo << code_loc << "isThermalProtectionNotifyEnabled: " << sen_d.isThermalProtectionNotifyEnabled().get();

    mys::tinfo << code_loc << mys::nl;
}
//---------------------------------------------------------------------------------------------------------------------
void direct(rvr::SensorsDirect& sen_d) {
    mys::tinfo << code_loc;

    general(sen_d);
    color(sen_d);
//    magnetometer(sen_d);

}

