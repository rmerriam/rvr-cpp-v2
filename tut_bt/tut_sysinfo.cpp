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
#include <chrono>
#include <string>
#include <thread>
using namespace std::literals;

#include <tut/tut.hpp>

#include "Trace.h"

#include "Blackboard.h"
#include "SystemInfo.h"

//=====================================================================================================================
//  Some of these values change as Sphero updates firmware so update them as necessary
//
std::string app { "7.1.435" };
std::string app2 { "7.1.422" };
std::string boot { "5.0.81" };
std::string boot2 { "5.0.155" };
std::string mac { "cf:d5:4a:57:73:73" };
std::string sku { "ROW" };
int64_t board { 5 };
int16_t stats { 23 };

// probably won't change but here for consistency
std::string proc { "ST" };
std::string proc2 { "Nordic" };
//---------------------------------------------------------------------------------------------------------------------
extern rvr::SendPacket* packet_send;
extern rvr::Blackboard* blackboard;

struct sys_test_data {
    rvr::Blackboard* bb;
    rvr::SystemInfo sys { *bb, *packet_send };
};
//=====================================================================================================================
namespace tut {

    using t_group = test_group<sys_test_data>;

    t_group sys("System Info Test");

    using sys_tests = t_group::object;
    //---------------------------------------------------------------------------------------------------------------------
#if 0
    sys.getMainAppVersion(RespYes);    // alt
    sys.getBootloaderVersion(RespYes);    //
    sys.getBoardRevision(RespYes);  // ??
    sys.getMacId(RespYes);  // ??
    sys.getStatsId(RespYes);
    sys.getProcessorName(RespYes);  // alt
    sys.getSku(RespYes);    // ??
    sys.getCoreUpTime(RespYes); //

    int64_t boardVersion();
    std::string bootVersion();
    std::string bootVersion2();
    std::string macAddress();
    std::string mainAppVersion();
    std::string mainAppVersion2();
    std::string processorName();
    std::string processorName2();
    int16_t statsId();
    std::string sku();
    int64_t coreUpTime();
#endif

    //=====================================================================================================================
    template <>
    template <>
    void sys_tests::test<1>() {
        set_test_name("getMainAppVersion");
//        mys::TraceOn t_on(terr);

        sys.getMainAppVersion();

        std::this_thread::sleep_for(50ms);

        std::string msg = sys.mainAppVersion();
        ensure_not("No response to sys.getMainAppVersion", msg.empty());
        ensure_equals("Response to sys.getMainAppVersion not correct", msg, app);
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void sys_tests::test<2>() {
        set_test_name("getMainAppVersion alt");

        sys.getMainAppVersion();

        std::this_thread::sleep_for(50ms);

        std::string msg { sys.mainAppVersion2() };
        ensure_not("No response to sys.getMainAppVersion", msg.empty());
        ensure_equals("Response to sys.getMainAppVersion not correct", msg, app2);
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void sys_tests::test<3>() {
        set_test_name("getProcessorName");

        sys.getProcessorName();

        std::this_thread::sleep_for(50ms);

        std::string msg { sys.processorName() };
        ensure_not("No response to sys.getProcessorName", msg.empty());
        ensure_equals("Response to sys.getProcessorName not correct", msg, proc);
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void sys_tests::test<4>() {
        set_test_name("getProcessorName alt");
        sys.getProcessorName(); // not really needed because got it in previous test

        std::this_thread::sleep_for(50ms);

        std::string msg { sys.processorName2() };
        ensure_not("No response to sys.getProcessorName2", msg.empty());
        ensure_equals("Response to sys.getProcessorName2 not correct", msg, proc2);
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void sys_tests::test<5>() {
        set_test_name("getBootloaderVersion");

        sys.getBootloaderVersion();

        std::this_thread::sleep_for(50ms);

        std::string msg { sys.bootVersion() };
        ensure_not("No response to sys.getBootloaderVersion", msg.empty());
        ensure_equals("Response to sys.getBootloaderVersion not correct", msg, boot);
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void sys_tests::test<6>() {
        set_test_name("getBootloaderVersion alt");

        sys.getBootloaderVersion(); // not really needed because got it in previous test

        std::this_thread::sleep_for(50ms);

        std::string msg { sys.bootVersion2() };
        ensure_not("No response to sys.getBootloaderVersion2", msg.empty());
        ensure_equals("Response to sys.getBootloaderVersion2 not correct", msg, boot2);
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void sys_tests::test<7>() {
        set_test_name("getMacId");
//        mys::TraceOn t_on(terr);

        sys.getMacId();

        std::this_thread::sleep_for(50ms);

        std::string msg { sys.macAddress() };
        ensure_not("No response to sys.getMacId", msg.empty());
        ensure_equals("Response to sys.getMacId not correct", msg, mac);
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void sys_tests::test<8>() {
        set_test_name("getSku");
//        mys::TraceOn t_on(terr);

        sys.getSku();

        std::this_thread::sleep_for(50ms);

        std::string msg { sys.sku() };
        ensure_not("No response to sys.getMacId", msg.empty());
        ensure_equals("Response to sys.getMacId not correct", msg, sku);
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void sys_tests::test<9>() {
        set_test_name("getCoreUpTime");
//        mys::TraceOn t_on(terr);

        sys.getCoreUpTime();

        std::this_thread::sleep_for(50ms);

        ensure("Zero value for sys.getCoreUpTime", sys.coreUpTime() != 0);
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void sys_tests::test<10>() {
        set_test_name("boardVersion");
//        mys::TraceOn t_on(terr);

        sys.getBoardRevision();

        std::this_thread::sleep_for(50ms);

        ensure_equals("Response to sys.getBoardRevision not correct", sys.boardVersion(), board);

    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void sys_tests::test<11>() {
        set_test_name("getStatsId");
//        mys::TraceOn t_on(terr);

        sys.getStatsId();

        std::this_thread::sleep_for(50ms);

        ensure_equals("Response to sys.getStatsId not correct", sys.statsId(), stats);

    }

} // namespace end
