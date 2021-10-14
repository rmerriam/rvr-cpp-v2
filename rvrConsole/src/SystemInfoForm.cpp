/*
 * GeneralStatusForm.cpp
 *
 *  Created on: Apr 2, 2017
 *      Author: rmerriam
 */

#include "DataField.h"
using namespace scr;

#include "SystemInfoForm.h"
//--------------------------------------------------------------------------------------------------------------------------
SystemInfoForm::SystemInfoForm(int const y, int const x, rvr::Blackboard& bb, rvr::SendPacket& req) :
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
    mMacAddr2 = NField::build_data_item(mFields, " ", item_row++);
    mMacAddr3 = NField::build_data_item(mFields, " ", item_row++);

    mStatsId = NField::build_data_item(mFields, "Stats Id:", item_row++, width);
    mSku = NField::build_data_item(mFields, "SKU:", item_row++, width);

    ++item_row;
    mPing = NField::build_data_item(mFields, "Echo:", item_row++, width);
    mPingAlt = NField::build_data_item(mFields, "Echo2:", item_row++, width);

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

    mBoardVer->setData(mSys.boardVersion().get_or());
    mBootVer->setData(mSys.bootVersionNordic().get_or());
    mBootVer2->setData(mSys.bootVersionBT().get_or());

    std::string m { mSys.macAddress().get_or(std::string { 18 }) };
    mMacAddr1->setData(m.substr(0, 6));
    mMacAddr2->setData(m.substr(6, 6));
    mMacAddr3->setData(m.substr(12, 6));

    mAppVer->setData(mSys.mainAppVersionNordic().get_or());
    mAppVer2->setData(mSys.mainAppVersionBT().get_or());
    mProcName->setData(mSys.processorNameNordic().get_or());
    mProcName2->setData(mSys.processorNameBT().get_or());
    mStatsId->setData(mSys.statsId().get_or());
    mSku->setData(mSys.sku().get_or());
    mUpTime->setData(mSys.coreUpTime().get_or());

    mRvrName->setData(mConn.name().get_or());

    mPing->setData(mApi.echoNordic().get_or());
    mPingAlt->setData(mApi.echoBT().get_or());

    wrefresh(mForm.win());
}
