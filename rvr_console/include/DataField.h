/*
 * DataField.h
 *
 *  Created on: Apr 2, 2017
 *      Author: rmerriam
 */

#ifndef DataField_H_
#define DataField_H_

#include <string>
using namespace std;

#include "NField.h"

namespace scr {
    //--------------------------------------------------------------------------------------------------------------------------
    class DataField : public NField {
    public:
        DataField(const int height, const int width, const int row, const int col, const int nmem = 0, const int nbuf = 0);

        template <typename T>
        void setData(const T& data);
        void setData(const string& data);
        void setData(const bool& data);

    };
    //--------------------------------------------------------------------------------------------------------------------------
    template <typename T>
    inline void DataField::setData(const T& data) {
        string txt { to_string(data) };
        write(txt);
    }
    //--------------------------------------------------------------------------------------------------------------------------
    inline void DataField::setData(const string& data) {
        write(data);
    }
    //--------------------------------------------------------------------------------------------------------------------------
    inline void DataField::setData(const bool& data) {
        string txt { data ? "true" : "false" };
        write(txt);
    }

} /* namespace scr */

#endif /* DataField_H_ */
