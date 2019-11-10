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

#include "Response.h"

#include "Drive.h"

int main() {

    cerr << std::hex << setfill('0') << std::uppercase;

    SerialPort serial { "/dev/ttyUSB1", 115200 };
    rvr::Request req { serial };
    rvr::Response resp { serial };

    rvr::Drive drive(req);

    drive.fixHeading();

    drive.stop(90, true);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    drive.fixHeading();

    drive.stop(180, true);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    drive.stop(270, true);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    drive.stop(0, true);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

#if 0
    drive.drive(25, 25, true);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    resp.read();
    std::cerr << std::endl;

    drive.drive( -25, 25, true);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    resp.read();
    std::cerr << std::endl;

    drive.spin_drive(0, 20);
    resp.read();
    std::cerr << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    drive.stop(90, true);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    resp.read();
    std::cerr << std::endl;

    drive.fixHeading();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    resp.read();
    std::cerr << std::endl;

    drive.drive(0, 0, true);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    resp.read();
    std::cerr << std::endl;

    drive.stop(270, true);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    resp.read();
    std::cerr << std::endl;

#endif

    //    drive.getMotorFault();
//    resp.read();
//    std::cerr << std::endl;
//
//    drive.getMotorStall();
//    resp.read();
//    std::cerr << std::endl;

//    drive.fixHeading();
//    resp.read();
//    std::cerr << std::endl;
}
