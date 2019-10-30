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

}
