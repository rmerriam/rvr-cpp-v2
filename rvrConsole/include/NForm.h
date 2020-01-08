/*
 * NForm.h
 *
 *  Created on: Apr 1, 2017
 *      Author: rmerriam
 */

#ifndef NFORM_H_
#define NFORM_H_

#include <form.h>

#include "NField.h"

namespace scr {
    using FormPtr = FORM*;
    using WindowPtr = WINDOW*;
//--------------------------------------------------------------------------------------------------------------------------
    class NForm {
    public:
        NForm(FieldVec& f, const int y, const int x) :
            mFields(f), mX(x), mY(y) {
        }

        const int box();

        void init();
        const int cols() const;
        const int rows() const;
        FormPtr operator ()() const;
        WindowPtr win() const;
        WindowPtr subWin() const;

        const int x() const;

    private:
        FieldVec& mFields;

        int mX;
        int mY;

        FormPtr mForm { };

        int mRows { };
        int mCols { };

        WindowPtr mWindow { };
        WindowPtr mSubWin { };
    };

    inline const int NForm::box() {
        return ::box(mWindow, 0, 0);
    }

    inline const int NForm::cols() const {
        return mCols;
    }

    inline const int NForm::rows() const {
        return mRows;
    }

    inline scr::FormPtr NForm::operator ()() const {
        return mForm;
    }

    inline scr::WindowPtr NForm::win() const {
        return mWindow;
    }

    inline scr::WindowPtr NForm::subWin() const {
        return mSubWin;
    }

    inline const int NForm::x() const {
        return mX;
    }

}
#endif /* NFORM_H_ */
