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
//     Created: Nov 10, 2019
//
//======================================================================================================================

#include "Blackboard.h"
#include "SystemInfo.h"
namespace rvr {
    //----------------------------------------------------------------------------------------------------------------------
    std::string SystemInfo::processorName() {
        std::any value { bb::entryValue(Devices::system, get_processor_name) };
        return (value.has_value()) ? std::any_cast<std::string>(value) : ""s;
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::string SystemInfo::sku() {
        std::any value { bb::entryValue(Devices::system, get_sku) };
        return (value.has_value()) ? std::any_cast<std::string>(value) : ""s;
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::string SystemInfo::mainAppVersion() {
        std::any value { bb::entryValue(Devices::system, get_main_application_version) };
        std::string ver;
        if (value.has_value()) {
            MsgArray msg { std::any_cast<MsgArray>(value) };
            ver = std::to_string(((msg[0] << 8) | msg[1])) + '.' + //
                std::to_string(((msg[2] << 8) | msg[3])) + '.' + //
                std::to_string(((msg[4] << 8) | msg[5]));
        }
        return ver;
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::string SystemInfo::bootVersion() {
        std::any value { bb::entryValue(Devices::system, get_bootloader_version) };
        std::string ver;
        if (value.has_value()) {
            MsgArray msg { std::any_cast<MsgArray>(value) };
            ver = std::to_string(((msg[0] << 8) | msg[1])) + '.' + //
                std::to_string(((msg[2] << 8) | msg[3])) + '.' + //
                std::to_string(((msg[4] << 8) | msg[5]));
        }
        return ver;
    }
    //----------------------------------------------------------------------------------------------------------------------
    int64_t SystemInfo::boardVersion() {
        std::any value { bb::entryValue(Devices::system, get_board_revision) };
        return (value.has_value()) ? std::any_cast<int64_t>(value) : 0;
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::string SystemInfo::macAddress() {
        std::any value { bb::entryValue(Devices::system, get_mac_address) };
        if (value.has_value()) {
            const char* colon { ":" };
            std::string mac { std::any_cast<std::string>(value) };
            mac.insert(10, colon);
            mac.insert(8, colon);
            mac.insert(6, colon);
            mac.insert(4, colon);
            mac.insert(2, colon);
            return mac;
        }
        return ""s;
    }
    //----------------------------------------------------------------------------------------------------------------------
    int64_t SystemInfo::statsId() {
        std::any value { bb::entryValue(Devices::system, get_stats_id) };
        return (value.has_value()) ? std::any_cast<int64_t>(value) : 0;
    }

}
