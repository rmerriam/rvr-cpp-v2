/*
 * DataField.h
 *
 *  Created on: Apr 2, 2017
 *      Author: rmerriam
 */

#ifndef DataField_H_
#define DataField_H_

#include <charconv>
#include <iostream>
#include <string>
using namespace std;

#include "NField.h"

using RvrMsg = std::basic_string<uint8_t>;

namespace scr {
    //--------------------------------------------------------------------------------------------------------------------------
    class DataField : public NField {
    public:
        DataField(uint8_t const height, uint8_t const width, uint8_t const row, uint8_t const col, uint8_t const nmem = 0,
            uint8_t const nbuf = 0);

        template <typename T>
        void setData(T const& data);
        void setData(string const& data);
        void setData(RvrMsg const& data);
        void setData(bool const& data);

    };
    //--------------------------------------------------------------------------------------------------------------------------
    template <typename T>
    inline void DataField::setData(T const& data) {
        string txt { to_string(data) };
        write(txt);
    }
} /* namespace scr */

#endif /* DataField_H_ */
