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
#include <cstdlib>
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
int main(int argc, char* argv[]) {
//    mys::TraceOff terr_off { terr };

    if (argc < 2) {
        tout << "Need serial port defined on command line" << mys::nl;
        return -1;
    }

    SerialPort serial { argv[1], 115200 };
    rvr::SendPacket req { serial };
    rvr::Blackboard bb;

    rvr::Power pow(bb, req);

    pow.powerOff(5);
    std::this_thread::sleep_for(50ms);

    return {std::system("sudo shutdown now")};
}
