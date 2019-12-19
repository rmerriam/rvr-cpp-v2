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

#include <ApiShell.h>
#include <Connection.h>
#include <SystemInfo.h>
//--------------------------------------------------------------------------------------------------------------------------
class SystemInfoForm : public FormBase {
    rvr::CommandBase::CommandResponse RespYes = rvr::CommandBase::resp_yes;

public:
    SystemInfoForm(int const y, int const x, rvr::Blackboard& bb, rvr::Request& req);

    virtual void fastData() override;
    virtual void onceData() override;

    virtual void requestData() override;
    virtual void updateScreen() override;

private:

    DataFieldPtr mBoardVer;
    DataFieldPtr mBootVer;
    DataFieldPtr mBootVer2;
    DataFieldPtr mMacAddr1;
    DataFieldPtr mMacAddr2;
    DataFieldPtr mMacAddr3;
    DataFieldPtr mAppVer;
    DataFieldPtr mAppVer2;
    DataFieldPtr mProcName;
    DataFieldPtr mProcName2;
    DataFieldPtr mStatsId;
    DataFieldPtr mSku;
    DataFieldPtr mUpTime;

    rvr::SystemInfo mSys;

    rvr::Connection mConn;
    DataFieldPtr mRvrName;

    rvr::ApiShell mApi;
    DataFieldPtr mPing;
    DataFieldPtr mPingAlt;

};
#endif /* GeneralStatusForm_H_ */
