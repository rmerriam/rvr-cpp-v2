#ifndef RVRDATA_H_
#define RVRDATA_H_
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
//     Created: Nov 12, 2019
//
//======================================================================================================================

namespace rvr {

    class RvrData {
    public:
        RvrData();
        RvrData(const RvrData& other) = delete;
        RvrData(RvrData&& other) = delete;
        RvrData& operator=(const RvrData& other) = delete;
    };

    RvrData::RvrData() {

    }

} /* namespace rvr */
