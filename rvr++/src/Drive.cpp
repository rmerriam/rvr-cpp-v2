#include "Drive.h"

#include "Request.h"
namespace rvr {
    //----------------------------------------------------------------------------------------------------------------------
    const std::tuple<unsigned char, unsigned char> Drive::speed_mode(const double& speed) const {
        uint8_t s { static_cast<uint8_t>(abs(speed) / 100.0 * 256) };
        uint8_t m { static_cast<uint8_t>((speed > 0) ? forward : ((speed < 0) ? reverse : off)) };
        return std::tie(s, m);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Drive::drive(const double& left, const double& right, const CommandResponse want_resp) {
        auto [l_speed, l_mode] = speed_mode(left);
        auto [r_speed, r_mode] = speed_mode(right);

        MsgArray msg { l_mode, l_speed, r_mode, r_speed };
        cmd_data(raw_motors, msg, want_resp);
//        MsgArray msg { buildFlags(want_resp), mTarget, mDevice, raw_motors, mRequest.sequence(), l_mode, l_speed, r_mode, r_speed };
//        mRequest.send(msg);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Drive::spin_drive(const double& speed, const int& heading, const CommandResponse want_resp) {
        auto [spd, mode] = speed_mode(speed);
        MsgArray msg { spd, static_cast<uint8_t>(heading >> 8), static_cast<uint8_t>(heading & 0xFF), mode };
        cmd_data(drive_with_heading, msg, want_resp);

//        MsgArray msg { buildFlags(want_resp), mTarget, mDevice, drive_with_heading, mRequest.sequence(), //
//            spd, static_cast<uint8_t>(heading >> 8), static_cast<uint8_t>(heading & 0xFF), mode };
//        mRequest.send(msg);
    }
}
