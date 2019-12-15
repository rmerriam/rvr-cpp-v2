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
    std::string SystemInfo::versionValue(rvr::CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd) {
        RvrMsg const& msg { mBlackboard.msgValue(target, mDevice, cmd) };

        std::string ver { };
        if (msg.size() >= 6) {
            ver = std::to_string(((msg[0] << 8) | msg[1])) + '.' + //
                std::to_string(((msg[2] << 8) | msg[3])) + '.' + //
                std::to_string(((msg[4] << 8) | msg[5]));
        }
        return ver;
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::string SystemInfo::processorName() {
        return mBlackboard.stringValue(mTarget, mDevice, get_processor_name);
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::string SystemInfo::processorName2() {
        return mBlackboard.stringValue(mAltTarget, mDevice, get_processor_name);
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::string SystemInfo::sku() {
        return mBlackboard.stringValue(mAltTarget, mDevice, get_sku);
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::string SystemInfo::mainAppVersion() {
        return versionValue(mTarget, mDevice, get_main_application_version);
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::string SystemInfo::mainAppVersion2() {
        return versionValue(mAltTarget, mDevice, get_main_application_version);
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::string SystemInfo::bootVersion() {
        return versionValue(mTarget, mDevice, get_bootloader_version);
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::string SystemInfo::bootVersion2() {
        return versionValue(mAltTarget, mDevice, get_bootloader_version);
    }
    //----------------------------------------------------------------------------------------------------------------------
    int64_t SystemInfo::boardVersion() {
        return mBlackboard.byteValue(mAltTarget, mDevice, get_board_revision);
    }
//----------------------------------------------------------------------------------------------------------------------
    std::string SystemInfo::macAddress() {
        RvrMsg msg { mBlackboard.msgValue(mAltTarget, mDevice, get_mac_address) };
        std::string mac { msg.begin(), msg.end() };
        if (mac.size() == 12) {
            static char const* colon { ":" };
            mac.insert(10, colon);
            mac.insert(8, colon);
            mac.insert(6, colon);
            mac.insert(4, colon);
            mac.insert(2, colon);
        }
        return mac;
    }
//----------------------------------------------------------------------------------------------------------------------
    int16_t SystemInfo::statsId() {
        return mBlackboard.intValue(mAltTarget, mDevice, get_stats_id);
    }
//----------------------------------------------------------------------------------------------------------------------
    int64_t SystemInfo::coreUpTime() {
        return mBlackboard.uint64Value(mTarget, mDevice, get_core_up_time_in_milliseconds);
    }

}
