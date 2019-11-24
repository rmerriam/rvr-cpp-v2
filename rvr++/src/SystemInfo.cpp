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
    std::string SystemInfo::versionValue(const uint8_t cmd, const TargetPort target, const Devices dev) {
        std::any value { bb::entryValue(target, mDevice, get_main_application_version) };
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
    std::string SystemInfo::processorName() {
        return bb::stringValue(get_processor_name, mTarget, mDevice);
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::string SystemInfo::processorName2() {
        return bb::stringValue(get_processor_name, mAltTarget, mDevice);
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::string SystemInfo::sku() {
        return bb::stringValue(get_sku, mAltTarget, mDevice);
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::string SystemInfo::mainAppVersion() {
        return versionValue(get_main_application_version, mTarget, mDevice);
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::string SystemInfo::mainAppVersion2() {
        return versionValue(get_main_application_version, mAltTarget, mDevice);
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::string SystemInfo::bootVersion() {
        return versionValue(get_bootloader_version, mTarget, mDevice);
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::string SystemInfo::bootVersion2() {
        return versionValue(get_bootloader_version, mAltTarget, mDevice);
    }
    //----------------------------------------------------------------------------------------------------------------------
    int64_t SystemInfo::boardVersion() {
        return bb::byte_value(get_board_revision, mAltTarget, mDevice);
    }
//----------------------------------------------------------------------------------------------------------------------
    std::string SystemInfo::macAddress() {
        std::string mac { bb::stringValue(get_mac_address, mAltTarget, mDevice) };

        if ( !mac.empty()) {
            const char *colon { ":" };
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
        std::any value { bb::entryValue(mAltTarget, mDevice, get_stats_id) };
        return (value.has_value()) ? std::any_cast<int64_t>(value) : 0;
    }
//----------------------------------------------------------------------------------------------------------------------
    int64_t SystemInfo::upTime() {
        std::any value { bb::entryValue(mTarget, mDevice, get_core_up_time_in_milliseconds) };
        return (value.has_value()) ? std::any_cast<int64_t>(value) : 0;
    }

}
