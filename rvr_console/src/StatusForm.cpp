/*
 * GeneralStatusForm.cpp
 *
 *  Created on: Apr 2, 2017
 *      Author: rmerriam
 */
#include <chrono>
#include <iomanip>
#include <sstream>

#include "DataField.h"
using namespace scr;

#include "StatusForm.h"
extern std::string n;
//--------------------------------------------------------------------------------------------------------------------------
StatusForm::StatusForm(int const y, int const x) :
    FormBase(y, x) {

    uint8_t item_row { 2 };
    int width { 18 };
    NField::build_header(mFields, "Application Status", 1, COLS - 3);

    mWallTime = NField::build_wide_data_item(mFields, "Wall Time:", item_row, width, 13, 2);
    mSerialName = NField::build_data_item(mFields, "Port:", item_row, width, (COLS - 3) / 3);

    mVersion = NField::build_data_item(mFields, "Version:", item_row, width, 2 * (COLS - 3) / 3);

    mMouse = NField::build_data_item(mFields, "Mouse:", item_row + 1, width, 2 * (COLS - 3) / 3);
    mForm.init();
}
//---------------------------------------------------------------------------------------------------------------------
void StatusForm::wall_time() {
    using namespace std::chrono;

    auto now = system_clock::now();
    auto timer = system_clock::to_time_t(now);

    stringstream s;
    s << std::put_time(std::localtime( &timer), "%H:%M:%S."); //
    s << std::dec << std::setfill('0') << std::setw(3);
    s << (duration_cast<milliseconds>(now.time_since_epoch()) % 1000).count();

    mWallTime->setData(s.str());
}
//--------------------------------------------------------------------------------------------------------------------------
extern float const version { 0.1 };
extern std::string serial_port;
extern int moused;
//--------------------------------------------------------------------------------------------------------------------------
void StatusForm::updateScreen() {

    wall_time();
    mSerialName->setData(serial_port);
    mVersion->setData(version);

    mMouse->setData(moused);

    wrefresh(mForm.win());
}
