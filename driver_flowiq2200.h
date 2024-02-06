/*
  Based on: https://github.com/wmbusmeters/wmbusmeters/blob/master/src/driver_flowiq2200.cc
  Copyright (C) 2017-2022 Fredrik Öhrström (gpl-3.0-or-later)
*/

#pragma once

#include "driver.h"

#include <vector>
#include <string>

struct Flowiq2200: Driver
{
  Flowiq2200(std::string key = "") : Driver(std::string("flowiq2200"), key) {};
  virtual esphome::optional<std::map<std::string, double>> get_values(std::vector<unsigned char> &telegram) override {
    std::map<std::string, double> ret_val{};

    add_to_map(ret_val, "total_water_m3", this->get_total_water_m3(telegram));
    add_to_map(ret_val, "target_water_m3", this->get_target_water_m3(telegram));
    // add_to_map(ret_val, "status", this->get_status(telegram));
    // add_to_map(ret_val, "max_flow_m3h", this->get_92013B(telegram));
    // add_to_map(ret_val, "min_flow_m3h", this->get_A2013B(telegram));
    // add_to_map(ret_val, "min_flow_temperature_c", this->get_A1015B(telegram));
    // add_to_map(ret_val, "max_flow_temperature_c", this->get_91015B(telegram));
    // add_to_map(ret_val, "min_external_temperature_c", this->get_A10167(telegram));

    if (ret_val.size() > 0) {
      return ret_val;
    }
    else {
      return {};
    }
  };

private:
  esphome::optional<double> get_total_water_m3(std::vector<unsigned char> &telegram) {
    esphome::optional<double> ret_val{};
    uint8_t tpl_ci_field = telegram[19];
    if (tpl_ci_field == 0x78) {
      ret_val = this->get_0413(telegram);  
    }
    else if (tpl_ci_field == 0x79) {
      uint32_t usage{0};
      uint8_t i = 29;
      usage = (((uint32_t)telegram[i+3] << 24) | ((uint32_t)telegram[i+2] << 16) |
               ((uint32_t)telegram[i+1] << 8)  | ((uint32_t)telegram[i+0]));
      ret_val = usage / 1000.0;
    }
    return ret_val;
  };

  esphome::optional<double> get_target_water_m3(std::vector<unsigned char> &telegram) {
    esphome::optional<double> ret_val{};
    uint8_t tpl_ci_field = telegram[19];
    if (tpl_ci_field == 0x78) {
      ret_val = this->get_4413(telegram);  
    }
    else if (tpl_ci_field == 0x79) {
      uint32_t usage{0};
      uint8_t i = 11;
      usage = (((uint32_t)telegram[i+3] << 24) | ((uint32_t)telegram[i+2] << 16) |
               ((uint32_t)telegram[i+1] << 8)  | ((uint32_t)telegram[i+0]));
      ret_val = usage / 1000.0;
    }
    return ret_val;
  };

  esphome::optional<double> get_status(std::vector<unsigned char> &telegram) {
    esphome::optional<double> ret_val{};
    uint8_t tpl_ci_field = telegram[19];
    if (tpl_ci_field == 0x78) {
      ret_val = this->get_04FF23(telegram);  
    }
    else if (tpl_ci_field == 0x79) {
      // ToDo
    }
    return ret_val;
  };

};