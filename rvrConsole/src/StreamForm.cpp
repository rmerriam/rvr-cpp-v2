/*
 * GeneralStreamForm.cpp
 *
 *  Created on: Apr 2, 2017
 *      Author: rmerriam
 */
#include <enum.h>
#include <SensorsDirect.h>

#include "DataField.h"
#include "StreamForm.h"

using namespace scr;
//--------------------------------------------------------------------------------------------------------------------------
StreamForm::StreamForm(int const y, int const x, rvr::Blackboard& bb, rvr::SendPacket& req) :
    FormBase(y, x), mStream { bb, req }, mSensors { bb, req } {

    mStream.disableAllStreaming();
    mStream.clearAllStreaming();

    uint8_t item_row { 2 };
    int width { 10 };
    NField::build_header(mFields, "Stream Data", 1, width + 7);

    NField::build_subhead(mFields, "Accelerometer", item_row++);
    mAccelX = NField::build_wide_data_item(mFields, "X:", item_row++, width, 5);
    mAccelY = NField::build_wide_data_item(mFields, "Y:", item_row++, width, 5);
    mAccelZ = NField::build_wide_data_item(mFields, "Z:", item_row++, width, 5);

    ++item_row;
    mAmbient = NField::build_wide_data_item(mFields, "Ambient:", item_row++, width, 5);

    NField::build_subhead(mFields, "Gyroscope", item_row++);
    mGyroMaxNotify = NField::build_wide_data_item(mFields, "Max Notify:", item_row++, width, 5);
    mGyroMaxNotify->invertText();

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

    NField::build_subhead(mFields, "Velocity", item_row++);
    mVelocityX = NField::build_wide_data_item(mFields, "X:", item_row++, width, 5);
    mVelocityY = NField::build_wide_data_item(mFields, "Y:", item_row++, width, 5);

    mForm.init();

    mStream.streamAmbient();
//    mStream.colorConfig();
//    mStream.coreNordicConfig();
////    mStream.coreBTConfig();
    mSensors.enableGyroMaxNotify();
    mStream.streamImuAccelGyro();
    mStream.streamSpeedVelocityLocator();

    mStream.startStreamingBT(50);
    mStream.startStreamingNordic(50);

}
//--------------------------------------------------------------------------------------------------------------------------
StreamForm::~StreamForm() {
    mStream.disableAllStreaming();
    mStream.clearAllStreaming();
}
//--------------------------------------------------------------------------------------------------------------------------
void StreamForm::updateScreen() {
    mAmbient->setData(mStream.ambient().get_or());

    auto [a_x, a_y, a_z] { mStream.accelerometer().get_or() };
    mAccelX->setData(a_x);
    mAccelY->setData(a_y);
    mAccelZ->setData(a_z);

    auto [g_x, g_y, g_z] { mStream.gyroscope().get_or() };
    mGyroMaxNotify->setData(mSensors.isGyroMaxNotifyEnabled().get_or());
    mGyroX->setData(g_x);
    mGyroY->setData(g_y);
    mGyroZ->setData(g_z);

    auto [i_x, i_y, i_z] { mStream.imu().get_or() };
    ;
    mRoll->setData(i_x);
    mPitch->setData(i_y);
    mYaw->setData(i_z);

    auto [l_x, l_y] { mStream.locator().get_or() };
    mLocatorX->setData(l_x);
    mLocatorY->setData(l_y);

    mSpeed->setData(mStream.speed().get_or());

    auto [v_x, v_y] { mStream.velocity().get_or() };
    mVelocityX->setData(v_x);
    mVelocityY->setData(v_y);

    wrefresh(mForm.win());
}
