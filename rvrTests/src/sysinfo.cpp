//======================================================================================================================
// 2021 Copyright Mystic Lake Software
//
// This is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//======================================================================================================================
//
//		 File: color.cpp
//
//     Author: rmerriam
//
//    Created: Jun 10, 2021
//
//======================================================================================================================
#include <thread>

#include <Trace.h>
#include <rvr++.h>
//---------------------------------------------------------------------------------------------------------------------
void sysinfo(rvr::SystemInfo& sys, rvr::Connection& cmd, rvr::ApiShell& api) {

    // Connection, SysInfo, APIShell
    cmd.bluetoothName(); //

    rvr::RvrMsg dead { 0xDE, 0xAD, 0xFE, 0xED };
    api.echo(dead); // alt

    sys.getMainAppVersion();    // alt
    sys.getBootloaderVersion(); //
    sys.getBoardRevision();     // ??
    sys.getMacId();             // ??
    sys.getStatsId();
    sys.getProcessorName(); // alt
    sys.getSku();           // ??
    sys.getCoreUpTime();    //
                            //

    std::this_thread::sleep_for(500ms);

    mys::tout << code_line;

    mys::tout << code_line;

    mys::tout << code_line << "BT Name: " << cmd.name().get_or();
    mys::tout << code_line << "App Version Nordic: " << sys.mainAppVersionNordic().get_or();
    mys::tout << code_line << "App Version BT: " << sys.mainAppVersionBT().get_or();
    mys::tout << code_line << "Boot Version Nordic: " << sys.bootVersionNordic().get_or();
    mys::tout << code_line << "Boot Version BT: " << sys.bootVersionBT().get_or();
    mys::tout << code_line << "Board Version: " << int(sys.boardVersion().get_or());
    mys::tout << code_line << "MAC Addr: " << sys.macAddress().get_or();
    mys::tout << code_line << "Stats Id: " << sys.statsId().get_or();
    mys::tout << code_line << "Processor BT: " << sys.processorNameBT().get_or();
    mys::tout << code_line << "Processor Nordic: " << sys.processorNameNordic().get_or();
    mys::tout << code_line << "SKU: " << sys.sku().get_or();
    mys::tout << code_line << "Up Time: " << sys.coreUpTime().get_or();

    mys::tout << code_line << "Echo BT: " << std::hex << api.echoBT().get_or();
    mys::tout << code_line << "Echo Nordic: " << std::hex << api.echoNordic().get_or();

    mys::tout << code_line;
}
