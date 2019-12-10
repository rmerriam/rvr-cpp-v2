#include "CommandBase.h"
namespace rvr {

    //----------------------------------------------------------------------------------------------------------------------
    CommandBase::CommandBase(Devices const device, Request& request, TargetPort const target) :
        mDevice { device }, mRequest { request }, mTarget { target }, mAltTarget { makeAltProc() } {
    }
    //----------------------------------------------------------------------------------------------------------------------
    uint8_t CommandBase::buildFlags(CommandResponse const want_resp) const {
        int flags { want_resp | activity | has_target };
        return static_cast<uint8_t>(flags);
    }
    //----------------------------------------------------------------------------------------------------------------------
    uint8_t CommandBase::makeAltProc() {
        uint8_t alt_target = (bluetoothSOC + nordic) - mTarget;
        return alt_target;
    }
    //----------------------------------------------------------------------------------------------------------------------
    void CommandBase::cmdBasic(uint8_t const cmd, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mTarget, mDevice, cmd, sequence() };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void CommandBase::cmdBasicAlt(uint8_t const cmd, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mAltTarget, mDevice, cmd, sequence() };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void CommandBase::cmdByte(uint8_t const cmd, uint8_t const data, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mTarget, mDevice, cmd, sequence(), data };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void CommandBase::cmdByteAlt(uint8_t const cmd, uint8_t const data, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mAltTarget, mDevice, cmd, sequence(), data };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void CommandBase::cmdByteId(uint8_t const cmd, uint8_t const data, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mTarget, mDevice, cmd, data, data };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void CommandBase::cmdByteAltId(uint8_t const cmd, uint8_t const data, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mAltTarget, mDevice, cmd, data, data };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void CommandBase::cmdData(uint8_t const cmd, RvrMsg const& data, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mTarget, mDevice, cmd, sequence() };
        msg.insert(msg.end(), data.begin(), data.end());
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void CommandBase::cmdDataAlt(uint8_t const cmd, RvrMsg const& data, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mAltTarget, mDevice, cmd, sequence() };
        msg.insert(msg.end(), data.begin(), data.end());
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void CommandBase::cmdInt(uint8_t const cmd, uint16_t const data, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mTarget, mDevice, cmd, sequence(), //
                     static_cast<uint8_t>(data >> 8), static_cast<uint8_t>(data & 0xFF) };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void CommandBase::cmdIntAlt(uint8_t const cmd, uint16_t const data, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mAltTarget, mDevice, cmd, sequence(), //
                     static_cast<uint8_t>(data >> 8), static_cast<uint8_t>(data & 0xFF) };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void CommandBase::cmdEnable(uint8_t const cmd, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mTarget, mDevice, cmd, enable, true };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void CommandBase::cmdEnableAlt(uint8_t const cmd, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mAltTarget, mDevice, cmd, enable, true };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void CommandBase::cmdDisable(uint8_t const cmd, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mTarget, mDevice, cmd, disable, false };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void CommandBase::cmdDisableAlt(uint8_t const cmd, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mAltTarget, mDevice, cmd, disable, false };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    CommandBase::TargetPort const CommandBase::altTarget() const {
        return mAltTarget;
    }
    //----------------------------------------------------------------------------------------------------------------------
    CommandBase::TargetPort const CommandBase::target() const {
        return mTarget;
    }
    //----------------------------------------------------------------------------------------------------------------------
    Devices const CommandBase::device() const {
        return mDevice;
    }
    //----------------------------------------------------------------------------------------------------------------------
    Request& CommandBase::request() const {
        return mRequest;
    }
    //----------------------------------------------------------------------------------------------------------------------
    uint8_t CommandBase::seq() {
        return mSeq;
    }

}
