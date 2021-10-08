//======================================================================================================================
// 2021 Copyright Mystic Lake Software
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
//     Created: May 29, 2021
//
//======================================================================================================================

#include "enum.h"
#include "Blackboard.h"
#include "ReadPacket.h"
#include "Response.h"

namespace rvr {

    using DeviceNames = std::unordered_map <uint8_t, std::string>;

    DeviceNames device_names { //
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
        rvr::RvrMsg in;
        rvr::RvrMsg packet;
        in.reserve(80);

        while (mStop.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) {
            mReadPacket.read(in, packet);

            if ( !packet.empty()) {
                decode(packet);
                packet.clear();
            }
        }
        return true;
    }
//----------------------------------------------------------------------------------------------------------------------
    void Response::decode_flags(uint8_t const f) {
        std::string flags {};

        if ((f & response) == 0) {
            flags += "notification | ";
        }

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
        mys::tout << code_line << flags;
    }
//----------------------------------------------------------------------------------------------------------------------
    void Response::decode_error(auto err_byte) {

        switch (err_byte) {
            case 1: {
                mys::tout << code_line << "bad_did";
                break;
            }
            case 2: {
                mys::tout << code_line << "bad_cid";
                break;
            }
            case 3: {
                mys::tout << code_line << "not_yet_implemented";
                break;
            }
            case 4: {
                mys::tout << code_line << "cannot be executed in current mode";
                break;
            }
            case 5: {
                mys::tout << code_line << "bad_data_length";
                break;
            }
            case 6: {
                mys::tout << code_line << "failed for command specific reason";
                break;
            }
            case 7: {
                mys::tout << code_line << "Bad Parameter Value";
                break;
            }
            case 8: {
                mys::tout << code_line << "busy";
                break;
            }
            case 9: {
                mys::tout << code_line << "bad_tid";
                break;
            }
            case 0xA: {
                mys::tout << code_line << "target_unavailable";
                break;
            }
        }
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Response::decode(RvrMsg packet) {
        mys::TraceOn tdbg_ctrl { mys::tout };
        mys::tout << code_line << "pkt: " << std::hex << packet;

        // typical positions of header bytes when target not present which is the usual case
        uint8_t flags { 0x00 };
//        uint8_t targ { 0x01 };   // usually not present
        uint8_t src { 0x01 };
        uint8_t dev { 0x02 };
        uint8_t cmd { 0x03 };
        uint8_t seq { 0x04 };
        uint8_t err_code { 0x05 };
        uint8_t data { 0x06 };

        /*
         * There are 2 types of received messages.
         *
         * Response: received when a request message sent and a response requested
         * Notification: received when the RVR sends a message not in response to a request.
         *      The sequence number for notifications is 0xFF
         *      There is no error field

         */
        bool const is_resp { (packet[flags] & response) == response };   // versus notification

        if (packet[flags] & has_target) {  //
            ++src;
            ++dev;
            ++cmd;
            ++seq;
            ++err_code;
            ++data;
        }

        decode_flags(packet[flags]);

        std::string device = device_names[packet[dev]];

        Blackboard::key_t key { mBlackboard.msgKey(TargetPort(packet[src]), Devices(packet[dev]), packet[cmd], //
                                                   (packet[seq] == 0xFF) ? packet[err_code] : packet[seq]) };

        std::string command { mBlackboard.entryName(key) };

        if (command.empty()) {
            mys::tout << code_line << "Command not in decode table " << device //
                << mys::sp << std::hex << std::setfill('0') << std::setw(8) << key << mys::sp << packet;
        }
        else {
            mys::tout << code_line << device << mys::sp << command;

            if (is_resp && packet[err_code]) {
                // a response will have a status of either 0 or and error code
                auto err_byte { packet[err_code] };
                mys::tout << code_line << "ERROR: " << (uint16_t)err_byte;
                decode_error(err_byte);
            }
            else {
                mys::tout << code_line << std::hex << (uint16_t)packet[cmd] << mys::tab << "pkt: " << std::hex << packet;
                mBlackboard.msgArray(key, packet[cmd], packet.begin() + seq, packet.end());
            }
        }
        mys::tout << __func__ << " **************" << std::dec;
    }
}
