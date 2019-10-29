#ifndef COMMANDBASE_H_
#define COMMANDBASE_H_
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
//     Created: Oct 27, 2019
//
//======================================================================================================================

namespace rvr {

    class CommandBase {
    protected:
        enum Devices : uint8_t {
            api = 0x10,     //
            system = 0x11,   //
            power = 0x13,   //
            drive = 0x16,   //
            sensors = 0x18,   //
            connection = 0x19,   //
            io_led = 0x1A,   //
            api_and_shell = 0x10
        };

        enum Ports : uint8_t {
            serial = 0x01,
        };
        explicit CommandBase(Devices const device, Request& request) :
            mDevice { device }, mRequest(request) {
        }

        void do_request(uint8_t const cmd, bool const get_response = false) {
            // uniform initialization using {} causes a narrowing warning
            uint8_t flags = (get_response ? Request::request_response : 0) | Request::has_target;
            MsgArray msg { flags, serial, mDevice, cmd, mRequest.sequence() };
            mRequest.send(msg);
        }

        uint8_t const mDevice;
        Request& mRequest;

        CommandBase(CommandBase const& other) = delete;
        CommandBase(CommandBase&& other) = delete;
        CommandBase& operator=(CommandBase const& other) = delete;
    };

} /* namespace rvr */

#endif
