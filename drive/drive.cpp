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

int main() {

    cerr << std::hex << setfill('0') << std::uppercase;

    SerialPort serial { "/dev/ttyUSB0", 115200 };
    rvr::Request req { serial };
    rvr::Response resp { serial };

    rvr::Drive drive(req);

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

    drive.spin_drive(0, 20);
    resp.read();
    std::cerr << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    drive.fixHeading();
    resp.read();
    std::cerr << std::endl;
}
