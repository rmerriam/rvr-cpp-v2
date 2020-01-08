/*
 * NCurses.cpp
 *
 *  Created on: Apr 7, 2016
 *      Author: rmerriam
 */
#include <iostream>
using namespace std;

#include "NCurses.h"

//--------------------------------------------------------------------------------------------------------------------------
NCurses::NCurses() {

    initscr();
    raw();
    keypad(stdscr, TRUE); /* We get F1, F2 etc..		*/
    noecho(); /* Don't echo() while we do getch */
    curs_set(cursor_off);
    start_color();
    getmaxyx(stdscr, mRows, mCols);
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    wattron(stdscr, COLOR_PAIR(1));

    mmask_t mask;
    mousemask(ALL_MOUSE_EVENTS, &mask);
    refresh();
}
//--------------------------------------------------------------------------------------------------------------------------
NCurses::~NCurses() {
    endwin();
}
//--------------------------------------------------------------------------------------------------------------------------
void NCurses::cursorPos(int& y, int& x) {
    getyx(stdscr, y, x);
}
extern int moused;
//--------------------------------------------------------------------------------------------------------------------------
int NCurses::check_input() {
    halfdelay(1);
    int cmoused = getch();
    return cmoused;
}
