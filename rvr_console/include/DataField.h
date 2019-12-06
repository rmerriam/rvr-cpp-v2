/*
 * DataField.h
 *
 *  Created on: Apr 2, 2017
 *      Author: rmerriam
 */

#ifndef DataField_H_
#define DataField_H_

#include <charconv>
#include <string>
using namespace std;

#include "NField.h"

namespace scr {
    //--------------------------------------------------------------------------------------------------------------------------
    class DataField : public NField {
    public:
        DataField(int const height, int const width, int const row, int const col, int const nmem = 0, int const nbuf = 0);

        template <typename T>
        void setData(T const& data);
        void setData(string const& data);
        void setData(u8string const& data);
        void setData(bool const& data);

    };
    //--------------------------------------------------------------------------------------------------------------------------
    template <typename T>
    inline void DataField::setData(T const& data) {
        string txt { to_string(data) };
        write(txt);
    }
    //--------------------------------------------------------------------------------------------------------------------------
    inline void DataField::setData(string const& data) {
        write(data);
    }
    //--------------------------------------------------------------------------------------------------------------------------
    inline void DataField::setData(u8string const& data) {
        string s { data.begin(), data.end() };
        write(s);
    }
    //--------------------------------------------------------------------------------------------------------------------------
    inline void DataField::setData(bool const& data) {
        string txt { data ? "true" : "false" };
        write(txt);
    }

} /* namespace scr */

#endif /* DataField_H_ */
