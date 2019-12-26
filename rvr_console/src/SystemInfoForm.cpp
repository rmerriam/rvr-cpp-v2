/*
 * GeneralStatusForm.cpp
 *
 *  Created on: Apr 2, 2017
 *      Author: rmerriam
 */
#include <chrono>
#include <iomanip>
#include <sstream>

#include "DataField.h"
using namespace scr;

#include "SystemInfoForm.h"
extern std::string n;

//--------------------------------------------------------------------------------------------------------------------------
SystemInfoForm::SystemInfoForm(int const y, int const x, rvr::Blackboard& bb, rvr::Request& req) :
    FormBase(y, x), mSys { bb, req }, mConn { bb, req }, mApi { bb, req } {

    uint8_t item_row { 2 };
    int width { 12 };
    NField::build_header(mFields, "System Information", 1, width + 9);

    mRvrName = NField::build_data_item(mFields, "BT Name", item_row++, width);
    ++item_row;

    mUpTime = NField::build_data_item(mFields, "Up Time", item_row++, width);
    mBoardVer = NField::build_data_item(mFields, "Board Ver:", item_row++, width);
    mBootVer = NField::build_data_item(mFields, "Boot Ver:", item_row++, width);
    mBootVer2 = NField::build_data_item(mFields, "Boot Ver2:", item_row++, width);
    mAppVer = NField::build_data_item(mFields, "App Ver:", item_row++, width);
    mAppVer2 = NField::build_data_item(mFields, "App Ver2:", item_row++, width);
    mProcName = NField::build_data_item(mFields, "Proc Name:", item_row++, width);
    mProcName2 = NField::build_data_item(mFields, "Proc Name2:", item_row++, width);

    mMacAddr1 = NField::build_data_item(mFields, "BT MAC Addr:", item_row++);
    mMacAddr2 = NField::build_data_item(mFields, "", item_row++);
    mMacAddr3 = NField::build_data_item(mFields, "", item_row++);

    mStatsId = NField::build_data_item(mFields, "Stats Id:", item_row++, width);
    mSku = NField::build_data_item(mFields, "SKU:", item_row++, width);

    ++item_row;
    mPing = NField::build_data_item(mFields, "Ping:", item_row++, width);
    mPingAlt = NField::build_data_item(mFields, "Ping2:", item_row++, width);

    mForm.init();
}
//--------------------------------------------------------------------------------------------------------------------------
void SystemInfoForm::requestData() {
}
//--------------------------------------------------------------------------------------------------------------------------
void SystemInfoForm::fastData() {
    mSys.getCoreUpTime();
}
//--------------------------------------------------------------------------------------------------------------------------
void SystemInfoForm::onceData() {

    mSys.getBoardRevision();
    mSys.getBootloaderVersion();
    mSys.getMacId();
    mSys.getMainAppVersion();
    mSys.getProcessorName();
    mSys.getSku();
    mSys.getStatsId();

    mConn.bluetoothName();

    rvr::RvrMsg dead { 'P', 'i', 'n', 'g' };
    mApi.echo(dead);
}
//--------------------------------------------------------------------------------------------------------------------------
void SystemInfoForm::updateScreen() {

    mBoardVer->setData(mSys.boardVersion());
    mBootVer->setData(mSys.bootVersion());
    mBootVer2->setData(mSys.bootVersion2());

    std::string m { mSys.macAddress() };
    mMacAddr1->setData(m.substr(0, 6));
    mMacAddr2->setData(m.substr(6, 6));
    mMacAddr3->setData(m.substr(12, 6));

    mAppVer->setData(mSys.mainAppVersion());
    mAppVer2->setData(mSys.mainAppVersion2());
    mProcName->setData(mSys.processorName());
    mProcName2->setData(mSys.processorName2());
    mStatsId->setData(mSys.statsId());
    mSku->setData(mSys.sku());
    mUpTime->setData(mSys.coreUpTime());

    mRvrName->setData(mConn.name());

    mPing->setData(mApi.echo());
    mPingAlt->setData(mApi.echoAlt());

    wrefresh(mForm.win());
}
