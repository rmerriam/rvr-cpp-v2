//======================================================================================================================
// 2021 Copyright Mystic Lake Software
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
// along with this program.  If not, see <http:             //www.gnu.org/licenses/>.
//======================================================================================================================
//
//     Author: rmerriam
//
//     Created: May 29, 2021
//
//======================================================================================================================
#include <PayloadDecode.h>
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
    void SensorsStream::startStreamingNordic(uint16_t const millis, CommandResponse const want_resp) {
        reqInt(start_streaming_service, millis, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void SensorsStream::startStreamingBT(uint16_t const millis, CommandResponse const want_resp) {
        reqIntAlt(start_streaming_service, millis, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void SensorsStream::enableStreaming(uint16_t const millis, CommandResponse const want_resp) {
        startStreamingNordic(millis, want_resp);
        startStreamingBT(millis, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void SensorsStream::disableStreamingNordic(CommandResponse const want_resp) {
        basic(stop_streaming_service, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void SensorsStream::disableStreamingBT(CommandResponse const want_resp) {
        basicAlt(stop_streaming_service, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void SensorsStream::disableAllStreaming(CommandResponse const want_resp) {
        disableStreamingNordic(want_resp);
        disableStreamingBT(want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void SensorsStream::clearStreamingNordic(CommandResponse const want_resp) {
        basic(clear_streaming_service, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void SensorsStream::clearStreamingBT(CommandResponse const want_resp) {
        basicAlt(clear_streaming_service, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void SensorsStream::clearAllStreaming(CommandResponse const want_resp) {
        clearStreamingNordic(want_resp);
        clearStreamingBT(want_resp);
    }
    //======================================================================================================================
    Result<AccelData> SensorsStream::accelerometer() {
        auto const msg { mBlackboard.entryValue(mAltTarget, mDevice, streaming_service_data_notify, imu_accel_gyro_token) };
        Result<AccelData> res;

        if ( !msg.empty()) {
            auto [out_min, out_max] { SensorFactors[accel_token] };

            PayloadDecode<uint32_t, uint32_t, uint32_t> payload(msg.substr(3 * sizeof(uint32_t)));
            res = GyroData { //
            normalize(payload.get<0>(), out_min, out_max), //
            normalize(payload.get<1>(), out_min, out_max), //
            normalize(payload.get<2>(), out_min, out_max), //
            };
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    ResultFloat SensorsStream::ambient() {
        auto const msg { mBlackboard.entryValue(mTarget, mDevice, streaming_service_data_notify, ambient_token) };
        ResultFloat res;

        if ( !msg.empty()) {
            auto [out_min, out_max] { SensorFactors[ambient_token] };
            res = normalize(decode_type<uint32_t>(msg), out_min, out_max);
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    rvr::Result<ColorStream> SensorsStream::colors() {
        auto const msg { mBlackboard.entryValue(mTarget, mDevice, streaming_service_data_notify, color_token) };
        Result<ColorStream> res;

        if ( !msg.empty()) {
            ColorStream cs { //
            msg[0], //
                msg[1], //
                msg[2], //
                msg[3], //
                normalize(msg[4], 0.0, 1.0) //
            };
            res = cs;
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    Result<GyroData> SensorsStream::gyroscope() {
        auto const msg { mBlackboard.entryValue(mAltTarget, mDevice, streaming_service_data_notify, imu_accel_gyro_token) };
        Result<GyroData> res;

        if ( !msg.empty()) {
            auto [out_min, out_max] { SensorFactors[gyro_token] };

            PayloadDecode<uint32_t, uint32_t, uint32_t> payload(msg.substr(6 * sizeof(uint32_t)));
            res = GyroData { //
            normalize(payload.get<0>(), out_min, out_max), //
            normalize(payload.get<1>(), out_min, out_max), //
            normalize(payload.get<2>(), out_min, out_max), //
            };
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    Result<ImuData> SensorsStream::imu() {
        auto const msg { mBlackboard.entryValue(mAltTarget, mDevice, streaming_service_data_notify, imu_accel_gyro_token) };
        Result<ImuData> res;

        if ( !msg.empty()) {
            auto [out_min, out_max] { SensorFactors[imu_token] };

            PayloadDecode<uint32_t, uint32_t, uint32_t> payload(msg);

            res = ImuData { //
            normalize(payload.get<0>(), out_min, out_max), //
            normalize(payload.get<1>(), out_min / 2, out_max / 2), //
            normalize(payload.get<2>(), out_min, out_max), //
            };
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    Result<QuatData> SensorsStream::quaternion() {
        auto const msg { mBlackboard.entryValue(mAltTarget, mDevice, streaming_service_data_notify, quaternion_token) };
        Result<QuatData> res;

        if ( !msg.empty()) {
            auto [out_min, out_max] { SensorFactors[quaternion_token] };
            PayloadDecode<uint32_t, uint32_t, uint32_t, uint32_t> payload(msg);

            res = QuatData { //
            normalize(payload.get<0>(), out_min, out_max), //
            normalize(payload.get<1>(), out_min, out_max), //
            normalize(payload.get<2>(), out_min, out_max), //
            normalize(payload.get<3>(), out_min, out_max) //
            };
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    Result<LocatorData> SensorsStream::locator() {
        auto const msg { mBlackboard.entryValue(mAltTarget, mDevice, streaming_service_data_notify,
                                                velocity_locator_speed_token) };
        Result<LocatorData> res;

        if ( !msg.empty()) {
            auto [out_min, out_max] { SensorFactors[velocity_token] };

            PayloadDecode<uint32_t, uint32_t> payload(msg.substr(2 * sizeof(uint32_t)));
            res = LocatorData { //
            normalize(payload.get<0>(), out_min, out_max), //
            normalize(payload.get<1>(), out_min, out_max), //
            };
        }
        return res;

    }
    //----------------------------------------------------------------------------------------------------------------------
    Result<EncodersData> SensorsStream::encoders() {
        auto const msg { mBlackboard.entryValue(mAltTarget, mDevice, streaming_service_data_notify, encoders_stream_token) };
        Result<EncodersData> res;

        if ( !msg.empty()) {
            auto [out_min, out_max] { SensorFactors[encoders_token] };

            PayloadDecode<uint32_t, uint32_t> payload(msg);
            res = EncodersData { //
            static_cast<uint32_t>(normalize(payload.get<0>(), out_min, out_max)), //
                static_cast<uint32_t>(normalize(payload.get<1>(), out_min, out_max)), //
            };
        }
        return res;

    }
//----------------------------------------------------------------------------------------------------------------------
    ResultFloat SensorsStream::speed() {
        auto speed { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 0,
                                             velocity_locator_speed_token) };
        ResultFloat res;

        if (speed.valid()) {
            auto [out_min, out_max] { SensorFactors[speed_token] };
            res = ResultFloat { normalize(static_cast<int32_t>(speed.get_or()), out_min, out_max) };
        }
        return res;
    }
//----------------------------------------------------------------------------------------------------------------------
    Result<VelocityData> SensorsStream::velocity() {
        auto const msg { mBlackboard.entryValue(mAltTarget, mDevice, streaming_service_data_notify,
                                                velocity_locator_speed_token) };
        Result<VelocityData> res;

        if ( !msg.empty()) {
            auto [out_min, out_max] { SensorFactors[velocity_token] };

            PayloadDecode<uint32_t, uint32_t> payload(msg.substr(0 * sizeof(uint32_t)));
            res = VelocityData { //
            normalize(payload.get<0>(), out_min, out_max), //
            normalize(payload.get<1>(), out_min, out_max), //
            };
        }
        return res;
    }
//----------------------------------------------------------------------------------------------------------------------
    ResultUInt64 SensorsStream::btTime() {
        auto const msg { mBlackboard.entryValue(mAltTarget, mDevice, streaming_service_data_notify, core_time_upper_token) };
        ResultUInt64 res;

        if ( !msg.empty()) {
            auto const msg2 { mBlackboard.entryValue(mTarget, mDevice, streaming_service_data_notify, core_time_lower_token) };
            res = (static_cast<uint64_t>(decode_type<uint32_t>(msg)) << 32) + decode_type<uint32_t>(msg2);
        }
        return res;
    }
//----------------------------------------------------------------------------------------------------------------------
    ResultUInt64 SensorsStream::nordicTime() {
        auto const msg { mBlackboard.entryValue(mTarget, mDevice, streaming_service_data_notify, core_time_upper_token) };
        ResultUInt64 res;

        if ( !msg.empty()) {
            auto const msg2 { mBlackboard.entryValue(mTarget, mDevice, streaming_service_data_notify, core_time_lower_token) };
            res = (static_cast<uint64_t>(decode_type<uint32_t>(msg)) << 32) + decode_type<uint32_t>(msg2);
        }
        return res;
    }
}
