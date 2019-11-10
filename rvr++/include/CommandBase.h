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
        //        static void decode_src(uint8_t const source);
//        static void decode_target(uint8_t const target);
//        static void decode_device(uint8_t const device);

    protected:
        enum Devices : uint8_t {
            api_and_shell = 0x10,    //
            system = 0x11,   //
            power = 0x13,   //
            drive = 0x16,   //
            sensors = 0x18,   //
            connection = 0x19,   //
            io_led = 0x1A,   //
        };

        enum SourcePort : uint8_t {
            serial = 0x01, //
            bluetooth = 0x02,
        };
        enum TargetPort : uint8_t {
            bluetoothSOC = 0x01, //
            microcontroller = 0x02,

        };
        explicit CommandBase(const Devices device, Request& request, const uint8_t target);
        uint8_t buildFlags(const bool get_response) const;
        void cmd_basic(const uint8_t cmd, const bool get_response = false);
        void cmd_basic_alt(const uint8_t cmd, const bool get_response = false);

        void cmd_byte(const uint8_t cmd, const uint8_t data, const bool get_response = false);
        void cmd_byte_alt(const uint8_t cmd, const uint8_t data, const bool get_response = false);

        void cmd_enable(const uint8_t cmd, const bool state, const bool get_response = false);
        void cmd_disable(const uint8_t cmd, const bool state, const bool get_response = false);

        const uint8_t mDevice;
        Request& mRequest;
        uint8_t mTarget;

        CommandBase(const CommandBase& other) = delete;
        CommandBase(CommandBase&& other) = delete;
        CommandBase& operator=(const CommandBase& other) = delete;
    };
    //----------------------------------------------------------------------------------------------------------------------
    inline CommandBase::CommandBase(const Devices device, Request& request, const uint8_t target) :
        mDevice { device }, mRequest { request }, mTarget { target } {
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline uint8_t CommandBase::buildFlags(const bool get_response) const {
        uint8_t flags { static_cast<uint8_t>((get_response ? Request::request_response : 0) | Request::has_target) };
        flags |= Request::activity;
        return flags;
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void CommandBase::cmd_basic(const uint8_t cmd, const bool get_response) {
        MsgArray msg { buildFlags(get_response), mTarget, mDevice, cmd, mRequest.sequence() };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void CommandBase::cmd_basic_alt(const uint8_t cmd, const bool get_response) {
        uint8_t alt_target = (bluetoothSOC + microcontroller) - mTarget;
        MsgArray msg { buildFlags(get_response), alt_target, mDevice, cmd, mRequest.sequence() };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void CommandBase::cmd_enable(const uint8_t cmd, const bool state, const bool get_response) {
        MsgArray msg { buildFlags(get_response), mTarget, mDevice, cmd, mRequest.sequence(), state };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void CommandBase::cmd_disable(const uint8_t cmd, const bool state, const bool get_response) {
        MsgArray msg { buildFlags(get_response), mTarget, mDevice, cmd, state, mRequest.sequence() };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void CommandBase::cmd_byte(const uint8_t cmd, const uint8_t data, const bool get_response) {
        MsgArray msg { buildFlags(get_response), mTarget, mDevice, cmd, mRequest.sequence(), data };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void CommandBase::cmd_byte_alt(const uint8_t cmd, const uint8_t data, const bool get_response) {
        uint8_t alt_target = (bluetoothSOC + microcontroller) - mTarget;
        MsgArray msg { buildFlags(get_response), alt_target, mDevice, cmd, mRequest.sequence(), data };
        mRequest.send(msg);
    }
} /* namespace rvr */

#endif
