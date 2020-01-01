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

    std::optional<std::string> Connection::name() {
        return mBlackboard.stringValue(mTarget, mDevice, get_bluetooth_advertising_name);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void ApiShell::echo(RvrMsg const& data) const {
        cmdData(echo_cmd, data, resp_yes);
        cmdDataAlt(echo_cmd, data, resp_yes);
    }
    //----------------------------------------------------------------------------------------------------------------------
    RvrMsgRet_t ApiShell::echo() const {
        return mBlackboard.msgValue(mTarget, mDevice, echo_cmd);
    }
    //----------------------------------------------------------------------------------------------------------------------
    RvrMsgRet_t ApiShell::echoAlt() const {
        return mBlackboard.msgValue(mAltTarget, mDevice, echo_cmd);
    }

}
