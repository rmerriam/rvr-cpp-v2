#ifndef TRACE_H_
#define TRACE_H_
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
//     Created: Oct 26, 2019
//
//======================================================================================================================
#include <iostream>
#include <iterator>
#include <vector>

constexpr char nl { '\n' };
//----------------------------------------------------------------------------------------------------------------------
template <typename T>
void trace(std::ostream& os, std::vector<T> const& data) {
    std::copy(data.begin(), data.end(), std::ostream_iterator<int>(os, " "));
    os << nl;
}
//----------------------------------------------------------------------------------------------------------------------
template <typename T>
void trace(std::ostream& os, std::vector<T> const& data, char const* const text) {
    os << text << ' ';
    std::copy(data.begin(), data.end(), std::ostream_iterator<int>(os, " "));
    os << nl;
}
//----------------------------------------------------------------------------------------------------------------------
template <typename T>
inline void trace(std::ostream& os, T const& data) {
    os << data;
}
//----------------------------------------------------------------------------------------------------------------------
template <typename T>
inline void trace(std::ostream& os, char const* text, T const& data) {
    trace(os, "cnt: ");
    os << data;
}
//----------------------------------------------------------------------------------------------------------------------
template <typename T>
inline void tracenl(std::ostream& os, char const* text, T const& data) {
    trace(os, text, data);
    os << nl;
}
//----------------------------------------------------------------------------------------------------------------------
inline void tracenl(std::ostream& os) {
    os << nl;
}
#endif /* TRACE_H_ */
