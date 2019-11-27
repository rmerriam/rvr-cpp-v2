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
        LabelField(const char* text, const int height, const int width, const int row, const int col, const int nmem = 0,
                const int nbuf = 0);

    };

} /* namespace scr */

#endif /* Label_H_ */
