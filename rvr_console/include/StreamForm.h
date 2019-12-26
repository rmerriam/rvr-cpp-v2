/*
 * GeneralStreamForm.h
 *
 *  Created on: Apr 2, 2017
 *      Author: rmerriam
 */

#ifndef GeneralStreamForm_H_
#define GeneralStreamForm_H_

#include <iostream>
using namespace std;

#include "NField.h"
#include "NForm.h"
using namespace scr;

#include "FormBase.h"
#include <SensorsDirect.h>
#include <SensorsStream.h>

//--------------------------------------------------------------------------------------------------------------------------
class StreamForm : public FormBase {
public:
    StreamForm(int const y, int const x, rvr::Blackboard& bb, rvr::Request& req);
    ~StreamForm();

    virtual void updateScreen() override;

private:

    DataFieldPtr mAccelX;
    DataFieldPtr mAccelY;
    DataFieldPtr mAccelZ;

    DataFieldPtr mAmbient;

    // Gyro
    DataFieldPtr mGyroX;
    DataFieldPtr mGyroY;
    DataFieldPtr mGyroZ;
    DataFieldPtr mGyroMaxNotify;

    //  IMU
    DataFieldPtr mRoll;
    DataFieldPtr mPitch;
    DataFieldPtr mYaw;

    //  Locator
    DataFieldPtr mLocatorX;
    DataFieldPtr mLocatorY;

    DataFieldPtr mSpeed;

    //  Locator
    DataFieldPtr mVelocityX;
    DataFieldPtr mVelocityY;

    rvr::SensorsStream mStream;
    rvr::SensorsDirect mSensors;

};
#endif /* GeneralStreamForm_H_ */
