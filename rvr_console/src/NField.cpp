/*
 * NField.cpp
 *
 *  Created on: Apr 1, 2017
 *      Author: rmerriam
 */

#include "NField.h"
#include "DataField.h"
#include "HeaderField.h"
#include "LabelField.h"

namespace scr {
//--------------------------------------------------------------------------------------------------------------------------
    void NField::build_header(FieldVec& vec, const string& lb, const uint8_t row, uint8_t width, const uint8_t col) {

        HeaderField* h { new HeaderField(lb.c_str(), 1, //
                                         (width == 0) ? (lb.size() * 2) + 1 : width, //
                                         row, col) };
        vec.push_back(( *h)());
    }
//--------------------------------------------------------------------------------------------------------------------------
    void NField::build_subhead(FieldVec& vec, const string& lb, const uint8_t row, uint8_t width, const uint8_t col) {
        if (width < lb.size()) {
            width = lb.size();
        }
        LabelField* l { new LabelField(lb.c_str(), 1, width * 2, row, col) };
        l->justify(JUSTIFY_LEFT);

        l->fore(A_BOLD | A_UNDERLINE);
        vec.push_back(( *l)());
    }
//--------------------------------------------------------------------------------------------------------------------------
    void NField::build_label_item(FieldVec& vec, const string& lb, const uint8_t row, uint8_t width, const uint8_t col) {
        if (width < lb.size()) {
            width = lb.size();
        }
        LabelField* l { new LabelField(lb.c_str(), 1, width * 2, row, col) };
        vec.push_back(( *l)());
    }

    DataFieldPtr NField::build_wide_data_item(FieldVec& vec, const string& lb, const uint8_t row, uint8_t width, const uint8_t d_width,
        const uint8_t col) {
        vec.push_back(( *new LabelField(lb.c_str(), 1, width, row, col))());
        DataFieldPtr l { new DataField(1, d_width, row, col + width + label_pad) };
        vec.push_back(( *l)());
        return l;
    }

}
