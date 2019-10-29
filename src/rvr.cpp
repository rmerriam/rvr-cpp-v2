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

#include "Drive.h"
#include "IoLed.h"
#include "Power.h"

int main() {

    cerr << std::hex << setfill('0') << std::uppercase;

    SerialPort serial { "/dev/ttyUSB0", 115200 };
    rvr::Request req { serial };
    rvr::Response resp { serial };
    rvr::Drive drive(req);
    rvr::Power pow(req);
    rvr::IoLed led(req);

#if 1
    led.allLed(true);
    resp.read();
    std::cerr << std::endl;

//    led.getColorId();
//    resp.read();
//    std::cerr << std::endl;
//
//    led.getColorPalette();
//    resp.read();
//    std::cerr << std::endl;

#elif 0
//    pow.awake();
//    resp.read();
//    std::cerr << std::endl;
//    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

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

    drive.drive(20, 40);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    resp.read();
    std::cerr << std::endl;

    drive.getMotorFault();
    resp.read();
    std::cerr << std::endl;

    drive.getMotorStall();
    resp.read();
    std::cerr << std::endl;

    drive.spin_drive(0, 20);
    resp.read();
    std::cerr << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    drive.fixHeading();
    resp.read();
    std::cerr << std::endl;

#endif
}
