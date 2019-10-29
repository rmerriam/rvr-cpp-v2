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
//     Created: Oct 21, 2019
//
//======================================================================================================================
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

#include "trace.h"
#include "Packet.h"
namespace rvr {

    uint8_t Packet::mSeq { 0x80 };
    //----------------------------------------------------------------------------------------------------------------------
    auto Packet::escape_char(MsgArray::iterator& p, MsgArray& payload) {

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
    uint8_t Packet::checksum(MsgArray const& payload) const {
        unsigned int sum { };
        sum = ~std::accumulate(payload.begin(), payload.end(), 0);
        return sum;
    }
    //----------------------------------------------------------------------------------------------------------------------
    bool Packet::isPacketChar(uint8_t const c) {
        return (c == SOP) || (c == EOP) || (c == ESC);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Packet::escape_msg(MsgArray& payload) {
        for (auto p { find_if(payload.begin(), payload.end(), isPacketChar) }; p != payload.end();
            p = find_if(p, payload.end(), isPacketChar)) {
            p = escape_char(p, payload);
        }
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Packet::unescape_char(auto& p, MsgArray& payload) {
        auto n { p + 1 };
        switch ( *n) {
            case escaped_SOP: {
                *n = SOP;
                break;
            }
            case escaped_EOP: {
                *n = EOP;
                break;
            }
            case escaped_ESC: {
                *n = ESC;
                break;
            }
        }
        payload.erase(p);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Packet::unescape_msg(MsgArray& payload) {

        for (auto p { find(payload.begin(), payload.end(), ESC) }; p != payload.end(); p = find(p + 1, payload.end(), ESC)) {
            unescape_char(p, payload);
        }
    }
}
