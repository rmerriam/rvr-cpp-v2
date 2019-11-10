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

#include <fcntl.h>
#include <stddef.h>
#include <termios.h>
#include <unistd.h>
#include <cstdint>
#include <cstdio>

#include "SerialPort.h"
//-----------------------------------------------------------------------------
char port_err[] = "        Bad Serial Port Handle";
//-----------------------------------------------------------------------------
SerialPort::SerialPort(const char* port_name, const uint32_t baud) {
    speed_t myBaud;

    switch (baud) {
        case 50:
            myBaud = B50;
            break;
        case 75:
            myBaud = B75;
            break;
        case 110:
            myBaud = B110;
            break;
        case 134:
            myBaud = B134;
            break;
        case 150:
            myBaud = B150;
            break;
        case 200:
            myBaud = B200;
            break;
        case 300:
            myBaud = B300;
            break;
        case 600:
            myBaud = B600;
            break;
        case 1200:
            myBaud = B1200;
            break;
        case 1800:
            myBaud = B1800;
            break;
        case 2400:
            myBaud = B2400;
            break;
        case 4800:
            myBaud = B4800;
            break;
        case 9600:
            myBaud = B9600;
            break;
        case 19200:
            myBaud = B19200;
            break;
        case 38400:
            myBaud = B38400;
            break;
        case 57600:
            myBaud = B57600;
            break;
        case 115200:
            myBaud = B115200;
            break;
        case 230400:
            myBaud = B230400;
            break;

        default:
            myBaud = B9600;
    }

    mFd = open(port_name, O_RDWR | O_NOCTTY);
    if (mFd == -1) {
        perror(port_name);
    }

// Get and modify current options:

    struct termios options;
    tcgetattr(mFd, &options);

    cfsetspeed( &options, myBaud);
    cfmakeraw( &options);

    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = 50;

    if (tcsetattr(mFd, TCSANOW | TCSAFLUSH, &options) < 0) {
        perror("init_serialport: Couldn't set term attributes");
    }
}
//-----------------------------------------------------------------------------
SerialPort::~SerialPort() {
    close(mFd);
}
//-----------------------------------------------------------------------------
void SerialPort::flush() {
    tcflush(mFd, TCIOFLUSH);
}
//-----------------------------------------------------------------------------
uint8_t SerialPort::read() const {
    uint16_t x { 0xDEAD };

    if (::read(mFd, &x, 1) != 1) return -1;

    return ((uint8_t)x) & 0xFF;
}
//-----------------------------------------------------------------------------
int64_t SerialPort::read(uint8_t buffer[], const uint32_t len) const {

    int64_t cnt = ::read(mFd, buffer, len);

    if (cnt < 0) {
        perror("error reading");
    }
    return cnt;
}
//-----------------------------------------------------------------------------
//void SerialPort::write(const uint8_t& ch) const {
//	::write(mFd, &ch, 1);
//}
//-----------------------------------------------------------------------------
int64_t SerialPort::write(const uint8_t buffer[], const uint32_t cnt) const {
    int64_t res = ::write(mFd, buffer, cnt);
    return res;
}
