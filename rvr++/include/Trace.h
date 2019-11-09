#ifndef TRACE_H_
#define TRACE_H_
//======================================================================================================================
// 2019 Copyright Mystic Lake Software
//
// This is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//======================================================================================================================
//
//     Author: rmerriam
//
//     Created: Nov 8, 2019
//
//======================================================================================================================
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <ostream>
#include <vector>
//---------------------------------------------------------------------------------------------------------------------
namespace mys {
    constexpr char tab { '\t' };
    constexpr char nl { '\n' };
    constexpr char sp { ' ' };
    //=====================================================================================================================
    class Trace {
    public:
        Trace(std::ostream& os);
        Trace(const Trace& other) = delete;
        Trace(Trace&& other) = delete;
        Trace& operator=(const Trace& other) = delete;

        template <typename T>
        Trace& operator<<(T&& value);

        template <typename T>
        Trace& operator<<(const std::vector<T>& data);

        void off();
        void on();

    protected:
        std::ostream &mOs;
    };
    //=====================================================================================================================
    class TraceStart : public Trace {
    public:
        TraceStart(std::ostream& os);
        TraceStart(const Trace& other) = delete;
        TraceStart(Trace&& other) = delete;
        TraceStart& operator=(const Trace& other) = delete;

        template <typename T>
        mys::Trace& operator<<(T& value);

        template <typename T>
        Trace& operator<<(const std::vector<T>& data);

    private:
        Trace &mTrace;

        void time_stamp();
    };
    //=====================================================================================================================
    class TraceOff {
    public:
        TraceOff(Trace& t);
        ~TraceOff();
    private:
        Trace &mTrace;
    };
    //=====================================================================================================================
    class TraceOn {
    public:
        TraceOn(Trace& t);
        ~TraceOn();
    private:
        Trace &mTrace;
    };
//=====================================================================================================================
#define code_loc __func__ << ':' << __LINE__ << mys::sp << __FILE__
    extern mys::TraceStart terr;
//=====================================================================================================================
    inline Trace::Trace(std::ostream& os) :
        mOs { os } {
    }
    //----------------------------------------------------------------------------------------------------------------------
    template <typename T>
    inline mys::Trace& Trace::operator <<(const std::vector<T>& data) {
        std::copy(data.begin(), data.end(), std::ostream_iterator<int>(std::cerr, " "));
        std::cerr << nl;
        return *this;
    }

    //---------------------------------------------------------------------------------------------------------------------
    template <typename T>
    inline mys::Trace& Trace::operator <<(T&& value) {
        mOs << value;
        return *this;
    }
    //---------------------------------------------------------------------------------------------------------------------
    inline void Trace::off() {
        mOs.setstate(std::ios_base::badbit);
    }
    //---------------------------------------------------------------------------------------------------------------------
    inline void Trace::on() {
        mOs.clear();
    }
    //---------------------------------------------------------------------------------------------------------------------
    inline TraceStart::TraceStart(std::ostream& os) :
        Trace { os }, mTrace { *this } {
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <typename T>
    inline mys::Trace& TraceStart::operator <<(T& value) {
        time_stamp();
        mOs << value;
        return mTrace;
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <typename T>
    inline Trace& TraceStart::operator <<(const std::vector<T>& data) {
        time_stamp();
        Trace::operator<<(data);
        return mTrace;
    }
    //---------------------------------------------------------------------------------------------------------------------
    inline void TraceStart::time_stamp() {
        using namespace std::chrono;
        auto now = system_clock::now();
        auto timer = system_clock::to_time_t(now);

        mTrace << std::put_time(std::localtime( &timer), "%n%y%m%d.%H%M%S."); //
        mTrace << std::setfill('0') << std::setw(3);
        mTrace << (duration_cast<milliseconds>(now.time_since_epoch()) % 1000).count() << ": ";
    }
    //---------------------------------------------------------------------------------------------------------------------
    inline TraceOff::TraceOff(Trace& t) :
        mTrace(t) {
        mTrace.off();
    }
    //---------------------------------------------------------------------------------------------------------------------
    inline TraceOff::~TraceOff() {
        mTrace.on();
    }
    //---------------------------------------------------------------------------------------------------------------------
    inline TraceOn::~TraceOn() {
        mTrace.off();
    }
    //---------------------------------------------------------------------------------------------------------------------
    inline TraceOn::TraceOn(Trace& t) :
        mTrace(t) {
        mTrace.on();
    }
}
#endif
