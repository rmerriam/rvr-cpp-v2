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
#include "ApiShell.h"
#include "Connection.h"

//---------------------------------------------------------------------------------------------------------------------
extern rvr::Request* request;

struct api_test_data {
    rvr::ApiShell api { *request };
    rvr::Connection cmd { *request };
};
//=====================================================================================================================
namespace tut {

    using t_group = test_group<api_test_data>;

    t_group api("Api and Connection Test");

    using api_tests = t_group::object;
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void api_tests::test<1>() {
        set_test_name("echo send");

        rvr::RvrMsg text { 0xDE, 0xAD, 0xFE, 0xED };
        api.echo(text);
        std::this_thread::sleep_for(50ms);

        rvr::RvrMsg msg { api.echo() };

        ensure_not("No response to api::echo", msg.empty());
        ensure_equals("Response to api::echo not correct", msg.begin(), msg.end(), text.begin(), text.end());
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void api_tests::test<2>() {
        set_test_name("alt echo send");

        rvr::RvrMsg text { 0xFE, 0xED };
        api.echo(text);
        std::this_thread::sleep_for(50ms);

        rvr::RvrMsg msg { api.echoAlt() };

        ensure_not("No response to api::echo", msg.empty());
        ensure_equals("Response to api::echo not correct", msg.begin(), msg.end(), text.begin(), text.end());
    }
    //=====================================================================================================================
    template <>
    template <>
    void api_tests::test<10>() {
        set_test_name("bluetoothName");

        cmd.bluetoothName();
        std::this_thread::sleep_for(500ms);

        std::string msg { cmd.name() };
        ensure_not("No response to Connection::bluetoothName", msg.empty());
        ensure_equals("Response to Connection::bluetoothName not correct", msg, "RV-7373");
    }

}	// namespace end
