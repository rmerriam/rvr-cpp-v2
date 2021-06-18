#ifndef TRACE_H_
#define TRACE_H_
//======================================================================================================================
// 2021 Copyright Mystic Lake Software
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
//     Created: May 29, 2021
//
//======================================================================================================================
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <mutex>
#include <ostream>
#include <string>
#include <type_traits>
#include <vector>
//---------------------------------------------------------------------------------------------------------------------
#if 1
constexpr bool trace_active { true };
#else
constexpr bool trace_active { false };
#endif

namespace mys {
//
//    template <typename T, typename = std::void_t<>>
//    struct has_value_type : std::false_type {
//    };
//
//    template <typename T>
//    struct has_value_type<T, std::void_t<typename T::value_type>> : std::true_type {
//    };
//
    constexpr char tab { '\t' };
    constexpr char nl { '\n' };
    constexpr char sp { ' ' };
    constexpr char comma[] { ", " };
    //=====================================================================================================================
    class Trace {
    protected:
        explicit Trace(std::ostream& os) :
            mOs { os } {
            mOs << std::boolalpha << std::fixed << std::setprecision(2) << std::uppercase;
        }
    public:
        Trace(Trace const& other) = delete;
        Trace(Trace&& other) = delete;
        Trace& operator=(Trace const& other) = delete;

        template <typename C>
        Trace& operator<<(C const& data);

        friend struct OnOffBase;
        friend class TraceOff;
        friend class TraceOn;

    protected:
        bool mEnabled { true };
        std::ostream& mOs;
//        std::mutex mOutputMutex;

        void off() {
            mEnabled = false;
        }
        void on() {
            mEnabled = true;
        }
    };
    //=====================================================================================================================
    class TraceStart : public Trace {
    public:
        TraceStart(std::ostream& os, char const ch);
        TraceStart(Trace const& other) = delete;
        TraceStart(Trace&& other) = delete;
        TraceStart& operator=(Trace const& other) = delete;

        template <typename T>
        mys::Trace& operator<<(T const& value);

    private:
        Trace& mTrace;
        char const mType;
        int16_t mCnt { };

        void time_stamp();
    };
    //=====================================================================================================================
    struct OnOffBase {
        OnOffBase(Trace& t) :
            mTrace { t }, mPrevEnable { mTrace.mEnabled } {
        }
        ~OnOffBase() {
            mTrace.mEnabled = mPrevEnable;
        }
        Trace& mTrace;
        bool const mPrevEnable;
    };
    //=====================================================================================================================
    class TraceOff : protected OnOffBase {
    public:
        TraceOff(Trace& t);
    };
    //=====================================================================================================================
    class TraceOn : protected OnOffBase {
    public:
        TraceOn(Trace& t);
    };
    //=====================================================================================================================
    template <typename C>
    inline mys::Trace& Trace::operator <<(C const& data) {
        if constexpr (trace_active) {
            if (mEnabled) {
                mOs << data;
            }
        }
        return *this;
    }
    //----------------------------------------------------------------------------------------------------------------------
    template <>
    inline mys::Trace& Trace::operator <<(std::string const& data) {
        if constexpr (trace_active) {
            if (mEnabled) {
                mOs << data;
            }
        }
        return *this;
    }
    //---------------------------------------------------------------------------------------------------------------------

    template <>
    inline mys::Trace& Trace::operator <<(std::basic_string<uint8_t> const& data) {
        if constexpr (trace_active) {
            if (mEnabled) {
                mOs << std::hex;
                for (auto const& c : data) {
                    mOs << (uint16_t)c << ' ';
                }
            }
        }
        return *this;
    } //---------------------------------------------------------------------------------------------------------------------
    inline TraceStart::TraceStart(std::ostream& os, char const ch) :
        Trace { os }, mTrace { *this }, mType { ch } {
    }
    //---------------------------------------------------------------------------------------------------------------------
    template <typename T>
    inline mys::Trace& TraceStart::operator <<(T const& value) {
        if constexpr (trace_active) {
            if (mEnabled) {
                time_stamp();
                mOs << value;
            }
        }
        return *this;
    }
    //---------------------------------------------------------------------------------------------------------------------
    inline void TraceStart::time_stamp() {
        if constexpr (trace_active) {
            using namespace std::chrono;
            auto now = system_clock::now();
            auto timer = system_clock::to_time_t(now);

            mOs << std::dec << nl << mType << ',';
            mOs << std::put_time(std::localtime( &timer), "%y%m%d.%H%M%S."); //
            mOs << std::setfill('0') << std::setw(3) << (duration_cast<milliseconds>(now.time_since_epoch()) % 1000).count()
                << ": ";
        }
    }
    //---------------------------------------------------------------------------------------------------------------------
    inline TraceOff::TraceOff(Trace& t) :
        OnOffBase { t } {
        mTrace.off();
    }
    //---------------------------------------------------------------------------------------------------------------------
    inline TraceOn::TraceOn(Trace& t) :
        OnOffBase { t } {
        mTrace.on();
    }
    //---------------------------------------------------------------------------------------------------------------------
    inline TraceStart tdbg { std::cerr, 'D' };
    inline TraceStart tlog { std::clog, 'L' };
    inline TraceStart tinfo { std::cout, 'I' };
}
//=====================================================================================================================
#define code_loc __FUNCTION__ << mys::tab
#define code_line __FUNCTION__ << mys::sp << __LINE__ << mys::tab
#define code_entry __FUNCTION__<< " entry"  << mys::tab
#define code_exit __FUNCTION__ << " exit" << mys::tab
#define code_return __FUNCTION__ << " return" << mys::tab

#endif
