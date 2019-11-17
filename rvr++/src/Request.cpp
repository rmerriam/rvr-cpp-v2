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
#include <algorithm>
#include <numeric>

#include "Trace.h"
#include "Request.h"

namespace rvr {
    //----------------------------------------------------------------------------------------------------------------------
    void Request::send(const MsgArray& p) {
        MsgArray payload { p };
        MsgArray mMsg;
        mMsg.clear();
        mMsg.push_back(SOP);

        // calculate checksum, add to end of payload, then escape payload & checksum
        uint8_t sum { checksum(payload) };
        payload.push_back(sum);

        escape_msg(payload);

        std::copy(payload.begin(), payload.end(), std::back_insert_iterator(mMsg));

        mMsg.push_back(EOP);

        mSerialPort.write(mMsg.data(), mMsg.size());

        terr << __func__ << mys::sp << std::hex << std::uppercase << mMsg;
    }
    //----------------------------------------------------------------------------------------------------------------------
    auto Request::escape_char(MsgArray::iterator& p, MsgArray& payload) {

        switch ( *p) {
            case SOP: {
                *p = ESC;
                payload.insert(p + 1, escaped_SOP);
                break;
            }
            case EOP: {
                *p = ESC;
                payload.insert(p + 1, escaped_EOP);
                break;
            }
            case ESC: {
                *p = ESC;
                payload.insert(p + 1, escaped_ESC);
                break;
            }
        }
        ++p;
        return p;
    }
    //----------------------------------------------------------------------------------------------------------------------
    uint8_t Request::checksum(const MsgArray& payload) const {
        unsigned int sum { };
        sum = ~std::accumulate(payload.begin(), payload.end(), 0);
        return sum;
    }
    //----------------------------------------------------------------------------------------------------------------------
    bool Request::isPacketChar(const uint8_t c) {
        return (c == SOP) || (c == EOP) || (c == ESC);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Request::escape_msg(MsgArray& payload) {
        for (auto p { find_if(payload.begin(), payload.end(), isPacketChar) }; p != payload.end();
            p = find_if(p, payload.end(), isPacketChar)) {
            p = escape_char(p, payload);
        }
    }

}