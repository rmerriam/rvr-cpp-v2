/*
 * GeneralDirectForm.h
 *
 *  Created on: Apr 2, 2017
 *      Author: rmerriam
 */

#ifndef GeneralDirectForm_H_
#define GeneralDirectForm_H_

#include <iostream>
using namespace std;

#include "NField.h"
#include "NForm.h"
using namespace scr;

#include "FormBase.h"
#include <SensorsDirect.h>

//--------------------------------------------------------------------------------------------------------------------------
class DirectForm : public FormBase {
public:
    DirectForm(int const y, int const x, rvr::Blackboard& bb, rvr::Request& req);

    virtual void requestData() override;
    virtual void updateScreen() override;

private:

    DataFieldPtr mColorDetection;
    DataFieldPtr mRgbSensorRed;
    DataFieldPtr mRgbSensorGrn;
    DataFieldPtr mRgbSensorBlue;
    DataFieldPtr mRgbConfidence;

    DataFieldPtr mAmbient;

    DataFieldPtr mLeftMotorTemp;
    DataFieldPtr mRightMotorTemp;

    rvr::SensorsDirect mSensors;

};
#endif /* GeneralDirectForm_H_ */
