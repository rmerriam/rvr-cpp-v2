#ifndef RESPONSE_H_
#define RESPONSE_H_
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
#include "trace.h"
#include "Packet.h"

namespace rvr {

    class Response : public Packet {
    public:
        Response(SerialPort& s) :
            Packet { s } {
        }
        Response(Response const& other) = delete;
        Response(Response&& other) = delete;
        Response& operator=(Response const& other) = delete;

        void read() {
            uint8_t resp[40];
            int cnt = mSerialPort.read(resp, 20);
            mMsg.assign(resp, &resp[cnt]);

            tracenl(std::cerr, "resp: ", cnt);
            trace(std::cerr, mMsg);
            tracenl(std::cerr);
        }

    private:

    };

} /* namespace rvr */

#endif /* RESPONSE_H_ */
