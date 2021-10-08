#ifndef ENUM_H_
#define ENUM_H_
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
#include "Packet.h"
#include <cstdint>

namespace rvr {
// general structs used for data return
    struct TripleFloat {
        float x;
        float y;
        float z;
    };
    struct DoubleFloat {
        float x;
        float y;
    };

    using AccelData = TripleFloat;
    using GyroData = TripleFloat;
    using LocatorData = DoubleFloat;
    using MagnetometerData = TripleFloat;
    using TemperatureData = DoubleFloat;
    using VelocityData = DoubleFloat;
    using VoltageThresholds = TripleFloat;

    struct ImuData {
        float pitch;
        float roll;
        float yaw;
    };

    struct QuatData {
        float w;
        float x;
        float y;
        float z;
    };

    struct EncodersData {
        uint32_t left;
        uint32_t right;
    };

    struct CoreTime {
        uint32_t upper;
        uint32_t lower;
    };

    struct ColorData {
        uint16_t red;
        uint16_t green;
        uint16_t blue;
        uint16_t clear;
    };

    struct ColorDetection {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        uint8_t confidence;
        uint8_t classification;
    };

    struct ColorStream {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        uint8_t index;
        float confidence;
    };

    enum struct VoltageType : uint8_t {
        CalibratedFiltered = 0,   //
        CalibratedUnfiltered = 1, //
        UncalibratedUnfiltered = 2,
    };
    enum struct ThermalStatus : uint8_t {
        okay, //
        warn, //
        critical
    };

    struct ThermalProtection {
        float leftMotorTemp;
        ThermalStatus leftStatus;
        float rightMotorTemp;
        ThermalStatus rightStatus;
    };

    enum struct CommandResponse : uint8_t {
        resp_no = no_response,       //
        resp_yes = request_response, //
        resp_on_error = request_error_response | request_response,
    };

    enum MotorIndexes : uint8_t {
        left_motor_index = 0, //
        right_motor_index = 1
    };

    enum struct TemperatureIndexes : uint8_t {
        left_motor_temperature = 4, //
        right_motor_temperature = 5, //
        nordic_die_temperature = 8
    };

    enum struct FloatSize : uint8_t {
        one_byte = 0, two_byte = 2, four_byte = 4
    };

} // namespace rvr

#endif /* ENUM_H_ */
