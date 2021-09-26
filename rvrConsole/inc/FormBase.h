#ifndef FORMBASE_H_
#define FORMBASE_H_
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
namespace scr {
    class FormBase {
    public:
        FormBase(const int y, const int x) :
            mFields { }, mForm { mFields, y, x } {
        }
        FormBase(const FormBase& other) = delete;
        FormBase(FormBase&& other) = delete;
        FormBase& operator=(const FormBase& other) = delete;
        virtual ~FormBase() = default;

        virtual void onceData();
        virtual void fastData();
        virtual void requestData();
        virtual void updateScreen() = 0;

        const int getX() const {
            return mForm.x() + mForm.cols();
        }

        FieldVec mFields;
        NForm mForm;
    };
    //--------------------------------------------------------------------------------------------------------------------------
    inline void FormBase::onceData() {
    }
    //--------------------------------------------------------------------------------------------------------------------------
    inline void FormBase::fastData() {
    }
    //--------------------------------------------------------------------------------------------------------------------------
    inline void FormBase::requestData() {
    }
} // namespace scr
#endif /* FORMBASE_H_ */
