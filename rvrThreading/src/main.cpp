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

#include "Trace.h"

#include "IoLed.h"
#include "Response.h"
//---------------------------------------------------------------------------------------------------------------------
mys::TraceStart terr { std::cerr };
//---------------------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[]) {

    terr << "Opening serial " << argv[1];

    SerialPort serial { argv[1], 115200 };
    rvr::Request req { serial };

    //---------------------------------------------------------------------------------------------------------------------
    //  Setup the LED handling
    rvr::IoLed led(req);

    uint32_t led32 { Led::headlight_left | Led::headlight_right };

    rvr::MsgArray colors[2] { { 0x00, 0x00, 0xFF, //
                                0xFF, 0x00, 0x00, }, //
    { 0xFF, 0x00, 0x00, //
      0x00, 0x00, 0xFF, }, //
    };

    //  Setup the thread to read responses
    std::promise<void> end_tasks;
    std::shared_future<void> end_future(end_tasks.get_future());
    rvr::Response resp { serial, end_future };

    auto resp_future = std::async(std::launch::async, std::ref(resp));

    for (auto i { 0 }; i < 10; ++i) {
        led.allLed(led32, colors[i % 2], true);
        terr << "blink";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    led.idleLeds();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    end_tasks.set_value();
    terr << std::boolalpha << resp_future.get();

    return 0;
}

