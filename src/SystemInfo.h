#ifndef SYSTEMINFO_H_
#define SYSTEMINFO_H_
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
//     Created: Oct 29, 2019
//
//======================================================================================================================
#include "Request.h"
#include "CommandBase.h"

namespace rvr {

    class SystemInfo : protected CommandBase {
    public:
        SystemInfo(Request& req) :
            CommandBase { Devices::system, req, microcontroller } {
        }
        SystemInfo(SystemInfo const& other) = delete;
        SystemInfo(SystemInfo&& other) = delete;
        SystemInfo& operator=(SystemInfo const& other) = delete;

        //----------------------------------------------------------------------------------------------------------------------
        inline void getMainAppVersion() {
            mTarget = bluetoothSOC;
            do_request(get_main_application_version, true);
            mTarget = microcontroller;
            do_request(get_main_application_version, true);
        }
        //----------------------------------------------------------------------------------------------------------------------
        inline void getBootloaderVersion() {
            do_request(get_bootloader_version, true);
        }
        //----------------------------------------------------------------------------------------------------------------------
        inline void getBoardRevision() {
            mTarget = bluetoothSOC;
            do_request(get_board_revision, true);
            mTarget = microcontroller;
//            do_request(get_board_revision, true);
        }
        //----------------------------------------------------------------------------------------------------------------------
        inline void getMacId() {
            mTarget = bluetoothSOC;
            do_request(get_mac_address, true);
            mTarget = microcontroller;
        }
        //----------------------------------------------------------------------------------------------------------------------
        inline void getStatsId() {
            mTarget = bluetoothSOC;
            do_request(get_stats_id, true);
            mTarget = microcontroller;
//            do_request(get_stats_id, true);
        }
        //----------------------------------------------------------------------------------------------------------------------
        inline void getProcessorName() {
            mTarget = bluetoothSOC;
            do_request(get_processor_name, true);
            mTarget = microcontroller;
            do_request(get_processor_name, true);
        }
        //----------------------------------------------------------------------------------------------------------------------
        inline void getUpTime() {
            do_request(get_core_up_time_in_milliseconds, true);
        }
        //----------------------------------------------------------------------------------------------------------------------
        inline void getSku() {
            mTarget = bluetoothSOC;
            do_request(get_sku, true);
            mTarget = microcontroller;
//            do_request(get_sku, true);
        }

        static void decode_cmd(uint8_t const cmd, rvr::MsgArray const packet);

    private:
        enum Cmd : uint8_t {
            get_main_application_version = 0x00, //
            get_bootloader_version = 0x01, //
            get_board_revision = 0x03, //
            get_mac_address = 0x06, //
            get_stats_id = 0x13, //
            get_processor_name = 0x1F, //
            get_sku = 0x38, //
            get_core_up_time_in_milliseconds = 0x39, //
        };

    };
} /* namespace rvr */

#endif /* SYSTEMINFO_H_ */
