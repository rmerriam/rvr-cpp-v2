/*
 * NForm.cpp
 *
 *  Created on: Apr 1, 2017
 *      Author: rmerriam
 */

#include "NForm.h"

namespace scr {

    void NForm::init() {
        mFields.push_back(nullptr);

        mForm = new_form(mFields.data());
        scale_form(mForm, &mRows, &mCols);

        mWindow = newwin(mRows + 2, mCols + 2, mY, mX);

        /* Set main window and sub window */
        set_form_win(mForm, mWindow);
        mSubWin = derwin(mWindow, mRows + 1, mCols, 0, 0);

        set_form_sub(mForm, mSubWin);
        post_form(mForm);

        box();

        wrefresh(mWindow);
    }

}
