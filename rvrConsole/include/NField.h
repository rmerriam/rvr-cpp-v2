/*
 * NField.h
 *
 *  Created on: Apr 1, 2017
 *      Author: rmerriam
 */

#ifndef NFIELD_H_
#define NFIELD_H_

#include <string>
#include <vector>
using namespace std;

#include <form.h>

namespace scr {
    class NField;
    using NFieldPtr = NField*;

    using FieldPtr = FIELD*;
    using FieldRef = FIELD&;
    using FieldList = FieldPtr*;
    using FieldVec = vector<FieldPtr>;

    class HeaderField;
    using HeaderFieldPtr = HeaderField*;

    class LabelField;
    using LabelFieldPtr = LabelField*;

    class DataField;
    using DataFieldPtr = DataField*;
    //--------------------------------------------------------------------------------------------------------------------------
    constexpr uint8_t label_pad { 2 };
    constexpr uint8_t label_col { 1 };
    constexpr uint8_t label_width { 10 };
    constexpr uint8_t data_width { 7 };
    constexpr uint8_t data_col { label_col + label_width + label_pad };
//--------------------------------------------------------------------------------------------------------------------------
    class NField {
    public:
        NField(uint8_t const height, uint8_t const width, uint8_t const row, uint8_t const col, uint8_t const nmem = 0,
            uint8_t const nbuf = 0) :
            mField { new_field(height, width, row, col, nmem, nbuf) }, mRow { row }, mCol { col }, mWidth { width } {
        }
//        NField(int const row, int const col, NField const& f) :
//            mField { dup_field(f.mField, row, col) } {
//        }
        virtual ~NField();

        FieldPtr field();
        void background(chtype const& bits);
        void fore(chtype const& bits);
        void invertText();

        void optionsOff(chtype const& bits);
        void optionsOn(chtype const& bits);
        void justify(chtype const& bits);       // NO_JUSTIFICATION, JUSTIFY_RIGHT, JUSTIFY_LEFT, or JUSTIFY_CENTER

        char* read();
        void write(string const& s);

        FieldPtr operator ()();

        static HeaderField* build_header(FieldVec& vec, string const& lb, uint8_t const row = 1, uint8_t width = 0, uint8_t const col = 1);
        static LabelField* build_subhead(FieldVec& vec, string const& lb, uint8_t const row, uint8_t width = label_width,
            uint8_t const col = label_col);
        static LabelField* build_label_item(FieldVec& vec, string const& lb, uint8_t const row, uint8_t width = label_width,
            uint8_t const col = label_col);

        static DataFieldPtr build_data_item(FieldVec& vec, string const& lb, uint8_t const row, uint8_t width = label_width,
            uint8_t const col = label_col);

        static DataFieldPtr build_wide_data_item(FieldVec& vec, string const& lb, uint8_t const row, uint8_t width,
            uint8_t const d_width = data_width, uint8_t const col = label_col);

    private:
        FieldPtr mField;
        uint8_t mRow;
        uint8_t mCol;
        uint8_t mWidth;
    };
    //--------------------------------------------------------------------------------------------------------------------------
    inline NField::~NField() {
        free_field(mField);
    }
    //--------------------------------------------------------------------------------------------------------------------------
    inline scr::FieldPtr NField::field() {
        return mField;
    }
    //--------------------------------------------------------------------------------------------------------------------------
    inline void NField::background(chtype const& bits) {
        set_field_back(mField, bits);
    }
    //--------------------------------------------------------------------------------------------------------------------------
    inline void NField::fore(chtype const& bits) {
        set_field_fore(mField, bits);
    }
    //--------------------------------------------------------------------------------------------------------------------------
    inline void NField::justify(chtype const& bits) {
        set_field_just(mField, bits);
    }
    //--------------------------------------------------------------------------------------------------------------------------
    inline scr::FieldPtr NField::operator ()() {
        return mField;
    }
    //--------------------------------------------------------------------------------------------------------------------------
    inline void NField::invertText() {
        fore(A_REVERSE | COLOR_PAIR(1));
        background(A_REVERSE | COLOR_PAIR(1));
    }
    //--------------------------------------------------------------------------------------------------------------------------
    inline void NField::optionsOff(chtype const& bits) {
        field_opts_off(mField, bits);
    }
    //--------------------------------------------------------------------------------------------------------------------------
    inline void NField::optionsOn(chtype const& bits) {
        field_opts_on(mField, bits);
    }
    //--------------------------------------------------------------------------------------------------------------------------
    inline char* NField::read() {
        return field_buffer(mField, 0);
    }
    //--------------------------------------------------------------------------------------------------------------------------
    inline void NField::write(string const& s) {
        set_field_buffer(mField, 0, s.c_str());
    }
    //--------------------------------------------------------------------------------------------------------------------------
    inline DataFieldPtr NField::build_data_item(FieldVec& vec, string const& lb, uint8_t const row, uint8_t width, uint8_t const col) {
        if (width < lb.size()) {
            width = lb.size();
        }
        return build_wide_data_item(vec, lb, row, width, data_width, col);
    }
}

#endif /* NFIELD_H_ */
