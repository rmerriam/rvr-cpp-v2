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
#include "Response.h"
#include "ResponseDecoder.h"

#include "ApiShell.h"
#include "Connection.h"
#include "Drive.h"
#include "IoLed.h"
#include "Power.h"
#include "Sensors.h"
#include "SystemInfo.h"
//---------------------------------------------------------------------------------------------------------------------
mys::TraceStart terr { std::cerr };
//---------------------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[]) {

    terr << "Opening serial " << argv[1] << std::setprecision(4) << std::fixed;

    SerialPort serial { argv[1], 115200 };
    rvr::Request req { serial };

    //---------------------------------------------------------------------------------------------------------------------
    //  Setup the thread to read responses
    std::promise<void> end_tasks;
    std::shared_future<void> end_future(end_tasks.get_future());
    rvr::Response resp { serial, end_future };

    auto resp_future = std::async(std::launch::async, std::ref(resp));

    rvr::ApiShell api(req);
    rvr::Connection cmd(req);
    rvr::Drive drive(req);
    rvr::IoLed led(req);
    rvr::Power pow(req);
    rvr::Sensors sen(req);
    rvr::SystemInfo sys(req);

#if 0
    /// change size back to 15 in sensors!!!!
    rvr::MsgArray accel { 0x01, 0x00, 0x02, 0x01, 0x00, 0x01, 0x01 };
    sen.configureStreaming(1, accel, 2);

//    rvr::MsgArray ambient { 0x02, 0x00, 0x0A, 0x02 };
//    sen.configureStreaming(2, ambient, 1);
//
//    rvr::MsgArray speed { 0x02, 0x00, 0x08, 0x01 };
//    sen.configureStreaming(2, speed, 2);
//
//    rvr::MsgArray velocity { 0x02, 0x00, 0x07, 0x02 };
//    sen.configureStreaming(2, velocity, 2);
//
//    rvr::MsgArray locator { 0x02, 0x00, 0x06, 0x02 };
//    sen.configureStreaming(2, locator, 0);

    std::this_thread::sleep_for(500ms);

    sen.enableStreaming(500);

    std::this_thread::sleep_for(2000ms);
    sen.disableStreaming();
    sen.clearStreaming();

#elif 0
    //---------------------------------------------------------------------------------------------------------------------
    //  Setup the LED handling

    uint32_t led32 { Led::headlight_left | Led::headlight_right };

    rvr::MsgArray colors[2] { { 0x00, 0x00, 0xFF, //
                                0xFF, 0x00, 0x00, }, //
    { 0xFF, 0x00, 0x00, //
      0x00, 0x00, 0xFF, }, //
    };

    for (auto i { 0 }; i < 10; ++i) {
        led.allLed(led32, colors[i % 2], true);
        terr << "blink";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    led.idleLeds();

#elif 1

    pow.awake();
//    pow.battery_voltage_state();
//
//    pow.battery_voltage(rvr::Power::VoltageType::CalibratedFiltered);
//    pow.battery_voltage(rvr::Power::VoltageType::CalibratedUnfiltered);
//    pow.battery_voltage(rvr::Power::VoltageType::UncalibratedUnfiltered);
//    pow.battery_volt_thresholds();
//    pow.battery_current(rvr::Power::MotorSide::left);
//    pow.battery_current(rvr::Power::MotorSide::right);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    pow.battery_precentage();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    pow.sleep();

#elif 0
    drive.fixHeading(true);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    drive.stop(90, true);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    drive.drive(25, 25, true);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    drive.spin_drive(0, 20, true);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    drive.fixHeading(true);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    drive.enableMotorStallNotify();
    drive.enableMotorFaultNotify();

    drive.disableMotorStallNotify();
    drive.disableMotorFaultNotify();

    drive.getMotorFault();

#elif 0
    rvr::MsgArray dead { 0xDE, 0xAD };
    api.echo(dead, true);

    cmd.bluetoothName();

    sys.getMainAppVersion();
    sys.getBootloaderVersion();
    sys.getBoardRevision();
    sys.getMacId();
    sys.getStatsId();
    sys.getUpTime();
    sys.getProcessorName();
    sys.getSku();
    sys.getMainAppVersion();
#endif

    std::this_thread::sleep_for(2s);

    end_tasks.set_value();
    terr << std::boolalpha << resp_future.get();

    return 0;
}

