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

#include "trace.h"
#include "Response.h"

#include "IoLed.h"

int main(int argc, char* argv[]) {

    mys::trace() << std::hex << setfill('0') << std::uppercase;
    mys::trace() << "Opening serial " << argv[1] << mys::nl;

    SerialPort serial { argv[1], 115200 };
    rvr::Request req { serial };
    rvr::Response resp { serial };

    rvr::IoLed led(req);

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

    led.getActiveColorPalette();
    resp.read();
    std::cerr << std::endl;

//    std::this_thread::sleep_for(std::chrono::milliseconds(4000));
//    led.idleLeds();
//    resp.read();
//    std::cerr << std::endl;
}
