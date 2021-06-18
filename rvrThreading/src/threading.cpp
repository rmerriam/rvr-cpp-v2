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
//     Author: rmerriam
//
//     Created: May 29, 2021
//
//======================================================================================================================
#include <chrono>
#include <iomanip>
#include <iostream>
#include <future>
#include <version>
#include <cmath>

using namespace std::literals;

#include <rvr++.h>

#include "opt_output.h"
//---------------------------------------------------------------------------------------------------------------------
void color(rvr::SensorsDirect& sen_d);
void direct(rvr::SensorsDirect& sen_d);
void leds_test(rvr::IoLed& led);
void streaming(rvr::SensorsStream& sen_s);
//---------------------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
//    mys::TraceOn mys::tdbg_on { mys::tdbg };
    mys::TraceOff tdbg_off { mys::tdbg };
    mys::tdbg << code_loc << " Opening serial " << argv[1];

    mys::tinfo << code_loc << " Opening serial " << argv[1];

    SerialPort serial { argv[1], 115200 };
    rvr::SendPacket req { serial };
    rvr::ReadPacket in_packet { serial };
    rvr::Blackboard bb;
//---------------------------------------------------------------------------------------------------------------------
//  Setup the response thread to read responses
    std::promise<void> end_tasks;
    std::shared_future<void> end_future(end_tasks.get_future());
    rvr::Response resp { in_packet, bb, end_future };
    mys::tdbg << code_loc << "----------------" << mys::nl;

    auto resp_future = std::async(std::launch::async, std::ref(resp));
    //---------------------------------------------------------------------------------------------------------------------
    rvr::Drive drive(bb, req);
    rvr::IoLed led(bb, req);
    rvr::Power pow(bb, req);
    rvr::SensorsDirect sen_d(bb, req);
    rvr::SensorsStream sen_s(bb, req);

    pow.awake(rvr::CommandResponse::resp_yes);
    std::this_thread::sleep_for(500ms);
//=====================================================================================================================
    try {

//        color(sen_d);
//        direct(sen_d);
//        leds_test(led);
        streaming(sen_s);

#if 0

        sen_d.calibrateMagnetometer(rvr::CommandResponse::resp_yes);
        std::this_thread::sleep_for(5000ms);
//        mys:;tinfo<< code_loc << "mag cal done: " << sen_d.isMagnetometerCalibrationDone().value() << mys::nl;
        std::this_thread::sleep_for(500ms);

        sen_d.calibrateMagnetometer(rvr::CommandResponse::resp_yes);
        std::this_thread::sleep_for(5000ms);

//        while (true)  //
//        {
//            sen_d.getMagnetometerData(rvr::CommandResponse::resp_yes);
//            std::this_thread::sleep_for(500ms);
//
//            auto [m_x, m_y, m_z] { sen_d.magnetometer().value_or(rvr::MagnetometerData { }) };
//            auto angle { std::atan2(m_x, m_y) };
//            mys:;tinfo<< code_loc << "magnetometer: " << std::setfill(' ') //
//                 << std::setw(8) << m_x << mys::sp  //
//                 << std::setw(8) << m_y << mys::sp //
//                 << std::setw(8) << m_z << mys::sp  //
//                 << std::setw(8) << angle << mys::sp  //
//                 << std::setw(8) << angle * 180.0 / M_PI;
//        }
//        {
//            sen_d.getMagnetometerData(rvr::CommandResponse::resp_yes);
//            std::this_thread::sleep_for(500ms);
//
//            auto [m_x, m_y, m_z] { sen_d.magnetometer().value_or(rvr::MagnetometerData { }) };
//            auto angle { std::atan2(m_y, m_x) };
//            mys:;tinfo<< code_loc << "magnetometer: " << std::setfill(' ')  //
//                 << std::setw(8) << m_x << mys::sp  //
//                 << std::setw(8) << m_y << mys::sp //
//                 << std::setw(8) << m_z << mys::sp  //
//                 << std::setw(8) << angle << mys::sp  //
//                 << std::setw(8) << angle * 180.0 / M_PI;
//        }

#endif

#if 0
        // POWER

        pow.batteryPercentage();
        pow.batteryVoltageState();

        pow.batteryVoltage(rvr::Power::VoltageType::CalibratedFiltered);
        pow.batteryVoltage(rvr::Power::VoltageType::CalibratedUnfiltered);
        pow.batteryVoltage(rvr::Power::VoltageType::UncalibratedUnfiltered);

        pow.enableBatteryStateChange();

        pow.batteryVoltThresholds();
        pow.batteryMotorCurrent(rvr::Power::MotorSide::left);
        pow.batteryMotorCurrent(rvr::Power::MotorSide::right);

        std::this_thread::sleep_for(1s);

        mys:;tinfo<< code_loc << mys::nl;
        mys:;tinfo<< code_loc << "Power";

        opt_output("VPercent: ", pow.batteryPercent());

        opt_output("Sleep Notify: ", pow.isDidSleepNotify());
        opt_output("State: ", pow.voltState());

        opt_output("Wake Notify: ", pow.isWakeNotify());

        opt_output("VoltageCF: ", pow.voltsCalibratedFiltered());
        opt_output("VoltageCUf: ", pow.voltsCalibratedUnfiltered());
        opt_output("VoltageUcUf: ", pow.voltsUncalibratedUnfiltered());
        opt_output("L Motor Current: ", pow.motorCurrent(rvr::Power::MotorSide::left));
        opt_output("R Motor Current: ", pow.motorCurrent(rvr::Power::MotorSide::right));
        opt_output("Critical Threshold: ", pow.voltThresholdCritical());
        opt_output("Low Threshold: ", pow.voltThresholdLow());
        opt_output("Hysteresis Threshold: ", pow.voltThresholdHysteresis());

        opt_output("Wake Notify Set?: ", pow.isWakeNotify());

        opt_output("Wake Notify Cleared?: ", pow.isWakeNotify());

        opt_output("Set State Change Enabled: ", pow.isBatteryStateChangeEnabled());

        mys:;tinfo<< code_loc << mys::nl;
        mys:;tinfo<< code_loc << "disableBatteryStateChange";
        pow.disableBatteryStateChange();
        std::this_thread::sleep_for(50ms);

        opt_output("Set State Change Enabled: ", pow.isBatteryStateChangeEnabled());

        mys:;tinfo<< code_loc << mys::nl;
#if 0
    pow.sleep();

    std::this_thread::sleep_for(5000ms);    // have to wait for notification
    mys:;tinfo<< code_loc  << "Did Sleep Notify: " << pow.isDidSleepNotify();
    mys:;tinfo<< code_loc << mys::nl;
#endif

#endif
#if 0
        // DRIVE
        rvr::Drive drive(bb, req);
        rvr::SensorsStream sen_s(bb, req);

        drive.resetYaw();
//    std::this_thread::sleep_for(std::chrono::milliseconds(50));
//
//    drive.stop(90, );
//    std::this_thread::sleep_for(std::chrono::milliseconds(100000));

//        drive.driveWithHeading(0, 90);
//        std::this_thread::sleep_for(1s);
//
//        for (auto i { 0 }; i < 10; ++i) {
//            drive.drive(75, 25);
//            std::this_thread::sleep_for(3s);
//            auto [l_x, l_y] { sen_s.locator().value_or(rvr::LocatorData { }) };
//            mys:;tinfo<< code_loc << "locator: " << l_x << mys::sp << l_y;
//        }

        drive.tank_normalized(15, 15);
        std::this_thread::sleep_for(1s);

//    drive.driveWithHeading(0, 20, );
//    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//
//    drive.resetYaw();
//    std::this_thread::sleep_for(std::chrono::milliseconds(50));
//
//    drive.enableMotorStallNotify();
//    drive.enableMotorFaultNotify();
//
//    drive.disableMotorStallNotify();
//    drive.disableMotorFaultNotify();
//
//    drive.getMotorFault();
//
//    std::this_thread::sleep_for(100ms);
//
//    mys:;tinfo<< code_loc;
//    mys:;tinfo<< code_loc;
//    mys:;tinfo<< code_loc << "drive";
//
//    mys:;tinfo<< code_loc << "Fault Notify State: " << drive.motorFaultState();
//    mys:;tinfo<< code_loc << "Fault Notify Set: " << drive.motorFaultNotifySet();
//
//    mys:;tinfo<< code_loc << "Stall Notify State: " << drive.motorFaultState();
//    mys:;tinfo<< code_loc << "Stall Notify Set: " << drive.motorStallNotifySet();
//
//    mys:;tinfo<< code_loc;
//    mys:;tinfo<< code_loc;

#endif
#if 0
        // Connection, SysInfo, APIShell
        rvr::Connection cmd(bb, req);
        cmd.bluetoothName();    //

        rvr::SystemInfo sys(bb, req);
        sys.getMainAppVersion();    // alt
        sys.getBootloaderVersion();    //
        sys.getBoardRevision();  // ??
        sys.getMacId();  // ??
        sys.getStatsId();
        sys.getProcessorName();  // alt
        sys.getSku();    // ??
        sys.getCoreUpTime(); //
//
        rvr::ApiShell api(bb, req);
        rvr::RvrMsg dead { 0xDE, 0xAD, 0xFE, 0xED };
        api.echo(dead);    // alt

        std::this_thread::sleep_for(100ms);
        mys:;tinfo<< code_loc << mys::nl;
        mys:;tinfo<< code_loc << mys::nl;
        mys:;tinfo<< code_loc << mys::nl << "Connection, SysInfo, APIShell";
        opt_output("App Version: ", sys.mainAppVersion());
        opt_output("App Version: ", sys.mainAppVersion2());
        opt_output("Boot Version: ", sys.bootVersion());
        opt_output("Boot Version: ", sys.bootVersion2());
        opt_output("Board Version: ", sys.boardVersion());
        opt_output("MAC Addr: ", sys.macAddress());
        opt_output("Stats Id: ", sys.statsId());
        opt_output("Processor: ", sys.processorName());
        opt_output("Processor: ", sys.processorName2());
        opt_output("SKU: ", sys.sku());
        opt_output("Up Time: ", sys.coreUpTime());

        opt_output("BT Name: ", cmd.name());

        opt_output_hex("Echo: ", api.echo());
        opt_output_hex("Echo Alt: ", api.echoAlt());

        mys:;tinfo<< code_loc << mys::nl;

#endif
#if 0
    constexpr float in_to_m { 0.0254 };

    rvr::Drive drive(bb, req);
    rvr::SensorsStream sen_s(bb, req);
    rvr::SensorsDirect sen_d(bb, req);

    sen_d.resetLocatorXY();
    drive.resetYaw();
    sen_s.clearAllStreaming();
    mys::TraceOff toff { mys:;tinfo};

    sen_s.locatorConfig();
    sen_s.enableStreaming(30);
    std::this_thread::sleep_for(30ms);
    mys:;tinfo<< code_loc << mys::nl;

    rvr::LocatorData l { sen_s.locator() };
    mys:;tinfo << code_loc << mys::nl;
    mys:;tinfo << code_loc << "locator: " << l.x / in_to_m << mys::sp << l.y / in_to_m;

    float forty_in = -40 * in_to_m;
    double sp { 25 };
    while (l.y > forty_in) {
//        drive.drive(sp, sp);
        drive.driveWithHeading(sp, 0);
        std::this_thread::sleep_for(30ms);
        l = sen_s.locator();
        mys:;tinfo << code_loc << "locator: " << l.x / in_to_m << mys::sp << l.y / in_to_m;

    }
    drive.stop(0);
    l = sen_s.locator();
    mys:;tinfo << code_loc << "locator: " << l.x / in_to_m << mys::sp << l.y / in_to_m;
    std::this_thread::sleep_for(30ms);

    l = sen_s.locator();
    mys:;tinfo << code_loc << "locator: " << l.x / in_to_m << mys::sp << l.y / in_to_m;

    while (l.y < 0) {
        drive.driveWithHeading( -sp, 0);
        std::this_thread::sleep_for(30ms);
        l = sen_s.locator();
    }
    drive.stop(0);
    l = sen_s.locator();
    mys:;tinfo << code_loc << "locator: " << l.x / in_to_m << mys::sp << l.y / in_to_m;
    std::this_thread::sleep_for(30ms);

    l = sen_s.locator();
    mys:;tinfo << code_loc << "locator: " << l.x / in_to_m << mys::sp << l.y / in_to_m;

#endif
    }
    catch (std::exception& e) {
        mys::tdbg << code_loc << e.what() << "=================================";
    }

    pow.sleep(rvr::CommandResponse::resp_yes);

    std::this_thread::sleep_for(1s);
    end_tasks.set_value();
    resp_future.get();

    bb.m_to_v();

    return 0;
}

