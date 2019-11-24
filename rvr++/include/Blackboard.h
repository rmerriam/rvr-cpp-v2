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
    public:
        Blackboard();
        Blackboard(const Blackboard& other) = delete;
        Blackboard(Blackboard&& other) = delete;
        Blackboard& operator=(const Blackboard& other) = delete;

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

        enum key_t : uint32_t {
        };

        struct key_s {
            key_s() {
            }
            key_s(const key_t k) :
                id { static_cast<uint8_t>(k & 0xFF) }, //
                    cmd { static_cast<uint8_t>((k >> 8) & 0xFF) },  //
                    dev { static_cast<uint8_t>((k >> 16) & 0xFF) },  //
                    proc { static_cast<uint8_t>((k >> 24) & 0xFF) } {
            }

            key_s(const CommandBase::TargetPort proc, const Devices dev, const uint8_t cmd, const uint8_t id) :
                id(id), cmd(cmd), dev(dev), proc(proc) {
            }
            operator key_t() {
                return key_t(proc << 24 | (dev << 16) | (cmd << 8) | id);
            }
            uint8_t id { };
            uint8_t cmd { };
            Devices dev { };
            CommandBase::TargetPort proc { };
        };

        using FuncPtr = void (*)(const key_t key, MsgArray::iterator , MsgArray::iterator );

        struct BlackboardEntry {
            std::string name;
            FuncPtr func;
            std::any value { };
        };

        static FuncPtr entryFunc(const key_t key);
        static std::string entryName(const key_t key);
        static std::any& entryValue(const key_t key);
        static std::any& entryValue(const CommandBase::TargetPort proc, const Devices dev, const uint8_t cmd);
        static std::any& entryValueId(const CommandBase::TargetPort proc, const Devices dev, const uint8_t cmd, const uint8_t id);

        static float float_convert(MsgArray::iterator begin, MsgArray::iterator end);
        static int32_t int_convert(MsgArray::iterator begin, MsgArray::iterator end);

        static uint8_t byteValue(const uint8_t cmd, const CommandBase::TargetPort target, const Devices dev);
        static int16_t intValue(const uint8_t cmd, const CommandBase::TargetPort target, const Devices dev);
        static uint16_t uintValue(const uint8_t cmd, const CommandBase::TargetPort target, const Devices dev);
        static uint64_t uint64Value(const uint8_t cmd, const CommandBase::TargetPort target, const Devices dev);

        static std::string stringValue(const uint8_t cmd, const CommandBase::TargetPort target, const Devices dev);

        static key_t entryKey(const CommandBase::TargetPort proc, const Devices dev, const uint8_t cmd, const uint8_t id = 0);

        using BBDictionary = std::unordered_map <key_t, BlackboardEntry>;

        static void dumpEntry(std::pair<const rvr::Blackboard::key_t, rvr::Blackboard::BlackboardEntry>& b);
        static void dump();
        static void m_to_v();
        static BBDictionary mDictionary;

    private:
        static uint64_t uintConvert(MsgArray::const_iterator begin, uint8_t n);
    };
    //----------------------------------------------------------------------------------------------------------------------
    inline std::ostream& operator<<(std::ostream& os, const Blackboard::key_s& k) {
        os << k.proc << mys::sp  //
           << k.dev << mys::sp //
           << k.cmd << mys::sp //
           << k.id;
        return os;
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline rvr::Blackboard::FuncPtr Blackboard::entryFunc(const key_t key) {
        return mDictionary[key].func;
    }
//    //----------------------------------------------------------------------------------------------------------------------
    inline std::string Blackboard::entryName(const key_t key) {
        return mDictionary[key].name;
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline std::any& Blackboard::entryValue(const CommandBase::TargetPort proc, const Devices dev, const uint8_t cmd) {
        return entryValue(entryKey(proc, dev, cmd, 0));
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline std::any& Blackboard::entryValueId(const CommandBase::TargetPort proc, const Devices dev, const uint8_t cmd, const uint8_t id) {
        return entryValue(entryKey(proc, dev, cmd, id));
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline std::any& Blackboard::entryValue(const key_t key) {
        return mDictionary[key].value;
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline Blackboard::key_t Blackboard::entryKey(const CommandBase::TargetPort proc, const Devices dev, const uint8_t cmd,
        const uint8_t id) {
        return static_cast<key_t>(key_s(proc, dev, cmd, id));
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Blackboard::dumpEntry(std::pair<const rvr::Blackboard::key_t, rvr::Blackboard::BlackboardEntry>& b) {
        auto be { b.second };
        terr << std::hex << std::uppercase << b.first //
             << mys::tab << std::setw(45) << std::setfill(' ') << std::left << be.name << //
             mys::tab << mys::tab << (( !be.value.has_value()) ? "" : be.value.type().name());
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Blackboard::dump() {

        for (auto b : rvr::Blackboard::mDictionary) {
            dumpEntry(b);
        }
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Blackboard::m_to_v() {
        struct v_map {
            Blackboard::key_t key;
            BlackboardEntry be;
        };
        std::vector<v_map> vec;
        for (auto b : rvr::Blackboard::mDictionary) {
            v_map v { b.first, b.second };
            vec.push_back(v);
        }
        std::sort(vec.begin(), vec.end(), //
                  [](const v_map& a, const v_map& b) {
                      return a.key < b.key;
                  }
        );
        for (auto &i : vec) {
            terr << std::hex << std::uppercase << i.key //
                 << mys::tab << std::setw(45) << std::setfill(' ') << std::left << i.be.name << //
                 mys::tab << mys::tab << (( !i.be.value.has_value()) ? "" : i.be.value.type().name());
        }
    }
} /* namespace rvr */

#endif /* BLACKBOARD_H_ */
