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
mys::TraceStart tout { std::cout };
//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void opt_output(std::string const& text, std::optional<T> v) {
    terr << code_loc << text << mys::sp << (v ? v.value() : T { });
}
//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void opt_output_hex(std::string const& text, std::optional<T> v) {
    terr << code_loc << text << mys::sp << std::hex << (v ? v.value() : T { });
}
//---------------------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[]) {

    terr << code_loc << " Opening serial " << argv[1] << std::setprecision(4) << std::fixed << std::boolalpha;

    SerialPort serial { argv[1], 115200 };
    rvr::SendPacket req { serial };
    rvr::ReadPacket in_packet { serial };
    rvr::Blackboard bb;

//---------------------------------------------------------------------------------------------------------------------
//  Setup the thread to read responses
    std::promise<void> end_tasks;
    std::shared_future<void> end_future(end_tasks.get_future());
    rvr::Response resp { in_packet, bb, end_future };

    auto resp_future = std::async(std::launch::async, std::ref(resp));

    rvr::Power pow(bb, req);

    pow.awake();
    std::this_thread::sleep_for(500ms);
//---------------------------------------------------------------------------------------------------------------------
    try {
#if 0
        //---------------------------------------------------------------------------------------------------------------------
        //  Setup the LED handling
        rvr::IoLed led(bb, req);
        led.ledsOff();

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
            led.allLed(led32, colors[i % 2]);
            terr << "blink";
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        led.idleLeds();
        std::this_thread::sleep_for(5000ms);

#endif
#if 0
        // Direct reading of sensors
        rvr::SensorsDirect sen_d(bb, req);

        sen_d.enableGyroMaxNotify();

        sen_d.resetLocatorXY();
        sen_d.setLocatorFlags(true);   // set/reset? special id flags

        sen_d.enableColorDetection();   // must preceed color detection to turn on bottom LEDs
        std::this_thread::sleep_for(50ms);

        sen_d.getRgbcSensorValue();
        sen_d.getAmbienLightSensorValue();

        sen_d.enableColorDetectionNotify(true, 100, 0);
        sen_d.getCurrentDectectedColor();

        sen_d.getLeftMotorTemp();
        sen_d.getRightMotorTemp();

        sen_d.getThermalProtectionStatus();
        sen_d.enableThermalProtectionNotify();  // responds when status changes

        std::this_thread::sleep_for(150ms);

        terr << code_loc << mys::nl;
        terr << code_loc << "sense direct";

        terr << code_loc << "isGyroMaxNotifyEnabled: " << sen_d.isGyroMaxNotifyEnabled().value();
        terr << code_loc << "isThermalProtectionNotifyEnabled: " << sen_d.isThermalProtectionNotifyEnabled().value();
        terr << code_loc << "isColorDetectionEnabled: " << sen_d.isColorDetectionEnabled().value();
        terr << code_loc << "isColorDetectionNotifyEnabled: " << sen_d.isColorDetectionNotifyEnabled().value();

        std::this_thread::sleep_for(50ms);

        auto [c_r, c_g, c_b, c_c] { sen_d.currentRGBValues().value_or(rvr::ColorData { }) };
        terr << code_loc << "currentRGBValues: " << c_r << mys::sp << c_g << mys::sp << c_b << mys::sp << c_c << mys::sp;

        auto [d_r, d_g, d_b, conf, classification] { sen_d.colorDetectionValues().value_or(rvr::ColorDetection { }) };
        terr << code_loc << "colorDetectionValues: " << (int)d_r << mys::sp << (int)d_g << mys::sp << (int)d_b << mys::sp  //
             << (int)conf << mys::sp << (int)classification << mys::sp;

        auto [left_temp, left_status, right_temp, right_status] { sen_d.thermalProtectionValues().value_or(rvr::ThermalProtection { }) };
        terr << code_loc << "thermalProtectionValues: " << left_temp << mys::sp << (int)left_status //
             << mys::sp << right_temp << mys::sp << (int)right_status;

        opt_output("Ambient"s, sen_d.ambient(), -1.0f);
        opt_output("Left Temp:"s, sen_d.leftMotorTemp(), -1.0f);
        opt_output("Right Temp:"s, sen_d.rightMotorTemp(), -1.0f);

        terr << code_loc << "Left Temp: " << sen_d.leftMotorTemp().value();
        terr << code_loc << "Right Temp: " << sen_d.rightMotorTemp().value();
        terr << code_loc << mys::nl;

        sen_d.enableColorDetectionNotify(false, 500, 0);
        sen_d.disableColorDetection(); // turns off bottom LEDs
        sen_d.disableGyroMaxNotify();
        sen_d.disableThermalProtectionNotify();
        std::this_thread::sleep_for(50ms);

        terr << code_loc << mys::nl;
        terr << code_loc << "isGyroMaxNotifyEnabled: " << sen_d.isGyroMaxNotifyEnabled().value();
        terr << code_loc << "isThermalProtectionNotifyEnabled: " << sen_d.isThermalProtectionNotifyEnabled().value();
        terr << code_loc << "isColorDetectionEnabled: " << sen_d.isColorDetectionEnabled().value();
        terr << code_loc << "isColorDetectionNotifyEnabled: " << sen_d.isColorDetectionNotifyEnabled().value();

        terr << code_loc << mys::nl;

#endif

#if 0
        //  Streaming data from sensors
        rvr::SensorsStream sen_s(bb, req);
        sen_s.disableStreaming();
        sen_s.clearStreaming();
        std::this_thread::sleep_for(50ms);

        rvr::Drive drive(bb, req);
        drive.resetYaw();
        drive.drive(25, 25);

        rvr::SensorsDirect sen_d(bb, req);
        sen_d.enableColorDetection();   // must precede color detection to turn on bottom LEDs

        terr << code_loc;
        terr << code_loc;
        terr << code_loc;

        sen_s.streamAmbient();
//    sen_s.colorConfig();
//    sen_s.coreNordicConfig();
//    sen_s.coreBTConfig();

        sen_s.streamImuAccelGyro();
        sen_s.streamSpeedVelocityLocator();
        sen_s.streamQuaternion();

        terr << code_loc;
        terr << code_loc;
        terr << code_loc;

        sen_s.startStreamingBT(50);
        sen_s.startStreamingNordic(50);
        std::this_thread::sleep_for(1500ms);

        //    sen_s.disableStreamingNordic();
        //    sen_s.clearStreamingNordic();
        //
        //    sen_s.disableStreamingBT();
        //    sen_s.clearStreamingBT();

        sen_s.disableStreaming();
        sen_s.clearStreaming();

        terr << code_loc << mys::nl;
        terr << code_loc << "Streaming";
        opt_output("Ambient"s, sen_s.ambient());
        terr << code_loc;

        auto [a_x, a_y, a_z] { sen_s.accelerometer().value_or(rvr::AccelData { }) };
        terr << code_loc << "accelerometer: " << a_x << mys::sp << a_y << mys::sp << a_z;

        auto [g_x, g_y, g_z] { sen_s.gyroscope().value_or(rvr::GyroData { }) };
        terr << code_loc << "gyroscope: " << g_x << mys::sp << g_y << mys::sp << g_z;

        auto [i_x, i_y, i_z] { sen_s.imu().value_or(rvr::ImuData { }) };
        terr << code_loc << "imu: " << i_x << mys::sp << i_y << mys::sp << i_z;

        auto [l_x, l_y] { sen_s.locator().value_or(rvr::LocatorData { }) };
        terr << code_loc << "locator: " << l_x << mys::sp << l_y;

        opt_output("Speed"s, sen_s.speed());

        auto [v_x, v_y] { sen_s.velocity().value_or(rvr::VelocityData { }) };
        terr << code_loc << "Velocity: " << v_x << mys::sp << v_y;

        auto [q_w, q_x, q_y, q_z] { sen_s.quaternion().value_or(rvr::QuatData { }) };
        terr << code_loc << "quaternion: " << q_w << mys::sp << q_x << mys::sp << q_y << mys::sp << q_z;

        terr << code_loc << mys::nl;
        drive.stop(0);

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

        terr << code_loc << mys::nl;
        terr << code_loc << "Power";

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

        terr << code_loc << mys::nl;
        terr << code_loc << "disableBatteryStateChange";
        pow.disableBatteryStateChange();
        std::this_thread::sleep_for(50ms);

        opt_output("Set State Change Enabled: ", pow.isBatteryStateChangeEnabled());

        terr << code_loc << mys::nl;
#if 0
    pow.sleep();

    std::this_thread::sleep_for(5000ms);    // have to wait for notification
    terr << code_loc  << "Did Sleep Notify: " << pow.isDidSleepNotify();
    terr << code_loc << mys::nl;
#endif

#endif
#if 1
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
//            terr << code_loc << "locator: " << l_x << mys::sp << l_y;
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
#if 1
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
        terr << code_loc << mys::nl;
        terr << code_loc << mys::nl;
        terr << code_loc << mys::nl << "Connection, SysInfo, APIShell";
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

        terr << code_loc << mys::nl;

#endif
#if 0
    constexpr float in_to_m { 0.0254 };

    rvr::Drive drive(bb, req);
    rvr::SensorsStream sen_s(bb, req);
    rvr::SensorsDirect sen_d(bb, req);

    sen_d.resetLocatorXY();
    drive.resetYaw();
    sen_s.clearStreaming();
    mys::TraceOff toff { terr };

    sen_s.locatorConfig();
    sen_s.enableStreaming(30);
    std::this_thread::sleep_for(30ms);
    terr << code_loc << mys::nl;

    rvr::LocatorData l { sen_s.locator() };
    tout << code_loc << mys::nl;
    tout << code_loc << "locator: " << l.x / in_to_m << mys::sp << l.y / in_to_m;

    float forty_in = -40 * in_to_m;
    double sp { 25 };
    while (l.y > forty_in) {
//        drive.drive(sp, sp);
        drive.driveWithHeading(sp, 0);
        std::this_thread::sleep_for(30ms);
        l = sen_s.locator();
        tout << code_loc << "locator: " << l.x / in_to_m << mys::sp << l.y / in_to_m;

    }
    drive.stop(0);
    l = sen_s.locator();
    tout << code_loc << "locator: " << l.x / in_to_m << mys::sp << l.y / in_to_m;
    std::this_thread::sleep_for(30ms);

    l = sen_s.locator();
    tout << code_loc << "locator: " << l.x / in_to_m << mys::sp << l.y / in_to_m;

    while (l.y < 0) {
        drive.driveWithHeading( -sp, 0);
        std::this_thread::sleep_for(30ms);
        l = sen_s.locator();
    }
    drive.stop(0);
    l = sen_s.locator();
    tout << code_loc << "locator: " << l.x / in_to_m << mys::sp << l.y / in_to_m;
    std::this_thread::sleep_for(30ms);

    l = sen_s.locator();
    tout << code_loc << "locator: " << l.x / in_to_m << mys::sp << l.y / in_to_m;

#endif
    }
    catch (std::exception& e) {
        terr << code_loc << e.what() << "=================================";
    }
    pow.sleep();
    terr << code_loc << "----------------" << mys::nl;

    std::this_thread::sleep_for(1s);
    end_tasks.set_value();
    resp_future.get();

    bb.m_to_v();

    return 0;
}

