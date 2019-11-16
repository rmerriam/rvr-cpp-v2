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
#include "Packet.h"

namespace rvr {

    class ReadPacket {
    public:
        ReadPacket(SerialPort& s) :
            mSerialPort { s } {
        }

        void read(rvr::MsgArray& in, rvr::MsgArray& out);

    private:

        void unescape_char(auto& p, MsgArray& payload);
        void unescape_msg(MsgArray& payload);
        void removeDelimiters(MsgArray& payload);

        void checkForData(rvr::MsgArray& in);
        void processData(rvr::MsgArray& in, rvr::MsgArray& out);

        SerialPort& mSerialPort;
    };
    //----------------------------------------------------------------------------------------------------------------------
    inline void ReadPacket::removeDelimiters(MsgArray& payload) {
        payload.erase(payload.begin());
        payload.erase(payload.end() - 1);
    }
}
#endif /* Packet_H_ */
