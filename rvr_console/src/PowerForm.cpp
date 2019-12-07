/*
 * RobotPoseForm.cpp
 *
 *  Created on: Apr 2, 2017
 *      Author: rmerriam
 */

#include "HeaderField.h"
#include "DataField.h"
using namespace scr;

#include "PowerForm.h"

//--------------------------------------------------------------------------------------------------------------------------
PowerForm::PowerForm(int const y, int const x, rvr::Request& req) :
    FormBase(y, x), mPow { req } {

    uint8_t item_row { 2 };
    constexpr int width { 20 };

    mHeader = NField::build_header(mFields, "Power", 1, 1.5 * width);

    NField::build_subhead(mFields, "Battery Voltage", item_row);

    mVoltState = NField::build_data_item(mFields, "State:", ++item_row, width);
    mPercentage = NField::build_data_item(mFields, "Percentage:", ++item_row, width);
    mCalFiltered = NField::build_data_item(mFields, "Calib Filtered:", ++item_row, width);
    mCalUnfiltered = NField::build_data_item(mFields, " Uncalib Filtered:", ++item_row, width);
    mUncalUnfiltered = NField::build_data_item(mFields, "Uncalib Unfiltered:", ++item_row, width);

    ++item_row;

    mCriticialVolt = NField::build_data_item(mFields, "Critical Volt:", ++item_row, width);
    mLowVolt = NField::build_data_item(mFields, "Low Volt:", ++item_row, width);
    mHysteresisVolt = NField::build_data_item(mFields, "Hysteresis Volt:", ++item_row, width);

    ++item_row;
    mLeftMotorCurrent = NField::build_data_item(mFields, "Left Motor Amps:", ++item_row, width);
    mRightMotorCurrent = NField::build_data_item(mFields, "Right Motor Amps:", ++item_row, width);

    ++item_row;
    NField::build_subhead(mFields, "Status", ++item_row);

    mBattStateChangeStatus = NField::build_data_item(mFields, "Batt State Enabled?", ++item_row, width);
    mBattStateChangeStatus->invertText();

    mWakeNotify = NField::build_data_item(mFields, " Wake Notify?", ++item_row, width);
    mWakeNotify->invertText();

    mForm.init();
    mPow.awake();
}
//--------------------------------------------------------------------------------------------------------------------------
void PowerForm::onceData() {
}
//--------------------------------------------------------------------------------------------------------------------------
void PowerForm::requestData() {

    mPow.batteryVoltageState(RespYes);
    mPow.batteryPercentage(RespYes);

    mPow.batteryVoltage(rvr::Power::VoltageType::CalibratedFiltered, RespYes);
    mPow.batteryVoltage(rvr::Power::VoltageType::CalibratedUnfiltered, RespYes);
    mPow.batteryVoltage(rvr::Power::VoltageType::UncalibratedUnfiltered, RespYes);

    mPow.batteryVoltThresholds(RespYes);

    mPow.batteryMotorCurrent(rvr::Power::left, RespYes);
    mPow.batteryMotorCurrent(rvr::Power::right, RespYes);
}
//--------------------------------------------------------------------------------------------------------------------------
void PowerForm::updateScreen() {
    mVoltState->setData(mPow.voltState());
    mPercentage->setData(mPow.batteryPercent());
    mCalFiltered->setData(mPow.voltsCalibratedFiltered());
    mCalUnfiltered->setData(mPow.voltsCalibratedUnfiltered());
    mUncalUnfiltered->setData(mPow.voltsUncalibratedUnfiltered());

    mCriticialVolt->setData(mPow.voltThresholdCritical());
    mLowVolt->setData(mPow.voltThresholdLow());
    mHysteresisVolt->setData(mPow.voltThresholdHysteresis());

    mLeftMotorCurrent->setData(mPow.motorCurrent(rvr::Power::left));
    mRightMotorCurrent->setData(mPow.motorCurrent(rvr::Power::right));

    mBattStateChangeStatus->setData(mPow.checkBatteryStateChange());
    mWakeNotify->setData(mPow.isWakeNotify());

    wrefresh(mForm.win());
}
//--------------------------------------------------------------------------------------------------------------------------
extern int moused;
void PowerForm::disableBatt() {
    static bool set { false };

    if (set) {
        mPow.disableBatteryStateChange(RespYes);
        set = false;
        moused = set;
    }
    else {
        mPow.enableBatteryStateChange(RespYes);
        set = true;
        moused = set;
    }
}
