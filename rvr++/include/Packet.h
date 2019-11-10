#ifndef Packet_H_
#define Packet_H_
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
//  Base class for building packets which are either Requests or Response
#include <vector>
#include "SerialPort.h"

namespace rvr {
    using MsgArray = std::vector<uint8_t>;

    class Packet {
    public:
        Packet(SerialPort& s) :
            mSerialPort { s } {
        }

        uint8_t sequence() {
            return ++mSeq;
        }

        static void unescape_msg(MsgArray& payload);
        static void escape_msg(MsgArray& payload);

        enum flags : uint8_t {
            no_response = 0x00,
            response = 0x01,
            request_response = 0x02,
            request_error_response = 0x04,
            activity = 0x08,
            has_target = 0x10,
            has_source = 0x20,
            has_more_flags = 0x80,
        };
        using RFlags = Packet::flags;

    protected:

        enum SpecialChars : uint8_t {
            SOP = 0x8D, EOP = 0xD8, ESC = 0xAB, escaped_SOP = 0x05, escaped_EOP = 0x50, escaped_ESC = 0x23,
        };

        uint8_t checksum(const MsgArray& payload) const;

        static bool isPacketChar(const uint8_t c);

        SerialPort& mSerialPort;
        MsgArray mMsg { 40 };

    private:
        static auto escape_char(MsgArray::iterator& p, MsgArray& payload);
        static void unescape_char(auto& p, MsgArray& payload);

        static uint8_t mSeq;
    };
}

#endif /* Packet_H_ */
