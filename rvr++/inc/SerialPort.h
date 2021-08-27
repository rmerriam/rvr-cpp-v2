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
//     Author: Rudyard Merriam
//
//     Created: May 29, 2021
//
//======================================================================================================================
//

#include <cstdint>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "Stream.h"

namespace rvr {

//-----------------------------------------------------------------------------
class SerialPort : public Stream {

   public:
   SerialPort(char const* port_name, uint32_t const baud);
   ~SerialPort();

   SerialPort(SerialPort const& other) = delete;
   SerialPort(SerialPort&& other) = delete;
   SerialPort& operator=(SerialPort const& other) = delete;
   SerialPort& operator=(SerialPort&& other) = delete;

   void flush();

   int count() const;

   virtual auto read() -> uint8_t const override;
   int64_t read(uint8_t buffer[], uint32_t const len = 1) const;

   virtual auto write(uint8_t const& ch) -> int64_t const override;
   int64_t write(uint8_t const buffer[], uint32_t const cnt) const;

   //        void write(uint8_t const buffer[]) const {
   //            write(buffer + 1, buffer[0]);
   //        }

   private:
   int mFd;
};
//----------------------------------------------------------------------------------------------------------------------
inline int SerialPort::count() const {
   int bytes_avail;
   ::ioctl(mFd, FIONREAD, &bytes_avail);
   return bytes_avail;
}
} // namespace rvr
