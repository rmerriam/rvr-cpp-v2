#ifndef ENUM_H_
#define ENUM_H_
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
//     Created: Dec 19, 2019
//
//======================================================================================================================
#include <cstdint>
#include "Packet.h"

namespace rvr {
    enum VoltageType : uint8_t {
        CalibratedFiltered = 0, CalibratedUnfiltered = 1, UncalibratedUnfiltered = 2,
    };
    enum ThermalStatus : uint8_t {
        okay, warn, critical
    };

    struct TripleFloat {
        float x;
        float y;
        float z;
    };
    using AccelData = TripleFloat;
    using GyroData = TripleFloat;

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

    struct ImuData {
        float pitch;
        float roll;
        float yaw;
    };

    struct DoubleFloat {
        float x;
        float y;
    };
    using LocatorData = DoubleFloat;
    using VelocityData = DoubleFloat;

    struct QuatData {
        float w;
        float x;
        float y;
        float z;
    };

    struct ThermalProtection {
        float leftMotorTemp;
        uint8_t leftStatus;
        float rightMotorTemp;
        uint8_t rightStatus;
    };

    enum CommandResponse : uint8_t {
        resp_no = no_response, //
        resp_yes = request_response, //
        resp_on_error = request_error_response,
    };

} // namespace rvr

#endif /* ENUM_H_ */
