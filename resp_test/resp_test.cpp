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

    rvr::MsgArray dead { 0xDE, 0xAD };
    api.echo(dead, true);

    cmd.bluetoothName();

#if 1
    sys.getMainAppVersion();

    sys.getBootloaderVersion();

    sys.getBoardRevision();

    sys.getMacId();

    sys.getStatsId();

    sys.getProcessorName();

    sys.getUpTime();

    sys.getSku();
#endif
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    std::cerr << "===================\n";
    rvr::MsgArray in;
    in.reserve(40);

    uint8_t const EopSop[] { 0xD8, 0x8D };

    for (auto i { 0 }; i < 25; ++i) {
        std::cerr << dec << i << " size " << in.size() << '\n';
        std::cerr << "buf " << serial.count() << '\n';
        std::cerr << hex;

        if (serial.count() != 0) {
            uint8_t r[in.capacity()];
            int cnt = serial.read(r, in.capacity());

            in.insert(in.end(), r, &r[cnt]);

            trace(std::cerr, in);
            tracenl(std::cerr);
        }

        auto pos = std::search(in.begin(), in.end(), EopSop, &EopSop[1]);
        trace(std::cerr, pos - in.begin());
        tracenl(std::cerr);

        rvr::MsgArray packet { in.begin(), pos + 1 };
        trace(std::cerr, packet);
        tracenl(std::cerr);

        in.erase(in.begin(), pos + 1);
        trace(std::cerr, in);
        tracenl(std::cerr);
    }
}
