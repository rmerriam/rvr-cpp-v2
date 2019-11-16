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
        SystemInfo(const SystemInfo& other) = delete;
        SystemInfo(SystemInfo&& other) = delete;
        SystemInfo& operator=(const SystemInfo& other) = delete;

        //----------------------------------------------------------------------------------------------------------------------
        inline void getMainAppVersion(const CommandResponse want_resp = resp_on_error) {
            cmd_basic(get_main_application_version, want_resp);
            cmd_basic_alt(get_main_application_version, want_resp);
        }
        //----------------------------------------------------------------------------------------------------------------------
        inline void getBootloaderVersion(const CommandResponse want_resp = resp_on_error) {
            cmd_basic(get_bootloader_version, want_resp);
            cmd_basic_alt(get_bootloader_version, want_resp);
        }
        //----------------------------------------------------------------------------------------------------------------------
        inline void getBoardRevision(const CommandResponse want_resp = resp_on_error) {
            cmd_basic_alt(get_board_revision, want_resp);
        }
        //----------------------------------------------------------------------------------------------------------------------
        inline void getMacId(const CommandResponse want_resp = resp_on_error) {
            cmd_basic_alt(get_mac_address, want_resp);
        }
        //----------------------------------------------------------------------------------------------------------------------
        inline void getStatsId(const CommandResponse want_resp = resp_on_error) {
            cmd_basic_alt(get_stats_id, want_resp);
        }
        //----------------------------------------------------------------------------------------------------------------------
        inline void getProcessorName(const CommandResponse want_resp = resp_on_error) {
            cmd_basic(get_processor_name, want_resp);
            cmd_basic_alt(get_processor_name, want_resp);
        }
        //----------------------------------------------------------------------------------------------------------------------
        inline void getUpTime(const CommandResponse want_resp = resp_on_error) {
            cmd_basic(get_core_up_time_in_milliseconds, want_resp);
        }
        //----------------------------------------------------------------------------------------------------------------------
        inline void getSku(const CommandResponse want_resp = resp_on_error) {
            cmd_basic_alt(get_sku, want_resp);
        }

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
