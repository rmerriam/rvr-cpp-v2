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
//     Created: Oct 22, 2019
//
//======================================================================================================================
#include "IoLed.h"
namespace rvr {

    void IoLed::allLed(uint32_t const led_bits, MsgArray const& colors, bool const want_resp) {

        MsgArray msg { buildFlags(want_resp), mTarget, mDevice, set_all_leds, mRequest.sequence() };

        MsgArray leds { //
        static_cast<uint8_t>(led_bits >> 24), //
            static_cast<uint8_t>((led_bits >> 16) & 0xFF), //
            static_cast<uint8_t>((led_bits >> 8) & 0xFF), //
            static_cast<uint8_t>(led_bits & 0xFF) };

        msg.insert(msg.end(), leds.begin(), leds.end());
        msg.insert(msg.end(), colors.begin(), colors.end());

        // error 1 - missing "has_target"
        // error 4 - more leds than colors
        // error 7 - more colors than leds

        mRequest.send(msg);
    }

}
