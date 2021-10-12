#pragma once
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
//		 File: Result.h
//
//     Author: rmerriam
//
//    Created: Oct 2, 2021
//
//======================================================================================================================
namespace rvr {
//----------------------------------------------------------------------------------------------------------------------
    template <typename T> struct Result {
    public:
        using type = T;
        explicit constexpr Result() noexcept {
        }

        constexpr Result(T const& t) :
            mOptional { t } {
        }

        constexpr Result(T&& t) :
            mOptional { std::move(t) } {
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

        [[nodiscard]] constexpr auto get_or(T const&& value = T {}) const -> T {
            return mOptional.value_or(value);
        }

    private:
        std::optional<T> mOptional {};
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
}

