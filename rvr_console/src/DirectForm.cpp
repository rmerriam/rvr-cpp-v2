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
    NField::build_header(mFields, "Direct Sensor", 1, 1.25 * width);

    ++item_row;
    mAmbient = NField::build_wide_data_item(mFields, "Ambient:", item_row++, width, 5);

    mColorDetection = NField::build_wide_data_item(mFields, "Color Enabled:", item_row++, width, 5);
    NField::build_subhead(mFields, "ColorSensor", item_row++);
    mRgbSensorRed = NField::build_wide_data_item(mFields, "Red:", item_row++, width, 5);
    mRgbSensorGrn = NField::build_wide_data_item(mFields, "Green:", item_row++, width, 5);
    mRgbSensorBlue = NField::build_wide_data_item(mFields, "Blue:", item_row++, width, 5);
    mRgbConfidence = NField::build_wide_data_item(mFields, "Confidence:", item_row++, width, 5);

    NField::build_subhead(mFields, "ColorDetect", item_row++);
    mRgbDetectRed = NField::build_wide_data_item(mFields, "Red:", item_row++, width, 5);
    mRgbDetectGrn = NField::build_wide_data_item(mFields, "Green:", item_row++, width, 5);
    mRgbDetectBlue = NField::build_wide_data_item(mFields, "Blue:", item_row++, width, 5);
    mRgbDetectConfidence = NField::build_wide_data_item(mFields, "Confidence:", item_row++, width, 5);
    mRgbClass = NField::build_wide_data_item(mFields, "Class:", item_row++, width, 5);

    NField::build_subhead(mFields, "Motor Temperature", item_row++);
    mLeftMotorTemp = NField::build_wide_data_item(mFields, "Left:", item_row++, width, 5);
    mRightMotorTemp = NField::build_wide_data_item(mFields, "Right:", item_row++, width, 5);

    NField::build_subhead(mFields, "Thermal Protection", item_row++);
    mLeftMotorThermal = NField::build_wide_data_item(mFields, "Left:", item_row++, width, 5);
    mLeftMotorStatus = NField::build_wide_data_item(mFields, "Status:", item_row++, width, 5);
    mRightMotorThermal = NField::build_wide_data_item(mFields, "Right:", item_row++, width, 5);
    mRightMotorStatus = NField::build_wide_data_item(mFields, "Status:", item_row++, width, 5);

    mForm.init();
}
//--------------------------------------------------------------------------------------------------------------------------
void DirectForm::requestData() {

    mSensors.enableColorDetectionNotify(true, 100, 0);

    mSensors.enableColorDetection();
    mSensors.getRgbcSensorValue();
    mSensors.getAmbienLightSensorValue();

    mSensors.getCurrentDectectedColor();
    mSensors.getThermalProtectionStatus();

    mSensors.getLeftMotorTemp();
    mSensors.getRightMotorTemp();
}
//--------------------------------------------------------------------------------------------------------------------------
void DirectForm::updateScreen() {
    mAmbient->setData(mSensors.ambient());

    mColorDetection->setData(mSensors.isColorDetectionEnabled());

    auto [r, g, b, c] = mSensors.currentRGBValues();
    mRgbSensorRed->setData(r);
    mRgbSensorGrn->setData(g);
    mRgbSensorBlue->setData(b);
    mRgbConfidence->setData(c);

    auto [d_r, d_g, d_b, conf, classification] = mSensors.colorDetectionValues();

    mRgbDetectRed->setData(d_r);
    mRgbDetectGrn->setData(d_g);
    mRgbDetectBlue->setData(d_b);
    mRgbDetectConfidence->setData(conf);
    mRgbClass->setData(classification);

    mLeftMotorTemp->setData(mSensors.leftMotorTemp());
    mRightMotorTemp->setData(mSensors.rightMotorTemp());

    auto [left_temp, left_status, right_temp, right_status] = mSensors.thermalProtectionValues();
//
    static std::string const thermal_status[] { "okay", "warn", "critical" };
    mLeftMotorThermal->setData(left_temp);
    mLeftMotorStatus->setData(thermal_status[left_status]);
    mRightMotorThermal->setData(right_temp);
    mRightMotorStatus->setData(thermal_status[right_status]);

    wrefresh(mForm.win());
}
