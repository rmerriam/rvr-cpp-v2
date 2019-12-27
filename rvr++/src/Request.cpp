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
#include <Request.h>
#include "Blackboard.h"

namespace rvr {
    //----------------------------------------------------------------------------------------------------------------------
    Request::Request(Blackboard& bb, Devices const device, SendPacket& request, TargetPort const target) :
        mBlackboard { bb }, mDevice { device }, mRequest { request }, mTarget { target }, mAltTarget { makeAltProc() } {
    }
    //----------------------------------------------------------------------------------------------------------------------
    uint8_t Request::buildFlags(CommandResponse const want_resp) const {
        int flags { want_resp | activity | has_target };
        return static_cast<uint8_t>(flags);
    }
    //----------------------------------------------------------------------------------------------------------------------
    uint8_t Request::makeAltProc() {
        uint8_t alt_target = (bluetoothSOC + nordic) - mTarget;
        return alt_target;
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Request::basic(uint8_t const cmd, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mTarget, mDevice, cmd, sequence() };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Request::basicAlt(uint8_t const cmd, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mAltTarget, mDevice, cmd, sequence() };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Request::reqByte(uint8_t const cmd, uint8_t const data, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mTarget, mDevice, cmd, sequence(), data };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Request::reqByteAlt(uint8_t const cmd, uint8_t const data, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mAltTarget, mDevice, cmd, sequence(), data };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Request::byteId(uint8_t const cmd, uint8_t const data, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mTarget, mDevice, cmd, data, data };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Request::byteAltId(uint8_t const cmd, uint8_t const data, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mAltTarget, mDevice, cmd, data, data };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Request::cmdData(uint8_t const cmd, RvrMsg const& data, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mTarget, mDevice, cmd, sequence() };
        msg.insert(msg.end(), data.begin(), data.end());
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Request::cmdDataAlt(uint8_t const cmd, RvrMsg const& data, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mAltTarget, mDevice, cmd, sequence() };
        msg.insert(msg.end(), data.begin(), data.end());
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Request::reqInt(uint8_t const cmd, uint16_t const data, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mTarget, mDevice, cmd, sequence(), //
                     static_cast<uint8_t>(data >> 8), static_cast<uint8_t>(data & 0xFF) };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Request::reqIntAlt(uint8_t const cmd, uint16_t const data, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mAltTarget, mDevice, cmd, sequence(), //
                     static_cast<uint8_t>(data >> 8), static_cast<uint8_t>(data & 0xFF) };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Request::cmdEnable(uint8_t const cmd, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mTarget, mDevice, cmd, enable, true };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Request::cmdEnableAlt(uint8_t const cmd, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mAltTarget, mDevice, cmd, enable, true };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Request::cmdDisable(uint8_t const cmd, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mTarget, mDevice, cmd, disable, false };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Request::cmdDisableAlt(uint8_t const cmd, CommandResponse const want_resp) const {
        RvrMsg msg { buildFlags(want_resp), mAltTarget, mDevice, cmd, disable, false };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    TargetPort const Request::altTarget() const {
        return mAltTarget;
    }
    //----------------------------------------------------------------------------------------------------------------------
    TargetPort const Request::target() const {
        return mTarget;
    }
    //----------------------------------------------------------------------------------------------------------------------
    Devices const Request::device() const {
        return mDevice;
    }
    //----------------------------------------------------------------------------------------------------------------------
    SendPacket& Request::request() const {
        return mRequest;
    }
    //----------------------------------------------------------------------------------------------------------------------
    uint8_t Request::seq() {
        return mSeq;
    }

}
