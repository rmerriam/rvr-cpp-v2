/*
 * GeneralStatusForm.h
 *
 *  Created on: Apr 2, 2017
 *      Author: rmerriam
 */

#ifndef SystemInfoForm_H_
#define SystemInfoForm_H_

#include <iostream>
using namespace std;

#include "NField.h"
#include "NForm.h"
#include "FormBase.h"
using namespace scr;

#include <Connection.h>
#include <SystemInfo.h>
//--------------------------------------------------------------------------------------------------------------------------
class SystemInfoForm : public FormBase {
    rvr::CommandBase::CommandResponse RespYes = rvr::CommandBase::resp_yes;

public:
    SystemInfoForm(const int y, const int x, rvr::Request& req);

    virtual void fastData() override;
    virtual void onceData() override;

    virtual void requestData() override;
    virtual void updateScreen() override;

private:

    DataFieldPtr mBoardVer;
    DataFieldPtr mBootVer;
    DataFieldPtr mBootVer2;
    DataFieldPtr mMacAddr;
    DataFieldPtr mAppVer;
    DataFieldPtr mAppVer2;
    DataFieldPtr mProcName;
    DataFieldPtr mProcName2;
    DataFieldPtr mStatsId;
    DataFieldPtr mSku;
    DataFieldPtr mUpTime;

    rvr::SystemInfo mSys;

    DataFieldPtr mRvrName;
    rvr::Connection mConn;

};
#endif /* GeneralStatusForm_H_ */
