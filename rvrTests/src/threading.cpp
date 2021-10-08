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
#include <thread>
using namespace std::literals;

#include <Trace.h>
#include <rvr++.h>
#include <TcpClient.h>
//---------------------------------------------------------------------------------------------------------------------
void direct(rvr::SensorsDirect& sen_d);
void notifications(rvr::SensorsDirect& sen_d);
void leds_test(rvr::IoLed& led);
void power(rvr::Power& pow);
void streaming(rvr::SensorsStream& sen_s);
void sysinfo(rvr::SystemInfo& sys, rvr::Connection& cmd, rvr::ApiShell& api);
//---------------------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    mys::TraceOn tdbg_on { mys::terr };
    mys::terr << code_line << " Opening serial " << argv[1];

    mys::tout << code_line << " Opening serial " << argv[1];

    rvr::SerialPort serial { argv[1], 115200 };
    rvr::SendPacket req { serial };
    rvr::ReadPacket in_packet { serial };
    rvr::Blackboard bb;
    //---------------------------------------------------------------------------------------------------------------------
    //  Setup the response thread to read responses
    std::promise<void> end_tasks;
    std::shared_future<void> end_future(end_tasks.get_future());
    rvr::Response resp { in_packet, bb, end_future };
    mys::tout << code_line << "----------------";

    auto resp_future = std::async(std::launch::async, std::ref(resp));
    //---------------------------------------------------------------------------------------------------------------------
    rvr::ApiShell api(bb, req);
    rvr::Connection cmd(bb, req);
    rvr::Drive drive(bb, req);
    rvr::IoLed led(bb, req);
    rvr::Power pow(bb, req);
    rvr::SensorsDirect sen_d(bb, req);
    rvr::SensorsStream sen_s(bb, req);
    rvr::SystemInfo sys(bb, req);

    pow.awake(rvr::CommandResponse::resp_yes);

    mys::tout << code_line << "is awake:  " << pow.isWakeNotify().get_or();
    std::this_thread::sleep_for(500ms);
    mys::tout << code_line << "is awake:  " << pow.isWakeNotify().get_or();
    //=====================================================================================================================
    try {

//        direct(sen_d);
//        leds_test(led);
//        notifications(sen_d);
//        power(pow);

        drive.tank_normalized(15, 15);

        streaming(sen_s);
        std::this_thread::sleep_for(1s);
        drive.tank_normalized(0, 0);
//        sysinfo(sys, cmd, api);

#if 0
        // DRIVE
        rvr::Drive drive(bb, req);
        rvr::SensorsStream sen_s(bb, req);

        drive.resetYaw();
//    std::this_thread::sleep_for(std::chrono::milliseconds(50));
//
//    drive.stop(90, );
//    std::this_thread::sleep_for(std::chrono::milliseconds(100000));

//        drive.driveWithHeading(0, 90);
//        std::this_thread::sleep_for(1s);
//
//        for (auto i { 0 }; i < 10; ++i) {
//            drive.drive(75, 25);
//            std::this_thread::sleep_for(3s);
//            auto [l_x, l_y] { sen_s.locator().value_or(rvr::LocatorData { }) };
//            mys:;tinfo<< code_line << "locator: " << l_x << mys::sp << l_y;
//        }

        drive.tank_normalized(15, 15);
        std::this_thread::sleep_for(1s);

//    drive.driveWithHeading(0, 20, );
//    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//
//    drive.resetYaw();
//    std::this_thread::sleep_for(std::chrono::milliseconds(50));
//
//    drive.enableMotorStallNotify();
//    drive.enableMotorFaultNotify();
//
//    drive.disableMotorStallNotify();
//    drive.disableMotorFaultNotify();
//
//    drive.getMotorFault();
//
//    std::this_thread::sleep_for(100ms);
//
//    mys:;tinfo<< code_line;
//    mys:;tinfo<< code_line;
//    mys:;tinfo<< code_line << "drive";
//
//    mys:;tinfo<< code_line << "Fault Notify State: " << drive.motorFaultState();
//    mys:;tinfo<< code_line << "Fault Notify Set: " << drive.motorFaultNotifySet();
//
//    mys:;tinfo<< code_line << "Stall Notify State: " << drive.motorFaultState();
//    mys:;tinfo<< code_line << "Stall Notify Set: " << drive.motorStallNotifySet();
//
//    mys:;tinfo<< code_line;
//    mys:;tinfo<< code_line;

#endif

#if 0
    constexpr float in_to_m { 0.0254 };

    rvr::Drive drive(bb, req);
    rvr::SensorsStream sen_s(bb, req);
    rvr::SensorsDirect sen_d(bb, req);

    sen_d.resetLocatorXY();
    drive.resetYaw();
    sen_s.clearAllStreaming();
    mys::TraceOff toff { mys:;tinfo};

    sen_s.locatorConfig();
    sen_s.enableStreaming(30);
    std::this_thread::sleep_for(30ms);
    mys:;tinfo<< code_line << mys::nl;

    rvr::LocatorData l { sen_s.locator() };
    mys:;tinfo << code_line << mys::nl;
    mys:;tinfo << code_line << "locator: " << l.x / in_to_m << mys::sp << l.y / in_to_m;

    float forty_in = -40 * in_to_m;
    double sp { 25 };
    while (l.y > forty_in) {
//        drive.drive(sp, sp);
        drive.driveWithHeading(sp, 0);
        std::this_thread::sleep_for(30ms);
        l = sen_s.locator();
        mys:;tinfo << code_line << "locator: " << l.x / in_to_m << mys::sp << l.y / in_to_m;

    }
    drive.stop(0);
    l = sen_s.locator();
    mys:;tinfo << code_line << "locator: " << l.x / in_to_m << mys::sp << l.y / in_to_m;
    std::this_thread::sleep_for(30ms);

    l = sen_s.locator();
    mys:;tinfo << code_line << "locator: " << l.x / in_to_m << mys::sp << l.y / in_to_m;

    while (l.y < 0) {
        drive.driveWithHeading( -sp, 0);
        std::this_thread::sleep_for(30ms);
        l = sen_s.locator();
    }
    drive.stop(0);
    l = sen_s.locator();
    mys:;tinfo << code_line << "locator: " << l.x / in_to_m << mys::sp << l.y / in_to_m;
    std::this_thread::sleep_for(30ms);

    l = sen_s.locator();
    mys:;tinfo << code_line << "locator: " << l.x / in_to_m << mys::sp << l.y / in_to_m;

#endif
    }
    catch (std::exception& e) {
        mys::terr << code_line << e.what() << "=================================";
    }

    pow.sleep(rvr::CommandResponse::resp_yes);

    std::this_thread::sleep_for(1s);
    end_tasks.set_value();
    resp_future.get();

    bb.m_to_v();

    return 0;
}
