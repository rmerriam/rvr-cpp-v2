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
        HeaderField(const char* text, const int height, const int width, const int row, const int col, const int nmem = 0, const int nbuf = 0);

    };

} /* namespace scr */

#endif /* HEADERFIELD_H_ */
