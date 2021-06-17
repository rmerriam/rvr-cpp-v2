#pragma once
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
//		 File: Magnetometer.h
//
//     Author: rmerriam
//
//    Created: Jun 7, 2021
//
//======================================================================================================================

#include <SensorBase.hx>

namespace rvr {

    class Magnetometer : public SensorBase {
    public:
        Magnetometer(Blackboard& bb, SendPacket& req) :
            SensorBase(bb, req) {
        }
        ~Magnetometer() = delete;
        Magnetometer(Magnetometer const& other) = delete;
        Magnetometer(Magnetometer&& other) = delete;
        Magnetometer& operator=(Magnetometer const& other) = delete;
        Magnetometer& operator=(Magnetometer&& other) = delete;

    };

} /* namespace rvr */

