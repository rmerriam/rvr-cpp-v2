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

#include "Blackboard.h"

#include "ApiShell.h"
#include "Connection.h"

using namespace std::literals;

namespace rvr {

    std::string Connection::name() {
        return bb::stringValue(mTarget, Devices::connection, get_bluetooth_advertising_name);
    }
    //----------------------------------------------------------------------------------------------------------------------
    RvrMsg ApiShell::echo() {
        return bb::msgValue(mTarget, mDevice, echo_cmd);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void ApiShell::echo(RvrMsg const& data, CommandResponse const want_resp) {
//        cmd_data(echo_cmd, data, want_resp);
//        cmd_data_alt(echo_cmd, data, want_resp);
    }

}
