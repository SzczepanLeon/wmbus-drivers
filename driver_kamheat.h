/*
  Based on: https://github.com/wmbusmeters/wmbusmeters/blob/master/src/driver_kamheat.cc
  Copyright (C) 2021-2022 Fredrik Öhrström (gpl-3.0-or-later)
*/

#pragma once

#include "driver.h"

#include <vector>
#include <string>

struct Kamheat: Driver
{
  Kamheat(std::string key = "") : Driver(std::string("kamheat"), key) {};
  virtual esphome::optional<std::map<std::string, double>> get_values(std::vector<unsigned char> &telegram) override {
    std::map<std::string, double> ret_val{};

    add_to_map(ret_val, "total_energy_consumption_kwh", this->get_0406(telegram));
    add_to_map(ret_val, "total_energy_consumption_kwh", this->get_040F(telegram));
    add_to_map(ret_val, "total_energy_consumption_kwh", this->get_total_energy_consumption_kwh(telegram));
    // add_to_map(ret_val, "total_forward_energy_m3c", this->get_04FF07(telegram));
    // add_to_map(ret_val, "total_return_energy_m3c", this->get_04FF08(telegram));
    // add_to_map(ret_val, "total_volume_m3", this->get_0414(telegram));
    // add_to_map(ret_val, "volume_flow_lh", this->get_043B(telegram)); // obliczyc w litrach
    // add_to_map(ret_val, "volume_flow_lh", this->get_volume_flow_lh(telegram)); // obliczyc w litrach
    // add_to_map(ret_val, "flow_temperature_c", this->get_0259(telegram));
    // add_to_map(ret_val, "flow_temperature_c", this->get_flow_temperature_c(telegram));
    // add_to_map(ret_val, "return_temperature_c", this->get_025D(telegram));
    // add_to_map(ret_val, "return_temperature_c", this->get_return_temperature_c(telegram));

    if (ret_val.size() > 0) {
      return ret_val;
    }
    else {
      return {};
    }
  };

private:
  esphome::optional<double> get_total_energy_consumption_kwh(std::vector<unsigned char> &telegram) {
    esphome::optional<double> ret_val{};
    uint8_t tpl_ci_field = telegram[19];
    if (tpl_ci_field == 0x79) {
      uint32_t usage{0};
      uint8_t i = 29;
      usage = (((uint32_t)telegram[i+3] << 24) | ((uint32_t)telegram[i+2] << 16) |
               ((uint32_t)telegram[i+1] << 8)  | ((uint32_t)telegram[i+0]));
      // in kWh
      ret_val = usage / 27777.0;
      ESP_LOGVV(TAG, "tpl_ci_field '0x79': '%d'->'%f'", usage, ret_val.value());
    }
    return ret_val;
  };

//   esphome::optional<double> get_forward_energy_m3c(std::vector<unsigned char> &telegram) {
//     esphome::optional<double> ret_val{};
//     size_t i = 19;
//     if ((telegram[0] == 0x40) && (telegram[19] == 0x78)) {  //longer frame (L=0x40) and 0x78 tpl-ci-field      
//       ret_val = (((uint32_t)telegram[i+13] << 24) + ((uint32_t)telegram[i+12] << 16) + ((uint32_t)telegram[i+11] << 8) + (uint32_t)telegram[i+10]);
//     }
//     return ret_val;
//   };

//   esphome::optional<double> get_return_energy_m3c(std::vector<unsigned char> &telegram) {
//     esphome::optional<double> ret_val{};
//     size_t i = 19;
//     if ((telegram[0] == 0x40) && (telegram[19] == 0x78)) {  //longer frame (L=0x40) and 0x78 tpl-ci-field   
//       ret_val = (((uint32_t)telegram[i+20] << 24) + ((uint32_t)telegram[i+19] << 16) + ((uint32_t)telegram[i+18] << 8) + (uint32_t)telegram[i+17]);
//     }
//     return ret_val;
//   };

//   esphome::optional<double> get_volume_flow_lh(std::vector<unsigned char> &telegram) {
//     esphome::optional<double> ret_val{};
//     size_t i = 19;
//     if ((telegram[0] == 0x31) && (telegram[19] == 0x79)) {  //short frame (L=0x31) and 0x79 tpl-ci-field (EN 13757-3 Application Layer with Compact frame (no tplh))
//       i-=11;
//       ret_val = (((uint32_t)telegram[i+37] << 24) + ((uint32_t)telegram[i+36] << 16) + ((uint32_t)telegram[i+35] << 8) + (uint32_t)telegram[i+34]);
//     }
//     else if ((telegram[0] == 0x40) && (telegram[19] == 0x78)) {  //longer frame (L=0x40) and 0x78 tpl-ci-field
//       ret_val = (((uint32_t)telegram[i+37] << 24) + ((uint32_t)telegram[i+36] << 16) + ((uint32_t)telegram[i+35] << 8) + (uint32_t)telegram[i+34]);
//     }
//     return ret_val;
//   };

//   esphome::optional<double> get_temperature_inlet_C(std::vector<unsigned char> &telegram) {
//     esphome::optional<double> ret_val{};
//     size_t i = 19;
//     if ((telegram[0] == 0x31) && (telegram[19] == 0x79)) {  //short frame (L=0x31) and 0x79 tpl-ci-field (EN 13757-3 Application Layer with Compact frame (no tplh))
//       i-=13;
//       ret_val = (((uint32_t)telegram[i+41] << 8) + (uint32_t)telegram[i+40]) / 100.0 ;
//     }
//     else if ((telegram[0] == 0x40) && (telegram[19] == 0x78)) {  //longer frame (L=0x40) and 0x78 tpl-ci-field
//       ret_val = (((uint32_t)telegram[i+41] << 8) + (uint32_t)telegram[i+40]) / 100.0 ;  // t1
//     }
//     return ret_val;
//   };

//   esphome::optional<double> get_temperature_outlet_C(std::vector<unsigned char> &telegram) {
//     esphome::optional<double> ret_val{};
//     size_t i = 19;
//     if ((telegram[0] == 0x31) && (telegram[19] == 0x79)) {  //short frame (L=0x31) and 0x79 tpl-ci-field (EN 13757-3 Application Layer with Compact frame (no tplh))
//       i-=15;
//       ret_val = (((uint32_t)telegram[i+45] << 8) + (uint32_t)telegram[i+44]) / 100.0;
//     }
//     else if ((telegram[0] == 0x40) && (telegram[19] == 0x78)) {  //longer frame (L=0x40) and 0x78 tpl-ci-field
//       ret_val = (((uint32_t)telegram[i+45] << 8) + (uint32_t)telegram[i+44]) / 100.0;  // t2
//     }
//     return ret_val;
//   };
};