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

        void read(rvr::RvrMsg& in, rvr::RvrMsg& out);

    private:

        void unescape_char(auto& p, RvrMsg& payload);
        void unescape_msg(RvrMsg& payload);
        void removeDelimiters(RvrMsg& payload);

        void checkForData(rvr::RvrMsg& in);
        void processData(rvr::RvrMsg& in, rvr::RvrMsg& out);

        SerialPort& mSerialPort;
    };

}
#endif /* Packet_H_ */
