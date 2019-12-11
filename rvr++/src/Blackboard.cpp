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
//     Created: Nov 10, 2019
//
//======================================================================================================================
#include <limits>
#include <string_view>

#include "Trace.h"

#include "Blackboard.h"

#include "CommandBase.h"
#include "ApiShell.h"
#include "Power.h"
#include "SensorsDirect.h"
#include "SensorsStream.h"

namespace rvr {

    using bb = Blackboard;
    //----------------------------------------------------------------------------------------------------------------------
    Blackboard::Blackboard() {
    }
    //----------------------------------------------------------------------------------------------------------------------
    using dev = Devices;
    constexpr CommandBase::TargetPort nordic = CommandBase::nordic;
    constexpr CommandBase::TargetPort btc = CommandBase::bluetoothSOC;

    bb::BBDictionary bb::mDictionary { //
//
    { entryKey(btc, dev::api_and_shell, 0x00), BlackboardEntry { "echo" } }, //
    { entryKey(nordic, dev::api_and_shell, 0x00), BlackboardEntry { "echo" } }, //
//
    { entryKey(nordic, dev::connection, 0x05), BlackboardEntry { "get_bluetooth_advertising_name" } }, //
#if 1
//
    { entryKey(btc, dev::drive, 0x01), BlackboardEntry { "raw_motors" } }, //
    { entryKey(btc, dev::drive, 0x06), BlackboardEntry { "reset_yaw" } }, //
    { entryKey(btc, dev::drive, 0x07), BlackboardEntry { "drive_with_heading" } }, //
    { entryKey(btc, dev::drive, 0x25), BlackboardEntry { "enable_motor_stall_notify" } }, //
    { entryKey(btc, dev::drive, 0x26), BlackboardEntry { "motor_stall_notify" } }, //
    { entryKey(btc, dev::drive, 0x27), BlackboardEntry { "enable_motor_fault_notify" } }, //
    { entryKey(btc, dev::drive, 0x28), BlackboardEntry { "motor_fault_notify" } }, //
    { entryKey(btc, dev::drive, 0x29), BlackboardEntry { "get_motor_fault_state" } },
//
    { entryKey(nordic, dev::io_led, 0x1A), BlackboardEntry { "set_all_leds" } }, //
    { entryKey(nordic, dev::io_led, 0x4E), BlackboardEntry { "release_led_requests" } }, //
//
    { entryKey(nordic, dev::power, 0x00), BlackboardEntry { "power off" } }, //
    { entryKey(nordic, dev::power, 0x01), BlackboardEntry { "snooze" } }, //
    { entryKey(nordic, dev::power, 0x0D), BlackboardEntry { "wake" } }, //
    { entryKey(nordic, dev::power, 0x10), BlackboardEntry { "get_battery_percentage" } }, //
    { entryKey(nordic, dev::power, 0x11), BlackboardEntry { "system_awake_notify" } }, //
    { entryKey(nordic, dev::power, 0x17), BlackboardEntry { "get_battery_voltage_state" } }, //
    { entryKey(nordic, dev::power, 0x19), BlackboardEntry { "will_sleep_notify" } }, //
    { entryKey(nordic, dev::power, 0x1A), BlackboardEntry { "did_sleep_notify" } }, //
    { entryKey(nordic, dev::power, 0x1B), BlackboardEntry { "enable_battery_voltage_state_change_notify" } }, //
    { entryKey(nordic, dev::power, 0x1C), BlackboardEntry { "battery_voltage_state_change_notify" } }, //
    { entryKey(nordic, dev::power, 0x25, Power::CalibratedFiltered), BlackboardEntry { "get_battery_voltage_in_volts" } }, //
    { entryKey(nordic, dev::power, 0x25, Power::CalibratedUnfiltered), BlackboardEntry { "get_battery_voltage_in_volts" } }, //
    { entryKey(nordic, dev::power, 0x25, Power::UncalibratedUnfiltered), BlackboardEntry { "get_battery_voltage_in_volts" } }, //
    { entryKey(nordic, dev::power, 0x26), BlackboardEntry { "get_battery_voltage_state_thresholds" } }, //
    { entryKey(nordic, dev::power, 0x26, 1), BlackboardEntry { "get_battery_voltage_state_thresholds" } }, //
    { entryKey(nordic, dev::power, 0x26, 2), BlackboardEntry { "get_battery_voltage_state_thresholds" } }, //
    { entryKey(btc, dev::power, 0x27), BlackboardEntry { "get_current_sense_amplifier_current left" } }, //
    { entryKey(btc, dev::power, 0x27, 1), BlackboardEntry { "get_current_sense_amplifier_current right" } }, //
//
    { entryKey(btc, dev::sensors, 0x0F), BlackboardEntry { "enable_gyro_max_notify" } }, //
    { entryKey(btc, dev::sensors, 0x10), BlackboardEntry { "gyro_max_notify" } }, //
    { entryKey(btc, dev::sensors, 0x13), BlackboardEntry { "reset_locator_x_and_y" } }, //
    { entryKey(btc, dev::sensors, 0x17), BlackboardEntry { "set_locator_flags " } }, //
    { entryKey(btc, dev::sensors, 0x22), BlackboardEntry { "get_bot_to_bot_infrared_readings " } }, //
    { entryKey(nordic, dev::sensors, 0x23), BlackboardEntry { "get_rgbc_sensor_values" } }, //
    { entryKey(btc, dev::sensors, 0x27), BlackboardEntry { "start_robot_to_robot_infrared_broadcasting" } }, //
    { entryKey(btc, dev::sensors, 0x28), BlackboardEntry { "start_robot_to_robot_infrared_following" } }, //
    { entryKey(btc, dev::sensors, 0x29), BlackboardEntry { "stop_robot_to_robot_infrared_broadcasting" } }, //
    { entryKey(btc, dev::sensors, 0x2C), BlackboardEntry { "robot_to_robot_infrared_message_received_notify" } }, //
    { entryKey(nordic, dev::sensors, 0x30), BlackboardEntry { "get_ambient_light_sensor_value" } }, //
    { entryKey(btc, dev::sensors, 0x32), BlackboardEntry { "stop_robot_to_robot_infrared_following" } }, //
    { entryKey(btc, dev::sensors, 0x33), BlackboardEntry { "start_robot_to_robot_infrared_evading" } }, //
    { entryKey(btc, dev::sensors, 0x34), BlackboardEntry { "stop_robot_to_robot_infrared_evading" } }, //

    { entryKey(nordic, dev::sensors, 0x35), BlackboardEntry { "enable_color_detection_notify" } }, //
    { entryKey(nordic, dev::sensors, 0x36), BlackboardEntry { "color_detection_notify" } }, //
    { entryKey(nordic, dev::sensors, 0x37), BlackboardEntry { "get_current_detected_color_reading" } }, //
    { entryKey(nordic, dev::sensors, 0x38), BlackboardEntry { "enable_color_detection" } }, //

    { entryKey(btc, dev::sensors, 0x39), BlackboardEntry { "configure_streaming_service" } }, //
    { entryKey(nordic, dev::sensors, 0x39), BlackboardEntry { "configure_streaming_service" } }, //
    { entryKey(btc, dev::sensors, 0x3A), BlackboardEntry { "start_streaming_service" } }, //
    { entryKey(nordic, dev::sensors, 0x3A), BlackboardEntry { "start_streaming_service" } }, //
    { entryKey(btc, dev::sensors, 0x3B), BlackboardEntry { "stop_streaming_service" } }, //
    { entryKey(nordic, dev::sensors, 0x3B), BlackboardEntry { "stop_streaming_service" } }, //
    { entryKey(btc, dev::sensors, 0x3C), BlackboardEntry { "clear_streaming_service" } }, //
    { entryKey(nordic, dev::sensors, 0x3C), BlackboardEntry { "clear_streaming_service" } }, //
    { entryKey(btc, dev::sensors, 0x3D), BlackboardEntry { "streaming_service_data_notify" } }, //
    { entryKey(nordic, dev::sensors, 0x3D), BlackboardEntry { "streaming_service_data_notify" } }, //
    //
    { entryKey(nordic, dev::sensors, 0x3D, 3), BlackboardEntry { "color stream" } }, //
    { entryKey(nordic, dev::sensors, 0x3D, 9), BlackboardEntry { "core stream" } }, //
    { entryKey(nordic, dev::sensors, 0x3D, 10), BlackboardEntry { "ambient stream" } }, //

    { entryKey(btc, dev::sensors, 0x3D, 2), BlackboardEntry { "accelerometer stream" } }, //
    { entryKey(btc, dev::sensors, 0x3D, 9), BlackboardEntry { "core stream" } }, //
    { entryKey(btc, dev::sensors, 0x3D, 4), BlackboardEntry { "gyro stream" } }, //
    { entryKey(btc, dev::sensors, 0x3D, 1), BlackboardEntry { "imu stream" } }, //
    { entryKey(btc, dev::sensors, 0x3D, 6), BlackboardEntry { "locator stream" } }, //
    { entryKey(btc, dev::sensors, 0x3D, 11), BlackboardEntry { "quat stream" } }, //
    { entryKey(btc, dev::sensors, 0x3D, 8), BlackboardEntry { "speed stream" } }, //
    { entryKey(btc, dev::sensors, 0x3D, 7), BlackboardEntry { "velocity stream" } }, //
    //
    { entryKey(btc, dev::sensors, 0x3E), BlackboardEntry { "enable_robot_infrared_message_notify" } }, //
    { entryKey(btc, dev::sensors, 0x3F), BlackboardEntry { "send_infrared_message" } }, //
    //
    { entryKey(btc, dev::sensors, 0x4A, 4), BlackboardEntry { "left_motor_temperature" } }, // left
    { entryKey(btc, dev::sensors, 0x4A, 5), BlackboardEntry { "right_motor_temperature" } }, // right
    { entryKey(btc, dev::sensors, 0x4B), BlackboardEntry { "get_motor_thermal_protection_status" } }, //
    { entryKey(btc, dev::sensors, 0x4C), BlackboardEntry { "enable_motor_thermal_protection_status_notify" } }, //
    { entryKey(btc, dev::sensors, 0x4D), BlackboardEntry { "motor_thermal_protection_status_notify" } }, //
    //
    { entryKey(btc, dev::system, 0x00), BlackboardEntry { "nordic_main_application_version" } }, //
    { entryKey(nordic, dev::system, 0x00), BlackboardEntry { "bt_main_application_version" } }, //
    { entryKey(btc, dev::system, 0x01), BlackboardEntry { "nordic_bootloader_version" } }, //
    { entryKey(nordic, dev::system, 0x01), BlackboardEntry { "bt_bootloader_version" } }, //
    { entryKey(nordic, dev::system, 0x03), BlackboardEntry { "board_revision" } }, //
    { entryKey(nordic, dev::system, 0x06), BlackboardEntry { "mac_address" } }, //
    { entryKey(nordic, dev::system, 0x13), BlackboardEntry { "stats_id" } }, //
    { entryKey(btc, dev::system, 0x1F), BlackboardEntry { "nordic_processor_name" } }, //
    { entryKey(nordic, dev::system, 0x1F), BlackboardEntry { "bt_processor_name" } }, //
    { entryKey(nordic, dev::system, 0x38), BlackboardEntry { "get_sku" } }, //
    { entryKey(btc, dev::system, 0x39), BlackboardEntry { "get_core_up_time_in_milliseconds" } }, //
    //

#endif
    };
    //======================================================================================================================
    //  Key methods
    //----------------------------------------------------------------------------------------------------------------------
    Blackboard::key_s::key_s(CommandBase::TargetPort const proc, Devices const dev, uint8_t const cmd, uint8_t const id) :
        id(id), cmd(cmd), dev(dev), proc(proc) {
    }
    //----------------------------------------------------------------------------------------------------------------------
    Blackboard::key_s::key_s(key_t const k) :
        id { static_cast<uint8_t>(k & 0xFF) }, //
            cmd { static_cast<uint8_t>((k >> 8) & 0xFF) }, //
            dev { static_cast<uint8_t>((k >> 16) & 0xFF) }, //
            proc { static_cast<uint8_t>((k >> 24) & 0xFF) } {
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline Blackboard::key_s::operator key_t() {
        return key_t(proc << 24 | (dev << 16) | (cmd << 8) | id);
    }
    //======================================================================================================================
    //  Methods to get dictionary entries
    //----------------------------------------------------------------------------------------------------------------------
    Blackboard::key_t Blackboard::entryKey(CommandBase::TargetPort const proc, Devices const dev, uint8_t const cmd, uint8_t const id) {
        return static_cast<key_t>(key_s(proc, dev, cmd, id));
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::string Blackboard::entryName(key_t key) {
        std::string s;

        auto it { mDictionary.find(key) };

        if (it == mDictionary.end()) {
            it = mDictionary.find(key & 0xFFFFFF00);
        }
        if (it != mDictionary.end()) {
            s = it->second.name;
        }
        return s;
    }
//----------------------------------------------------------------------------------------------------------------------
    RvrMsg& Blackboard::entryValue(CommandBase::TargetPort const proc, Devices const dev, uint8_t const cmd, uint8_t const id) {
        return entryValue(entryKey(proc, dev, cmd, id));
    }
//----------------------------------------------------------------------------------------------------------------------
    RvrMsg& Blackboard::entryValue(key_t const key) {
        RvrMsg& msg { mDictionary[key].value };
        return msg;
    }
//======================================================================================================================
//  Method to put RvrMsg data into dictionary
//----------------------------------------------------------------------------------------------------------------------
    void Blackboard::msgArray(bb::key_t key, RvrMsg::iterator begin, RvrMsg::iterator end) {
        RvrMsg msg { begin, end };
        if (msg.empty()) {
            msg.push_back(0xFF);
        }
        else if (msg.size() >= 2) {
            uint8_t seq { msg.front() };

            if (seq == 0xFF) {
                key &= static_cast<bb::key_t>(0xFFFFFF00);
                key |= static_cast<bb::key_t>(msg[1]);
            }
            else if ((seq < 0x80) && (seq >= 0x04)) {
                // message seq has special flags that are not sequence number (> 0x80) or ids (< enable (0x20) - its a hack
                // because of the protocol
                switch (seq) {
                    // special case for motor temperatures
                    case 4:
                    case 5:
                        msg.erase(msg.begin() + 2);
                        break;

                        // handling enable / disable messages
                    case CommandBase::enable:
                        key &= static_cast<bb::key_t>(0xFFFFFF00);
                        msg[1] = true;
                        break;
                    case CommandBase::disable:
                        key &= static_cast<bb::key_t>(0xFFFFFF00);
                        msg[1] = false;
                        break;
                }
            }

        }
        entryValue(key) = msg;
        terr << code_loc << std::setfill('0') << std::hex << key << mys::sp << msg;
    }
//======================================================================================================================
//  Methods to calculate values from RvrMsg in dictionary
//----------------------------------------------------------------------------------------------------------------------
    float Blackboard::floatConvert(RvrMsg::const_iterator begin) {
        union {
            uint8_t buf[4];
            float result { NaN };
        };
        buf[0] = begin[2];
        buf[1] = begin[3];
        buf[2] = begin[1];
        buf[3] = begin[0];

        return result;
    }
//----------------------------------------------------------------------------------------------------------------------
    uint64_t Blackboard::uintConvert(RvrMsg::const_iterator begin, uint8_t const n) {
        uint64_t res { };
        for (auto it { begin }; it != begin + n; ++it) {
            res <<= 8;
            res |= *it;
        }
        return res;
    }

    RvrMsg fake_msg(3, 0);
//----------------------------------------------------------------------------------------------------------------------
    bool Blackboard::boolValue(CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd) {
        return (byteValue(target, dev, cmd) != 0);
    }
//----------------------------------------------------------------------------------------------------------------------
    uint8_t Blackboard::byteValue(CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd) {
        RvrMsg const& msg { bb::entryValue(target, dev, cmd) };
        uint8_t res { };
        if (msg.size() >= sizeof(uint8_t)) {
            res = msg[2];
        }
        return res;
    }
//----------------------------------------------------------------------------------------------------------------------
// these two methods are for notifications that return a single 0xFF
    bool Blackboard::notifyState(CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd) {
        RvrMsg const& msg { bb::entryValue(target, dev, cmd) };

        bool res { };
        if (msg.size() >= 1) {
            res = msg[0] != 0;
        }
        return res;
    }
//----------------------------------------------------------------------------------------------------------------------
    void Blackboard::resetNotify(CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd) {
        RvrMsg& msg { entryValue(target, dev, cmd) };
        msg = fake_msg;
    }
//----------------------------------------------------------------------------------------------------------------------
    uint16_t Blackboard::uintValue(CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd, uint8_t const pos) {

        RvrMsg const& msg { bb::entryValue(target, dev, cmd) };

        auto begin { msg.begin() + 2 };
        begin += (pos * sizeof(uint16_t));

        uint16_t res { };
//                if (msg.size() >= sizeof(uint16_t))
        {
            res = uintConvert(begin, sizeof(uint16_t));
        }
        return res;
    }
//----------------------------------------------------------------------------------------------------------------------
    int16_t Blackboard::intValue(CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd) {
        return static_cast<int16_t>(uintValue(target, dev, cmd));
    }
    //----------------------------------------------------------------------------------------------------------------------
    uint32_t Blackboard::uint32Value(CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd, uint8_t const id) {
        RvrMsg const& msg { bb::entryValue(target, dev, cmd, id) };
        uint32_t res { };

        if (msg.size() >= sizeof(uint32_t)) {
            res = uintConvert(msg.begin() + 2, sizeof(uint32_t));
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    uint64_t Blackboard::uint64Value(CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd) {
        RvrMsg const& msg { bb::entryValue(target, dev, cmd) };
        uint64_t res { };

        if (msg.size() >= sizeof(uint64_t)) {
            res = uintConvert(msg.begin() + 2, sizeof(uint64_t));
        }
        return res;
    }
//----------------------------------------------------------------------------------------------------------------------
    float Blackboard::floatValue(CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd, uint8_t pos,
        uint8_t const id) {
        RvrMsg const& msg { entryValue(target, dev, cmd, id) };

        auto begin { msg.begin() + 2 };
        begin += (pos * 4);

        float result { };
        {
            result = floatConvert(begin);
        }
        return result;
    }
//----------------------------------------------------------------------------------------------------------------------
    bool Blackboard::getNotify(CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd) {
        RvrMsg msg { entryValue(target, dev, cmd) };
        if (msg.empty()) {
            msg = fake_msg;
        }
        return msg[1] != 0;
    }
//----------------------------------------------------------------------------------------------------------------------
    std::string Blackboard::stringValue(CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd) {
        RvrMsg const& msg { entryValue(target, dev, cmd) };
        return std::string { msg.begin() + 2, msg.end() - 1 };    // response is a zero terminated C string
    }
//----------------------------------------------------------------------------------------------------------------------
    RvrMsg const Blackboard::msgValue(CommandBase::TargetPort const target, Devices const dev, uint8_t const cmd, uint8_t const id) {
        RvrMsg const& msg { entryValue(target, dev, cmd, id) };
        return RvrMsg { msg.begin() + 2, msg.end() };
    }
//======================================================================================================================
    std::ostream& operator <<(std::ostream& os, Blackboard::key_s const& k) {
        os << k.proc << mys::sp << k.dev << mys::sp << k.cmd << mys::sp << k.id;
        return os;
    }
//----------------------------------------------------------------------------------------------------------------------
    void Blackboard::m_to_v() {
        struct v_map {
            Blackboard::key_t key;
            BlackboardEntry be;
        };
        std::vector<v_map> vec;
        for (auto b : rvr::Blackboard::mDictionary) {
            v_map v { b.first, b.second };
            vec.push_back(v);
        }
        std::sort(vec.begin(), vec.end(), //
                  [](v_map const& a, v_map const& b) {
                      return a.key < b.key;
                  }
        );
        for (auto& i : vec) {
            terr << std::hex << std::uppercase << i.key << mys::tab << std::setw(45) << std::setfill(' ') << std::left << i.be.name << //
                 mys::tab << mys::tab << i.be.value;
        }
    }

}
/* namespace rvr */
