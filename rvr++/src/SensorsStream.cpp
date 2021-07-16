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
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//======================================================================================================================
//
//     Author: rmerriam
//
//     Created: May 29, 2021
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
        Result<AccelData> res;
        auto x { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 3, imu_accel_gyro_token) };
        if (x.valid()) {
            auto y {
                mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 4, imu_accel_gyro_token).get() };
            auto z {
                mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 5, imu_accel_gyro_token).get() };

            auto [out_min, out_max] { SensorFactors[accel_token] };
            res = Result<AccelData> { AccelData {
                normalize(x.get(), out_min, out_max), normalize(y, out_min, out_max), normalize(z, out_min, out_max), } };
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    ResultFloat SensorsStream::ambient() {
        ResultFloat res;
        auto value { mBlackboard.uint32Value(mTarget, mDevice, streaming_service_data_notify, 0, ambient_token) };

        if (value.valid()) {
            auto [out_min, out_max] { SensorFactors[ambient_token] };
            res = ResultFloat { normalize(static_cast<int32_t>(value.get()), out_min, out_max) };
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    rvr::Result<ColorStream> SensorsStream::colors() {
        auto const msg { mBlackboard.msgValue(mTarget, mDevice, streaming_service_data_notify, color_token) };
        Result<ColorStream> res;
        if ( !msg.empty()) {
            ColorStream cs {
                static_cast<uint8_t>(msg[0]), static_cast<uint8_t>(msg[1]), static_cast<uint8_t>(msg[2]),
                static_cast<uint8_t>(msg[3]), static_cast<float>(msg[4]) };
            res = cs;
//            res = Result<ColorStream> { cs };
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    Result<GyroData> SensorsStream::gyroscope() {
        Result<GyroData> res;
        auto x { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 6, imu_accel_gyro_token) };
        if (x.valid()) {

            auto y {
                mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 7, imu_accel_gyro_token).get() };
            auto z {
                mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 8, imu_accel_gyro_token).get() };

            auto [out_min, out_max] { SensorFactors[gyro_token] };
            res = Result<GyroData> { GyroData {
                normalize(x.get(), out_min, out_max), normalize(y, out_min, out_max), normalize(z, out_min, out_max), } };
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    Result<ImuData> SensorsStream::imu() {
        Result<ImuData> res;
        auto x { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 0, imu_accel_gyro_token) };

        if (x.valid()) {
            auto y {
                mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 1, imu_accel_gyro_token).get() };
            auto z {
                mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 2, imu_accel_gyro_token).get() };
            auto [out_min, out_max] { SensorFactors[imu_token] };
            res = Result<ImuData> { ImuData { normalize(x.get(), out_min, out_max), //
            normalize(y, out_min / 2, out_max / 2), //
            normalize(z, out_min, out_max) } };
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    Result<QuatData> SensorsStream::quaternion() {
        Result<QuatData> res;
        auto w { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 0, quaternion_token) };

        if (w.valid()) {
            auto x { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 1, quaternion_token) };
            auto y { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 2, quaternion_token) };
            auto z { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 3, quaternion_token) };

            auto [out_min, out_max] { SensorFactors[quaternion_token] };

            res = Result<QuatData> { QuatData { normalize(w.get(), out_min, out_max), //
            normalize(x.get(), out_min, out_max), //
            normalize(y.get(), out_min / 2, out_max / 2), //
            normalize(z.get(), out_min, out_max) } //
            };
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    Result<LocatorData> SensorsStream::locator() {
        Result<LocatorData> res;
        auto x { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 3, speed_velocity_locator_token) };

        if (x.valid()) {
            auto y { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 4,
                                             speed_velocity_locator_token).get() };
            auto [out_min, out_max] { SensorFactors[locator_token] };
            res = Result<LocatorData> { LocatorData(normalize(x.get(), out_min, out_max), normalize(y, out_min, out_max)) };
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    ResultFloat SensorsStream::speed() {
        ResultFloat res;
        auto speed { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 0,
                                             speed_velocity_locator_token) };
        if (speed.valid()) {
            auto [out_min, out_max] { SensorFactors[speed_token] };
            res = ResultFloat { normalize(static_cast<int32_t>(speed.get()), out_min, out_max) };
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    Result<VelocityData> SensorsStream::velocity() {
        Result<VelocityData> res;
        auto x { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 1, speed_velocity_locator_token) };
        if (x.valid()) {

            auto y { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 2,
                                             speed_velocity_locator_token).get() };

            auto [out_min, out_max] { SensorFactors[velocity_token] };
            res = Result<VelocityData> { VelocityData(normalize(x.get(), out_min, out_max), normalize(y, out_min, out_max)) };
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    ResultUInt64 SensorsStream::btTime() {
        ResultUInt64 res;
        auto lower { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 0, core_time_lower_token) };
        if (lower.valid()) {
            auto upper { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 0,
                                                 core_time_upper_token).get() };
            res = ResultUInt64 { (static_cast<uint64_t>(upper) << 32) + lower.get() };
        }
        return res;
    }
//----------------------------------------------------------------------------------------------------------------------
    ResultUInt64 SensorsStream::nordicTime() {
        ResultUInt64 res;
        auto lower { mBlackboard.uint32Value(mTarget, mDevice, streaming_service_data_notify, 0, core_time_lower_token) };
        if (lower.valid()) {
            auto upper {
                mBlackboard.uint32Value(mTarget, mDevice, streaming_service_data_notify, 0, core_time_upper_token).get() };
            res = ResultUInt64 { (static_cast<uint64_t>(upper) << 32) + lower.get() };
        }
        return res;
    }
}
