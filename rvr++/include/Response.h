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
#include <future>
#include <unordered_map>

#include "Blackboard.h"
#include "ReadPacket.h"

namespace rvr {
    class Blackboard;

    class Response : public ReadPacket {
    public:
        enum ErrorCode : uint8_t {
            success = 0x00,
            bad_did = 0x01,
            bad_cid = 0x02,
            not_yes_implemented = 0x03,
            restricted = 0x04,
            bad_data_length = 0x05,
            failed = 0x06,
            bad_data_value = 0x07,
            busy = 0x08,
            bad_tid = 0x09,
            target_unavailable = 0x0A,
        };

        Response(SerialPort& s, std::shared_future<void> end) :
            ReadPacket { s }, mEnd { end } {
        }
        Response(Response const& other) = delete;
        Response(Response&& other) = default;
        Response& operator=(Response const& other) = delete;

        bool operator ()();

        static void decode(RvrMsg packet);

    private:
        static void decode_flags(uint8_t const f);
        static void decode_error(auto err_byte);

        static Blackboard::key_t msgKey(CommandBase::TargetPort const src, Devices const dev, uint8_t const cmd, uint8_t const seq);

        std::shared_future<void> mEnd;
    };

} /* namespace rvr */

#endif /* RESPONSE_H_ */
