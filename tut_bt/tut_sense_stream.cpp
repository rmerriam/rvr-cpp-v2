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
#include "SensorsStream.h"

using stream = rvr::SensorsStream;
rvr::CommandBase::CommandResponse RespYes = rvr::CommandBase::resp_yes;

//---------------------------------------------------------------------------------------------------------------------
extern rvr::Request* request;

struct stream_test_data {
    rvr::SensorsStream stream { *request };
};
//=====================================================================================================================
namespace tut {

    using t_group = test_group<stream_test_data>;

    t_group stream("Sensors Stream Test");

    using stream_tests = t_group::object;
    //---------------------------------------------------------------------------------------------------------------------
    template <>
    template <>
    void stream_tests::test<1>() {
        set_test_name("getRgbcSensorValue");
        mys::TraceOn t_on(terr);
        stream.clearStreaming(RespYes);
        std::this_thread::sleep_for(50ms);

        rvr::RvrMsg ambient { 2, 0x00, 0x0A, 0x02 };
        stream.configureStreaming(ambient, RespYes);
        std::this_thread::sleep_for(50ms);

        stream.enableStreaming(50, RespYes);
        std::this_thread::sleep_for(100ms);

        stream.disableStreaming(RespYes);

//        ensure_equals("Value bad SensorsStream::getRgbcSensorValue r", r, 0, 2000);
//        ensure_equals("Value bad SensorsStream::getRgbcSensorValue g", g, 0, 2000);
//        ensure_equals("Value bad SensorsStream::getRgbcSensorValue b", b, 0, 2000);
//        ensure_equals("Value bad SensorsStream::getRgbcSensorValue c", c, 0, 6000);
    }
}   // namespace end
