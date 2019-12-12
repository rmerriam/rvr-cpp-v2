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
#include <limits>
#include "Blackboard.h"
#include "Request.h"
#include "CommandBase.h"

/*
 * For sensor ranges see: ~/devr/nodejs/src/modules/controls/v1.0/sensor-control.ts
 *                      https://sdk.sphero.com/docs/general_documentation/sensors/
 */

namespace rvr {

    class SensorsStream : protected CommandBase {
        using bb = Blackboard;

    public:
        enum Sensor : uint8_t {
            quaternion_token = 0, //
            imu_token = 1, //
            accel_token = 2, //
            color_token = 3, //
            gyro_token = 4, //
            core_time_lower_token = 5, //
            locator_token = 6, //
            velocity_token = 7, //
            speed_token = 8, //
            core_time_upper_token = 9, //
            ambient_token = 10,
        };

        SensorsStream(Request& req) :
            CommandBase { Devices::sensors, req, nordic } {
        }

        SensorsStream(SensorsStream const& other) = delete;
        SensorsStream(SensorsStream&& other) = delete;
        SensorsStream& operator=(SensorsStream const& other) = delete;

        void accelerometerConfig(CommandResponse const want_resp = resp_on_error);
        void ambientConfig(CommandResponse const want_resp = resp_on_error);
        void colorConfig(CommandResponse const want_resp = resp_on_error);
        void coreNordicConfig(CommandResponse const want_resp = resp_on_error);
        void coreBTConfig(CommandResponse const want_resp = resp_on_error);
        void gyroConfig(CommandResponse const want_resp = resp_on_error);
        void imuConfig(CommandResponse const want_resp = resp_on_error);
        void locatorConfig(CommandResponse const want_resp = resp_on_error);
        void quaternionConfig(CommandResponse const want_resp = resp_on_error);
        void speedConfig(CommandResponse const want_resp = resp_on_error);
        void velocityConfig(CommandResponse const want_resp = resp_on_error);

        void configureStreamingNordic(RvrMsg const& cfg, CommandResponse const want_resp = resp_on_error);
        void configureStreamingBT(RvrMsg const& cfg, CommandResponse const want_resp = resp_on_error);
        void configureStreaming(RvrMsg const& cfg, CommandResponse const want_resp = resp_on_error);

        void enableStreaming(uint16_t const millis, CommandResponse const want_resp = resp_on_error);
        void enableStreamingNordic(uint16_t const millis, CommandResponse const want_resp = resp_on_error);
        void enableStreamingBT(uint16_t const millis, CommandResponse const want_resp = resp_on_error);

        void disableStreaming(CommandResponse const want_resp = resp_on_error);
        void disableStreamingNordic(CommandResponse const want_resp = resp_on_error);
        void disableStreamingBT(CommandResponse const want_resp = resp_on_error);

        void clearStreaming(CommandResponse const want_resp = resp_on_error);
        void clearStreamingNordic(CommandResponse const want_resp = resp_on_error);
        void clearStreamingBT(CommandResponse const want_resp = resp_on_error);

        template <TargetPort TP, Sensor ID>
        void streamConfig(CommandResponse const want_resp) {
            if constexpr (TP == nordic) {
                configureStreamingNordic(rvr::RvrMsg { ID, 0x00, ID, 0x02 }, want_resp);
            }
            else {
                configureStreamingBT(rvr::RvrMsg { ID, 0x00, ID, 0x02 }, want_resp);
            }
        }

        void x(CommandResponse const want_resp) {
            streamConfig<rvr::CommandBase::bluetoothSOC, Sensor::accel_token>(want_resp);
        }

        //======================================================================================================================
        // data access methods
        float normalize(uint32_t const value, uint32_t const min, float const out_min, float const out_max);

        float ambient();
        float speed();
        std::tuple<float, float> velocity();
        std::tuple<float, float> locator();

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

        using NormalizeFactor = std::tuple<uint32_t, float, float>;
        std::array<NormalizeFactor, 12> SensorFactors { //
        NormalizeFactor { std::numeric_limits<uint32_t>::min(), -1.0, 1.0 },         // 0 - quat
        NormalizeFactor { std::numeric_limits<uint32_t>::min(), -180.0, 180.0 },     // 1 - imu  -90/+90 roll
        NormalizeFactor { std::numeric_limits<uint32_t>::min(), -16.0, 16.0 },       // 2 - accel
        NormalizeFactor { 0, 0.0, std::numeric_limits<uint8_t>::max() },            // 3 - color
        NormalizeFactor { std::numeric_limits<uint32_t>::min(), -2000.0, 2000.0 },   // 4 - gyro
//
        NormalizeFactor { 0, 0.0, std::numeric_limits<uint32_t>::max() },           // 5 - core lower
        NormalizeFactor { std::numeric_limits<uint32_t>::min(), -16000.0, 16000.0 }, // 6 - locator
        NormalizeFactor { std::numeric_limits<uint32_t>::min(), -5.0, 5.0 },         // 7 - velocity
        NormalizeFactor { 0, 0.0, 5.0 },                                            // 8 - speed
        NormalizeFactor { 0, 0.0, std::numeric_limits<uint32_t>::max() },           // 9 - core upper
//
        NormalizeFactor { 0, 0.0, 1200000.0 },                                      // 10- ambient
        NormalizeFactor { std::numeric_limits<uint32_t>::min(), -1.0, 1.0, },        // 11 - quat
        };
    }
    ;
//----------------------------------------------------------------------------------------------------------------------
    inline void SensorsStream::accelerometerConfig(CommandResponse const want_resp) {
        configureStreamingBT(rvr::RvrMsg { accel_token, 0x00, accel_token, 0x02 }, want_resp);
    }
//----------------------------------------------------------------------------------------------------------------------
    inline void SensorsStream::ambientConfig(CommandResponse const want_resp) {
        configureStreamingNordic(rvr::RvrMsg { 10, 0x00, 10, 0x02 }, want_resp);
    }
//----------------------------------------------------------------------------------------------------------------------
    inline void SensorsStream::colorConfig(CommandResponse const want_resp) {
        configureStreamingNordic(rvr::RvrMsg { 0x03, 0x00, 0x3, 0x02 }, want_resp);
    }
//----------------------------------------------------------------------------------------------------------------------
    inline void SensorsStream::coreNordicConfig(CommandResponse const want_resp) {
        configureStreamingNordic(rvr::RvrMsg { 9, 0x00, 0x09, 0x02 }, want_resp);
    }
//----------------------------------------------------------------------------------------------------------------------
    inline void SensorsStream::coreBTConfig(CommandResponse const want_resp) {
        configureStreamingBT(rvr::RvrMsg { 9, 0x00, 0x09, 0x02 }, want_resp);
    }
//----------------------------------------------------------------------------------------------------------------------
    inline void SensorsStream::gyroConfig(CommandResponse const want_resp) {
        configureStreamingBT(rvr::RvrMsg { 4, 0x00, 0x04, 0x02 }, want_resp);
    }
//----------------------------------------------------------------------------------------------------------------------
    inline void SensorsStream::imuConfig(CommandResponse const want_resp) {
        configureStreamingBT(rvr::RvrMsg { 1, 0x00, 0x01, 0x02 }, want_resp);
    }
//----------------------------------------------------------------------------------------------------------------------
    inline void SensorsStream::locatorConfig(CommandResponse const want_resp) {
        configureStreamingBT(rvr::RvrMsg { locator_token, 0x00, locator_token, 0x02 }, want_resp);
    }
//----------------------------------------------------------------------------------------------------------------------
    inline void SensorsStream::quaternionConfig(CommandResponse const want_resp) {
        configureStreamingBT(rvr::RvrMsg { 11, 0x00, 0x00, 0x02 }, want_resp);
    }
//----------------------------------------------------------------------------------------------------------------------
    inline void SensorsStream::speedConfig(CommandResponse const want_resp) {
        configureStreamingBT(rvr::RvrMsg { speed_token, 0x00, speed_token, 0x02 }, want_resp);
    }
//----------------------------------------------------------------------------------------------------------------------
    inline void SensorsStream::velocityConfig(CommandResponse const want_resp) {
        configureStreamingBT(rvr::RvrMsg { velocity_token, 0x00, velocity_token, 0x02 }, want_resp);
    }
//======================================================================================================================
    inline float SensorsStream::normalize(uint32_t const value, uint32_t const min, float const out_min, float const out_max) {
        auto max { std::numeric_limits<uint32_t>::max() };
//        terr << code_loc << value << mys::sp << min << mys::sp << max << mys::sp << out_min << mys::sp << out_max;

        return (((value - min) / float(max - min)) * (out_max - out_min)) + out_min;
    }

} /* namespace rvr */

#endif
