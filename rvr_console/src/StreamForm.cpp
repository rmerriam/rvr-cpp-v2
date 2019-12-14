/*
 * GeneralStreamForm.cpp
 *
 *  Created on: Apr 2, 2017
 *      Author: rmerriam
 */
#include <chrono>
#include <iomanip>
#include <sstream>

#include "DataField.h"
using namespace scr;

#include "StreamForm.h"
#include <SensorsDirect.h>

extern std::string n;
//--------------------------------------------------------------------------------------------------------------------------
StreamForm::StreamForm(int const y, int const x, rvr::Request& req) :
    FormBase(y, x), mStream { req } {

    uint8_t item_row { 2 };
    int width { 10 };
    NField::build_header(mFields, "Stream Data", 1, 1.5 * width);

    NField::build_subhead(mFields, "Accelerometer", item_row++);
    mAccelX = NField::build_wide_data_item(mFields, "X:", item_row++, width, 5);
    mAccelY = NField::build_wide_data_item(mFields, "Y:", item_row++, width, 5);
    mAccelZ = NField::build_wide_data_item(mFields, "Z:", item_row++, width, 5);

    ++item_row;
    mAmbient = NField::build_wide_data_item(mFields, "Ambient:", item_row++, width, 5);

    NField::build_subhead(mFields, "Gyroscope", item_row++);
    mGyroX = NField::build_wide_data_item(mFields, "X:", item_row++, width, 5);
    mGyroY = NField::build_wide_data_item(mFields, "Y:", item_row++, width, 5);
    mGyroZ = NField::build_wide_data_item(mFields, "Z:", item_row++, width, 5);

    NField::build_subhead(mFields, "IMU", item_row++);
    mRoll = NField::build_wide_data_item(mFields, "Pitch:", item_row++, width, 5);
    mPitch = NField::build_wide_data_item(mFields, "Roll:", item_row++, width, 5);
    mYaw = NField::build_wide_data_item(mFields, "Yaw:", item_row++, width, 5);

    NField::build_subhead(mFields, "Locator", item_row++);
    mLocatorX = NField::build_wide_data_item(mFields, "X:", item_row++, width, 5);
    mLocatorY = NField::build_wide_data_item(mFields, "Y:", item_row++, width, 5);

    ++item_row;
    mSpeed = NField::build_wide_data_item(mFields, "Speed:", item_row++, width, 5);

    mForm.init();

    rvr::SensorsDirect sd(req);
    sd.enableColorDetection();

    mStream.clearStreaming();

    mStream.accelConfig();
    mStream.ambientConfig();
//    mStream.colorConfig();
//    mStream.coreNordicConfig();
////    mStream.coreBTConfig();
    mStream.gyroConfig();
    mStream.imuConfig();
    mStream.locatorConfig();
//    mStream.quaternionConfig();
    mStream.speedConfig();
//    mStream.velocityConfig();

    mStream.enableStreamingBT(50);
    mStream.enableStreamingNordic(50);

}
//--------------------------------------------------------------------------------------------------------------------------
void StreamForm::updateScreen() {
    mAmbient->setData(mStream.ambient());

    auto [a_x, a_y, a_z] = mStream.accelerometer();
    mAccelX->setData(a_x);
    mAccelY->setData(a_y);
    mAccelZ->setData(a_z);

    auto [g_x, g_y, g_z] = mStream.gyroscope();
    mGyroX->setData(g_x);
    mGyroY->setData(g_y);
    mGyroZ->setData(g_z);

    auto [i_x, i_y, i_z] = mStream.imu();
    mRoll->setData(i_x);
    mPitch->setData(i_y);
    mYaw->setData(i_z);

    auto [l_x, l_y] = mStream.locator();
    mLocatorX->setData(l_x);
    mLocatorY->setData(l_y);

    mSpeed->setData(mStream.speed());
//    auto [v_x, v_y] = mStream.velocity();

    wrefresh(mForm.win());
}
