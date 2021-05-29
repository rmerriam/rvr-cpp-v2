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
//        terr << code_loc << flags;
    }
//----------------------------------------------------------------------------------------------------------------------
    void Response::decode_error(auto err_byte) {
        switch (err_byte) {
            case 1: {
                terr << code_loc << "bad_did";
                break;
            }
            case 2: {
                terr << code_loc << "bad_cid";
                break;
            }
            case 3: {
                terr << code_loc << "not_ye1_implemented";
                break;
            }
            case 4: {
                terr << code_loc << "cannot be executed in current mode";
                break;
            }
            case 5: {
                terr << code_loc << "bad_data_length";
                break;
            }
            case 6: {
                terr << code_loc << "failed for command specific reason";
                break;
            }
            case 7: {
                terr << code_loc << "Bad Parameter Value";
                break;
            }
            case 8: {
                terr << code_loc << "busy";
                break;
            }
            case 9: {
                terr << code_loc << "bad_tid";
                break;
            }
            case 0xA: {
                terr << code_loc << "target_unavailable";
                break;
            }
        }
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Response::decode(RvrMsg packet) {
        mys::TraceOff terr_off { terr };
        terr << code_loc << "pkt: " << std::hex << packet;

        // typical positions of header bytes when target not present, the usual case
        uint8_t flags { 0x00 };   //
//        uint8_t targ { 0x01 };   // usually not present
        uint8_t src { 0x01 };     //
        uint8_t dev { 0x02 };     //
        uint8_t cmd { 0x03 };     //
        uint8_t seq { 0x04 };     //
        uint8_t status { 0x05 };  //
        uint8_t data { 0x06 };    //

        bool const is_resp { (packet[flags] & response) == response };   // versus notification

        if (packet[flags] & has_target) {  //
            ++src;
            ++dev;
            ++cmd;
            ++seq;
            ++status;
            ++data;
        }

        decode_flags(packet[flags]);

        std::string device = device_names[packet[dev]];

        Blackboard::key_t key { mBlackboard.msgKey(TargetPort(packet[src]), Devices(packet[dev]), packet[cmd], packet[seq]) };

        std::string command { mBlackboard.entryName(key) };

        if (command.empty()) {
            terr << code_loc << "Command not in decode table " << device //
                 << mys::sp << std::hex << std::setfill('0') << std::setw(8) << key;
        }
        else {
            terr << code_loc << device << mys::sp << command;

            if (is_resp && packet[status]) {
                // a response will have a status of either 0 or and error code
                auto err_byte { packet[status] };
                terr << code_loc << "ERROR: " << (uint16_t)err_byte;
                decode_error(err_byte);
            }
            else {
                mBlackboard.msgArray(key, packet.begin() + seq, packet.end());
            }
        }
        terr << __func__ << " **************";
    }
}
