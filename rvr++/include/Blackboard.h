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
#include <unordered_map>

#include "ReadPacket.h"

namespace rvr {

    class Blackboard {
    public:
        Blackboard();
        Blackboard(const Blackboard& other) = delete;
        Blackboard(Blackboard&& other) = delete;
        Blackboard& operator=(const Blackboard& other) = delete;

        enum key_t : uint32_t {
        };

        using FuncPtr = void (*)(const key_t key, MsgArray::const_iterator , MsgArray::const_iterator );

        struct BlackboardEntry {
            std::string name;
            FuncPtr func;
            std::any value { };
        };

        static FuncPtr entryFunc(const key_t key);
        static std::string entryName(const key_t key);
        static std::any& entryValue(const key_t key);
        static std::any entryValue(const uint8_t dev, const uint8_t cmd);
        static std::any entryValueId(const uint8_t dev, const uint8_t cmd, const uint8_t id);

        static float float_convert(MsgArray::const_iterator begin, MsgArray::const_iterator end);
        static int64_t int_convert(MsgArray::const_iterator begin, MsgArray::const_iterator end);

        static constexpr key_t entryKey(const uint8_t dev, const uint8_t cmd, const uint8_t id = 0);

        using BBDictionary = std::unordered_map <key_t, BlackboardEntry>;

        static void dumpEntry(std::pair<const rvr::Blackboard::key_t, rvr::Blackboard::BlackboardEntry>& b);
        static void dump();
        static BBDictionary mDictionary;

    private:

    };
    //----------------------------------------------------------------------------------------------------------------------
    inline rvr::Blackboard::FuncPtr Blackboard::entryFunc(const key_t key) {
        return mDictionary[key].func;
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline std::string Blackboard::entryName(const key_t key) {
        return mDictionary[key].name;
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline std::any Blackboard::entryValue(const uint8_t dev, const uint8_t cmd) {
        return entryValue(entryKey(dev, cmd));
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline std::any Blackboard::entryValueId(const uint8_t dev, const uint8_t cmd, const uint8_t id) {
        return entryValue(entryKey(dev, cmd, id));
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline std::any& Blackboard::entryValue(const key_t key) {
        return mDictionary[key].value;
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline constexpr Blackboard::key_t Blackboard::entryKey(const uint8_t dev, const uint8_t cmd, const uint8_t id) {
        return static_cast<key_t>(id << 16 | dev << 8 | cmd);
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Blackboard::dumpEntry(std::pair<const rvr::Blackboard::key_t, rvr::Blackboard::BlackboardEntry>& b) {
        auto be { b.second };
        terr << std::hex << std::uppercase << (b.first) << mys::sp << be.name << //
             mys::tab << ((be.value.has_value()) ? "has value" : "no value");
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline void Blackboard::dump() {
        for (auto b : rvr::Blackboard::mDictionary) {
            dumpEntry(b);
        }
    }

} /* namespace rvr */

#endif /* BLACKBOARD_H_ */
