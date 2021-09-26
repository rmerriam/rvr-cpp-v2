/*
 * Label.h
 *
 *  Created on: Apr 2, 2017
 *      Author: rmerriam
 */

#ifndef Label_H_
#define Label_H_
#include "NField.h"

namespace scr {
    //--------------------------------------------------------------------------------------------------------------------------
    class LabelField : public NField {
    public:
        LabelField(char const* text, uint8_t const height, uint8_t const width, uint8_t const row, uint8_t const col,
            uint8_t const nmem = 0, uint8_t const nbuf = 0);

    };

} /* namespace scr */

#endif /* Label_H_ */
