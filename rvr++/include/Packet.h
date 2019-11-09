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
#if 0

struct Header {
    enum Flags : uint8_t {
        resp = 1,   // expect error byte
        tid = 0x10, // has target id
        sid = 0x20, // has source id
    };
    uint8_t Tid;    // target id
    uint8_t Sid;    // source id
    uint8_t Did;    // device id
    uint8_t Cid;    // command id
    uint8_t Seq;    // sequence
    uint8_t Err;    // error
};

In the meantime, I can tell you that the header bytes, in order are:
TID - Target ID (if there is one) - the address of the target, expressed as a port ID and a node ID
SID - Source ID (if there is one) - the address of the sorce, expressed as a port ID and a node ID
DID - Device ID - The command group (eg Power or LEDs or Driving )
CID - Command ID - The command to execute
Seq - A token (random number) that the sender sends to the receiver and the receiver sends back
Err - Error (if there is one)

There are also flags, as you noted, that modify the behavior of the packet:
Bit 0 indicates whether the packet is a response(1) or a command (0); if the packet is a response, you can expect there to be an error code byte in the header.
Bit 4 indicates whether the packet has a Target ID in the header (1) or not (0).
Bit 5 indicates whether the packet has a Source ID in the header (1) or not (0).

#endif

#endif /* Packet_H_ */
