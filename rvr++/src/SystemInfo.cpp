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
// along with this program.  If not, see <http:             //www.gnu.org/licenses/>.
//======================================================================================================================
//
//     Author: rmerriam
//
//     Created: May 29, 2021
//
//======================================================================================================================

#include "Blackboard.h"
#include <PayloadDecode.h>
#include "SystemInfo.h"
namespace rvr {

    //----------------------------------------------------------------------------------------------------------------------
    ResultString SystemInfo::versionValue(rvr::TargetPort const target, Devices const dev, uint8_t const cmd) {
        RvrMsgView const& msg { mBlackboard.entryValue(target, mDevice, cmd) };
        ResultString res;
        if ( !msg.empty()) {

            PayloadDecode<uint16_t, uint16_t, uint16_t> payload(msg);

            res = std::to_string(payload.get<0>()) + '.' +              //
                std::to_string(payload.get<1>()) + '.' +              //
                std::to_string(payload.get<2>());
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    ResultString SystemInfo::processorName() {
//        return decode_type<std::string>(mBlackboard.entryValue(mTarget, mDevice, get_processor_name));
        return decode_type<std::string>(mBlackboard.entryValue(mTarget, mDevice, get_processor_name));
    }
    //----------------------------------------------------------------------------------------------------------------------
    ResultString SystemInfo::processorName2() {
        return decode_type<std::string>(mBlackboard.entryValue(mAltTarget, mDevice, get_processor_name));
    }
    //----------------------------------------------------------------------------------------------------------------------
    ResultString SystemInfo::sku() {
        return decode_type<std::string>(mBlackboard.entryValue(mAltTarget, mDevice, get_sku));
    }
    //----------------------------------------------------------------------------------------------------------------------
    ResultString SystemInfo::mainAppVersion() {
        return versionValue(mTarget, mDevice, get_main_application_version);
    }
    //----------------------------------------------------------------------------------------------------------------------
    ResultString SystemInfo::mainAppVersion2() {
        return versionValue(mAltTarget, mDevice, get_main_application_version);
    }
    //----------------------------------------------------------------------------------------------------------------------
    ResultString SystemInfo::bootVersion() {
        return versionValue(mTarget, mDevice, get_bootloader_version);
    }
    //----------------------------------------------------------------------------------------------------------------------
    ResultString SystemInfo::bootVersion2() {
        return versionValue(mAltTarget, mDevice, get_bootloader_version);
    }
    //----------------------------------------------------------------------------------------------------------------------
    ResultUInt8 SystemInfo::boardVersion() {
        auto const& msg { mBlackboard.entryValue(mAltTarget, mDevice, get_board_revision) };
        return decode_type<uint8_t>(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    ResultString SystemInfo::macAddress() {
        RvrMsgView msg { mBlackboard.entryValue(mAltTarget, mDevice, get_mac_address) };
        ResultString res;

        if ( !msg.empty()) {
            constexpr char colon { ':' };

            std::string mac { msg.begin(), msg.end() };

            mac.insert(10, 1, colon);
            mac.insert(8, 1, colon);
            mac.insert(6, 1, colon);
            mac.insert(4, 1, colon);
            mac.insert(2, 1, colon);
            res = mac;
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    ResultInt16 SystemInfo::statsId() {
        auto const& msg { mBlackboard.entryValue(mAltTarget, mDevice, get_stats_id) };
        return decode_type<int16_t>(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    ResultInt64 SystemInfo::coreUpTime() {
        auto const& msg { mBlackboard.entryValue(mTarget, mDevice, get_core_up_time_in_milliseconds) };
        return decode_type<int64_t>(msg);
    }

}
