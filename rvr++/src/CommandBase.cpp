#include "CommandBase.h"
namespace rvr {

    //----------------------------------------------------------------------------------------------------------------------
    uint8_t CommandBase::buildFlags(CommandResponse const want_resp) const {
        int flags { want_resp | activity | has_target };
        return static_cast<uint8_t>(flags);
    }

    //----------------------------------------------------------------------------------------------------------------------
    void CommandBase::cmd_basic(uint8_t const cmd, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mTarget, mDevice, cmd, sequence() };
        mRequest.send(msg);
    }

    //----------------------------------------------------------------------------------------------------------------------
    void CommandBase::cmd_basic_alt(uint8_t const cmd, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mAltTarget, mDevice, cmd, sequence() };
        mRequest.send(msg);
    }

    //----------------------------------------------------------------------------------------------------------------------
    void CommandBase::cmd_byte(uint8_t const cmd, uint8_t const data, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mTarget, mDevice, cmd, sequence(), data };
        mRequest.send(msg);
    }

    //----------------------------------------------------------------------------------------------------------------------
    void CommandBase::cmd_byte_alt(uint8_t const cmd, uint8_t const data, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mAltTarget, mDevice, cmd, sequence(), data };
        mRequest.send(msg);
    }

    //----------------------------------------------------------------------------------------------------------------------
    void CommandBase::cmd_byte_id(uint8_t const cmd, uint8_t const data, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mTarget, mDevice, cmd, data, data };
        mRequest.send(msg);
    }

    //----------------------------------------------------------------------------------------------------------------------
    void CommandBase::cmd_byte_alt_id(uint8_t const cmd, uint8_t const data, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mAltTarget, mDevice, cmd, data, data };
        mRequest.send(msg);
    }

    //----------------------------------------------------------------------------------------------------------------------
    void CommandBase::cmd_int(uint8_t const cmd, uint16_t const data, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mTarget, mDevice, cmd, sequence(), //
            static_cast<uint8_t>(data >> 8), static_cast<uint8_t>(data & 0xFF) };
        mRequest.send(msg);
    }

    //----------------------------------------------------------------------------------------------------------------------
    void CommandBase::cmd_int_alt(uint8_t const cmd, uint16_t const data, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mAltTarget, mDevice, cmd, sequence(), //
            static_cast<uint8_t>(data >> 8), static_cast<uint8_t>(data & 0xFF) };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void CommandBase::cmd_data(uint8_t const cmd, RvrMsg const& data, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mTarget, mDevice, cmd, sequence() };
        msg.insert(msg.end(), data.begin(), data.end());
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void CommandBase::cmd_data_alt(uint8_t const cmd, RvrMsg const& data, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mAltTarget, mDevice, cmd, sequence() };
        msg.insert(msg.end(), data.begin(), data.end());
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void CommandBase::cmd_enable(uint8_t const cmd, bool const state, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mTarget, mDevice, cmd, sequence(), state };
        mRequest.send(msg);
    } //----------------------------------------------------------------------------------------------------------------------

    void CommandBase::cmd_enable_alt(uint8_t const cmd, bool const state, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mAltTarget, mDevice, cmd, sequence(), state };
        mRequest.send(msg);
    }

    //----------------------------------------------------------------------------------------------------------------------
    void CommandBase::cmd_disable(uint8_t const cmd, bool const state, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mTarget, mDevice, cmd, state, sequence() };
        mRequest.send(msg);
    }

    //----------------------------------------------------------------------------------------------------------------------
    CommandBase::CommandBase(Devices const device, Request& request, TargetPort const target) :
        mDevice { device }, mRequest { request }, mTarget { target }, mAltTarget { makeAltProc() } {
    }

    //----------------------------------------------------------------------------------------------------------------------
    uint8_t CommandBase::makeAltProc() {
        uint8_t alt_target = (bluetoothSOC + nordic) - mTarget;
        return alt_target;
    }

}
