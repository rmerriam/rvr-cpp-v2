#ifndef BLACKBOARD_H_
#define BLACKBOARD_H_
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
#include <any>
#include <string>
#include <map>
#include <unordered_map>

#include "CommandBase.h"
#include "ReadPacket.h"

namespace rvr {

    class Blackboard {
        friend class Response;

    public:
        Blackboard();
        Blackboard(Blackboard const& other) = delete;
        Blackboard(Blackboard&& other) = delete;
        Blackboard& operator=(Blackboard const& other) = delete;

        static bool boolValue(CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd);
        static uint8_t byteValue(CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd);
        static int16_t intValue(CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd);
        static uint16_t uintValue(CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd, uint8_t const pos = 0);
        static uint64_t uint64Value(CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd);
        static float floatValue(CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd, uint8_t const pos = 0,
            uint8_t const id = 0);

        static bool notifyState(CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd);

        static bool getNotify(CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd);
        static void resetNotify(CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd);

        static std::string stringValue(CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd);
        static RvrMsg const& msgValue(CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd);

        static float floatConvert(RvrMsg::const_iterator begin);

        static void m_to_v();

    private:
        inline static float const NaN { (0.0f / 0.0f) };    // something to return when there is no value for float

        /*  =============================================================================================================
         * Key stuff is gnarly because of using unorderedmap which uses a hash table but has limited hashing capability
         *  The key is "key_t" but is a combination of processor, device, command, and id. That's not easy to deal with
         *  so key_s is a struct with those fields with a conversion operator. That allows constructing the key by fields
         *  and then converting it to the key_t.
         *
         *  Id is another hack. There are some messages that use a parameter to ask for different information. But the
         *  response doesn't contain that parameter. In those cases the parameter is plugged in as sequence number. The
         *  actual sequence numbers are limited to > 0x80h so anything less is an Id.
         *
         */

        using key_t = uint32_t;

        struct key_s {
            key_s() {
            }
            key_s(key_t const k);

            key_s(CommandBase::TargetPort const proc, Devices const dev, uint8_t const cmd, uint8_t const id);
            operator key_t();
            uint8_t id { };
            uint8_t cmd { };
            Devices dev { };
            CommandBase::TargetPort proc { };
        };

        struct BlackboardEntry {
            std::string name;
            RvrMsg value { };
        };

        using BBDictionary = std::unordered_map <key_t, BlackboardEntry>;
        static BBDictionary mDictionary;

        static std::string entryName(key_t key);
        static RvrMsg& entryValue(key_t const key);
        static RvrMsg& entryValue(CommandBase::TargetPort const proc, Devices const dev, uint8_t const cmd, uint8_t const id = 0);

        static key_t entryKey(CommandBase::TargetPort const proc, Devices const dev, uint8_t const cmd, uint8_t const id = 0);

        // methods for processing received responses
        static void msgArray(key_t key, RvrMsg::iterator begin, RvrMsg::iterator end);

        // methods for calculating values from dictionary entry
        static uint64_t uintConvert(RvrMsg::const_iterator begin, uint8_t n);

        friend std::ostream& operator <<(std::ostream& os, Blackboard::key_s const& k);
    };
    //----------------------------------------------------------------------------------------------------------------------
    std::ostream& operator <<(std::ostream& os, Blackboard::key_s const& k);

} /* namespace rvr */

#endif /* BLACKBOARD_H_ */
