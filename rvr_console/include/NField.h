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
    constexpr uint8_t label_pad { 1 };
    constexpr uint8_t label_col { 1 };
    constexpr uint8_t label_width { 10 };
    constexpr uint8_t data_width { 8 };
    constexpr uint8_t data_col { label_col + label_width + label_pad };
//--------------------------------------------------------------------------------------------------------------------------
    class NField {
    public:
        NField(const int height, const int width, const int row, const int col, const int nmem = 0, const int nbuf = 0) :
            mField { new_field(height, width, row, col, nmem, nbuf) } {
        }
        NField(const int row, const int col, const NField& f) :
            mField { dup_field(f.mField, row, col) } {

        }
        virtual ~NField();

        FieldPtr field();
        void background(const chtype& bits);
        void fore(const chtype& bits);

        void optionsOff(const chtype& bits);
        void optionsOn(const chtype& bits);
        void justify(const chtype& bits);       // NO_JUSTIFICATION, JUSTIFY_RIGHT, JUSTIFY_LEFT, or JUSTIFY_CENTER

        char* read();
        void write(const string& s);

        FieldPtr operator ()();

        static void build_header(FieldVec& vec, const string& lb, const uint8_t row = 1, uint8_t width = 0, const uint8_t col = 1);
        static void build_subhead(FieldVec& vec, const string& lb, const uint8_t row, uint8_t width = label_width,
            const uint8_t col = label_col);
        static void build_label_item(FieldVec& vec, const string& lb, const uint8_t row, uint8_t width = label_width,
            const uint8_t col = label_col);

        static scr::DataFieldPtr build_data_item(FieldVec& vec, const string& lb, const uint8_t row, uint8_t width = label_width,
            const uint8_t col = label_col);

        static DataFieldPtr build_wide_data_item(FieldVec& vec, const string& lb, const uint8_t row, uint8_t width,
            const uint8_t d_width = data_width, const uint8_t col = label_col);

    private:
        FieldPtr mField;
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
    inline void NField::background(const chtype& bits) {
        set_field_back(mField, bits);
    }
    //--------------------------------------------------------------------------------------------------------------------------
    inline void NField::fore(const chtype& bits) {
        set_field_fore(mField, bits);
    }
    //--------------------------------------------------------------------------------------------------------------------------
    inline void NField::justify(const chtype& bits) {
        set_field_just(mField, bits);
    }
    //--------------------------------------------------------------------------------------------------------------------------
    inline scr::FieldPtr NField::operator ()() {
        return mField;
    }
    //--------------------------------------------------------------------------------------------------------------------------
    inline void NField::optionsOff(const chtype& bits) {
        field_opts_off(mField, bits);
    }
    //--------------------------------------------------------------------------------------------------------------------------
    inline void NField::optionsOn(const chtype& bits) {
        field_opts_on(mField, bits);
    }
    //--------------------------------------------------------------------------------------------------------------------------
    inline char* NField::read() {
        return field_buffer(mField, 0);
    }
    //--------------------------------------------------------------------------------------------------------------------------
    inline void NField::write(const string& s) {
        set_field_buffer(mField, 0, s.c_str());
    }
    //--------------------------------------------------------------------------------------------------------------------------
    inline DataFieldPtr NField::build_data_item(FieldVec& vec, const string& lb, const uint8_t row, uint8_t width, const uint8_t col) {
        if (width < lb.size()) {
            width = lb.size();
        }
        return build_wide_data_item(vec, lb, row, width, data_width, col);
    }
}

#endif /* NFIELD_H_ */
