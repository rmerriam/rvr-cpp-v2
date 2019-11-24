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

        explicit CommandBase(const Devices device, Request& request, const TargetPort target);

        uint8_t buildFlags(const CommandResponse want_resp) const;

        void cmd_basic(const uint8_t cmd, const CommandResponse want_resp = resp_on_error);
        void cmd_basic_alt(const uint8_t cmd, const CommandResponse want_resp = resp_on_error);

        void cmd_byte(const uint8_t cmd, const uint8_t data, const CommandResponse want_resp = resp_on_error);
        void cmd_byte_alt(const uint8_t cmd, const uint8_t data, const CommandResponse want_resp = resp_on_error);

        void cmd_byte_id(const uint8_t cmd, const uint8_t data, const CommandResponse want_resp = resp_on_error);
        void cmd_byte_alt_id(const uint8_t cmd, const uint8_t data, const CommandResponse want_resp = resp_on_error);

        void cmd_int(const uint8_t cmd, const uint16_t data, const CommandResponse want_resp = resp_on_error);
        void cmd_int_alt(const uint8_t cmd, const uint16_t data, const CommandResponse want_resp = resp_on_error);

        void cmd_enable(const uint8_t cmd, const bool state, const CommandResponse want_resp = resp_on_error);
        void cmd_enable_alt(const uint8_t cmd, const bool state, const CommandResponse want_resp = resp_on_error);
        void cmd_disable(const uint8_t cmd, const bool state, const CommandResponse want_resp = resp_on_error);

        void cmd_data(const uint8_t cmd, const MsgArray& data, const CommandResponse want_resp = resp_on_error);
        void cmd_data_alt(const uint8_t cmd, const MsgArray& data, const CommandResponse want_resp = resp_on_error);

        const Devices mDevice;
        Request& mRequest;
        TargetPort mTarget;
        TargetPort mAltTarget;

        CommandBase(const CommandBase& other) = delete;
        CommandBase(CommandBase&& other) = delete;
        CommandBase& operator=(const CommandBase& other) = delete;

    protected:
        // NOTE: HACK ALERT!!!
        // The sequence field is used for debugging only. It isn't used for tracking messages.
        // It is used as a special field for some message that pass an ID. The ID is put in the sequence field.
        // During deserialization any sequence less than 0x80 is used as part of the dictionary key for updating
        // values in the dictionary.
        uint8_t sequence() {
            return ++mSeq | 0x80;
        }
    private:
        static inline uint8_t mSeq { 0x0 };

        uint8_t makeAltProc();
    };

//----------------------------------------------------------------------------------------------------------------------
    inline CommandBase::CommandBase(const Devices device, Request& request, const TargetPort target) :
        mDevice { device }, mRequest { request }, mTarget { target }, mAltTarget { makeAltProc() } {
    }
//----------------------------------------------------------------------------------------------------------------------
    inline uint8_t CommandBase::buildFlags(const CommandResponse want_resp) const {
        int flags { want_resp | activity | has_target };
        return static_cast<uint8_t>(flags);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline uint8_t CommandBase::makeAltProc() {
        uint8_t alt_target = (bluetoothSOC + nordic) - mTarget;
        return alt_target;
    }
//----------------------------------------------------------------------------------------------------------------------
    inline void CommandBase::cmd_basic(const uint8_t cmd, const CommandResponse want_resp) {
        MsgArray msg { buildFlags(want_resp), mTarget, mDevice, cmd, sequence() };
        mRequest.send(msg);
    }
//----------------------------------------------------------------------------------------------------------------------
    inline void CommandBase::cmd_basic_alt(const uint8_t cmd, const CommandResponse want_resp) {
        MsgArray msg { buildFlags(want_resp), mAltTarget, mDevice, cmd, sequence() };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void CommandBase::cmd_enable(const uint8_t cmd, const bool state, const CommandResponse want_resp) {
        MsgArray msg { buildFlags(want_resp), mTarget, mDevice, cmd, sequence(), state };
        mRequest.send(msg);
    }        //----------------------------------------------------------------------------------------------------------------------
    inline void CommandBase::cmd_enable_alt(const uint8_t cmd, const bool state, const CommandResponse want_resp) {
        MsgArray msg { buildFlags(want_resp), mAltTarget, mDevice, cmd, sequence(), state };
        mRequest.send(msg);
    }
//----------------------------------------------------------------------------------------------------------------------
    inline void CommandBase::cmd_disable(const uint8_t cmd, const bool state, const CommandResponse want_resp) {
        MsgArray msg { buildFlags(want_resp), mTarget, mDevice, cmd, state, sequence() };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void CommandBase::cmd_byte(const uint8_t cmd, const uint8_t data, const CommandResponse want_resp) {
        MsgArray msg { buildFlags(want_resp), mTarget, mDevice, cmd, sequence(), data };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void CommandBase::cmd_byte_alt(const uint8_t cmd, const uint8_t data, const CommandResponse want_resp) {
        MsgArray msg { buildFlags(want_resp), mAltTarget, mDevice, cmd, sequence(), data };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void CommandBase::cmd_byte_id(const uint8_t cmd, const uint8_t data, const CommandResponse want_resp) {
        MsgArray msg { buildFlags(want_resp), mTarget, mDevice, cmd, data, data };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void CommandBase::cmd_byte_alt_id(const uint8_t cmd, const uint8_t data, const CommandResponse want_resp) {
        MsgArray msg { buildFlags(want_resp), mAltTarget, mDevice, cmd, data, data };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void CommandBase::cmd_int(const uint8_t cmd, const uint16_t data, const CommandResponse want_resp) {
        MsgArray msg { buildFlags(want_resp), mTarget, mDevice, cmd, sequence(), //
                       static_cast<uint8_t>(data >> 8), static_cast<uint8_t>(data & 0xFF) };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void CommandBase::cmd_int_alt(const uint8_t cmd, const uint16_t data, const CommandResponse want_resp) {
        MsgArray msg { buildFlags(want_resp), mAltTarget, mDevice, cmd, sequence(), //
                       static_cast<uint8_t>(data >> 8), static_cast<uint8_t>(data & 0xFF) };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void CommandBase::cmd_data(const uint8_t cmd, const MsgArray& data, const CommandResponse want_resp) {
        MsgArray msg { buildFlags(want_resp), mTarget, mDevice, cmd, sequence() };
        msg.insert(msg.end(), data.begin(), data.end());
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void CommandBase::cmd_data_alt(const uint8_t cmd, const MsgArray& data, const CommandResponse want_resp) {
        MsgArray msg { buildFlags(want_resp), mAltTarget, mDevice, cmd, sequence() };
        msg.insert(msg.end(), data.begin(), data.end());
        mRequest.send(msg);
    }

} /* namespace rvr */

#endif
