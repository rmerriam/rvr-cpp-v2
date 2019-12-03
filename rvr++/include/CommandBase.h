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

#include "Request.h"
namespace rvr {

    class CommandBase {
    public:

        enum CommandResponse : uint8_t {
            resp_no = no_response, //
            resp_yes = request_response, //
            resp_on_error = request_error_response,
        };

        enum SourcePort : uint8_t {
            serial = 0x01, //
            bluetooth = 0x02,
        };
        enum TargetPort : uint8_t {
            bluetoothSOC = 0x01, //
            nordic = 0x02,
        };

        explicit CommandBase(Devices const device, Request& request, TargetPort const target);

        uint8_t buildFlags(CommandResponse const want_resp) const;

        void cmd_basic(uint8_t const cmd, CommandResponse const want_resp = resp_on_error) const;
        void cmd_basic_alt(uint8_t const cmd, CommandResponse const want_resp = resp_on_error) const;

        void cmd_byte(uint8_t const cmd, uint8_t const data, CommandResponse const want_resp = resp_on_error) const;
        void cmd_byte_alt(uint8_t const cmd, uint8_t const data, CommandResponse const want_resp = resp_on_error) const;

        void cmd_byte_id(uint8_t const cmd, uint8_t const data, CommandResponse const want_resp = resp_on_error) const;
        void cmd_byte_alt_id(uint8_t const cmd, uint8_t const data, CommandResponse const want_resp = resp_on_error) const;

        void cmd_int(uint8_t const cmd, uint16_t const data, CommandResponse const want_resp = resp_on_error) const;
        void cmd_int_alt(uint8_t const cmd, uint16_t const data, CommandResponse const want_resp = resp_on_error) const;

        void cmd_enable(uint8_t const cmd, bool const state, CommandResponse const want_resp = resp_on_error) const;
        void cmd_enable_alt(uint8_t const cmd, bool const state, CommandResponse const want_resp = resp_on_error) const;
        void cmd_disable(uint8_t const cmd, bool const state, CommandResponse const want_resp = resp_on_error) const;

        void cmd_data(uint8_t const cmd, RvrMsg const& data, CommandResponse const want_resp = resp_on_error) const;
        void cmd_data_alt(uint8_t const cmd, RvrMsg const& data, CommandResponse const want_resp = resp_on_error) const;

        Devices const mDevice;
        Request& mRequest;
        TargetPort mTarget;
        TargetPort mAltTarget;

        CommandBase(CommandBase const& other) = delete;
        CommandBase(CommandBase&& other) = delete;
        CommandBase& operator=(CommandBase const& other) = delete;

    protected:
        // NOTE: HACK ALERT!!!
        // The sequence field is used for debugging only. It isn't used for tracking messages.
        // It is used as a special field for some message that pass an ID. The ID is put in the sequence field.
        // During deserialization any sequence less than 0x80 is used as part of the dictionary key for updating
        // values in the dictionary.
        static uint8_t sequence() {
            return ++mSeq | 0x80;
        }
    private:
        static inline uint8_t mSeq { 0x0 };

        uint8_t makeAltProc();
    };

} /* namespace rvr */

#endif
