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
#if 0
    // Direct reading of sensors
    rvr::SensorsDirect sen_d(req);

    sen_d.enableGyroMaxNotify(RespYes);

    sen_d.resetLocatorXY(RespYes);
    sen_d.setLocatorFlags(true, RespYes);   // set/reset? special id flags

    sen_d.enableColorDetection();   // must preceed color detection to turn on bottom LEDs
    std::this_thread::sleep_for(50ms);

    sen_d.getRgbcSensorValue();
    sen_d.getAmbienLightSensorValue();

    sen_d.enableColorDetectionNotify(true, 100, 0);
    std::this_thread::sleep_for(500ms);
    sen_d.getCurrentDectectedColor();

    sen_d.getLeftMotorTemp();
    sen_d.getRightMotorTemp();

    sen_d.getThermalProtectionStatus();
    sen_d.enableThermalProtectionNotify();

    std::this_thread::sleep_for(50ms);

    terr << code_loc << mys::nl;
    terr << code_loc << "sense direct";

    terr << code_loc << "isGyroMaxNotifyEnabled: " << sen_d.isGyroMaxNotifyEnabled();
    terr << code_loc << "isThermalProtectionNotifyEnabled: " << sen_d.isThermalProtectionNotifyEnabled();
    terr << code_loc << "isColorDetectionEnabled: " << sen_d.isColorDetectionEnabled();
    terr << code_loc << "isColorDetectionNotifyEnabled: " << sen_d.isColorDetectionNotifyEnabled();

    {   // allow reuse of rgb
        auto [r, g, b, c] = sen_d.currentRGBValues();
        terr << code_loc << "currentRGBValues: " << r << mys::sp << g << mys::sp << b << mys::sp << c << mys::sp;
        terr << code_loc << "Red: " << (r >> 8) << mys::sp << (g >> 8);
    }
    {
        auto [r, g, b, conf, classification] = sen_d.colorDetectionValues();
        terr << code_loc << "colorDetectionValues: " << r << mys::sp << g << mys::sp << b << mys::sp  //
            << conf << mys::sp << classification << mys::sp;
    }
    auto [left_temp, left_status, right_temp, right_status] = sen_d.thermalProtectionValues();
    terr << code_loc << "thermalProtectionValues: " << left_temp << mys::sp << (int)left_status //
        << mys::sp << right_temp << mys::sp << (int)right_status;

    terr << code_loc << "Ambient: " << sen_d.ambient();
    terr << code_loc << "Left Temp: " << sen_d.leftMotorTemp();
    terr << code_loc << "Right Temp: " << sen_d.rightMotorTemp();
    terr << code_loc << mys::nl;

    sen_d.enableColorDetectionNotify(false, 500, 0);
    sen_d.disableColorDetection(); // turns off LEDs
    sen_d.disableGyroMaxNotify();
//    sen_d.disableThermalProtectionNotify();
    std::this_thread::sleep_for(5000ms);

    terr << code_loc << mys::nl;
    terr << code_loc << "isGyroMaxNotifyEnabled: " << sen_d.isGyroMaxNotifyEnabled();
    terr << code_loc << "isThermalProtectionNotifyEnabled: " << sen_d.isThermalProtectionNotifyEnabled();
    terr << code_loc << "isColorDetectionEnabled: " << sen_d.isColorDetectionEnabled();
    terr << code_loc << "isColorDetectionNotifyEnabled: " << sen_d.isColorDetectionNotifyEnabled();

    terr << code_loc << mys::nl;

#endif

#if 1
    //  Streaming data from sensors
    rvr::SensorsStream sen_s(req);
    rvr::Drive drive(req);

    drive.resetYaw();
//    drive.driveWithHeading(0, 270);
//    std::this_thread::sleep_for(500ms);
//    drive.drive( -25, -25);
//    std::this_thread::sleep_for(1000ms);
//    drive.drive(25, 25);
//    std::this_thread::sleep_for(1000ms);

    rvr::SensorsDirect sen_d(req);
    sen_d.enableColorDetection();   // must precede color detection to turn on bottom LEDs

    sen_s.clearStreaming(RespYes);

    sen_s.accelConfig(RespYes);

    sen_s.ambientConfig();
//    sen_s.colorConfig(RespYes);
//    sen_s.coreNordicConfig();
//    sen_s.coreBTConfig();
    sen_s.gyroConfig();
    sen_s.imuConfig();
    sen_s.locatorConfig();
    sen_s.quaternionConfig();
    sen_s.speedConfig();
//    sen_s.velocityConfig();

    sen_s.enableStreamingBT(50, RespYes);
    sen_s.enableStreamingNordic(50, RespYes);

    std::this_thread::sleep_for(500ms);

    //    sen_s.disableStreamingNordic(RespYes);
    //    sen_s.clearStreamingNordic(RespYes);
    //
    //    sen_s.disableStreamingBT(RespYes);
    //    sen_s.clearStreamingBT(RespYes);

//    sen_s.disableStreaming(RespYes);
//    sen_s.clearStreaming(RespYes);
//    drive.stop(0);

    terr << code_loc << mys::nl;
    terr << code_loc << "Streaming";

    auto [a_x, a_y, a_z] = sen_s.accelerometer();
    terr << code_loc << "accelerometer: " << a_x << mys::sp << a_y << mys::sp << a_z;

    terr << code_loc << "Ambient: " << sen_s.ambient();

    auto [g_x, g_y, g_z] = sen_s.gyroscope();
    terr << code_loc << "gyroscope: " << g_x << mys::sp << g_y << mys::sp << g_z;

    auto [i_x, i_y, i_z] = sen_s.imu();
    terr << code_loc << "imu: " << i_x << mys::sp << i_y << mys::sp << i_z;

    auto [l_x, l_y] = sen_s.locator();
    terr << code_loc << "locator: " << l_x << mys::sp << l_y;

    terr << code_loc << "Speed: " << sen_s.speed();

    auto [v_x, v_y] = sen_s.velocity();
    terr << code_loc << "Velocity: " << v_x << mys::sp << v_y;

    terr << code_loc << mys::nl;

#endif
#if 0
    // POWER

    pow.batteryPercentage();
    pow.batteryVoltageState();

    pow.batteryVoltage(rvr::Power::VoltageType::CalibratedFiltered);
    pow.batteryVoltage(rvr::Power::VoltageType::CalibratedUnfiltered);
    pow.batteryVoltage(rvr::Power::VoltageType::UncalibratedUnfiltered);

    pow.enableBatteryStateChange(RespYes);

    pow.batteryVoltThresholds();
    pow.batteryMotorCurrent(rvr::Power::MotorSide::left);
    pow.batteryMotorCurrent(rvr::Power::MotorSide::right);

    std::this_thread::sleep_for(1s);

    terr << code_loc << mys::nl;
    terr << code_loc << "Power";

    terr << code_loc  << "VPercent: " << pow.batteryPercent();

    terr << code_loc  << "Sleep Notify: " << pow.isDidSleepNotify();
    terr << code_loc  << "State: " << pow.voltState();

    terr << code_loc  << "Wake Notify: " << pow.isWakeNotify();

    terr << code_loc  << "VoltageCF: " << pow.voltsCalibratedFiltered();
    terr << code_loc  << "VoltageCUf: " << pow.voltsCalibratedUnfiltered();
    terr << code_loc  << "VoltageUcUf: " << pow.voltsUncalibratedUnfiltered();
    terr << code_loc  << "L Motor Current: " << pow.motorCurrent(rvr::Power::MotorSide::left);
    terr << code_loc  << "R Motor Current: " << pow.motorCurrent(rvr::Power::MotorSide::right);
    terr << code_loc  << "Critical Threshold: " << pow.voltThresholdCritical();
    terr << code_loc  << "Low Threshold: " << pow.voltThresholdLow();
    terr << code_loc  << "Hysteresis Threshold: " << pow.voltThresholdHysteresis();

    terr << code_loc  << "Wake Notify Set?: " << pow.isWakeNotify();
    terr << code_loc  << "resetWakeNotify ";
    pow.resetWakeNotify();
    terr << code_loc  << "Wake Notify Cleared?: " << pow.isWakeNotify();

    terr << code_loc  << "Set State Change Enabled: " << pow.isBatteryStateChangeEnabled();

    terr << code_loc << mys::nl;
    terr << code_loc  << "disableBatteryStateChange";
    pow.disableBatteryStateChange();
    std::this_thread::sleep_for(50ms);

    terr << code_loc  << "Set State Change Enabled: " << pow.isBatteryStateChangeEnabled();

    terr << code_loc << mys::nl;
#if 1
    pow.sleep();

    std::this_thread::sleep_for(5000ms);    // have to wait for notification
    terr << code_loc  << "Did Sleep Notify: " << pow.isDidSleepNotify();
    terr << code_loc << mys::nl;
#endif

#endif
#if 1
    // DRIVE
//    rvr::Drive drive(req);

    drive.resetYaw(RespYes);
//    std::this_thread::sleep_for(std::chrono::milliseconds(50));
//
//    drive.stop(90, RespYes);
//    std::this_thread::sleep_for(std::chrono::milliseconds(100000));

    drive.driveWithHeading(0, 90);
    std::this_thread::sleep_for(1s);

    for (auto i { 0 }; i < 100; ++i) {
        drive.drive(75, 25);
        std::this_thread::sleep_for(1s);
        auto [l_x, l_y] = sen_s.locator();
        terr << code_loc << "locator: " << l_x << mys::sp << l_y;
    }

//    drive.driveWithHeading(0, 20, RespYes);
//    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//
//    drive.resetYaw(RespYes);
//    std::this_thread::sleep_for(std::chrono::milliseconds(50));
//
//    drive.enableMotorStallNotify(RespYes);
//    drive.enableMotorFaultNotify(RespYes);
//
//    drive.disableMotorStallNotify(RespYes);
//    drive.disableMotorFaultNotify(RespYes);
//
//    drive.getMotorFault(RespYes);
//
//    std::this_thread::sleep_for(100ms);
//
//    terr << code_loc;
//    terr << code_loc;
//    terr << code_loc << "drive";
//
//    terr << code_loc << "Fault Notify State: " << drive.motorFaultState();
//    terr << code_loc << "Fault Notify Set: " << drive.motorFaultNotifySet();
//
//    terr << code_loc << "Stall Notify State: " << drive.motorFaultState();
//    terr << code_loc << "Stall Notify Set: " << drive.motorStallNotifySet();
//
//    terr << code_loc;
//    terr << code_loc;

#endif
#if 0
    // Connection, SysInfo, APIShell
    rvr::Connection cmd(req);
    cmd.bluetoothName(RespYes);    //

    rvr::SystemInfo sys(req);
    sys.getMainAppVersion(RespYes);    // alt
    sys.getBootloaderVersion(RespYes);    //
    sys.getBoardRevision(RespYes);  // ??
    sys.getMacId(RespYes);  // ??
    sys.getStatsId(RespYes);
    sys.getProcessorName(RespYes);  // alt
    sys.getSku(RespYes);    // ??
    sys.getCoreUpTime(RespYes); //

    rvr::ApiShell api(req);
    rvr::RvrMsg dead { 0xDE, 0xAD, 0xFE, 0xED };
    api.echo(dead);    // alt

    std::this_thread::sleep_for(100ms);
    terr << code_loc << mys::nl;
    terr << code_loc << mys::nl;
    terr << code_loc << mys::nl << "Connection, SysInfo, APIShell";
    terr << code_loc << "App Version: " << std::hex << sys.mainAppVersion();
    terr << code_loc << "App Version: " << std::hex << sys.mainAppVersion2();
    terr << code_loc << "Boot Version: " << std::hex << sys.bootVersion();
    terr << code_loc << "Boot Version: " << std::hex << sys.bootVersion2();
    terr << code_loc << "Board Version: " << std::hex << sys.boardVersion();
    terr << code_loc << "MAC Addr: " << sys.macAddress();
    terr << code_loc << "Stats Id: " << sys.statsId();
    terr << code_loc << "Processor: " << sys.processorName();
    terr << code_loc << "Processor: " << sys.processorName2();
    terr << code_loc << "SKU: " << sys.sku();
    terr << code_loc << "Up Time: " << sys.coreUpTime();

    terr << code_loc << "BT Name: " << cmd.name();

    terr << code_loc << "Echo: " << std::hex << api.echo();
    terr << code_loc << "Echo Alt: " << std::hex << api.echoAlt();

    terr << code_loc << mys::nl;

#endif

    std::this_thread::sleep_for(1s);
    terr << code_loc << "----------------" << mys::nl;

    pow.sleep();

    end_tasks.set_value();
    resp_future.get();

    rvr::Blackboard::m_to_v();

    return 0;
}

