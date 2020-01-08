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
    HeaderField* NField::build_header(FieldVec& vec, string const& lb, uint8_t const row, uint8_t width, uint8_t const col) {

        HeaderField* h { new HeaderField(lb.c_str(), 1, //
                                         (width == 0) ? (lb.size() * 2) + 1 : width, //
                                         row, col) };
        vec.push_back(( *h)());
        return h;
    }
    //--------------------------------------------------------------------------------------------------------------------------
    LabelField* NField::build_subhead(FieldVec& vec, string const& lb, uint8_t const row, uint8_t width, uint8_t const col) {
        if (width < lb.size()) {
            width = lb.size();
        }
        LabelField* l { new LabelField(lb.c_str(), 1, width, row, col) };
        l->justify(JUSTIFY_CENTER);

        l->fore(A_BOLD | A_UNDERLINE);
        vec.push_back(( *l)());
        return l;
    }
    //--------------------------------------------------------------------------------------------------------------------------
    LabelField* NField::build_label_item(FieldVec& vec, string const& lb, uint8_t const row, uint8_t width, uint8_t const col) {
        if (width < lb.size()) {
            width = lb.size();
        }
        LabelField* l { new LabelField(lb.c_str(), 1, width, row, col) };
        vec.push_back(( *l)());
        return l;
    }

    DataFieldPtr NField::build_wide_data_item(FieldVec& vec, string const& lb, uint8_t const row, uint8_t width, uint8_t const d_width,
        uint8_t const col) {
        vec.push_back(( *new LabelField(lb.c_str(), 1, width, row, col))());
        DataFieldPtr l { new DataField(1, d_width, row, col + width + label_pad) };
        vec.push_back(( *l)());
        return l;
    }

}
