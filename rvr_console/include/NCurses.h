/*
 * NCurses.h
 *
 *  Created on: Apr 7, 2016
 *      Author: rmerriam
 */

#ifndef NCURSES_H_
#define NCURSES_H_

#include <ncurses.h>

//--------------------------------------------------------------------------------------------------------------------------
class NCurses {
public:
    enum {
        cursor_off = 0, cursor_on, cursor_highlight
    };

    NCurses();
    virtual ~NCurses();

    void cursorPos(int& y, int& x);

    int getCols() const;
    int getRows() const;

    static int check_input();

private:
    int mRows;
    int mCols;
};
//--------------------------------------------------------------------------------------------------------------------------
inline int NCurses::getCols() const {
    return mCols;
}
//--------------------------------------------------------------------------------------------------------------------------
inline int NCurses::getRows() const {
    return mRows;
}

#endif /* NCURSES_H_ */
