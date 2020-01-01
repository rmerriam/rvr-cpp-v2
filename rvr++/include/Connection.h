#ifndef Connection_H_
#define Connection_H_
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
//     Created: Oct 29, 2019
//
//======================================================================================================================
#include <Request.h>
#include <SendPacket.h>
//----------------------------------------------------------------------------------------------------------------------
namespace rvr {
    class Blackboard;

    class Connection : protected Request {
    public:
        Connection(Blackboard& bb, SendPacket& req) :
            Request { bb, Devices::connection, req, nordic } {
        }
        Connection(Connection const& other) = delete;
        Connection(Connection&& other) = delete;
        Connection& operator=(Connection const& other) = delete;

        void bluetoothName(CommandResponse const want_resp = resp_yes) const;

        std::optional<std::string> name();

    private:
        enum Cmd : uint8_t {
            get_bluetooth_advertising_name = 0x05, //
        };

    };
    //----------------------------------------------------------------------------------------------------------------------
    inline void Connection::bluetoothName(CommandResponse const want_resp) const {
        basic(get_bluetooth_advertising_name, want_resp);
    }

} /* namespace rvr */

#endif /* Connection_H_ */
