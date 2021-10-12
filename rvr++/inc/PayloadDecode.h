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
// along with this program.  If not, see <http:             //www.gnu.org/licenses/>.
//======================================================================================================================
//
//		 File: PayloadDecode.h
//
//     Author: rmerriam
//
//    Created: Sep 25, 2021
//
//======================================================================================================================
#include <Packet.h>
#include <Result.h>
#include <tuple>

namespace rvr {

    //----------------------------------------------------------------------------------------------------------------------
    template <typename ... Ts>
    struct conv_overload : Ts... {
        using Ts::operator( )...;
    };
    //----------------------------------------------------------------------------------------------------------------------
    template <typename ... Ts>
    class PayloadDecode {
    public:
        PayloadDecode(rvr::RvrMsgView const& msg) {
            converter(msg);
        }
        ~PayloadDecode() = default;
        PayloadDecode(PayloadDecode const& other) = delete;
        PayloadDecode(PayloadDecode&& other) = delete;
        PayloadDecode& operator=(PayloadDecode const& other) = delete;
        PayloadDecode& operator=(PayloadDecode&& other) = delete;

        std::tuple<Ts...> values() const {
            return mTup;
        }

        template <std::size_t I>
        auto const& get() noexcept {
            return std::get<I>(mTup);
        }

    private:

        std::tuple<Ts...> mTup;
        auto converter(RvrMsgView const& msg) {

            int16_t pos {};                 // position in msg

            auto convert = conv_overload {              //

            [&pos](RvrMsgView const& msg, bool& value) {
                value = (msg[pos++ ] != 0);
            },

            [&pos](RvrMsgView const& msg, char& value) {
                value = msg[pos++ ];
            },

            [&pos](RvrMsgView const& msg, uint8_t& value) {
                value = msg[pos++ ];
            },

            [&pos](RvrMsgView const& msg, int8_t& value) {
                value = msg[pos++ ];
            },

            [&pos](RvrMsgView const& msg, int16_t& value) {
                value = (msg[pos] << 8) + msg[pos + 1];
                pos += 2;
            },

            [&pos](RvrMsgView const& msg, uint16_t& value) {
                value = (msg[pos] << 8) + msg[pos + 1];
                pos += 2;
            },

            [&pos, this](RvrMsgView const& msg, int32_t& value) {
                value = int_convert<int32_t>( &msg[pos]);
                pos += sizeof(int32_t);
            },

            [&pos, this](RvrMsgView const& msg, uint32_t& value) {
                value = int_convert<uint32_t>( &msg[pos]);
                pos += sizeof(uint32_t);
            },

            [&pos, this](RvrMsgView const& msg, int64_t& value) {
                value = int_convert<int64_t>( &msg[pos]);
                pos += sizeof(int64_t);
            },

            [&pos, this](RvrMsgView const& msg, std::string& value) {
                value = std::string(msg.begin(), msg.end());
                pos += msg.size();
            },

            [&pos](RvrMsgView const& msg, float& value) {
                union {
                    uint8_t buf[4];
                    float result { 0 };
                };
                buf[0] = msg[pos + 2];
                buf[1] = msg[pos + 3];
                buf[2] = msg[pos + 1];
                buf[3] = msg[pos + 0];
                pos += 4;
                value = result;
            },

            };

            std::apply([&](auto& ... value) {
            (
                convert(msg, value), ...);
        }
                       ,
                       mTup);
            return mTup;
        }

        template <typename I>
        I int_convert(RvrMsgView const& msg) {
            I res {};
            for (std::size_t it { 0 }; it < sizeof(I); ++it) {
                res <<= 8;
                res |= msg[it];
            }
            return res;
        }
    };

//----------------------------------------------------------------------------------------------------------------------
    template <typename T>
    inline auto decode_type(rvr::RvrMsgView const& msg) -> T {
        T res {};
        if ( !msg.empty()) {
            PayloadDecode<T> payload(msg);

            auto tup { payload.values() };
            res = std::get<0>(tup);
        }
        return res;
    }
//----------------------------------------------------------------------------------------------------------------------
//    template <typename T>
//    inline T decode_type(rvr::RvrMsgView const& msg) {
//        return msg.empty() ? T {} : decode_type<typename T::type>(msg);
//    }

}
