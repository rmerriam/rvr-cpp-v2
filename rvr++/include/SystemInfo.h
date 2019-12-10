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
//#include "Blackboard.h"
#include "Request.h"
#include "CommandBase.h"

using namespace std::literals;

namespace rvr {

    class SystemInfo : protected CommandBase {
    public:
        SystemInfo(Request& req) :
            CommandBase { Devices::system, req, bluetoothSOC } {
        }
        SystemInfo(SystemInfo const& other) = delete;
        SystemInfo(SystemInfo&& other) = delete;
        SystemInfo& operator=(SystemInfo const& other) = delete;

        void getMainAppVersion(CommandResponse const want_resp = resp_yes);
        void getBootloaderVersion(CommandResponse const want_resp = resp_yes);
        void getBoardRevision(CommandResponse const want_resp = resp_yes);
        void getMacId(CommandResponse const want_resp = resp_yes);
        void getStatsId(CommandResponse const want_resp = resp_yes);
        void getProcessorName(CommandResponse const want_resp = resp_yes);
        void getSku(CommandResponse const want_resp = resp_yes);
        void getCoreUpTime(CommandResponse const want_resp = resp_yes);

        int64_t boardVersion();
        std::string bootVersion();
        std::string bootVersion2();
        std::string macAddress();
        std::string mainAppVersion();
        std::string mainAppVersion2();
        std::string processorName();
        std::string processorName2();
        int16_t statsId();
        std::string sku();
        int64_t coreUpTime();

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

        std::string versionValue(rvr::CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd);
    };
    //----------------------------------------------------------------------------------------------------------------------
    inline void SystemInfo::getMainAppVersion(CommandResponse const want_resp) {
        cmdBasic(get_main_application_version, want_resp);
        cmdBasicAlt(get_main_application_version, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SystemInfo::getBootloaderVersion(CommandResponse const want_resp) {
        cmdBasic(get_bootloader_version, want_resp);
        cmdBasicAlt(get_bootloader_version, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SystemInfo::getBoardRevision(CommandResponse const want_resp) {
        cmdBasicAlt(get_board_revision, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SystemInfo::getMacId(CommandResponse const want_resp) {
        cmdBasicAlt(get_mac_address, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SystemInfo::getStatsId(CommandResponse const want_resp) {
        cmdBasicAlt(get_stats_id, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SystemInfo::getProcessorName(CommandResponse const want_resp) {
        cmdBasic(get_processor_name, want_resp);
        cmdBasicAlt(get_processor_name, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SystemInfo::getCoreUpTime(CommandResponse const want_resp) {
        cmdBasic(get_core_up_time_in_milliseconds, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void SystemInfo::getSku(CommandResponse const want_resp) {
        cmdBasicAlt(get_sku, want_resp);
    }

} /* namespace rvr */

#endif /* SYSTEMINFO_H_ */
