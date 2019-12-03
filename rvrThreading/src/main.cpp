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
#include "Blackboard.h"
#include "Response.h"

#include "ApiShell.h"
#include "Connection.h"
#include "Drive.h"
#include "IoLed.h"
#include "Power.h"
#include "SensorsDirect.h"
#include "SensorsStream.h"
#include "SystemInfo.h"
//---------------------------------------------------------------------------------------------------------------------
mys::TraceStart terr { std::cerr };
rvr::CommandBase::CommandResponse RespYes = rvr::CommandBase::resp_yes;
//---------------------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[]) {

    terr << code_loc << " Opening serial " << argv[1] << std::setprecision(4) << std::fixed << std::boolalpha;

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
    std::this_thread::sleep_for(500ms);
    //---------------------------------------------------------------------------------------------------------------------

#if 0
    //---------------------------------------------------------------------------------------------------------------------
    //  Setup the LED handling
    rvr::IoLed led(req);
    led.idleLeds(RespYes);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    uint32_t led32 {    //
    Led::status_indication_left | Led::status_indication_right |    //
        Led::headlight_left | Led::headlight_right };

    rvr::RvrMsg colors[] { //
    { 0x00, 0x00, 0xFF, //
        0xFF, 0x00, 0x00, //
        0x00, 0x00, 0xFF, //
        0xFF, 0x00, 0x00, }, //
        { 0xFF, 0x00, 0x00, //
            0x00, 0x00, 0xFF, //
            0xFF, 0x00, 0x00, //
            0x00, 0x00, 0xFF, }, //
    };

    for (auto i { 0 }; i < 10; ++i) {
        led.allLed(led32, colors[i % 2], RespYes);
        terr << "blink";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

//    led.idleLeds(RespYes);

#endif
#if 1
    // Direct reading of sensors
    rvr::SensorsDirect sen_d(req);

    sen_d.enableThermal(RespYes);
    sen_d.enableColorDetection(RespYes);
    sen_d.enabeColorDetectionNotify(true, 500, 0, RespYes);
    sen_d.enableGyroMaxNotify(RespYes);

//    terr << code_loc << mys::sp << "Notify Thermal?: " << sen_d.isThermalNotifySet();
//    terr << code_loc << mys::sp << "Notify Max Gyro?: " << sen_d.isMaxGyroNotifySet();
    terr << code_loc;
    terr << code_loc;

    sen_d.getAmbient(RespYes);

    std::this_thread::sleep_for(500ms);

    sen_d.getCurrentColor(RespYes);

    sen_d.setLocatorFlags(true, RespYes);
    sen_d.resetLocatorXY(RespYes);

    sen_d.getRightMotorTemp(RespYes);
    sen_d.getLeftMotorTemp(RespYes);
    sen_d.getThermalProtectionStatus(RespYes);

    std::this_thread::sleep_for(100ms);
    sen_d.disableColorDetection(RespYes);
    sen_d.disableGyroMaxNotify(RespYes);
    sen_d.disableThermal(RespYes);

    std::this_thread::sleep_for(500ms);

    terr << code_loc;
    terr << code_loc;
    terr << code_loc << "sense direct";

    terr << code_loc << mys::sp << "Ambient: " << sen_d.ambient();
    terr << code_loc << mys::sp << "Left Temp: " << sen_d.leftMotorTemp();
    terr << code_loc << mys::sp << "Right Temp: " << sen_d.rightMotorTemp();

#endif

#if 0
    //  Streaming data from sensors
    rvr::SensorsStream sen_s(req);

/// change size back to 15 in sensors!!!!
//    rvr::RvrMsg accel { 0x01, 0x00, 0x02, 0x01, 0x00, 0x01, 0x01 };
//    sen_s.configureStreaming(1, accel, 2, RespYes);

//    rvr::RvrMsg core { 0x02, 0x00, 0x09, 0x01 };
//    sen_s.configureStreaming(3, core, 1, RespYes);
//
//    rvr::RvrMsg speed { 0x02, 0x00, 0x08, 0x01 };
//    sen_s.configureStreaming(2, speed, 2, RespYes);
//
//    rvr::RvrMsg velocity { 0x02, 0x00, 0x07, 0x02 };
//    sen_s.configureStreaming(2, velocity, 2, RespYes);
//
    rvr::RvrMsg ambient { 2, 0x00, 0x0A, 0x02 };
    sen_s.configureStreaming(ambient, RespYes);
//
//    rvr::RvrMsg locator { 0x07, 0x00, 0x08, 0x02 };
//    sen_s.configureStreaming(1, locator, 0, RespYes);

    std::this_thread::sleep_for(20ms);
    sen_s.enableStreaming(500, RespYes);
//    sen_d.enableStreaming(500, RespYes);
    std::this_thread::sleep_for(2000ms);
//
    sen_s.disableStreaming(RespYes);
    sen_s.clearStreaming(RespYes);

//

    std::this_thread::sleep_for(500ms);

#endif
#if 1

    terr << code_loc << mys::sp << "Set State Change?: " << pow.checkBatteryStateChange();

    pow.enableBatteryStateChange(RespYes);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    terr << code_loc << mys::sp << "Set State Change on?: " << pow.checkBatteryStateChange();

//    pow.disableBatteryStateChange(RespYes);
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

    terr << code_loc << mys::nl;

    terr << code_loc << mys::sp << "Sleep Notify: " << pow.isSleepNotify();
    terr << code_loc << mys::sp << "State: " << pow.voltState();

    terr << code_loc << mys::sp << "Wake Notify: " << pow.isWakeNotify();
    pow.resetWakeNotify();
    terr << code_loc << mys::sp << "Wake Notify: " << pow.isWakeNotify();

    terr << code_loc << mys::sp << "VoltageCF: " << pow.voltsCalibratedFiltered();
    terr << code_loc << mys::sp << "VoltageCUf: " << pow.voltsCalibratedUnfiltered();
    terr << code_loc << mys::sp << "VoltageUcUf: " << pow.voltsUncalibratedUnfiltered();
    terr << code_loc << mys::sp << "L Motor Current: " << pow.motorCurrent(rvr::Power::MotorSide::left);
    terr << code_loc << mys::sp << "R Motor Current: " << pow.motorCurrent(rvr::Power::MotorSide::right);
    terr << code_loc << mys::sp << "Critical Threshold: " << pow.voltThresholdCritical();
    terr << code_loc << mys::sp << "Low Threshold: " << pow.voltThresholdLow();
    terr << code_loc << mys::sp << "Hysteresis Threshold: " << pow.voltThresholdHysteresis();
    terr << code_loc << mys::sp << "VPercent: " << pow.batteryPercent();

    terr << code_loc << mys::sp << "Set State Change?: " << pow.checkBatteryStateChange();
    terr << code_loc << mys::nl;

#endif
#if 0
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
#endif
#if 0
    rvr::ApiShell api(req);
    rvr::Connection cmd(req);
    rvr::SystemInfo sys(req);

    rvr::RvrMsg dead { 0xDE, 0xAD, 0xFE, 0xED };
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
    terr << code_loc << "Boot Version: " << std::hex << sys.bootVersion();
    terr << code_loc << "Boot Version: " << std::hex << sys.bootVersion2();
    terr << code_loc << "Board Version: " << std::hex << sys.boardVersion();
    terr << code_loc << "Processor: " << sys.processorName();
    terr << code_loc << "Processor: " << sys.processorName2();
    terr << code_loc << "MAC Addr: " << sys.macAddress();
    terr << code_loc << "SKU: " << sys.sku();
    terr << code_loc << "Stats Id: " << sys.statsId();
    terr << code_loc << "Up Time: " << sys.upTime();
    terr << code_loc << "Echo: " << std::hex << api.echo();
    terr << code_loc << "BT Name: " << cmd.name();

    terr << code_loc << mys::nl;

#endif

    std::this_thread::sleep_for(1s);
    terr << code_loc << "----------------" << mys::nl;

    pow.sleep();

//    pow.powerOff(5);
//    std::this_thread::sleep_for(10s);
//    terr << code_loc << mys::sp << "Sleep Notify: " << pow.isSleepNotify() << " =========";

    end_tasks.set_value();
    resp_future.get();

    rvr::Blackboard::m_to_v();

    return 0;
}

