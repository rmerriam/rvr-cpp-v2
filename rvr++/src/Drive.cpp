#include "Drive.h"

#include "Request.h"
namespace rvr {
    //----------------------------------------------------------------------------------------------------------------------
    std::tuple<unsigned char, unsigned char> const Drive::speed_mode(double const& speed) const {
        uint8_t s { static_cast<uint8_t>(abs(speed) / 100.0 * 256) };
        uint8_t m { static_cast<uint8_t>((speed > 0) ? forward : ((speed < 0) ? reverse : off)) };
        return std::tie(s, m);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Drive::drive(double const& left, double const& right, CommandResponse const want_resp) const {
        auto [l_speed, l_mode] = speed_mode(left);
        auto [r_speed, r_mode] = speed_mode(right);

        RvrMsg msg { l_mode, l_speed, r_mode, r_speed };
        cmd_data(raw_motors, msg, want_resp);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Drive::spin_drive(double const& speed, int const& heading, CommandResponse const want_resp) const {
        auto [spd, mode] = speed_mode(speed);
        RvrMsg msg { spd, static_cast<uint8_t>(heading >> 8), static_cast<uint8_t>(heading & 0xFF), mode };
        cmd_data(drive_with_heading, msg, want_resp);
    }
}
