#ifndef REQUEST_H_
#define REQUEST_H_
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
//  Build a message to send to the RVR

#include <algorithm>
#include "trace.h"
#include "Packet.h"

namespace rvr {

    class Request : public Packet {
    public:

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

        Request(SerialPort& s) :
            Packet { s } {
        }

        void send(MsgArray const& p);
    private:

    };
}
using RFlags = rvr::Request::flags;

#endif /* REQUEST_H_ */
