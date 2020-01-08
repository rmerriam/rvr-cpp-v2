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
    void SensorsStream::disableStreaming(CommandResponse const want_resp) {
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
    void SensorsStream::clearStreaming(CommandResponse const want_resp) {
        clearStreamingNordic(want_resp);
        clearStreamingBT(want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<AccelData> SensorsStream::accelerometer() {
        auto x { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 3, imu_accel_gyro_token) };
        if (x) {
            auto y { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 4, imu_accel_gyro_token) };
            auto z { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 5, imu_accel_gyro_token) };

            auto [out_min, out_max] { SensorFactors[accel_token] };
            return AccelData { normalize(x.value(), out_min, out_max), //
                               normalize(y.value(), out_min, out_max), //
                               normalize(z.value(), out_min, out_max), //
            };
        }
        return {};
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<float> SensorsStream::ambient() {
        auto value { mBlackboard.uint32Value(mTarget, mDevice, streaming_service_data_notify, 0, ambient_token) };
        if (value) {
            auto [out_min, out_max] { SensorFactors[ambient_token] };
            return normalize(static_cast<int32_t>(value.value()), out_min, out_max);
        }
        return std::nullopt;
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<GyroData> SensorsStream::gyroscope() {
        auto x { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 6, imu_accel_gyro_token) };
        if (x) {

            auto y { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 7, imu_accel_gyro_token) };
            auto z { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 8, imu_accel_gyro_token) };

            auto [out_min, out_max] { SensorFactors[gyro_token] };
            return GyroData { normalize(x.value(), out_min, out_max), //
                              normalize(y.value(), out_min, out_max), //
                              normalize(z.value(), out_min, out_max), //
            };
        }
        return {};
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<ImuData> SensorsStream::imu() {
        auto x { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 0, imu_accel_gyro_token) };

        if (x) {
            auto y { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 1, imu_accel_gyro_token) };
            auto z { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 2, imu_accel_gyro_token) };
            auto [out_min, out_max] { SensorFactors[imu_token] };
            return ImuData { normalize(x.value(), out_min, out_max), //
                             normalize(y.value(), out_min / 2, out_max / 2), //
                             normalize(z.value(), out_min, out_max), //
            };
        }
        return {};
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<QuatData> SensorsStream::quaternion() {
        auto x { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 1, quaternion_token) };

        if (x) {
            auto w { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 0, quaternion_token) };
            auto y { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 2, quaternion_token) };
            auto z { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 3, quaternion_token) };

            auto [out_min, out_max] { SensorFactors[quaternion_token] };
            return QuatData { normalize(w.value(), out_min, out_max), //
                              normalize(x.value(), out_min, out_max), //
                              normalize(y.value(), out_min / 2, out_max / 2), //
                              normalize(z.value(), out_min, out_max), //
            };
        }
        return {};
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<LocatorData> SensorsStream::locator() {
        auto x { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 3, speed_velocity_locator_token) };

        if (x) {
            auto y { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 4, speed_velocity_locator_token) };
            auto [out_min, out_max] { SensorFactors[locator_token] };
            return LocatorData { normalize(x.value(), out_min, out_max), //
                                 normalize(y.value(), out_min, out_max), //
            };
        }
        return {};
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<float> SensorsStream::speed() {
        auto speed { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 0, speed_velocity_locator_token) };
        if (speed) {
            auto [out_min, out_max] { SensorFactors[speed_token] };
            terr << code_loc << static_cast<int32_t>(speed.value());
            return normalize(static_cast<int32_t>(speed.value()), out_min, out_max);
        }
        return {};
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<VelocityData> SensorsStream::velocity() {
        auto x { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 1, speed_velocity_locator_token) };
        if (x) {

            auto y { mBlackboard.uint32Value(mAltTarget, mDevice, streaming_service_data_notify, 2, speed_velocity_locator_token) };

            auto [out_min, out_max] { SensorFactors[velocity_token] };
            return VelocityData { normalize(x.value(), out_min, out_max), //
                                  normalize(y.value(), out_min, out_max), //
            };
        }
        return {};
    }

}
