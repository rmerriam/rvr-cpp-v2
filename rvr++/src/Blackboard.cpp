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
// along with this program.  If not, see <http:                                  //www.gnu.org/licenses/>.
//======================================================================================================================
//
//     Author: rmerriam
//
//     Created: Nov 10, 2019
//
//======================================================================================================================
#include <Request.h>
#include <limits>
#include <optional>
#include <string_view>

#include "Trace.h"

#include "Blackboard.h"

#include "ApiShell.h"
#include "Power.h"
#include "SensorsDirect.h"
#include "SensorsStream.h"

namespace rvr {

    //----------------------------------------------------------------------------------------------------------------------
    Blackboard::Blackboard() {
    }
    //----------------------------------------------------------------------------------------------------------------------
    using dev = Devices;
    using bb = Blackboard;

    Blackboard::BBDictionary Blackboard::mDictionary {                                   //
    { bb::entryKey(bluetoothSOC, dev::api_and_shell, 0x00), bb::BlackboardEntry { "echo" } },                                   //
    { bb::entryKey(nordic, dev::api_and_shell, 0x00), bb::BlackboardEntry { "echo" } },                                   //
    //
    { bb::entryKey(nordic, dev::connection, 0x05), bb::BlackboardEntry { "get_bluetooth_advertising_name" } },                            //
    //
    { bb::entryKey(bluetoothSOC, dev::drive, 0x01), bb::BlackboardEntry { "raw_motors" } },                                   //
    { bb::entryKey(bluetoothSOC, dev::drive, 0x06), bb::BlackboardEntry { "reset_yaw" } },                                   //
    { bb::entryKey(bluetoothSOC, dev::drive, 0x07), bb::BlackboardEntry { "drive_with_heading" } },                                   //
    { bb::entryKey(bluetoothSOC, dev::drive, 0x25), bb::BlackboardEntry { "enable_motor_stall_notify" } },                                //
    { bb::entryKey(bluetoothSOC, dev::drive, 0x26), bb::BlackboardEntry { "motor_stall_notify" } },                                   //
    { bb::entryKey(bluetoothSOC, dev::drive, 0x27), bb::BlackboardEntry { "enable_motor_fault_notify" } },                                //
    { bb::entryKey(bluetoothSOC, dev::drive, 0x28), bb::BlackboardEntry { "motor_fault_notify" } },                                   //
    { bb::entryKey(bluetoothSOC, dev::drive, 0x29), bb::BlackboardEntry { "get_motor_fault_state" } },
    //
    { bb::entryKey(nordic, dev::io_led, 0x1A), bb::BlackboardEntry { "set_all_leds" } },                                   //
    { bb::entryKey(nordic, dev::io_led, 0x4E), bb::BlackboardEntry { "release_led_requests" } },                                   //
    //
    { bb::entryKey(nordic, dev::power, 0x00), bb::BlackboardEntry { "power off" } },                                   //
    { bb::entryKey(nordic, dev::power, 0x01), bb::BlackboardEntry { "snooze" } },                                   //
    { bb::entryKey(nordic, dev::power, 0x0D), bb::BlackboardEntry { "wake" } },                                   //
    { bb::entryKey(nordic, dev::power, 0x10), bb::BlackboardEntry { "get_battery_percentage" } },                                   //
    { bb::entryKey(nordic, dev::power, 0x11), bb::BlackboardEntry { "system_awake_notify" } },                                   //
    { bb::entryKey(nordic, dev::power, 0x17), bb::BlackboardEntry { "get_battery_voltage_state" } },                                   //
    { bb::entryKey(nordic, dev::power, 0x19), bb::BlackboardEntry { "will_sleep_notify" } },                                   //
    { bb::entryKey(nordic, dev::power, 0x1A), bb::BlackboardEntry { "did_sleep_notify" } },                                   //
    { bb::entryKey(nordic, dev::power, 0x1B), bb::BlackboardEntry { "enable_battery_voltage_state_change_notify" } },                     //
    { bb::entryKey(nordic, dev::power, 0x1C), bb::BlackboardEntry { "battery_voltage_state_change_notify" } },                            //
    { bb::entryKey(nordic, dev::power, 0x25, Power::CalibratedFiltered), bb::BlackboardEntry { "get_battery_voltage_in_volts" } },        //
    { bb::entryKey(nordic, dev::power, 0x25, Power::CalibratedUnfiltered), bb::BlackboardEntry { "get_battery_voltage_in_volts" } },      //
    { bb::entryKey(nordic, dev::power, 0x25, Power::UncalibratedUnfiltered), bb::BlackboardEntry { "get_battery_voltage_in_volts" } },    //
    { bb::entryKey(nordic, dev::power, 0x26), bb::BlackboardEntry { "get_battery_voltage_state_thresholds" } },                           //
    { bb::entryKey(nordic, dev::power, 0x26, 1), bb::BlackboardEntry { "get_battery_voltage_state_thresholds" } },                        //
    { bb::entryKey(nordic, dev::power, 0x26, 2), bb::BlackboardEntry { "get_battery_voltage_state_thresholds" } },                        //
    { bb::entryKey(bluetoothSOC, dev::power, 0x27), bb::BlackboardEntry { "get_current_sense_amplifier_current left" } },                 //
    { bb::entryKey(bluetoothSOC, dev::power, 0x27, 1), bb::BlackboardEntry { "get_current_sense_amplifier_current right" } },             //
    //
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x0F), bb::BlackboardEntry { "enable_gyro_max_notify" } },                                 //
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x10), bb::BlackboardEntry { "gyro_max_notify" } },                                   //
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x13), bb::BlackboardEntry { "reset_locator_x_and_y" } },                                  //
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x17), bb::BlackboardEntry { "set_locator_flags " } },                                   //
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x22), bb::BlackboardEntry { "get_bot_to_bot_infrared_readings " } },                      //
    { bb::entryKey(nordic, dev::sensors, 0x23), bb::BlackboardEntry { "get_rgbc_sensor_values" } },                                   //
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x27), bb::BlackboardEntry { "start_robot_to_robot_infrared_broadcasting" } },             //
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x28), bb::BlackboardEntry { "start_robot_to_robot_infrared_following" } },                //
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x29), bb::BlackboardEntry { "stop_robot_to_robot_infrared_broadcasting" } },              //
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x2C), bb::BlackboardEntry { "robot_to_robot_infrared_message_received_notify" } },        //
    { bb::entryKey(nordic, dev::sensors, 0x30), bb::BlackboardEntry { "get_ambient_light_sensor_value" } },                               //
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x32), bb::BlackboardEntry { "stop_robot_to_robot_infrared_following" } },                 //
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x33), bb::BlackboardEntry { "start_robot_to_robot_infrared_evading" } },                  //
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x34), bb::BlackboardEntry { "stop_robot_to_robot_infrared_evading" } },                   //
    //
    { bb::entryKey(nordic, dev::sensors, 0x35), bb::BlackboardEntry { "enable_color_detection_notify" } },                                //
    { bb::entryKey(nordic, dev::sensors, 0x36), bb::BlackboardEntry { "color_detection_notify" } },                                   //
    { bb::entryKey(nordic, dev::sensors, 0x37), bb::BlackboardEntry { "get_current_detected_color_reading" } },                           //
    { bb::entryKey(nordic, dev::sensors, 0x38), bb::BlackboardEntry { "enable_color_detection" } },                                   //
    //
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x39), bb::BlackboardEntry { "configure_streaming_service" } },                            //
    { bb::entryKey(nordic, dev::sensors, 0x39), bb::BlackboardEntry { "configure_streaming_service" } },                                  //
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x3A), bb::BlackboardEntry { "start_streaming_service" } },                                //
    { bb::entryKey(nordic, dev::sensors, 0x3A), bb::BlackboardEntry { "start_streaming_service" } },                                   //
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x3B), bb::BlackboardEntry { "stop_streaming_service" } },                                 //
    { bb::entryKey(nordic, dev::sensors, 0x3B), bb::BlackboardEntry { "stop_streaming_service" } },                                   //
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x3C), bb::BlackboardEntry { "clear_streaming_service" } },                                //
    { bb::entryKey(nordic, dev::sensors, 0x3C), bb::BlackboardEntry { "clear_streaming_service" } },                                   //
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x3D), bb::BlackboardEntry { "streaming_service_data_notify" } },                          //
    { bb::entryKey(nordic, dev::sensors, 0x3D), bb::BlackboardEntry { "streaming_service_data_notify" } },                                //
    //
    { bb::entryKey(nordic, dev::sensors, 0x3D, 3), bb::BlackboardEntry { "color stream" } },                                   //
    { bb::entryKey(nordic, dev::sensors, 0x3D, 9), bb::BlackboardEntry { "core stream" } },                                   //
    { bb::entryKey(nordic, dev::sensors, 0x3D, 10), bb::BlackboardEntry { "ambient stream" } },                                   //
    //
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x3D, 2), bb::BlackboardEntry { "accelerometer stream" } },                                //
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x3D, 9), bb::BlackboardEntry { "core stream" } },                                   //
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x3D, 4), bb::BlackboardEntry { "gyro stream" } },                                   //
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x3D, 1), bb::BlackboardEntry { "imu stream" } },                                   //
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x3D, 6), bb::BlackboardEntry { "locator stream" } },                                   //
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x3D, 11), bb::BlackboardEntry { "quat stream" } },                                   //
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x3D, 8), bb::BlackboardEntry { "speed stream" } },                                   //
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x3D, 7), bb::BlackboardEntry { "velocity stream" } },                                   //
    //
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x3D, 12), bb::BlackboardEntry { "imu accel gyro stream" } },                              //
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x3D, 13), bb::BlackboardEntry { "speed velocity locator token" } },                       //
    //
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x3E), bb::BlackboardEntry { "enable_robot_infrared_message_notify" } },                   //
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x3F), bb::BlackboardEntry { "send_infrared_message" } },                                  //
    //
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x4A, 4), bb::BlackboardEntry { "left_motor_temperature" } },                         // left
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x4A, 5), bb::BlackboardEntry { "right_motor_temperature" } },                       // right
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x4B), bb::BlackboardEntry { "get_motor_thermal_protection_status" } },                    //
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x4C), bb::BlackboardEntry { "enable_motor_thermal_protection_status_notify" } },          //
    { bb::entryKey(bluetoothSOC, dev::sensors, 0x4D), bb::BlackboardEntry { "motor_thermal_protection_status_notify" } },                 //
    //
    { bb::entryKey(bluetoothSOC, dev::system, 0x00), bb::BlackboardEntry { "nordic_main_application_version" } },                         //
    { bb::entryKey(nordic, dev::system, 0x00), bb::BlackboardEntry { "bt_main_application_version" } },                                   //
    { bb::entryKey(bluetoothSOC, dev::system, 0x01), bb::BlackboardEntry { "nordic_bootloader_version" } },                               //
    { bb::entryKey(nordic, dev::system, 0x01), bb::BlackboardEntry { "bt_bootloader_version" } },                                   //
    { bb::entryKey(nordic, dev::system, 0x03), bb::BlackboardEntry { "board_revision" } },                                   //
    { bb::entryKey(nordic, dev::system, 0x06), bb::BlackboardEntry { "mac_address" } },                                   //
    { bb::entryKey(nordic, dev::system, 0x13), bb::BlackboardEntry { "stats_id" } },                                   //
    { bb::entryKey(bluetoothSOC, dev::system, 0x1F), bb::BlackboardEntry { "nordic_processor_name" } },                                   //
    { bb::entryKey(nordic, dev::system, 0x1F), bb::BlackboardEntry { "bt_processor_name" } },                                   //
    { bb::entryKey(nordic, dev::system, 0x38), bb::BlackboardEntry { "get_sku" } },                                   //
    { bb::entryKey(bluetoothSOC, dev::system, 0x39), bb::BlackboardEntry { "get_core_up_time_in_milliseconds" } },                       //
    };
    //======================================================================================================================
    //  Key methods
    //----------------------------------------------------------------------------------------------------------------------
    Blackboard::key_s::key_s(TargetPort const proc, Devices const dev, uint8_t const cmd, uint8_t const id) :
        id(id), cmd(cmd), dev(dev), proc(proc) {
    }
    //----------------------------------------------------------------------------------------------------------------------
    Blackboard::key_s::key_s(key_t const k) :
        id { static_cast<uint8_t>(k & 0xFF) },                                   //
            cmd { static_cast<uint8_t>((k >> 8) & 0xFF) },                                   //
            dev { static_cast<uint8_t>((k >> 16) & 0xFF) },                                   //
            proc { static_cast<uint8_t>((k >> 24) & 0xFF) } {
    }
    //----------------------------------------------------------------------------------------------------------------------
    inline Blackboard::key_s::operator key_t() {
        return key_t(proc << 24 | (dev << 16) | (cmd << 8) | id);
    }
    //======================================================================================================================
    //  Methods to get dictionary entries
    //----------------------------------------------------------------------------------------------------------------------
    Blackboard::key_t Blackboard::entryKey(TargetPort const target, Devices const dev, uint8_t const cmd, uint8_t const id) {
        return static_cast<key_t>(key_s(target, dev, cmd, id));
    }
    //----------------------------------------------------------------------------------------------------------------------
    rvr::Blackboard::key_t Blackboard::msgKey(TargetPort const src, Devices const dev, uint8_t const cmd, uint8_t const seq) {
        Blackboard::key_t key;
        if (seq >= 0x80) {
            key = Blackboard::entryKey(src, dev, cmd);
        }
        else {
            key = Blackboard::entryKey(src, dev, cmd, seq);
        }
        return key;
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
    void Blackboard::addMsgValue(key_t const key, RvrMsg value) {
        mDictionary[key].value = value;
    }
    //----------------------------------------------------------------------------------------------------------------------
    RvrMsgRet_t Blackboard::entryValue(TargetPort const target, Devices const dev, uint8_t const cmd, uint8_t const id) const {
        RvrMsgRet_t msg_opt { entryValue(entryKey(target, dev, cmd, id)) };
        return msg_opt;
    }
    //----------------------------------------------------------------------------------------------------------------------
    RvrMsgRet_t Blackboard::entryValue(key_t const key) const {
        if (auto it = mDictionary.find(key); (it != mDictionary.end()) & ( !it->second.value.empty())) {
            return it->second.value;
        }
        return {};
    }
    //======================================================================================================================
    //  Method to put RvrMsg data into dictionary
    //----------------------------------------------------------------------------------------------------------------------
    void Blackboard::msgArray(Blackboard::key_t key, RvrMsg::iterator begin, RvrMsg::iterator end) {
        RvrMsg msg { begin, end };
        if (msg.empty()) {
            msg.push_back(0xFF);
        }
        else if (msg.size() >= 2) {
            uint8_t seq { msg.front() };

            if (seq == 0xFF) {
                key &= static_cast<Blackboard::key_t>(0xFFFFFF00);
                key |= static_cast<Blackboard::key_t>(msg[1]);
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
                    case SpecialSeq::enable:
                        key &= static_cast<Blackboard::key_t>(0xFFFFFF00);
                        msg[1] = true;
                        break;
                    case SpecialSeq::disable:
                        key &= static_cast<Blackboard::key_t>(0xFFFFFF00);
                        msg[1] = false;
                        break;
                }
            }

        }
        addMsgValue(key, msg);
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
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<bool> Blackboard::boolValue(TargetPort const target, Devices const dev, uint8_t const cmd) {
        return (byteValue(target, dev, cmd) != 0);
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<uint8_t> Blackboard::byteValue(TargetPort const target, Devices const dev, uint8_t const cmd) {
        RvrMsgRet_t msg { entryValue(target, dev, cmd) };
        uint8_t res { };

        if (msg) {
            res = msg.value()[2];
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    // these two methods are for notifications that return a single 0xFF
    std::optional<bool> Blackboard::notifyState(TargetPort const target, Devices const dev, uint8_t const cmd) {
        RvrMsgRet_t msg { entryValue(target, dev, cmd) };

        bool res { };
        if (msg) {
            res = msg.value()[0] != 0;
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    void Blackboard::resetNotify(TargetPort const target, Devices const dev, uint8_t const cmd) {
        addMsgValue(entryKey(target, dev, cmd, 0), { });
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<uint16_t> Blackboard::uintValue(TargetPort const target, Devices const dev, uint8_t const cmd, uint8_t const pos) {

        RvrMsgRet_t msg { entryValue(target, dev, cmd) };

        uint16_t res { };
        if (msg) {

            auto begin { msg.value().begin() + 2 };
            begin += (pos * sizeof(uint16_t));

            res = uintConvert(begin, sizeof(uint16_t));
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<int16_t> Blackboard::intValue(TargetPort const target, Devices const dev, uint8_t const cmd) {
        return static_cast<std::optional<int16_t>>(uintValue(target, dev, cmd));
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<uint32_t> Blackboard::uint32Value(TargetPort const target, Devices const dev, uint8_t const cmd, uint8_t const pos,
        uint8_t const id) {
        RvrMsgRet_t msg { entryValue(target, dev, cmd, id) };
        uint32_t res { };

        if (msg) {
            auto begin { msg.value().begin() + 2 };
            begin += (pos * sizeof(uint32_t));

            res = uintConvert(begin, sizeof(uint32_t));
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<uint64_t> Blackboard::uint64Value(TargetPort const target, Devices const dev, uint8_t const cmd) {
        RvrMsgRet_t msg { entryValue(target, dev, cmd) };
        uint64_t res { };

        if (msg) {
            res = uintConvert(msg.value().begin() + 2, sizeof(uint64_t));
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<float> Blackboard::floatValue(TargetPort const target, Devices const dev, uint8_t const cmd, float const pos,
        uint8_t const id) {
        RvrMsgRet_t msg { entryValue(target, dev, cmd, id) };

        float result { };
        if (msg) {
            auto begin { msg.value().begin() + 2 };
            begin += (pos * sizeof(float));

            result = floatConvert(begin);
        }
        return result;
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<bool> Blackboard::getNotify(TargetPort const target, Devices const dev, uint8_t const cmd) {
        RvrMsgRet_t msg { entryValue(target, dev, cmd) };

        bool res { };
        if (msg) {
            res = { msg.value()[1] != 0 };
        }
        return res;
    }
    //----------------------------------------------------------------------------------------------------------------------
    std::optional<std::string> Blackboard::stringValue(TargetPort const target, Devices const dev, uint8_t const cmd) {
        RvrMsgRet_t msg { entryValue(target, dev, cmd) };
        if (msg) {
            return std::string { msg.value().begin() + 2, msg.value().end() - 1 }; // response is a zero terminated C string
        }
        return {};
    }
    //----------------------------------------------------------------------------------------------------------------------
    RvrMsgRet_t const Blackboard::msgValue(TargetPort const target, Devices const dev, uint8_t const cmd, uint8_t const id) {
        RvrMsgRet_t msg { entryValue(target, dev, cmd, id) };
//        terr << code_loc << std::hex << std::setw(2) << (int)target << (int)dev << (int)cmd << (int)id;

        if (msg) {
//            terr << code_loc << msg.value() << std::hex << target;
            return RvrMsg { msg.value().begin() + 2, msg.value().end() };
        }
        return {};
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
            bb::BlackboardEntry be;
        };
        std::vector<v_map> vec;
        for (auto b : rvr::Blackboard::mDictionary) {
            v_map v { b.first, b.second };
            vec.push_back(v);
        }
        std::sort(vec.begin(), vec.end(),                                   //
                  [](v_map const& a, v_map const& b) {
                      return a.key < b.key;
                  }
        );
        for (auto& i : vec) {
            terr << std::hex << std::uppercase << i.key << mys::tab << std::setw(45) << std::setfill(' ') << std::left << i.be.name <<    //
                 mys::tab << mys::tab << i.be.value;
        }
    }

}
/* namespace rvr */
