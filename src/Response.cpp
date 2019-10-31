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
//     Created: Oct 25, 2019
//
//======================================================================================================================
#include "Response.h"
namespace rvr {
    //----------------------------------------------------------------------------------------------------------------------
    int Response::read() {
        uint8_t resp[120];
        int cnt = mSerialPort.read(resp, 120);
        mMsg.assign(resp, &resp[cnt]);

        traceln(std::cerr, "resp: ", cnt);

        unescape_msg(mMsg);
        trace(std::cerr, mMsg);
        traceln(std::cerr);

        return cnt;
    }

}
