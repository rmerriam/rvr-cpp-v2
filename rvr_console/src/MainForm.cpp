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
//     Created: Nov 18, 2019
//
//======================================================================================================================
#include <future>

#include "NWindow.h"
#include "NForm.h"
#include "NField.h"
#include "DataField.h"
#include "HeaderField.h"
#include "LabelField.h"

#include "MainForm.h"

int moused { };
//---------------------------------------------------------------------------------------------------------------------
MainForm::MainForm(rvr::Request& req) :
    mRequest { req } {
}
//---------------------------------------------------------------------------------------------------------------------
void MainForm::runWindows() {

    power_form.onceData();
    status_form.onceData();
    sysinfo_form.onceData();

    bool run { true };
    while (run) {

        switch (NCurses::check_input()) {
            case KEY_F(2):
                run = false;
                break;

            case KEY_MOUSE:
                MEVENT mevent;
                getmouse( &mevent);
                moused = mevent.x;
                if ((mevent.y == 17) && (mevent.x < 31)) {
                    power_form.disableBatt();
                }
                break;
        }

        power_form.updateScreen();
        status_form.updateScreen();
        sysinfo_form.updateScreen();
        refresh();

        updateRequests();

        std::this_thread::sleep_for(50ms);
    }
    power_form.done();
}
//---------------------------------------------------------------------------------------------------------------------
void MainForm::updateRequests() {
    static int8_t passes { -1 };

    // data needed every update
    power_form.fastData();
    status_form.fastData();
    sysinfo_form.fastData();

    // limit requests for data while letting streaming updates occur
    ++passes;
    passes %= 10;
    if ( !passes) {
        //            pose_form.requestData();
        power_form.requestData();
        status_form.requestData();
        sysinfo_form.requestData();
    }

}
