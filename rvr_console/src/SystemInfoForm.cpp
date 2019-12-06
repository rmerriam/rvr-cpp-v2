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
SystemInfoForm::SystemInfoForm(int const y, int const x, rvr::Request& req) :
    FormBase(y, x), mSys { req }, mConn { req }, mApi { req } {

    uint8_t item_row { 2 };
    int width { 15 };
    NField::build_header(mFields, "System Information", 1, width + 20);

    ++item_row;
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
    mMacAddr = NField::build_wide_data_item(mFields, "BT MAC Addr:", item_row++, width, 17);
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
    mSys.getUpTime(RespYes);
}
//--------------------------------------------------------------------------------------------------------------------------
void SystemInfoForm::onceData() {

    mSys.getBoardRevision(RespYes);
    mSys.getBootloaderVersion(RespYes);
    mSys.getMacId(RespYes);
    mSys.getMainAppVersion(RespYes);
    mSys.getProcessorName(RespYes);
    mSys.getSku(RespYes);
    mSys.getStatsId(RespYes);

    mConn.bluetoothName(RespYes);

    rvr::RvrMsg dead { 'P', 'i', 'n', 'g' };
    mApi.echo(dead, RespYes);
}
//--------------------------------------------------------------------------------------------------------------------------
void SystemInfoForm::updateScreen() {

    mBoardVer->setData(mSys.boardVersion());
    mBootVer->setData(mSys.bootVersion());
    mBootVer2->setData(mSys.bootVersion2());
    mMacAddr->setData(mSys.macAddress());
    mAppVer->setData(mSys.mainAppVersion());
    mAppVer2->setData(mSys.mainAppVersion2());
    mProcName->setData(mSys.processorName());
    mProcName2->setData(mSys.processorName2());
    mStatsId->setData(mSys.statsId());
    mSku->setData(mSys.sku());
    mUpTime->setData(mSys.upTime());

    mRvrName->setData(mConn.name());

    mPing->setData(mApi.echo());
    mPingAlt->setData(mApi.echoAlt());

    wrefresh(mForm.win());
}
