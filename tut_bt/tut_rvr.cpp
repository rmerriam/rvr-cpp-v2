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
//     Created: Oct 26, 2019
//
//======================================================================================================================
#include <tut/tut.hpp>
#include <tut/tut_reporter.hpp>

#include <future>
#include <iomanip>
#include <string>

#include "Trace.h"

#include "Blackboard.h"
#include "SerialPort.h"
#include "Response.h"
#include "ReadPacket.h"
#include "SendPacket.h"

//---------------------------------------------------------------------------------------------------------------------
namespace tut {
    test_runner_singleton runner;
}
//---------------------------------------------------------------------------------------------------------------------
rvr::Blackboard* blackboard;
rvr::SendPacket* packet_send;
mys::TraceStart terr { std::cerr };
//---------------------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    if (argc < 2) {
        terr << "Need to define serial port name";
        return -5;
    }

    std::string serial_port_name = argv[1];
    terr << code_loc << " Starting Tests using Serial Port" << serial_port_name << std::setprecision(4) << std::fixed << std::boolalpha;
    mys::TraceOff t_off(terr);
    std::cout << std::setprecision(5) << std::fixed;
    std::cerr << std::setprecision(5) << std::fixed;
    std::clog << std::setprecision(5) << std::fixed;

    SerialPort serial { serial_port_name.c_str(), 115200 };
    rvr::SendPacket send_packet { serial };
    rvr::ReadPacket read_packet { serial };
    packet_send = &send_packet;

    rvr::Blackboard bb;
    ::blackboard = &bb;
    //---------------------------------------------------------------------------------------------------------------------
    //  Setup the thread to read responses
    std::promise<void> end_tasks;
    std::shared_future<void> end_future(end_tasks.get_future());
    rvr::Response resp { read_packet, bb, end_future };

    auto resp_future = std::async(std::launch::async, std::ref(resp));
    //---------------------------------------------------------------------------------------------------------------------

    tut::reporter reporter;
    tut::runner.get().set_callback( &reporter);

    tut::test_result tr;
    tut::runner.get().run_tests("Api and Connection Test");
    tut::runner.get().run_tests("Sensors Direct Test");
    tut::runner.get().run_tests("Sensors Stream Test");
    tut::runner.get().run_tests("System Info Test");

    // last because it the sleep / awake has time delays
    tut::runner.get().run_tests("Power Test");

    //---------------------------------------------------------------------------------------------------------------------
    // close response thread
    end_tasks.set_value();
    resp_future.get();

    mys::TraceOn t_on(terr);
    bb.m_to_v();

    return !reporter.all_ok();
}

