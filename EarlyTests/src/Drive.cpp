#include "Drive.h"

#include "Request.h"
namespace rvr {
    //----------------------------------------------------------------------------------------------------------------------
    inline std::tuple<unsigned char, unsigned char> const Drive::speed_mode(double const& speed) const {
        uint8_t s { static_cast<uint8_t>(abs(speed) / 100.0 * 256) };
        uint8_t m { static_cast<uint8_t>((speed > 0) ? forward : ((speed < 0) ? reverse : off)) };
        return std::tie(s, m);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Drive::drive(double const& left, double const& right, bool const want_resp) {
        auto [l_speed, l_mode] = speed_mode(left);
        auto [r_speed, r_mode] = speed_mode(right);

        MsgArray msg { buildFlags(want_resp), mTarget, mDevice, raw_motors, mRequest.sequence(), l_mode, l_speed, r_mode, r_speed };
        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Drive::spin_drive(double const& speed, int const& heading, bool const want_resp) {
        auto [spd, mode] = speed_mode(speed);

        MsgArray msg { buildFlags(want_resp), mTarget, mDevice, drive_with_heading, mRequest.sequence(), //
            spd, static_cast<uint8_t>(heading >> 8), static_cast<uint8_t>(heading & 0xFF), mode };
        mRequest.send(msg);
    }
}