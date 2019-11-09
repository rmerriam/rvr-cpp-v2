/*
 * trace.h
 *
 *  Created on: Jan 3, 2017
 *      Author: rmerriam
 */

#ifndef trace_H_
#define trace_H_
#include <iostream>
#include <iomanip>
#include <iterator>
#include <vector>

namespace mys {
//---------------------------------------------------------------------------------------------------------------------
    struct trace_off {
        trace_off() {
            std::cerr.setstate(std::ios_base::badbit);
        }
        ~trace_off() {
            std::cerr.clear();
        }
    };
//---------------------------------------------------------------------------------------------------------------------
    struct trace_on {
        trace_on() {
            std::cerr.clear();
        }
        ~trace_on() {
            std::cerr.setstate(std::ios_base::badbit);
        }
    };
//---------------------------------------------------------------------------------------------------------------------
    constexpr char tab { '\t' };
    constexpr char nl { '\n' };
    constexpr char sp { ' ' };
    //---------------------------------------------------------------------------------------------------------------------
    inline std::ostream& trace(std::ostream& os) {
        return os;
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <typename T>
    inline std::ostream& trace(std::ostream& os, const T& v) {
        os << '[' << v << ']';
        return os;
    }
    //---------------------------------------------------------------------------------------------------------------------
    inline std::ostream& trace(std::ostream& os, const auto& v1, const auto& v2) {
        os << v1 << mys::tab << v2 << mys::tab;
        return os;
    }
//---------------------------------------------------------------------------------------------------------------------
    template <typename T>
    std::ostream& trace(std::ostream& os, const T x, const T y, const T z) {
        os << '[' << x << ", " << y << ", " << z << ']';
        return os;
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <typename T>
    std::ostream& trace(std::ostream& os, const T x, const T y, const T z, const T w) {
        os << '[' << x << ", " << y << ", " << z << ", " << w << ']';
        return os;
    }
    //---------------------------------------------------------------------------------------------------------------------
    std::ostream& trace(auto& os, const auto& x, const auto& y) {
        os << '[' << x << ", " << y << ']';
        return os;
    }
    //---------------------------------------------------------------------------------------------------------------------
    inline std::ostream& trace(const auto& v1, const auto& v2, const auto& v3) {
        std::cerr << v1 << sp << v2 << mys::tab << v3 << mys::tab;
        return std::cerr;
    }
    //---------------------------------------------------------------------------------------------------------------------
    inline std::ostream& trace() {
        return trace(std::cerr);
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <typename T>
    inline std::ostream& trace(const T& v) {
//        std::cerr << '[' << v << ']';
//        return std::cerr;
        return trace(std::cerr, v);
    }
//---------------------------------------------------------------------------------------------------------------------
    inline std::ostream& trace(const int& depth) {
        std::cerr << std::setw(depth * 3) << " ";
        return std::cerr;
    }
//---------------------------------------------------------------------------------------------------------------------
    template <typename T>
    std::ostream& trace(const T x, const T y, const T z) {
        std::cerr << '[' << x << ", " << y << ", " << z << ']';
        return std::cerr;
    }
//---------------------------------------------------------------------------------------------------------------------
    template <typename T>
    std::ostream& trace(const T x, const T y, const T z, const T w) {
        std::cerr << '[' << x << ", " << y << ", " << z << ", " << w << ']';
        return std::cerr;
    }

//----------------------------------------------------------------------------------------------------------------------
    template <typename T>
    std::ostream& trace(const std::vector<T>& data) {
        std::copy(data.begin(), data.end(), std::ostream_iterator<int>(std::cerr, " "));
        std::cerr << nl;
        return std::cerr;
    }
}

#endif /* trace_H_ */
