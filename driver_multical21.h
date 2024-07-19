/*
  Based on: https://github.com/wmbusmeters/wmbusmeters/blob/master/src/driver_multical21.cc
  Copyright (C) 2017-2022 Fredrik Öhrström (gpl-3.0-or-later)
*/

#pragma once

#include "driver.h"

#include <vector>
#include <string>

struct Multical21: Driver
{
  Multical21(std::string key = "") : Driver(std::string("multical21"), key) {};
  virtual esphome::optional<std::map<std::string, double>> get_values(std::vector<unsigned char> &telegram) override {
    std::map<std::string, double> ret_val{};

    add_to_map(ret_val, "total_water_m3", this->get_total_water_m3(telegram));
    add_to_map(ret_val, "target_water_m3", this->get_target_water_m3(telegram));
    add_to_map(ret_val, "flow_temperature_c", this->get_flow_temperature_c(telegram));
    add_to_map(ret_val, "external_temperature_c", this->get_external_temperature_c(telegram));

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
      return this->get_0413(telegram);
    }

    if (tpl_ci_field == 0x79) {  // check !!!!
      uint32_t total_water{0};
      uint8_t i = 26;  // check !!!!
      total_water = ((uint32_t) telegram[i + 3] << 24) | ((uint32_t) telegram[i + 2] << 16) |
                    ((uint32_t) telegram[i + 1] << 8) | ((uint32_t) telegram[i + 0]);
      // in m3
      ret_val = total_water / 1000.0;
      ESP_LOGVV(TAG, "Found total_water_consumption with '%d'->'%f'", total_water, ret_val.value());
    }

    return ret_val;
  };

  esphome::optional<double> get_target_water_m3(std::vector<unsigned char> &telegram) {
    esphome::optional<double> ret_val{};
    uint8_t tpl_ci_field = telegram[19];

    if (tpl_ci_field == 0x78) {
      return this->get_4413(telegram);
    }

    if (tpl_ci_field == 0x79) {  // check !!!!
      uint32_t target_water{0};
      uint8_t i = 30;  // check !!!!
      target_water = ((uint32_t) telegram[i + 3] << 24) | ((uint32_t) telegram[i + 2] << 16) |
                     ((uint32_t) telegram[i + 1] << 8) | ((uint32_t) telegram[i + 0]);
      // in m3
      ret_val = target_water / 1000.0;
      ESP_LOGVV(TAG, "Found target_water_consumption with '%d'->'%f'", target_water, ret_val.value());
    }

    return ret_val;
  };

  esphome::optional<double> get_flow_temperature_c(std::vector<unsigned char> &telegram) {
    esphome::optional<double> ret_val{};
    uint8_t l_field = telegram[0];
    uint8_t tpl_ci_field = telegram[19];

    if ((tpl_ci_field == 0x78) && (l_field >= 39)) {
      return this->get_615B(telegram);
    }

    if ((tpl_ci_field == 0x79) && (l_field >= 34)) {  // check !!!!
      uint32_t temperature{0};
      uint8_t i = 34;  // check !!!!
      temperature = ((uint32_t) telegram[i + 0]);
      // in celcius degrees
      ret_val = temperature / 1.0;
      ESP_LOGVV(TAG, "Found flow_temperature_c with '%d'->'%f'", temperature, ret_val.value());
    }

    return ret_val;
  };

  esphome::optional<double> get_external_temperature_c(std::vector<unsigned char> &telegram) {
    esphome::optional<double> ret_val{};
    uint8_t l_field = telegram[0];
    uint8_t tpl_ci_field = telegram[19];

    if ((tpl_ci_field == 0x78) && (l_field >= 42)) {
      return this->get_6167(telegram);
    }

    if ((tpl_ci_field == 0x79) && (l_field >= 35)) {  // check !!!!
      uint32_t temperature{0};
      uint8_t i = 35;  // check !!!!
      temperature = ((uint32_t) telegram[i + 0]);
      // in celcius degrees
      ret_val = temperature / 1.0;
      ESP_LOGVV(TAG, "Found external_temperature_c with '%d'->'%f'", temperature, ret_val.value());
    }

    return ret_val;
  };
};