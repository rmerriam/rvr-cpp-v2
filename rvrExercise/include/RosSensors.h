#ifndef ROSSENSORS_H_
#define ROSSENSORS_H_
//======================================================================================================================
// 2020 Copyright Mystic Lake Software
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
//     Created: Jan 7, 2020
//
//======================================================================================================================
#include <optional>
#include "enum.h"

#include "SensorsStream.h"

class RosSensors {
public:
    struct RosQuat {
        float x;
        float y;
        float z;
        float w;
    };

    RosSensors(rvr::SensorsStream& sensors);

    RosSensors(RosSensors const& other) = delete;
    RosSensors(RosSensors&& other) = delete;
    RosSensors& operator=(RosSensors const& other) = delete;

    std::optional<rvr::AccelData> accelerometer();
    std::optional<rvr::GyroData> gyroscope();
    std::optional<rvr::ImuData> imu();
    std::optional<RosQuat> quaternion();
    std::optional<float> speed();
    std::optional<rvr::VelocityData> velocity();
    std::optional<rvr::LocatorData> locator();

private:
    rvr::SensorsStream& mSensors;
};
//----------------------------------------------------------------------------------------------------------------------
inline RosSensors::RosSensors(rvr::SensorsStream& sensors) :
    mSensors(sensors) {
}
//----------------------------------------------------------------------------------------------------------------------
inline std::optional<rvr::AccelData> RosSensors::accelerometer() {
    std::optional<rvr::AccelData> data { mSensors.accelerometer() };

    if (data.has_value()) {
        return data;
    }

    return {};
}
//----------------------------------------------------------------------------------------------------------------------
inline std::optional<rvr::GyroData> RosSensors::gyroscope() {
    std::optional<rvr::GyroData> data { mSensors.gyroscope() };

    if (data.has_value()) {
        return data;
    }

    return {};
}
//----------------------------------------------------------------------------------------------------------------------
inline std::optional<rvr::ImuData> RosSensors::imu() {
    std::optional<rvr::ImuData> data { mSensors.imu() };

    if (data.has_value()) {
        return data;
    }

    return {};
}
//----------------------------------------------------------------------------------------------------------------------
inline std::optional<RosSensors::RosQuat> RosSensors::quaternion() {
    std::optional<rvr::QuatData> data { mSensors.quaternion() };

    if (data.has_value()) {
        return RosSensors::RosQuat { data.value().x, data.value().y, data.value().z, data.value().w };
    }

    return {};
}
//----------------------------------------------------------------------------------------------------------------------
inline std::optional<float> RosSensors::speed() {
    std::optional<float> data { mSensors.speed() };

    if (data.has_value()) {
        return data;
    }

    return {};
}
//----------------------------------------------------------------------------------------------------------------------
inline std::optional<rvr::VelocityData> RosSensors::velocity() {
    std::optional<rvr::VelocityData> data { mSensors.velocity() };

    if (data.has_value()) {
        return data;
    }

    return {};
}
//----------------------------------------------------------------------------------------------------------------------
inline std::optional<rvr::LocatorData> RosSensors::locator() {
    std::optional<rvr::LocatorData> data { mSensors.locator() };

    if (data.has_value()) {
        return data;
    }

    return {};
}
#endif /* ROSSENSORS_H_ */
