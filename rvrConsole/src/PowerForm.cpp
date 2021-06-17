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
PowerForm::PowerForm(int const y, int const x, rvr::Blackboard& bb, rvr::SendPacket& req) :
    FormBase(y, x), mPow { bb, req } {

    mPow.awake();

    uint8_t item_row { 2 };
    constexpr int width { 17 };

    mHeader = NField::build_header(mFields, "Power", 1, 1.5 * width);

    NField::build_subhead(mFields, "Battery Voltage", item_row);

    mVoltState = NField::build_wide_data_item(mFields, "State:", ++item_row, width, 6);
    mPercentage = NField::build_wide_data_item(mFields, "Percentage:", ++item_row, width, 6);
    mCalFiltered = NField::build_wide_data_item(mFields, "Calib Filtered:", ++item_row, width, 6);
    mCalUnfiltered = NField::build_wide_data_item(mFields, " Uncalib Filtered:", ++item_row, width, 6);
    mUncalUnfiltered = NField::build_wide_data_item(mFields, "Uncalib Unfiltered:", ++item_row, width, 6);

    ++item_row;

    mCriticialVolt = NField::build_wide_data_item(mFields, "Critical Volt:", ++item_row, width, 6);
    mLowVolt = NField::build_wide_data_item(mFields, "Low Volt:", ++item_row, width, 6);
    mHysteresisVolt = NField::build_wide_data_item(mFields, "Hysteresis Volt:", ++item_row, width, 6);

    ++item_row;
    mLeftMotorCurrent = NField::build_wide_data_item(mFields, "Left Motor Amps:", ++item_row, width, 6);
    mRightMotorCurrent = NField::build_wide_data_item(mFields, "Right Motor Amps:", ++item_row, width, 6);

    ++item_row;
    NField::build_subhead(mFields, "Status", ++item_row);

    mBattStateChangeStatus = NField::build_wide_data_item(mFields, "Batt State Enabled?", ++item_row, width, 6);
    mBattStateChangeStatus->invertText();

    mWakeNotify = NField::build_wide_data_item(mFields, " Wake Notify?", ++item_row, width, 6);
    mWakeNotify->invertText();

    mForm.init();
}
//--------------------------------------------------------------------------------------------------------------------------
void PowerForm::onceData() {
}
//--------------------------------------------------------------------------------------------------------------------------
void PowerForm::requestData() {

    mPow.batteryVoltageState();
    mPow.batteryPercentage();

    mPow.batteryVoltage(rvr::Power::VoltageType::CalibratedFiltered);
    mPow.batteryVoltage(rvr::Power::VoltageType::CalibratedUnfiltered);
    mPow.batteryVoltage(rvr::Power::VoltageType::UncalibratedUnfiltered);

    mPow.batteryVoltThresholds();

    mPow.batteryMotorCurrent(rvr::Power::left);
    mPow.batteryMotorCurrent(rvr::Power::right);
}

//--------------------------------------------------------------------------------------------------------------------------
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

    mBattStateChangeStatus->setData(mPow.isBatteryStateChangeEnabled());
    mWakeNotify->setData(mPow.isWakeNotify());

    wrefresh(mForm.win());
}
//--------------------------------------------------------------------------------------------------------------------------
extern int moused;
void PowerForm::disableBatt() {
    static bool set { false };

    if (set) {
        mPow.disableBatteryStateChange();
        set = false;
        moused = set;
    }
    else {
        mPow.enableBatteryStateChange();
        set = true;
        moused = set;
    }
}