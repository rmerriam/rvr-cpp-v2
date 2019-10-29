#ifndef SerialPort_Include
#define SerialPort_Include
//======================================================================================================================
// 2010 Copyright Mystic Lake Software
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
//      $Id: $
//
//======================================================================================================================
//

#include <cstdint>
using namespace std;
//-----------------------------------------------------------------------------
class SerialPort {

public:
    SerialPort(const char* port_name, const uint32_t baud);
    ~SerialPort();

    void flush();

    uint8_t read() const;;
    int64_t read(uint8_t buffer[], const uint32_t len = 1) const;

    int64_t write(const uint8_t buffer[], const uint32_t cnt) const;
    void write(const uint8_t& ch) const;
    void write(const uint8_t buffer[]) const { write(buffer+1, buffer[0]); }

private:
    int mFd;

};
#endif
