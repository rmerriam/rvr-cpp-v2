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

#include "ApiShell.h"
#include "Connection.h"
#include "SystemInfo.h"

int main() {

    cerr << std::hex << setfill('0') << std::uppercase;

    SerialPort serial { "/dev/ttyUSB0", 115200 };
    rvr::Request req { serial };
    rvr::Response resp { serial };

    rvr::ApiShell api(req);
    rvr::Connection cmd(req);
    rvr::SystemInfo sys(req);

    // API commands
    rvr::MsgArray dead { 0xDE, 0xAD };
    api.echo(dead, true);
    resp.read();
    std::cerr << std::endl;

    // Connection commands
    cmd.bluetoothName();
    resp.read();
    std::cerr << std::endl;

    // SysInfo Commands
    sys.getMainAppVersion();
    resp.read();
    std::cerr << std::endl;

    sys.getBootloaderVersion();
    resp.read();
    std::cerr << std::endl;

    sys.getBoardRevision();
    resp.read();
    std::cerr << std::endl;

    sys.getMacId();
    resp.read();
    std::cerr << std::endl;

    sys.getStatsId();
    resp.read();
    std::cerr << std::endl;

    sys.getProcessorName();
    resp.read();
    std::cerr << std::endl;

    sys.getUpTime();
    resp.read();
    std::cerr << std::endl;

    sys.getSku();
    resp.read();
    std::cerr << std::endl;
}
