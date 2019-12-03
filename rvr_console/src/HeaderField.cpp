/*
 * Header.cpp
 *
 *  Created on: Apr 2, 2017
 *      Author: rmerriam
 */
#include "HeaderField.h"
namespace scr {

    //--------------------------------------------------------------------------------------------------------------------------
    HeaderField::HeaderField(const char* text, const int height, const int width, const int row, const int col, const int nmem,
        const int nbuf) :
        NField { height, width, row, col, nmem, nbuf } {

        justify(JUSTIFY_CENTER);

        optionsOff(O_ACTIVE | O_EDIT);
        fore(A_REVERSE | COLOR_PAIR(1));
        background(A_REVERSE | COLOR_PAIR(1));
        write(text);
    }

}

