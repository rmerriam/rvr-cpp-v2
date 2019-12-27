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
//     Created: Oct 27, 2019
//
//======================================================================================================================
#include "SendPacket.h"
#include "enum.h"
//----------------------------------------------------------------------------------------------------------------------
namespace rvr {
    class Blackboard;

    class Request {
    public:

        explicit Request(Blackboard& bb, Devices const device, SendPacket& request, TargetPort const target);

        uint8_t buildFlags(CommandResponse const want_resp) const;

        void basic(uint8_t const cmd, CommandResponse const want_resp = resp_on_error) const;
        void basicAlt(uint8_t const cmd, CommandResponse const want_resp = resp_on_error) const;

        void reqByte(uint8_t const cmd, uint8_t const data, CommandResponse const want_resp = resp_on_error) const;
        void reqByteAlt(uint8_t const cmd, uint8_t const data, CommandResponse const want_resp = resp_on_error) const;

        void byteId(uint8_t const cmd, uint8_t const data, CommandResponse const want_resp = resp_on_error) const;
        void byteAltId(uint8_t const cmd, uint8_t const data, CommandResponse const want_resp = resp_on_error) const;

        void reqInt(uint8_t const cmd, uint16_t const data, CommandResponse const want_resp = resp_on_error) const;
        void reqIntAlt(uint8_t const cmd, uint16_t const data, CommandResponse const want_resp = resp_on_error) const;

        void cmdEnable(uint8_t const cmd, CommandResponse const want_resp = resp_on_error) const;
        void cmdEnableAlt(uint8_t const cmd, CommandResponse const want_resp = resp_on_error) const;
        void cmdDisable(uint8_t const cmd, CommandResponse const want_resp = resp_on_error) const;
        void cmdDisableAlt(uint8_t const cmd, CommandResponse const want_resp = resp_on_error) const;

        void cmdData(uint8_t const cmd, RvrMsg const& data, CommandResponse const want_resp = resp_on_error) const;
        void cmdDataAlt(uint8_t const cmd, RvrMsg const& data, CommandResponse const want_resp = resp_on_error) const;

        Request(Request const& other) = delete;
        Request(Request&& other) = delete;
        Request& operator=(Request const& other) = delete;

        TargetPort const altTarget() const;
        Devices const device() const;
        SendPacket& request() const;
        TargetPort const target() const;

        static uint8_t seq();

    protected:
        // NOTE: HACK ALERT!!!
        // The sequence field is used for debugging only. It isn't used for tracking messages.
        // It is used as a special field for some message that pass an ID. The ID is put in the sequence field.
        // During deserialization any sequence less than 0x80 is used as part of the dictionary key for updating
        // values in the dictionary.
        static uint8_t sequence() {
            return ++mSeq | 0x80;
        }

        Blackboard& mBlackboard;
        Devices const mDevice;
        SendPacket& mRequest;
        TargetPort const mTarget;
        TargetPort const mAltTarget;

    private:
        static inline uint8_t mSeq { 0x0 };

        uint8_t makeAltProc();
    };

} /* namespace rvr */

#endif
