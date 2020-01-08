/*
 * main.cpp
 *
 *  Created on: Apr 6, 2016
 *      Author: rmerriam
 */

#include <future>
#include <iomanip>

#include <unistd.h>
#include <panel.h>
#include <form.h>

#include <Trace.h>
#include <SerialPort.h>
#include <Blackboard.h>
#include <Response.h>
#include <SendPacket.h>

#include "MainForm.h"

float const version { 0.1 };
std::string serial_port;
//--------------------------------------------------------------------------------------------------------------------------
mys::TraceStart terr { std::cerr };
//--------------------------------------------------------------------------------------------------------------------------
int main(int argc, char** argv) {
    mys::TraceOff off(terr);
    try {
        serial_port = argv[1];
        SerialPort serial { argv[1], 115200 };
        rvr::ReadPacket in_packet { serial };
        rvr::Blackboard bb;

        //---------------------------------------------------------------------------------------------------------------------
        //  Setup the window updating
        rvr::SendPacket req { serial };

        //---------------------------------------------------------------------------------------------------------------------
        //  Setup the thread to read responses
        std::promise<void> end_tasks;
        std::shared_future<void> end_future(end_tasks.get_future());
        rvr::Response resp { in_packet, bb, end_future };
        auto resp_future = std::async(std::launch::async, std::ref(resp));

        NCurses nc;

        MainForm mf(bb, req);
        mf.runWindows();

        end_tasks.set_value();
//        resp_future.get();

        endwin();

    }
    catch (exception& e) {
        cerr << "\n\n  exception " << e.what() << '\n';
    }
    return 0;
}

