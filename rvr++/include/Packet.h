#ifndef PACKET_H_
#define PACKET_H_
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
//     Created: Nov 15, 2019
//
//======================================================================================================================
#include <string>

#include "Trace.h"

#include "SerialPort.h"
//----------------------------------------------------------------------------------------------------------------------
using char_ptr = char const *;
//----------------------------------------------------------------------------------------------------------------------

namespace rvr {

    using RvrMsg = std::basic_string<uint8_t>;
    using RvrMsgRet_t = std::optional<RvrMsg>;

    enum flags : uint8_t {
        no_response = 0x00, //
        response = 0x01,  //
        request_response = 0x02,  //
        request_error_response = 0x06, // must request response to get error response
        activity = 0x08, //
        has_target = 0x10, //
        has_source = 0x20, //
        has_more_flags = 0x80,
    };

    enum SpecialChars : uint8_t {
        SOP = 0x8D, //
        EOP = 0xD8,  //
        ESC = 0xAB,  //
        escaped_SOP = 0x05,   //
        escaped_EOP = 0x50,   //
        escaped_ESC = 0x23,
    };

    enum Devices : uint8_t {
        api_and_shell = 0x10,    //
        system = 0x11,   //
        power = 0x13,   //
        drive = 0x16,   //
        sensors = 0x18,   //
        connection = 0x19,   //
        io_led = 0x1A,   //
    };

    enum TargetPort : uint8_t {
        nordic = 0x01,  //
        bluetoothSOC = 0x02, //
    };
    enum SpecialSeq : uint8_t {
        enable = 0x20, disable,
    };
}

#endif /* PACKET_H_ */
