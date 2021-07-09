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
//		 File: Stream.h
//
//     Author: rmerriam
//
//    Created: Jun 27, 2021
//
//======================================================================================================================

namespace rvr {

    class Stream {
    public:
        Stream() {
        }
        virtual ~Stream()= 0;

        Stream(Stream const& other) = delete;
        Stream(Stream&& other) = delete;
        Stream& operator=(Stream const& other) = delete;
        Stream& operator=(Stream&& other) = delete;

        virtual auto read() -> uint8_t const = 0;
        virtual auto write(uint8_t const& ch) -> int64_t const = 0;

//        virtual uint8_t read() const = 0;
//        virtual int64_t write(uint8_t const& ch) const = 0;
    };

} /* namespace rvr */

