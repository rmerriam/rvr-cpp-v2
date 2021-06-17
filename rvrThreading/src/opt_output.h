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
//		 File: opt_output.h
//
//     Author: rmerriam
//
//    Created: Jun 16, 2021
//
//======================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void opt_output(std::string const& text, std::optional<T> v) {
    std::cout << code_loc << text << mys::sp << (v ? v.value() : T { }) << mys::nl;
}
//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void opt_output_hex(std::string const& text, std::optional<T> v) {
    std::cout << code_loc << text << mys::sp << std::hex << (v ? v.value() : T { }) << mys::nl;
}
