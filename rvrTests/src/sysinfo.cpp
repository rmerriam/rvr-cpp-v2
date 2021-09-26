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

   sys.getMainAppVersion();    // alt
   sys.getBootloaderVersion(); //
   sys.getBoardRevision();     // ??
   sys.getMacId();             // ??
   sys.getStatsId();
   sys.getProcessorName(); // alt
   sys.getSku();           // ??
   sys.getCoreUpTime();    //
                           //
   rvr::RvrMsg dead { 0xDE, 0xAD, 0xFE, 0xED };
   api.echo(dead); // alt

   std::this_thread::sleep_for(100ms);

   mys::tout << code_line << mys::nl;

   mys::tout << code_line << mys::nl;

   mys::tout << code_line << "App Version: " << sys.mainAppVersion().get();
   mys::tout << code_line << "App Version: " << sys.mainAppVersion2().get();
   mys::tout << code_line << "Boot Version: " << sys.bootVersion().get();
   mys::tout << code_line << "Boot Version: " << sys.bootVersion2().get();
   mys::tout << code_line << "Board Version: " << int(sys.boardVersion().get());
   mys::tout << code_line << "MAC Addr: " << sys.macAddress().get();
   mys::tout << code_line << "Stats Id: " << sys.statsId().get();
   mys::tout << code_line << "Processor: " << sys.processorName().get();
   mys::tout << code_line << "Processor: " << sys.processorName2().get();
   mys::tout << code_line << "SKU: " << sys.sku().get();
   mys::tout << code_line << "Up Time: " << sys.coreUpTime().get();

   mys::tout << code_line << "BT Name: " << cmd.name().get();

   mys::tout << code_line << "Echo: " << api.echo().get();
   mys::tout << code_line << "Echo Alt: " << api.echoAlt().get();

   mys::tout << code_line << mys::nl;
}
