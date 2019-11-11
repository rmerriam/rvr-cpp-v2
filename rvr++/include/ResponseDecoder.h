#ifndef RESPONSEDECODER_H_
#define RESPONSEDECODER_H_
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
#include <string>
#include <unordered_map>

#include "Packet.h"

namespace rvr {

    class ResponseDecoder {
    public:
        ResponseDecoder();
        ResponseDecoder(const ResponseDecoder& other) = delete;
        ResponseDecoder(ResponseDecoder&& other) = delete;
        ResponseDecoder& operator=(const ResponseDecoder& other) = delete;

        static std::string getName(const uint16_t key) {
            return decoder_map[key].name;
        }

        using FuncPtr = void (*)(MsgArray::const_iterator , MsgArray::const_iterator );

        static FuncPtr getFunc(const uint16_t key) {
            return decoder_map[key].func;
        }

    private:

        struct RespDecoder {
            std::string name;
            FuncPtr func;
        };
        using DecoderMap = std::unordered_map <uint16_t, RespDecoder>;

        static DecoderMap decoder_map;
    };

} /* namespace rvr */

using RespDecode = rvr::ResponseDecoder;
//using RespMap = rvr::ResponseDecoder::DecoderMap;

#endif /* RESPONSEDECODER_H_ */
