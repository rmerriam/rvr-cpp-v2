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
#include <optional>
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

        template <typename T>
        void setData(std::optional<T> const& data);
        void setData(std::optional<string> const& data);
        void setData(std::optional<RvrMsg> const& data);

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
    //--------------------------------------------------------------------------------------------------------------------------
    template <typename T>
    inline void DataField::setData(std::optional<T> const& data) {
        string txt { to_string((data ? data.value() : T { })) };
        write(txt);
    }
    //--------------------------------------------------------------------------------------------------------------------------
    inline void DataField::setData(std::optional<string> const& data) {
        if (data) {
            string txt { data.value().begin(), data.value().end() };
            write(txt);
        }
        else {
            write("");
        }
    }
    //--------------------------------------------------------------------------------------------------------------------------
    inline void DataField::setData(std::optional<RvrMsg> const& data) {
        if (data) {
            string txt { data.value().begin(), data.value().end() };
            write(txt);
        }
        else {
            write("");
        }
    }

} /* namespace scr */

#endif /* DataField_H_ */