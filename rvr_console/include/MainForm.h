#ifndef MAINFORM_H_
#define MAINFORM_H_
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
#include "NCurses.h"

#include "PowerForm.h"
#include "StatusForm.h"
#include "SystemInfoForm.h"
//--------------------------------------------------------------------------------------------------------------------------

class MainForm {
public:
    MainForm();
    MainForm(rvr::Request& req);

    MainForm(const MainForm& other) = delete;
    MainForm(MainForm&& other) = delete;
    MainForm& operator=(const MainForm& other) = delete;
    virtual ~MainForm() = default;

    void runWindows();

private:
    void updateRequests();

    rvr::Request& mRequest;

    static constexpr int row { 0 };

    PowerForm power_form { row, 0, mRequest };
    SystemInfoForm sysinfo_form { row, power_form.getX() + 2, mRequest };
    StatusForm status_form { LINES - 8, 0 };
};
#endif
