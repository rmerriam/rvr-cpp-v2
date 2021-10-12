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
//     Created: Nov 18, 2019
//
//======================================================================================================================
#include "MainForm.h"

#include <thread>

#include "DataField.h"
#include "HeaderField.h"
#include "LabelField.h"
//---------------------------------------------------------------------------------------------------------------------
uint32_t led32 {    //
Led::battery_door_rear | Led::power_button_rear |    //
    Led::brakelight_left | Led::brakelight_right };

rvr::RvrMsg colors[] { //
{ 0x00, 0x00, 0xFF, //
0xFF, 0x00, 0x00, //
    0x00, 0x00, 0xFF, //
    0xFF, 0x00, 0x00, }, //
    { 0xFF, 0x00, 0x00, //
    0x00, 0x00, 0xFF, //
        0xFF, 0x00, 0x00, //
        0x00, 0x00, 0xFF, }, //
};
//---------------------------------------------------------------------------------------------------------------------
int moused {};
//---------------------------------------------------------------------------------------------------------------------
MainForm::MainForm(rvr::Blackboard& bb, rvr::SendPacket& req) :    //
    mRequest { req }, //
        sysinfo_form { row, 0, bb, mRequest }, //
        power_form { row, sysinfo_form.getX() + 2, bb, mRequest }, //
        direct_form { row, power_form.getX() + 2, bb, mRequest }, //
        stream_form { row, direct_form.getX() + 2, bb, mRequest }, //
        mLeds { bb, mRequest }, //
        mDrive { bb, mRequest } {
    std::this_thread::sleep_for(200ms);
    mDrive.resetYaw();
    mLeds.ledsOff();
}
//---------------------------------------------------------------------------------------------------------------------
void MainForm::mouseEvent() {
    MEVENT mevent;
    getmouse( &mevent);
    moused = mevent.x;
    if ((mevent.y == 17) && ((mevent.x >= 44) & (mevent.x <= 49))) {
        power_form.disableBatt();
    }
}
//---------------------------------------------------------------------------------------------------------------------
void MainForm::runWindows() {

    power_form.onceData();
    sysinfo_form.onceData();
    stream_form.onceData();

    status_form.onceData();

    bool run { true };
    while (run) {

        switch (NCurses::check_input()) {
            case KEY_UP:
                mLSpeed += 5;
                mRSpeed += 5;
                break;

            case KEY_DOWN:
                mLSpeed -= 5;
                mRSpeed -= 5;
                break;

            case KEY_LEFT:
                mLSpeed += 5;
                mRSpeed -= 5;
                break;

            case KEY_RIGHT:
                mLSpeed -= 5;
                mRSpeed += 5;
                break;

            case KEY_END:
                mLSpeed = 0;
                mRSpeed = 0;
                break;

            case KEY_F(2):
                run = false;
                break;

            case KEY_MOUSE:
                mouseEvent();
                break;
        }
        updateRequests();
        std::this_thread::sleep_for(250ms);

        power_form.updateScreen();
        sysinfo_form.updateScreen();
        direct_form.updateScreen();
        stream_form.updateScreen();

        status_form.updateScreen();
        refresh();

        mDrive.drive(mLSpeed, mRSpeed);

    }
    power_form.done();
}
//---------------------------------------------------------------------------------------------------------------------
void MainForm::updateRequests() {
    static int8_t passes { -1 };

// data needed every update
    power_form.fastData();
    sysinfo_form.fastData();
    direct_form.fastData();

    status_form.fastData();
// limit requests for data while letting streaming updates occur
    ++passes;
    passes %= 10;
    if ( !passes) {
        //            pose_form.requestData();
        power_form.requestData();
        sysinfo_form.requestData();
        direct_form.requestData();
        stream_form.requestData();

        status_form.requestData();
    }
    mLeds.allLed(led32, colors[passes % 2]);
}
