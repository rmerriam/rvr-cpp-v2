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
//		 File: leds.cpp
//
//     Author: rmerriam
//
//    Created: Jun 10, 2021
//
//======================================================================================================================
#include <future>
#include <thread>

#include <rvr++.h>
//---------------------------------------------------------------------------------------------------------------------
void leds_test(rvr::IoLed& led) {

   uint32_t led32 {
      Led::all_lights |
      //    Led::power_button_front | //
      //    Led::power_button_rear |    //

      //        Led::status_indication_left | //
      //        Led::status_indication_right |    //
      //        Led::brakelights | //
      //        Led::headlights //| //
      //        Led::brakelight_left |    //
      //        Led::brakelight_right |    //
      0 //
   };

   rvr::RvrMsg colors[] {
      //
      {
          0x00, 0x00, 0xFF, //
          0xFF, 0x00, 0x00, //
          0x00, 0x00, 0xFF, //
          0xFF, 0x00, 0x00, //
          0x00, 0x00, 0xFF, //
          0xFF, 0x00, 0x00, //
          0x00, 0x00, 0xFF, //
          0xFF, 0x00, 0x00, //
          0x00, 0x00, 0xFF, //
          0xFF, 0x00, 0x00, //
                            //        0x00, 0x00, //
      },                    //
      {
          //
          0xFF, 0x00, 0x00, //
          0x00, 0x00, 0xFF, //
          0xFF, 0x00, 0x00, //
          0x00, 0x00, 0xFF, //
          0xFF, 0x00, 0x00, //
          0x00, 0x00, 0xFF, //
          0xFF, 0x00, 0x00, //
          0x00, 0x00, 0xFF, //
          0xFF, 0x00, 0x00, //
          0x00, 0x00, 0xFF, //
                            //            0x00, 0x00, //
      }                     //
   };

   mys::tout << code_line;

   //    led.ledsOff(rvr::CommandResponse::resp_yes);
   led.idleLeds(rvr::CommandResponse::resp_yes);

   std::this_thread::sleep_for(1000ms);

   for(auto i { 0 }; i < 10; ++i) {
      led.allLed(led32, colors[i % 2], rvr::CommandResponse::resp_yes);
      mys::tout << code_line << "blink";
      std::this_thread::sleep_for(1000ms);
   }

   led.idleLeds();
   std::this_thread::sleep_for(50ms);
   mys::tout << code_line << mys::nl;
}
