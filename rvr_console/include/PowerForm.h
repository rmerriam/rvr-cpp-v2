/*
 * RobotPoseForm.h
 *
 *  Created on: Apr 2, 2017
 *      Author: rmerriam
 */

#ifndef RobotPoseForm_H_
#define RobotPoseForm_H_

#include <iostream>
using namespace std;

#include "NField.h"
#include "NForm.h"
using namespace scr;

#include "Power.h"

#include "FormBase.h"
//--------------------------------------------------------------------------------------------------------------------------
class PowerForm : public FormBase {
    rvr::CommandBase::CommandResponse RespYes = rvr::CommandBase::resp_yes;

public:
    PowerForm(int const y, int const x, rvr::Blackboard& bb, rvr::Request& req);

    virtual void onceData() override;

    virtual void requestData() override;
    virtual void updateScreen() override;

    void done() {
        mPow.sleep();
    }
    void disableBatt();

private:

    HeaderField* mHeader;

    DataFieldPtr mVoltState;
    DataFieldPtr mPercentage;
    DataFieldPtr mCalFiltered;
    DataFieldPtr mCalUnfiltered;
    DataFieldPtr mUncalUnfiltered;
    DataFieldPtr mCriticialVolt;
    DataFieldPtr mLowVolt;
    DataFieldPtr mHysteresisVolt;
    DataFieldPtr mLeftMotorCurrent;
    DataFieldPtr mRightMotorCurrent;

    DataFieldPtr mBattStateChangeStatus;
    DataFieldPtr mWakeNotify;

    rvr::Power mPow;

    void invertText();
};
#endif /* RobotPoseForm_H_ */
