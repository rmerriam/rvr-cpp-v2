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
#include <Blackboard.h>
#include <ReadPacket.h>
#include <chrono>
#include <string>
#include <unordered_map>

#include "Packet.h"
#include "Response.h"

namespace rvr {

    using DeviceDecoder = std::unordered_map <uint8_t, std::string>;
    DeviceDecoder device_names { //
    { 0x10, "api_and_shell" }, //
    { 0x11, "system" }, //
    { 0x13, "power" }, //
    { 0x16, "drive" }, //
    { 0x18, "sensors" }, //
    { 0x19, "connection" }, //
    { 0x1A, "io_led" }, //
    };
    //----------------------------------------------------------------------------------------------------------------------
    bool Response::operator ()() {
        rvr::MsgArray in;
        rvr::MsgArray packet;
        in.reserve(80);

        while (mEnd.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) {
            read(in, packet);

            if ( !packet.empty()) {
                terr << __func__ << mys::sp << "pkt: " << std::hex << packet;
                decode(packet);
                packet.clear();
            }
        }
        terr << __func__ << mys::sp << " exit";
        return true;
    }

//----------------------------------------------------------------------------------------------------------------------
    void Response::decode_flags(const uint8_t f) {
        std::string flags { };

        for (auto mask { 0x01 }; mask != 0; mask <<= 1) {
            switch (mask & f) {
                case response:
                    flags += "response | ";
                    break;
                case request_response:
                    flags += "request_response | ";
                    break;
                case request_error_response:
                    flags += "request_error_response | ";
                    break;
                case activity:
                    flags += "activity | ";
                    break;
                case has_target:
                    flags += "has_target | ";
                    break;
                case has_source:
                    flags += "has_source | ";
                    break;
                case has_more_flags:
                    flags += "has_more_flags | ";
                    break;
            }
        }
        if ((f & response) == 0) {
            flags += "no_response";
        }
        terr << __func__ << mys::sp << flags;
    }
//----------------------------------------------------------------------------------------------------------------------
    void Response::decode_error(auto err_byte) {
        switch (err_byte) {
            case 1: {
                terr << __func__ << mys::sp << "bad_did";
                break;
            }
            case 2: {
                terr << __func__ << mys::sp << "bad_cid";
                break;
            }
            case 3: {
                terr << __func__ << mys::sp << "not_yes_implemented";
                break;
            }
            case 4: {
                terr << __func__ << mys::sp << "restricted";
                break;
            }
            case 5: {
                terr << __func__ << mys::sp << "bad_data_length";
                break;
            }
            case 6: {
                terr << __func__ << mys::sp << "failed";
                break;
            }
            case 7: {
                terr << __func__ << mys::sp << "bad bad_data_value";
                break;
            }
            case 8: {
                terr << __func__ << mys::sp << "busy";
                break;
            }
            case 9: {
                terr << __func__ << mys::sp << "bad_tid";
                break;
            }
            case 0xA: {
                terr << __func__ << mys::sp << "target_unavailable";
                break;
            }
        }
    }
//----------------------------------------------------------------------------------------------------------------------
    void Response::decode(MsgArray packet) {
        enum BytePositions : uint8_t {
            flags = 0x01,   //
            //            targ = 0x02,   //
            src = 0x02,            //
            dev = 0x03,            //
            cmd = 0x04,            //
            seq = 0x05,            //
            status = 0x06,            //
            data = 0x07,            //
        };

        const bool is_resp { (packet[flags] & response) == response };   // versus notification

        // if there is a target byte then increment byte position by 1
        int8_t offset = (packet[flags] & has_target) ? 1 : 0;
        decode_flags(packet[flags]);

        std::string device = device_names[packet[dev + offset]];

        const uint16_t key = packet[dev + offset] << 8 | packet[cmd + offset];
        std::string command { RespDecode::getName(key) };

        if (command.empty()) {
            terr << __func__ << " Command not in decode table " << device << mys::sp << std::hex << key;
        }
        else {
            terr << __func__ << mys::sp << device << mys::sp << command;

            if (is_resp) {
                if (packet[status + offset]) {
                    auto err_byte { packet[status + offset] };
                    terr << __func__ << mys::sp << "ERROR: " << (uint16_t)err_byte;
                    decode_error(err_byte);
                }
                else {
                    RespDecode::FuncPtr decode_func { RespDecode::getFunc(key) };
                    decode_func(packet.begin() + data + offset, packet.end() - 2);
                }
            }
            else {  // notification - no sequence number
                terr << __func__ << " notification " << std::hex << (int)packet[seq];
                RespDecode::FuncPtr decode_func { RespDecode::getFunc(key) };
                decode_func(packet.begin() + seq, packet.end() - 2);
            }
        }
        terr << __func__ << " **************";
    }
}
