//============================================================================
// Name        : rvr_serial.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <chrono>
#include <iomanip>
#include <iostream>
#include <thread>
using namespace std;

#include "Response.h"

#include "ApiShell.h"
#include "Connection.h"
#include "Drive.h"
#include "IoLed.h"
#include "Power.h"
#include "SystemInfo.h"

int main() {

    cerr << std::hex << setfill('0') << std::uppercase;

    SerialPort serial { "/dev/ttyUSB0", 115200 };
    rvr::Request req { serial };
    rvr::Response resp { serial };

    rvr::ApiShell api(req);
    rvr::Connection cmd(req);
    rvr::Drive drive(req);
    rvr::Power pow(req);
    rvr::IoLed led(req);
    rvr::SystemInfo sys(req);

#if 0
    rvr::MsgArray dead { 0xDE, 0xAD };
    api.echo(dead, true);
    resp.read();
    std::cerr << std::endl;

    cmd.bluetoothName();
    resp.read();
    std::cerr << std::endl;

#elif 0

    sys.getMainAppVersion();
    resp.read();
    std::cerr << std::endl;

    sys.getBootloaderVersion();
    resp.read();
    std::cerr << std::endl;

    sys.getBoardRevision();
    resp.read();
    std::cerr << std::endl;

    sys.getMacId();
    resp.read();
    std::cerr << std::endl;

    sys.getStatsId();
    resp.read();
    std::cerr << std::endl;

    sys.getProcessorName();
    resp.read();
    std::cerr << std::endl;

    sys.getUpTime();
    resp.read();
    std::cerr << std::endl;

    sys.getSku();
    resp.read();
    std::cerr << std::endl;

#elif 0

    uint32_t led32 { Led::headlight_left | Led::headlight_right };

    rvr::MsgArray colors { 0x00, 0x00, 0xFF, //
        0xFF, 0x00, 0x00, //
    };
    led.allLed(led32, colors, true);
    resp.read();
    std::cerr << std::endl;

    led32 = Led::undercarriage_white;
    colors.clear();
    colors.push_back(0x00);
//    colors.push_back(0xFF);
//    colors.push_back(0xFF);

    led.allLed(led32, colors, true);
    resp.read();
    std::cerr << std::endl;

//    led.getColorId();
//    resp.read();
//    std::cerr << std::endl;

//    led.getActiveColorPalette();
//    resp.read();
//    std::cerr << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(4000));
    led.idleLeds();
    resp.read();
    std::cerr << std::endl;

#elif 1
    pow.awake();
    resp.read();
    std::cerr << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

//    resp.read();
//    std::cerr << std::endl;

    pow.battery_precentage();
    resp.read();
    std::cerr << std::endl;

    pow.battery_voltage_state();
    resp.read();
    std::cerr << std::endl;

    pow.battery_voltage();
    resp.read();
    std::cerr << std::endl;

    pow.battery_volt_thresholds();
    resp.read();
    std::cerr << std::endl;

    pow.battery_current();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    resp.read();
    std::cerr << std::endl;

//    pow.sleep();
//    resp.read();
//    std::cerr << std::endl;

#elif 1

//    pow.sleep();
//    resp.read();
//    std::cerr << std::endl;
//    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    drive.drive(20, 40, true);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    resp.read();
    std::cerr << std::endl;

    drive.getMotorFault();
    resp.read();
    std::cerr << std::endl;

    drive.getMotorStall();
    resp.read();
    std::cerr << std::endl;

    drive.spin_drive(0, 20, true);
    resp.read();
    std::cerr << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    drive.fixHeading();
    resp.read();
    std::cerr << std::endl;

#endif
}
