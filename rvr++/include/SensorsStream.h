#ifndef SensorsNordic_H_
#define SensorsNordic_H_
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
#include <Request.h>
#include <SendPacket.h>
#include <limits>
#include "enum.h"
#include "Blackboard.h"
//----------------------------------------------------------------------------------------------------------------------
/*
 * For sensor ranges see: ~/devr/nodejs/src/modules/controls/v1.0/sensor-control.ts
 *                      https://sdk.sphero.com/docs/general_documentation/sensors/
 */
//----------------------------------------------------------------------------------------------------------------------
namespace rvr {
    class Blackboard;

    // creating these outside the class so they are easier out

    class SensorsStream : protected Request {

    public:
        enum Sensor : uint8_t {
            ambient_token = 0x0A, //
            color_token = 0x03, //

            quaternion_id = 0x00, //
            quaternion_token = 0x0B, //
            imu_token = 0x01, //
            accel_token = 0x02, //
            gyro_token = 0x04, //
            locator_token = 0x06, //
            velocity_token = 0x07, //
            speed_token = 0x08, //
            core_time_lower_token = 0x05, //
            core_time_upper_token = 0x09, //

            imu_accel_gyro_token = 0x0C, //
            speed_velocity_locator_token = 0x0D, //
        };

        SensorsStream(Blackboard& bb, SendPacket& req) :
            Request { bb, Devices::sensors, req, nordic } {
        }

        SensorsStream(SensorsStream const& other) = delete;
        SensorsStream(SensorsStream&& other) = delete;
        SensorsStream& operator=(SensorsStream const& other) = delete;

        void streamAmbient(CommandResponse const want_resp = resp_on_error);
        void streamColor(CommandResponse const want_resp = resp_on_error);
        void streamNordicTime(CommandResponse const want_resp = resp_on_error);

        void streamImuAccelGyro(CommandResponse const want_resp = resp_on_error);
        void streamSpeedVelocityLocator(CommandResponse const want_resp = resp_on_error);
        void streamBTTime(CommandResponse const want_resp = resp_on_error);
        void streamQuaternion(CommandResponse const want_resp = resp_on_error);

        void configureStreamingNordic(RvrMsg const& cfg, CommandResponse const want_resp = resp_on_error);
        void configureStreamingBT(RvrMsg const& cfg, CommandResponse const want_resp = resp_on_error);
        void configureStreaming(RvrMsg const& cfg, CommandResponse const want_resp = resp_on_error);

        void enableStreaming(uint16_t const millis, CommandResponse const want_resp = resp_on_error);
        void startStreamingNordic(uint16_t const millis, CommandResponse const want_resp = resp_on_error);
        void startStreamingBT(uint16_t const millis, CommandResponse const want_resp = resp_on_error);

        void disableStreaming(CommandResponse const want_resp = resp_on_error);
        void disableStreamingNordic(CommandResponse const want_resp = resp_on_error);
        void disableStreamingBT(CommandResponse const want_resp = resp_on_error);

        void clearStreaming(CommandResponse const want_resp = resp_on_error);
        void clearStreamingNordic(CommandResponse const want_resp = resp_on_error);
        void clearStreamingBT(CommandResponse const want_resp = resp_on_error);
        //======================================================================================================================
        // data access methods
        float normalize(uint32_t const value, float const out_min, float const out_max);

        std::optional<AccelData> accelerometer();
        std::optional<float> ambient();
        std::optional<GyroData> gyroscope();
        std::optional<ImuData> imu();
        std::optional<QuatData> quaternion();
        std::optional<float> speed();
        std::optional<VelocityData> velocity();
        std::optional<LocatorData> locator();

    private:

        enum ThermalProtectionStatusEnum : uint8_t {
            ok = 0, warn = 1, critical = 2,
        };

        enum StreamingDataSizesEnum : uint8_t {
            eight_bit = 0x00, sixteen_bit = 0x01, thirty_two_bit = 0x02,
        };

        enum struct GyroMaxFlagsBitmask : uint8_t {
            none = 0, max_plus_x = 1, max_minus_x = 2, max_plus_y = 4, max_minus_y = 8, max_plus_z = 16, max_minus_z = 32,
        };

        enum struct LocatorFlagsBitmask : uint8_t {
            none = 0, auto_calibrate = 1,
        };

        enum struct IrLocationMake : uint32_t {
            none = 0, front_left = 0x000000FF, front_right = 0x0000FF00, back_right = 0x00FF0000, back_left = 0xFF000000,
        };

        enum Cmd : uint8_t {
            enable_gyro_max_notify = 0x0F, //
            gyro_max_notify = 0x10,
            //
            reset_locator_x_and_y = 0x13,
            set_locator_flags = 0x17,
            //
            get_bot_to_bot_infrared_readings = 0x22,
            //
            get_rgbc_sensor_values = 0x23,
            //
            start_robot_to_robot_infrared_broadcasting = 0x27,
            start_robot_to_robot_infrared_following = 0x28,
            stop_robot_to_robot_infrared_broadcasting = 0x29,
            robot_to_robot_infrared_message_received_notify = 0x2C,
            get_ambient_light_sensor_value = 0x30,
            stop_robot_to_robot_infrared_following = 0x32,
            start_robot_to_robot_infrared_evading = 0x33,
            stop_robot_to_robot_infrared_evading = 0x34,
            //
            enable_color_detection_notify = 0x35,
            color_detection_notify = 0x36,
            get_current_detected_color_reading = 0x37,
            enable_color_detection = 0x38,
            //
            configure_streaming_service = 0x39,
            start_streaming_service = 0x3A,
            stop_streaming_service = 0x3B,
            clear_streaming_service = 0x3C,
            streaming_service_data_notify = 0x3D,
            //
            enable_robot_infrared_message_notify = 0x3E,
            send_infrared_message = 0x3F,
            //
            get_motor_temperature = 0x4A,
            get_motor_thermal_protection_status = 0x4B,
            enable_motor_thermal_protection_status_notify = 0x4C,
            motor_thermal_protection_status_notify = 0x4D,
        };

        using NormalizeFactor = std::tuple<float, float>;
        std::array<NormalizeFactor, 12> SensorFactors { //
        NormalizeFactor { -1.0, 1.0 },         // 0 - quat
        NormalizeFactor { -180.0, 180.0 },     // 1 - imu  -90/+90 roll
        NormalizeFactor { -16.0, 16.0 },       // 2 - accel
        NormalizeFactor { 0.0, std::numeric_limits<uint8_t>::max() },            // 3 - color
        NormalizeFactor { -2000.0, 2000.0 },   // 4 - gyro
//
        NormalizeFactor { 0.0, std::numeric_limits<uint32_t>::max() },           // 5 - core lower
        NormalizeFactor { -16000.0, 16000.0 }, // 6 - locator
        NormalizeFactor { -5.0, 5.0 },         // 7 - velocity
        NormalizeFactor { 0.0, 5.0 },                                            // 8 - speed
        NormalizeFactor { 0.0, std::numeric_limits<uint32_t>::max() },           // 9 - core upper
//
        NormalizeFactor { 0.0, 120000.0 },                                      // 10- ambient
        NormalizeFactor { -1.0, 1.0, },        // 11 - quat
        };

        template <TargetPort TP, Sensor ID>
        void streamConfig(CommandResponse const want_resp) {
            if constexpr (TP == nordic) {
                configureStreamingNordic(rvr::RvrMsg { ID, 0x00, ID, 0x02 }, want_resp);
            }
            else {
                configureStreamingBT(rvr::RvrMsg { ID, 0x00, ID, 0x02 }, want_resp);
            }
        }
    };
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsStream::streamImuAccelGyro(CommandResponse const want_resp) {
        configureStreamingBT(rvr::RvrMsg { imu_accel_gyro_token, //
        0x00, imu_token, 0x02, //
        0x00, accel_token, 0x02, //
        0x00, gyro_token, 0x02, //
            }, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsStream::streamSpeedVelocityLocator(CommandResponse const want_resp) {
        configureStreamingBT(rvr::RvrMsg { speed_velocity_locator_token, //
        0x00, speed_token, 0x02, //
        0x00, velocity_token, 0x02, //
        0x00, locator_token, 0x02, //
            }, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsStream::streamAmbient(CommandResponse const want_resp) {
        configureStreamingNordic(rvr::RvrMsg { ambient_token, 0x00, ambient_token, 0x02 }, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsStream::streamColor(CommandResponse const want_resp) {
        configureStreamingNordic(rvr::RvrMsg { color_token, 0x00, color_token, 0x02 }, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsStream::streamNordicTime(CommandResponse const want_resp) {
        configureStreamingNordic(rvr::RvrMsg { core_time_upper_token, 0x00, core_time_upper_token, 0x02 }, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SensorsStream::streamBTTime(CommandResponse const want_resp) {
        configureStreamingBT(rvr::RvrMsg { core_time_upper_token, 0x00, core_time_upper_token, 0x02 }, want_resp);
    }
//----------------------------------------------------------------------------------------------------------------------
    inline void SensorsStream::streamQuaternion(CommandResponse const want_resp) {
        configureStreamingBT(rvr::RvrMsg { quaternion_token, 0x00, quaternion_id, 0x02 }, want_resp);
    }
//======================================================================================================================
    inline float SensorsStream::normalize(uint32_t const value, float const out_min, float const out_max) {
        static auto max { std::numeric_limits<uint32_t>::max() };
        float res { };
        if (value != 0) res = (value / static_cast<float>(max) * (out_max - out_min)) + out_min;
        return res;
    }

} /* namespace rvr */

#endif
