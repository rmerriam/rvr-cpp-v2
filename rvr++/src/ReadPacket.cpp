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
#include "ReadPacket.h"

namespace rvr {

    //----------------------------------------------------------------------------------------------------------------------
    void ReadPacket::read(rvr::RvrMsg& in, rvr::RvrMsg& out) {
        checkForData(in);
        processData(in, out);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void ReadPacket::removeDelimiters(RvrMsg& payload) {
        payload.erase(payload.begin()); // SOP
        payload.erase(payload.end() - 1); // EOP
        payload.erase(payload.end() - 1); // sum
    }
    //----------------------------------------------------------------------------------------------------------------------
    void ReadPacket::checkForData(rvr::RvrMsg& in) {
        if (mSerialPort.count() != 0) {
            uint8_t r[in.capacity()];
            int cnt = mSerialPort.read(r, in.capacity());
            in.insert(in.end(), r, &r[cnt]);
        }
    }
    //----------------------------------------------------------------------------------------------------------------------
    void ReadPacket::processData(rvr::RvrMsg& in, rvr::RvrMsg& packet) {
        constexpr uint8_t EopSop[] { EOP, SOP };

        auto pos = std::search(in.begin(), in.end(), EopSop, &EopSop[1]);

        if (pos != in.end()) {
            packet.insert(packet.begin(), in.begin(), pos + 1);
            in.erase(in.begin(), pos + 1);
            unescape_msg(packet);
            removeDelimiters(packet);
        }
    }
    //----------------------------------------------------------------------------------------------------------------------
    void ReadPacket::unescape_char(auto& p, RvrMsg& payload) {
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
    void ReadPacket::unescape_msg(RvrMsg& payload) {
        for (auto p { find(payload.begin(), payload.end(), ESC) }; p != payload.end(); p = find(p + 1, payload.end(), ESC)) {
            unescape_char(p, payload);
        }
    }
}

