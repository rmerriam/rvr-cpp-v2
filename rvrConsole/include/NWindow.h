/*
 * NWindow.h
 *
 *  Created on: Mar 31, 2017
 *      Author: rmerriam
 */

#ifndef NWINDOW_H_
#define NWINDOW_H_

#include <panel.h>
//--------------------------------------------------------------------------------------------------------------------------
class NWindow {
public:
    NWindow(const int lines, const int cols, const int y, const int x) :
            mWin(newwin(lines, cols, y, x)) {
        box(mWin, 0, 0);
        wrefresh(mWin);
    }
    ~NWindow() {
        delwin(mWin);
    }

    WINDOW* window() {
        return mWin;
    }
private:
    WINDOW* mWin;
};

#endif /* NWINDOW_H_ */
