/*
 * Header.h
 *
 *  Created on: Apr 2, 2017
 *      Author: rmerriam
 */

#ifndef HEADERFIELD_H_
#define HEADERFIELD_H_
#include "NField.h"

namespace scr {
    //--------------------------------------------------------------------------------------------------------------------------
    class HeaderField : public NField {
    public:
        HeaderField(char const* text, uint8_t const height, uint8_t const width, uint8_t const row, uint8_t const col,
            uint8_t const nmem = 0, uint8_t const nbuf = 0);

    };

} /* namespace scr */

#endif /* HEADERFIELD_H_ */
