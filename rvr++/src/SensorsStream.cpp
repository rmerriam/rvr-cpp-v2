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
//     Created: Oct 26, 2019
//
//======================================================================================================================

#include "SensorsStream.h"

namespace rvr {
    //----------------------------------------------------------------------------------------------------------------------
    void SensorsStream::configureStreamingNordic(RvrMsg const& cfg, CommandResponse const want_resp) {
        cmdData(configure_streaming_service, cfg, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void SensorsStream::configureStreamingBT(RvrMsg const& cfg, CommandResponse const want_resp) {
        cmdDataAlt(configure_streaming_service, cfg, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void SensorsStream::configureStreaming(RvrMsg const& cfg, CommandResponse const want_resp) {
        configureStreamingNordic(cfg, want_resp);
        configureStreamingBT(cfg, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void SensorsStream::enableStreamingNordic(uint16_t const millis, CommandResponse const want_resp) {
        cmdInt(start_streaming_service, millis, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void SensorsStream::enableStreamingBT(uint16_t const millis, CommandResponse const want_resp) {
        cmdIntAlt(start_streaming_service, millis, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void SensorsStream::enableStreaming(uint16_t const millis, CommandResponse const want_resp) {
        enableStreamingNordic(millis, want_resp);
        enableStreamingBT(millis, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void SensorsStream::disableStreamingNordic(CommandResponse const want_resp) {
        cmdBasic(stop_streaming_service, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void SensorsStream::disableStreamingBT(CommandResponse const want_resp) {
        cmdBasicAlt(stop_streaming_service, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void SensorsStream::disableStreaming(CommandResponse const want_resp) {
        disableStreamingNordic(want_resp);
        disableStreamingBT(want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void SensorsStream::clearStreamingNordic(CommandResponse const want_resp) {
        cmdBasic(clear_streaming_service, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void SensorsStream::clearStreamingBT(CommandResponse const want_resp) {
        cmdBasicAlt(clear_streaming_service, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void SensorsStream::clearStreaming(CommandResponse const want_resp) {
        clearStreamingNordic(want_resp);
        clearStreamingBT(want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    float SensorsStream::ambient() {
        uint32_t value { bb::uint32Value(mTarget, mDevice, streaming_service_data_notify, ambient_sense) };
        return normalize(value, 0, SensorFactors[ambient_sense].first, SensorFactors[ambient_sense].second);
    }
    //----------------------------------------------------------------------------------------------------------------------
    float SensorsStream::speed() {
        uint32_t value { bb::uint32Value(mAltTarget, mDevice, streaming_service_data_notify, speed_sense) };
        return normalize(value, 0, SensorFactors[speed_sense].first, SensorFactors[speed_sense].second);
    }
//----------------------------------------------------------------------------------------------------------------------
//    auto SensorsStream::velocity() {
//    }
}
