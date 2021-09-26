#ifndef BLACKBOARD_H_
#define BLACKBOARD_H_
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
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//======================================================================================================================
//
//     Author: rmerriam
//
//     Created: May 29, 2021
//
//======================================================================================================================
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>

#include "ReadPacket.h"
#include <Request.h>

namespace rvr {
//----------------------------------------------------------------------------------------------------------------------
    template <typename T> struct Result {
    public:
        explicit constexpr Result() noexcept = default;

        constexpr Result(T const& t) :
            mOptional { t } {
        }

        constexpr Result(T&& t) :
            mOptional { t } {
        }

        [[nodiscard]] constexpr bool valid() const noexcept {
            return mOptional.has_value();
        }

        [[nodiscard]] constexpr bool invalid() const noexcept {
            return !valid();
        }

        [[nodiscard]] constexpr auto get() const -> T {
            return mOptional.value();
        }

        [[nodiscard]] constexpr auto get_or(T const&& value = T { }) const -> T {
            return mOptional.value_or(value);
        }

    private:
        std::optional<T> mOptional;
    };
//----------------------------------------------------------------------------------------------------------------------
    using ResultBool = Result<bool>;
    using ResultUInt8 = Result<uint8_t>;
    using ResultInt16 = Result<int16_t>;
    using ResultUInt16 = Result<uint16_t>;
    using ResultInt32 = Result<int32_t>;
    using ResultUInt32 = Result<uint32_t>;
    using ResultInt64 = Result<int64_t>;
    using ResultUInt64 = Result<uint64_t>;
    using ResultFloat = Result<float>;
    using ResultString = Result<std::string>;
    using ResultMsg = Result<RvrMsgView>;
//----------------------------------------------------------------------------------------------------------------------
    class Blackboard {

    public:
        using key_t = uint32_t;

        Blackboard();
        Blackboard(Blackboard const& other) = delete;
        Blackboard(Blackboard&& other) = delete;
        Blackboard& operator=(Blackboard const& other) = delete;

        ResultBool boolValue(TargetPort const target, Devices const dev, uint8_t const cmd);
        ResultUInt8 byteValue(TargetPort const target, Devices const dev, uint8_t const cmd);

        ResultInt16 int16Value(TargetPort const target, Devices const dev, uint8_t const cmd, uint8_t const pos = 0);
        ResultUInt16 uint16Value(TargetPort const target, Devices const dev, uint8_t const cmd, uint8_t const pos = 0);

        ResultUInt32 uint32Value(TargetPort const target, Devices const dev, uint8_t const cmd, uint8_t const pos = 0,
            uint8_t const id = 0);

        ResultInt64 int64Value(TargetPort const target, Devices const dev, uint8_t const cmd);
        ResultUInt64 uint64Value(TargetPort const target, Devices const dev, uint8_t const cmd);

        ResultFloat floatValue(TargetPort const target, Devices const dev, uint8_t const cmd, uint8_t const pos = 0,
            uint8_t const id = 0);

        ResultBool notifyState(TargetPort const target, Devices const dev, uint8_t const cmd);

        ResultBool getNotify(TargetPort const target, Devices const dev, uint8_t const cmd);
        void resetNotify(TargetPort const target, Devices const dev, uint8_t const cmd);

        ResultString stringValue(TargetPort const target, Devices const dev, uint8_t const cmd);

        uint64_t uintConvert(RvrMsgView::const_iterator begin, uint8_t n);

        void m_to_v();

        std::string entryName(key_t key);
        void msgArray(key_t key, uint8_t const cmd, RvrMsg::iterator begin, RvrMsg::iterator end);
        Blackboard::key_t msgKey(TargetPort const src, Devices const dev, uint8_t const cmd, uint8_t const seq);

        float floatConvert(RvrMsg::const_iterator begin) const;
        float floatConvert(uint8_t const* begin) const {
            return floatConvert(RvrMsg::const_iterator { begin });
        }

        RvrMsgView entryValue(TargetPort const target, Devices const dev, uint8_t const cmd, uint8_t const id = 0) const;

    private:
        inline static float const NaN { (0.0f / 0.0f) }; // something to return when there is no value for float

        /*  =============================================================================================================
         *  Key stuff is gnarly because of using unorderedmap which uses a hash table
         * but has limited hashing capability The key is "key_t" but is a combination
         * of processor, device, command, and id. That's not easy to deal with so
         * key_s is a struct with those fields with a conversion operator. That allows
         * constructing the key by fields and then converting it to the key_t.
         *
         *  Id is another hack. There are some messages that use a parameter to ask
         * for different information. But the response doesn't contain that parameter.
         * In those cases the parameter is plugged in as sequence number. The actual
         * sequence numbers are limited to > 0x80h so anything less is an Id.
         *
         */

        struct key_s {
            key_s() {
            }
            key_s(key_t const k);

            key_s(TargetPort const proc, Devices const dev, uint8_t const cmd, uint8_t const id);
            operator key_t();
            uint8_t id { };
            uint8_t cmd { };
            Devices dev { };
            TargetPort proc { };
        };

        struct BlackboardEntry {
            std::string name;
            RvrMsg value { };
        };

        using BBDictionary = std::unordered_map<key_t, BlackboardEntry>;
        static BBDictionary mDictionary;

        void addentryValue(key_t const key, RvrMsg value);

        RvrMsgView entryValue(key_t const key) const;
        static key_t entryKey(TargetPort const target, Devices const dev, uint8_t const cmd, uint8_t const id = 0);
        // methods for processing received responses

        // methods for calculating values from dictionary entry
        //        uint64_t uintConvert(RvrMsg::const_iterator begin, uint8_t n);

        //        static inline RvrMsg fake_msg { 3, 0 };

        friend std::ostream& operator<<(std::ostream& os, Blackboard::key_s const& k);
    };
//----------------------------------------------------------------------------------------------------------------------
    std::ostream& operator<<(std::ostream& os, Blackboard::key_s const& k);

} /* namespace rvr */

#endif /* BLACKBOARD_H_ */