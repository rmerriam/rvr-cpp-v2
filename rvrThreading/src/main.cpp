//======================================================================================================================
// 2019 Copyright Mystic Lake Software
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
//     Author: rmerriam
//
//     Created: Nov 8, 2019
//
//======================================================================================================================
#include <chrono>
#include <iomanip>
#include <iostream>
#include <future>
#include <version>

using namespace std::literals;

#include "Trace.h"
#include "Response.h"
#include <Blackboard.h>

#include "ApiShell.h"
#include "Connection.h"
#include "Drive.h"
#include "IoLed.h"
#include "Power.h"
#include "Sensors.h"
#include "SystemInfo.h"
//---------------------------------------------------------------------------------------------------------------------
mys::TraceStart terr { std::cerr };
rvr::CommandBase::CommandResponse RespYes = rvr::CommandBase::resp_yes;
//---------------------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[]) {

    terr << code_loc << " Opening serial " << argv[1] << std::setprecision(4) << std::fixed;

    SerialPort serial { argv[1], 115200 };
    rvr::Request req { serial };

    //---------------------------------------------------------------------------------------------------------------------
    //  Setup the thread to read responses
    std::promise<void> end_tasks;
    std::shared_future<void> end_future(end_tasks.get_future());
    rvr::Response resp { serial, end_future };

    auto resp_future = std::async(std::launch::async, std::ref(resp));

    rvr::Power pow(req);

//    rvr::Blackboard::dump();
    pow.awake();
    std::this_thread::sleep_for(100ms);

#if 1
    rvr::Sensors sen(req);

//    sen.enableGyroMaxNotify(RespYes);
//    sen.disableGyroMaxNotify(RespYes);
//    sen.getAmbient(RespYes);
//
//    sen.enableThermal(RespYes);
//    sen.getThermalData(RespYes);
//    std::this_thread::sleep_for(500ms);
//
//    sen.getRightMotorTemp(RespYes);
//    sen.getLeftMotorTemp(RespYes);
//    sen.disableThermal(RespYes);

    /// change size back to 15 in sensors!!!!
    rvr::MsgArray accel { 0x01, 0x00, 0x02, 0x01, 0x00, 0x01, 0x01 };
    sen.configureStreaming(1, accel, 2, RespYes);

    rvr::MsgArray core { 0x02, 0x00, 0x09, 0x01 };
    sen.configureStreaming(3, core, 1, RespYes);

    rvr::MsgArray speed { 0x02, 0x00, 0x08, 0x01 };
    sen.configureStreaming(2, speed, 2, RespYes);

    rvr::MsgArray velocity { 0x02, 0x00, 0x07, 0x02 };
    sen.configureStreaming(2, velocity, 2, RespYes);

    rvr::MsgArray ambient { 0x02, 0x00, 0x0A, 0x01 };
    sen.configureStreaming(2, ambient, 1, RespYes);

    rvr::MsgArray locator { 0x01, 0x00, 0x06, 0x02 };
    sen.configureStreaming(2, locator, 0, RespYes);

    std::this_thread::sleep_for(500ms);

    sen.enableStreaming(1000);

    std::this_thread::sleep_for(2000ms);
    sen.disableStreaming();
    sen.clearStreaming();

#elif 0
    //---------------------------------------------------------------------------------------------------------------------
    //  Setup the LED handling
    rvr::IoLed led(req);

    uint32_t led32 { Led::headlight_left | Led::headlight_right };

    rvr::MsgArray colors[2] { { 0x00, 0x00, 0xFF, //
                                0xFF, 0x00, 0x00, }, //
    { 0xFF, 0x00, 0x00, //
      0x00, 0x00, 0xFF, }, //
    };

    for (auto i { 0 }; i < 10; ++i) {
        led.allLed(led32, colors[i % 2], RespYes);
        terr << "blink";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    led.idleLeds(RespYes);

#elif 1
    terr << code_loc << mys::sp << "Sleep Notify: " << pow.isSleepNotify() << " =========";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    terr << code_loc << mys::sp << "Set State Change?: " << pow.checkBatteryStateChange();

    pow.enableBatteryStateChange(RespYes);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    terr << code_loc << mys::sp << "Set State Change on?: " << pow.checkBatteryStateChange();

    pow.disableBatteryStateChange(RespYes);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    terr << code_loc << mys::sp << "Set State Change off?: " << pow.checkBatteryStateChange();

    pow.batteryVoltageState(RespYes);
    pow.batteryVoltage(rvr::Power::VoltageType::CalibratedFiltered, RespYes);
    pow.batteryVoltage(rvr::Power::VoltageType::CalibratedUnfiltered, RespYes);
    pow.batteryVoltage(rvr::Power::VoltageType::UncalibratedUnfiltered, RespYes);

    pow.batteryVoltThresholds(RespYes);
    pow.batteryMotorCurrent(rvr::Power::MotorSide::left, RespYes);
    pow.batteryMotorCurrent(rvr::Power::MotorSide::right, RespYes);
    pow.batteryVoltageState(RespYes);

    pow.batteryPercentage(RespYes);

    std::this_thread::sleep_for(1s);
//    rvr::Blackboard::dump();
    terr << code_loc << mys::nl;
    terr << code_loc << mys::sp << "Wake Notify: " << pow.isWakeNotify() << " =========";
    pow.resetWakeNotify();
    terr << code_loc << mys::sp << "Wake Notify: " << pow.isWakeNotify() << " =========";

    terr << code_loc << mys::sp << "VoltageCF: " << pow.voltsCalibratedFiltered();
    terr << code_loc << mys::sp << "VoltageCUf: " << pow.voltsCalibratedUnfiltered();
    terr << code_loc << mys::sp << "VoltageUcUf: " << pow.voltsUncalibratedUnfiltered();
    terr << code_loc << mys::sp << "State: " << pow.voltState();
    terr << code_loc << mys::sp << "L Motor Current: " << pow.motorCurrent(rvr::Power::MotorSide::left);
    terr << code_loc << mys::sp << "R Motor Current: " << pow.motorCurrent(rvr::Power::MotorSide::right);
    terr << code_loc << mys::sp << "Critical Threshold: " << pow.voltThresholdCritical();
    terr << code_loc << mys::sp << "Low Threshold: " << pow.voltThresholdLow();
    terr << code_loc << mys::sp << "Hysteresis Threshold: " << pow.voltThresholdHysteresis();
    terr << code_loc << mys::sp << "VPercent: " << pow.batteryPercent();

    terr << code_loc << mys::sp << "Set State Change?: " << pow.checkBatteryStateChange();
    terr << code_loc << mys::nl;

#elif 0
    rvr::Drive drive(req);

    drive.fixHeading(RespYes);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    drive.stop(90, RespYes);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    drive.drive(25, 25, RespYes);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    drive.spin_drive(0, 20, RespYes);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    drive.fixHeading(RespYes);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    drive.enableMotorStallNotify(RespYes);
    drive.enableMotorFaultNotify(RespYes);

    drive.disableMotorStallNotify(RespYes);
    drive.disableMotorFaultNotify(RespYes);

    drive.getMotorFault(RespYes);

#elif 0
    rvr::ApiShell api(req);
    rvr::Connection cmd(req);
    rvr::SystemInfo sys(req);

    rvr::MsgArray dead { 0xDE, 0xAD, 0xFE, 0xED };
    api.echo(dead, RespYes);    // alt
    cmd.bluetoothName(RespYes);    //
    sys.getBoardRevision(RespYes);  // ??
    sys.getBootloaderVersion(RespYes);    //
    sys.getMacId(RespYes);  // ??
    sys.getMainAppVersion(RespYes);    // alt
    sys.getProcessorName(RespYes);  // alt
    sys.getSku(RespYes);    // ??
    sys.getStatsId(RespYes);
    sys.getUpTime(RespYes); //

    std::this_thread::sleep_for(1s);
    terr << code_loc << mys::nl;

    terr << code_loc << "App Version: " << std::hex << sys.mainAppVersion();
    terr << code_loc << "App Version: " << std::hex << sys.mainAppVersion2();
    terr << code_loc << "Board Version: " << std::hex << sys.boardVersion();
    terr << code_loc << "Boot Version: " << std::hex << sys.bootVersion();
    terr << code_loc << "Boot Version: " << std::hex << sys.bootVersion2();
    terr << code_loc << "BT Name: " << cmd.name();
    terr << code_loc << "Echo: " << std::hex << api.echo();
    terr << code_loc << "MAC Addr: " << sys.macAddress();
    terr << code_loc << "Processor: " << sys.processorName();
    terr << code_loc << "Processor: " << sys.processorName2();
    terr << code_loc << "SKU: " << sys.sku();
    terr << code_loc << "Stats Id: " << sys.statsId();
    terr << code_loc << "Up Time: " << sys.upTime();

    terr << code_loc << mys::nl;

#endif

    std::this_thread::sleep_for(1s);
    terr << code_loc << mys::nl;

    pow.sleep();
    std::this_thread::sleep_for(std::chrono::milliseconds(15000));
    terr << code_loc << mys::sp << "Sleep Notify: " << pow.isSleepNotify() << " =========";

    end_tasks.set_value();
    resp_future.get();
//    pow.powerOff();

    return 0;
}

