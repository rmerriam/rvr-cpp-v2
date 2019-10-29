#ifndef IoLed_H_
#define IoLed_H_
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
#include "Request.h"
#include "CommandBase.h"

namespace rvr {

    class IoLed : protected CommandBase {

    public:

        enum LedMask : uint32_t {
            right_headlight_red = 0x00000001,   //
            right_headlight_green = 0x00000002,   //
            right_headlight_blue = 0x00000004,   //
            left_headlight_red = 0x00000008,   //
            left_headlight_green = 0x00000010,   //
            left_headlight_blue = 0x00000020,   //
            left_status_indication_red = 0x00000040,   //
            left_status_indication_green = 0x00000080,   //
            left_status_indication_blue = 0x00000100,   //
            right_status_indication_red = 0x00000200,   //
            right_status_indication_green = 0x00000400,   //
            right_status_indication_blue = 0x00000800,   //
            battery_door_rear_red = 0x00001000,   //
            battery_door_rear_green = 0x00002000,   //
            battery_door_rear_blue = 0x00004000,   //
            battery_door_front_red = 0x00008000,   //
            battery_door_front_green = 0x00010000,   //
            battery_door_front_blue = 0x00020000,   //
            power_button_front_red = 0x00040000,   //
            power_button_front_green = 0x00080000,   //
            power_button_front_blue = 0x00100000,   //
            power_button_rear_red = 0x00200000,   //
            power_button_rear_green = 0x00400000,   //
            power_button_rear_blue = 0x00800000,   //
            left_brakelight_red = 0x01000000,   //
            left_brakelight_green = 0x02000000,   //
            left_brakelight_blue = 0x04000000,   //
            right_brakelight_red = 0x08000000,   //
            right_brakelight_green = 0x10000000,   //
            right_brakelight_blue = 0x20000000,   //
            undercarriage_white = 0x40000000,   //
        };

        IoLed(Request& req) :
            CommandBase { Devices::io_led, req } {
        }

        IoLed(IoLed const& other) = delete;
        IoLed(IoLed&& other) = delete;
        IoLed& operator=(IoLed const& other) = delete;

        void allLed(bool const get_response = false) { // uint32_t const& led_group, LedArray const& right, bool const get_response = false) {

            // uniform initialization using {} causes a narrowing warning
            uint8_t flags = (get_response ? Request::request_response : 0) | Request::has_target;

            MsgArray msg { flags, serial, mDevice, set_all_leds, mRequest.sequence() };

            uint32_t led32 {    //
            right_brakelight_blue | right_brakelight_red | right_brakelight_green | //
                power_button_rear_blue | power_button_rear_red | power_button_rear_green | //
//                undercarriage_white | //
                0 };

            MsgArray leds { led32 >> 24, (led32 >> 16) & 0xFF, (led32 >> 8) & 0xFF, (led32 & 0xFF) };

//            MsgArray colors { 0xFF, 0xFF, 0xFF, };
            MsgArray colors { 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, //
//                0xff, 0xff, 0xff, 0xff, //
            };

            msg.insert(msg.end(), leds.begin(), leds.end());
            msg.insert(msg.end(), colors.begin(), colors.end());

            // error 4 - more leds than colors
            //
            // error 7 - more colors than leds

            mRequest.send(msg);
        }

        //----------------------------------------------------------------------------------------------------------------------
        inline void getColorPalette() {
            do_request(get_active_color_palette, true);
        }
        //----------------------------------------------------------------------------------------------------------------------
        inline void getColorId() {
            do_request(get_color_identification_report, true);
        }
//-
    private:

        enum Cmd : uint8_t {
            set_all_leds = 0x1A, //
            get_active_color_palette = 0x44, //
            set_active_color_palette = 0x45, //
            get_color_identification_report = 0x46, //
            load_color_palette = 0x47, //
            save_color_palette = 0x48, //
            release_led_requests = 0x4E, //
        };
    }
    ;

} /* namespace rvr */

#endif
