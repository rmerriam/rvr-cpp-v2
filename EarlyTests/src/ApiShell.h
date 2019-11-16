#ifndef ApiShell_H_
#define ApiShell_H_
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
//     Created: Oct 29, 2019
//
//======================================================================================================================
#include "Request.h"
#include "CommandBase.h"

namespace rvr {

    class ApiShell : protected CommandBase {
    public:
        ApiShell(Request& req) :
            CommandBase { Devices::api_and_shell, req, microcontroller } {
        }
        ApiShell(const ApiShell& other) = delete;
        ApiShell(ApiShell&& other) = delete;
        ApiShell& operator=(const ApiShell& other) = delete;

        void echo(const MsgArray& data, const CmdResp want_resp = false);

    private:
        enum Cmd : uint8_t {
            echo_cmd = 0x00, //
        };
    };
    //----------------------------------------------------------------------------------------------------------------------
    inline void ApiShell::echo(const MsgArray& data, const CmdResp want_resp) {
        MsgArray msg { buildFlags(want_resp), mTarget, mDevice, echo_cmd, mRequest.sequence() };
        msg.insert(msg.end(), data.begin(), data.end());
        mRequest.send(msg);
    }

} /* namespace rvr */

#endif /* ApiShell_H_ */
