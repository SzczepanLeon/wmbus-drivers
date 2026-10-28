/*
  Based on: https://github.com/wmbusmeters/wmbusmeters/blob/master/src/driver_qheat.cc
  Copyright (C) 2021-2022 Fredrik Öhrström (gpl-3.0-or-later)
*/

#pragma once

#include "driver.h"
#include <vector>
#include <string>

struct Qheat : Driver
{
  Qheat(std::string key = "") : Driver(std::string("qheat"), key){};
  virtual esphome::optional<std::map<std::string, double>> get_values(std::vector<unsigned char> &telegram) override {
    std::map<std::string, double> ret_val{};

    add_to_map(ret_val, "total_energy_consumption_gj", this->get_current_hca(telegram));
    add_to_map(ret_val, "total_energy_consumption_kwh", this->get_previous_hca(telegram));
    add_to_map(ret_val, "flow_temperature_c", this->get_temperature(telegram));

    if (ret_val.size() > 0) {
      return ret_val;
    }
    else {
      return {};
    }
  };

private:
  esphome::optional<double> get_current_hca(std::vector<unsigned char> &telegram) {
    if (telegram.size() >= 60 && telegram[10] == 0x72) {
      for (size_t i = 20; i < 26; i++) {
        if (telegram[i] == 0x0C && telegram[i+1] == 0x0D) {
          return (double)bcd_2_int(telegram, i + 2, 4);
        }
      }
    }
    return {};
  };

  esphome::optional<double> get_previous_hca(std::vector<unsigned char> &telegram) {
    if (telegram.size() >= 60 && telegram[10] == 0x72) {
      for (size_t i = 25; i < 35; i++) {
        if (telegram[i] == 0x4C && telegram[i+1] == 0x0D) {
          return (double)bcd_2_int(telegram, i + 2, 4);
        }
      }
    }
    return {};
  };

  esphome::optional<double> get_temperature(std::vector<unsigned char> &telegram) {
    if (telegram.size() >= 60 && telegram[10] == 0x72) {
      for (size_t i = 30; i + 6 < telegram.size(); i++) {
        if (telegram[i] == 0xCC && telegram[i+1] == 0x08 && telegram[i+2] == 0x0D) {
          return ((double)bcd_2_int(telegram, i + 3, 4)) / 100.0;
        }
      }
    }
    return {};
  };
};
