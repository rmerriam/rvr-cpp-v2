/*
 * Header.cpp
 *
 *  Created on: Apr 2, 2017
 *      Author: rmerriam
 */
#include "HeaderField.h"
namespace scr {

    //--------------------------------------------------------------------------------------------------------------------------
    HeaderField::HeaderField(char const* text, uint8_t const height, uint8_t const width, uint8_t const row, uint8_t const col,
        uint8_t const nmem, uint8_t const nbuf) :
        NField { height, width, row, col, nmem, nbuf } {

        justify(JUSTIFY_CENTER);

        optionsOff(O_ACTIVE | O_EDIT);
        fore(A_REVERSE | COLOR_PAIR(1));
        background(A_REVERSE | COLOR_PAIR(1));
        write(text);
    }

}

