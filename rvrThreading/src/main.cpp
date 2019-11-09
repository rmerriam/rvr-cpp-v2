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
#include <thread>
#include <version>
using namespace std;

#include "Trace.h"

#include "IoLed.h"
//---------------------------------------------------------------------------------------------------------------------
mys::TraceStart terr { std::cerr };
//---------------------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[]) {

    terr << "Opening serial " << argv[1];

    SerialPort serial { argv[1], 115200 };
    rvr::Request req { serial };

    rvr::IoLed led(req);

    uint32_t led32 { Led::headlight_left | Led::headlight_right };

    rvr::MsgArray colors[2] { { 0x00, 0x00, 0xFF, //
                                0xFF, 0x00, 0x00, }, //
    { 0xFF, 0x00, 0x00, //
      0x00, 0x00, 0xFF, }, //
    };

    for (auto i { 0 }; i < 10; ++i) {
        led.allLed(led32, colors[i % 2], true);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        terr << "blink";
    }

    terr << __TIME__ << mys::tab << __func__ << mys::tab << code_loc;

    return 0;
}

