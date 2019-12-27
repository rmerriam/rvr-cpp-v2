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
#include <Request.h>
#include <SendPacket.h>
//----------------------------------------------------------------------------------------------------------------------
namespace rvr {
    class IoLed;
}
using Led = rvr::IoLed;
// defined to make it easier to access LedMask enum, i.e. Led::right_headlight_red

namespace rvr {
    class Blackboard;

    class IoLed : protected Request {

    public:
        //----------------------------------------------------------------------------------------------------------------------
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
        //----------------------------------------------------------------------------------------------------------------------
        enum LedGroup : uint32_t {
            status_indication_left = left_status_indication_red |    //
                left_status_indication_green |    //
                left_status_indication_blue,

            status_indication_right = right_status_indication_red |   //
                right_status_indication_green |   //
                right_status_indication_blue,

            headlight_left = left_headlight_red |   //
                left_headlight_green |   //
                left_headlight_blue,

            headlight_right = right_headlight_red |   //
                right_headlight_green |   //
                right_headlight_blue,

            battery_door_front = battery_door_front_red |   //
                battery_door_front_green |   //
                battery_door_front_blue,

            battery_door_rear = battery_door_rear_red |   //
                battery_door_rear_green |   //
                battery_door_rear_blue,

            power_button_front = power_button_front_red |   //
                power_button_front_green |   //
                power_button_front_blue,

            power_button_rear = power_button_rear_red |   //
                power_button_rear_green |   //
                power_button_rear_blue,

            brakelight_left = left_brakelight_red |   //
                left_brakelight_green |   //
                left_brakelight_blue,

            brakelight_right = right_brakelight_red |   //
                right_brakelight_green |   //
                right_brakelight_blue,

            all_lights = status_indication_left |   //
                status_indication_right |   //
                headlight_left |   //
                headlight_right |   //
                battery_door_front |   //
                battery_door_rear |   //
                power_button_front |   //
                power_button_rear |   //
                brakelight_left |   //
                brakelight_right,

            undercarriage = undercarriage_white
        };
        //----------------------------------------------------------------------------------------------------------------------
        IoLed(Blackboard& bb, SendPacket& req) :
            Request { bb, Devices::io_led, req, nordic } {
        }

        IoLed(IoLed const& other) = delete;
        IoLed(IoLed&& other) = delete;
        IoLed& operator=(IoLed const& other) = delete;

        void allLed(uint32_t const led_bits, RvrMsg const& colors, CommandResponse const want_resp = resp_on_error) const;

        void getActiveColorPalette(CommandResponse const want_resp = resp_on_error) const;
        void getColorId(CommandResponse const want_resp = resp_on_error) const;
        void idleLeds(CommandResponse const want_resp = resp_on_error) const;
        void ledsOff(CommandResponse const want_resp = resp_on_error) const;

        enum Cmd : uint8_t {
            set_all_leds = 0x1A, //
            get_active_color_palette = 0x44, //
            set_active_color_palette = 0x45, //
            get_color_identification_report = 0x46, //
            load_color_palette = 0x47, //
            save_color_palette = 0x48, //
            release_led_requests = 0x4E, //
        };
    };
    //---------------------------------------------------------------------------------------------------------------------
    inline void IoLed::idleLeds(CommandResponse const want_resp) const {
        basic(release_led_requests, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void IoLed::getActiveColorPalette(CommandResponse const want_resp) const {
        basic(get_active_color_palette, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void IoLed::getColorId(CommandResponse const want_resp) const {
        basic(get_color_identification_report, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void IoLed::ledsOff(CommandResponse const want_resp) const {
        static rvr::RvrMsg off = { 0x00, 0x00, 0x00, //
                                   0x00, 0x00, 0x00, //
                                   0x00, 0x00, 0x00, //
                                   0x00, 0x00, 0x00, //
                                   0x00, 0x00, 0x00, //
                                   0x00, 0x00, 0x00, //
                                   0x00, 0x00, 0x00, //
                                   0x00, 0x00, 0x00, //
                                   0x00, 0x00, 0x00, //
                                   0x00, 0x00, 0x00, //
        };
        allLed(all_lights, off, want_resp);
    }

} /* namespace rvr */

#endif
