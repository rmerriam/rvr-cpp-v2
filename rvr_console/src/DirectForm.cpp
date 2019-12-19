/*
 * GeneralDirectForm.cpp
 *
 *  Created on: Apr 2, 2017
 *      Author: rmerriam
 */
#include <chrono>
#include <iomanip>
#include <sstream>

#include "DataField.h"
using namespace scr;

#include "DirectForm.h"
#include <SensorsDirect.h>

extern std::string n;
//--------------------------------------------------------------------------------------------------------------------------
DirectForm::DirectForm(int const y, int const x, rvr::Blackboard& bb, rvr::Request& req) :
    FormBase(y, x), mSensors { bb, req } {

    uint8_t item_row { 2 };
    int width { 14 };
    NField::build_header(mFields, "Direct Sensor", 1, 1.5 * width);

    ++item_row;
    mAmbient = NField::build_wide_data_item(mFields, "Ambient:", item_row++, width, 5);

    NField::build_subhead(mFields, "ColorSensor", item_row++);
    mRgbSensorRed = NField::build_wide_data_item(mFields, "Red:", item_row++, width, 5);
    mRgbSensorGrn = NField::build_wide_data_item(mFields, "Green:", item_row++, width, 5);
    mRgbSensorBlue = NField::build_wide_data_item(mFields, "Blue:", item_row++, width, 5);
    mRgbConfidence = NField::build_wide_data_item(mFields, "Confidence:", item_row++, width, 5);
    mColorDetection = NField::build_wide_data_item(mFields, "Color Enabled:", item_row++, width, 5);

    NField::build_subhead(mFields, "Motor Temperature", item_row++);
    mLeftMotorTemp = NField::build_wide_data_item(mFields, "Left:", item_row++, width, 5);
    mRightMotorTemp = NField::build_wide_data_item(mFields, "Right:", item_row++, width, 5);

    mForm.init();

    mSensors.enableColorDetection();
}
//--------------------------------------------------------------------------------------------------------------------------
void DirectForm::requestData() {

    mSensors.enableColorDetection();
    mSensors.getRgbcSensorValue();
    mSensors.getAmbienLightSensorValue();

//    mSensors.getCurrentDectectedColor();

    mSensors.getLeftMotorTemp();
    mSensors.getRightMotorTemp();
}
//--------------------------------------------------------------------------------------------------------------------------
void DirectForm::updateScreen() {
    mAmbient->setData(mSensors.ambient());

    auto [r, g, b, c] = mSensors.currentRGBValues();
    mRgbSensorRed->setData(r);
    mRgbSensorGrn->setData(g);
    mRgbSensorBlue->setData(b);
    mRgbConfidence->setData(c);

    mColorDetection->setData(r);

    mLeftMotorTemp->setData(mSensors.leftMotorTemp());
    mRightMotorTemp->setData(mSensors.rightMotorTemp());

    wrefresh(mForm.win());
}
