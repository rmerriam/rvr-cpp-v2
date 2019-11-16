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
#include <ReadPacket.h>
#include <string>
#include <unordered_map>

namespace rvr {

    class Blackboard {
    public:
        Blackboard();
        Blackboard(const Blackboard& other) = delete;
        Blackboard(Blackboard&& other) = delete;
        Blackboard& operator=(const Blackboard& other) = delete;

        static std::string getName(const uint16_t key) {
            return decoder_map[key].name;
        }

        using FuncPtr = void (*)(MsgArray::const_iterator , MsgArray::const_iterator );

        static FuncPtr getFunc(const uint16_t key) {
            return decoder_map[key].func;
        }

        static float float_convert(MsgArray::const_iterator begin, MsgArray::const_iterator end);
        static long long int_convert(MsgArray::const_iterator begin, MsgArray::const_iterator end);

    private:

        struct RespDecoder {
            std::string name;
            FuncPtr func;
        };
        using DecoderMap = std::unordered_map <uint16_t, RespDecoder>;

        static DecoderMap decoder_map;
    };

} /* namespace rvr */

using RespDecode = rvr::Blackboard;
//using RespMap = rvr::ResponseDecoder::DecoderMap;

#endif /* BLACKBOARD_H_ */
