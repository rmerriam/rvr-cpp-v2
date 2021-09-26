#ifndef MAINFORM_H_
#define MAINFORM_H_
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
#include <Blackboard.h>
#include <Drive.h>
#include <IoLed.h>

#include "DirectForm.h"
#include "NCurses.h"
#include "PowerForm.h"
#include "StatusForm.h"
#include "StreamForm.h"
#include "SystemInfoForm.h"
//--------------------------------------------------------------------------------------------------------------------------
class MainForm {
public:
    MainForm(rvr::Blackboard& bb, rvr::SendPacket& req);

    MainForm(MainForm const& other) = delete;
    MainForm(MainForm&& other) = delete;
    MainForm& operator=(MainForm const& other) = delete;

    void runWindows();

private:
    void updateRequests();
    void mouseEvent();

    rvr::SendPacket& mRequest;

    static constexpr int row { 0 };

    SystemInfoForm sysinfo_form;
    PowerForm power_form;
    DirectForm direct_form;
    StreamForm stream_form;

    StatusForm status_form { LINES - 8, 0 };

    rvr::IoLed mLeds;

    rvr::Drive mDrive;
    double mLSpeed { };
    double mRSpeed { };
};
#endif
