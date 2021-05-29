#ifndef BLACKBOARDX_H_
#define BLACKBOARDX_H_
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
//     Created: Nov 13, 2019
//
//======================================================================================================================
#include <map>
#include <string>
#include <vector>

using MsgArray = std::vector<uint8_t>;

namespace rvr {

    // data name, value
    //
    // using DataTuple = std::pair<uint8_t, uint8_t>;
//    using MsgArray = std::variant<int32_t, int64_t, float, std::string>;

    using DataTag = uint8_t[3];
    // target, cmd, id (for voltage measurement)

    class Blackboard {
    public:
        // would prefer unordered_map but there is not hash for tuples
        using DataMap = std::map <uint32_t, MsgArray>;

        Blackboard() :
            mDataMap { } {
        }
        Blackboard(const Blackboard& other) = delete;
        Blackboard(Blackboard&& other) = delete;
        Blackboard& operator=(const Blackboard& other) = delete;

        template <typename T>
        void insert(const uint8_t proc, const uint8_t cmd, T value) {
            insert(proc, cmd, 0, value);
        }

        int32_t make_key(const uint8_t proc, const uint8_t cmd, const uint8_t id) {
            return ((proc << 16) | (cmd << 8) | id);
        }

        template <typename T>
        void insert(const uint8_t proc, const uint8_t cmd, const uint8_t id, T value) {
            mDataMap[make_key(proc, cmd, id)] = MsgArray().emplace<T>(value);
        }

//        float get(const uint8_t proc, const uint8_t cmd) {
//            return get(proc, cmd, 0);
//        }
//
//        float get(const uint8_t proc, const uint8_t cmd, const uint8_t id) {
//            MsgArray dv { mDataMap[make_key(proc, cmd, id)] };
//            return MsgArray_cast<float>(dv);
//        }

    private:

        DataMap mDataMap;
    };

} /* namespace rvr */

#endif /* BLACKBOARDX_H_ */
