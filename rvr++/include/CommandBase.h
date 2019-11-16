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
    public:

        enum CommandResponse : uint8_t {
            resp_no = no_response, //
            resp_yes = request_response, //
            resp_on_error = request_error_response,
        };

    protected:

        enum SourcePort : uint8_t {
            serial = 0x01, //
            bluetooth = 0x02,
        };
        enum TargetPort : uint8_t {
            bluetoothSOC = 0x01, //
            microcontroller = 0x02,

        };
        explicit CommandBase(const Devices device, Request& request, const uint8_t target);

        uint8_t buildFlags(const CommandResponse want_resp) const;

        void cmd_basic(const uint8_t cmd, const CommandResponse want_resp = resp_on_error);
        void cmd_basic_alt(const uint8_t cmd, const CommandResponse want_resp = resp_on_error);

        void cmd_byte(const uint8_t cmd, const uint8_t data, const CommandResponse want_resp = resp_on_error);
        void cmd_byte_alt(const uint8_t cmd, const uint8_t data, const CommandResponse want_resp = resp_on_error);

        void cmd_int(const uint8_t cmd, const uint16_t data, const CommandResponse want_resp = resp_on_error);

        void cmd_enable(const uint8_t cmd, const bool state, const CommandResponse want_resp = resp_on_error);
        void cmd_disable(const uint8_t cmd, const bool state, const CommandResponse want_resp = resp_on_error);

        void cmd_data(const uint8_t cmd, const MsgArray& data, const CommandResponse want_resp = resp_on_error);
        void cmd_data_alt(const uint8_t cmd, const MsgArray& data, const CommandResponse want_resp = resp_on_error);

        const uint8_t mDevice;
        Request& mRequest;
        uint8_t mTarget;

        CommandBase(const CommandBase& other) = delete;
        CommandBase(CommandBase&& other) = delete;
        CommandBase& operator=(const CommandBase& other) = delete;

    private:
        static inline uint8_t mSeq { 0x80 };

        uint8_t sequence() {
            return ++mSeq;
        }

        uint8_t makeAltProc();
    };

//----------------------------------------------------------------------------------------------------------------------
    inline CommandBase::CommandBase(const Devices device, Request& request, const uint8_t target) :
        mDevice { device }, mRequest { request }, mTarget { target } {
    }
//----------------------------------------------------------------------------------------------------------------------
    inline uint8_t CommandBase::buildFlags(const CommandResponse want_resp) const {
        int flags { want_resp | activity | has_target };
        return static_cast<uint8_t>(flags);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline uint8_t CommandBase::makeAltProc() {
        uint8_t alt_target = (bluetoothSOC + microcontroller) - mTarget;
        return alt_target;
    }
//----------------------------------------------------------------------------------------------------------------------
    inline void CommandBase::cmd_basic(const uint8_t cmd, const CommandResponse want_resp) {
        MsgArray msg { buildFlags(want_resp), mTarget, mDevice, cmd, sequence() };
        mRequest.send(msg);
    }
//----------------------------------------------------------------------------------------------------------------------
    inline void CommandBase::cmd_basic_alt(const uint8_t cmd, const CommandResponse want_resp) {
        uint8_t alt_target = (bluetoothSOC + microcontroller) - mTarget;
        MsgArray msg { buildFlags(want_resp), alt_target, mDevice, cmd, sequence() };
        mRequest.send(msg);
    }
//----------------------------------------------------------------------------------------------------------------------
    inline void CommandBase::cmd_enable(const uint8_t cmd, const bool state, const CommandResponse want_resp) {
        MsgArray msg { buildFlags(want_resp), mTarget, mDevice, cmd, sequence(), state };
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
        MsgArray msg { buildFlags(want_resp), makeAltProc(), mDevice, cmd, sequence(), data };
        mRequest.send(msg);
    }
//----------------------------------------------------------------------------------------------------------------------
    inline void CommandBase::cmd_int(const uint8_t cmd, const uint16_t data, const CommandResponse want_resp) {
        MsgArray msg { buildFlags(want_resp), mTarget, mDevice, cmd, sequence(), //
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
        MsgArray msg { buildFlags(want_resp), makeAltProc(), mDevice, cmd, sequence() };
        msg.insert(msg.end(), data.begin(), data.end());
        mRequest.send(msg);
    }

} /* namespace rvr */

#endif
