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
  virtual esphome::optional<std::map<std::string, float>> get_values(std::vector<unsigned char> &telegram) override {
    std::map<std::string, float> ret_val{};

    add_to_map(ret_val, "total_energy_consumption_gj", this->get_current_heating_GJ(telegram));
    if (ret_val.size() > 0) {
      return ret_val;
    }
    else {
      return {};
    }
  };

private:
  esphome::optional<float> get_current_heating_GJ(std::vector<unsigned char> &telegram) {
      esphome::optional<float> ret_val{};
      size_t i = 19;

      ret_val = (((uint32_t)telegram[i+6] << 32) + (uint32_t)telegram[i+5] << 16) + (uint32_t)telegram[i+4] << 8) + (uint32_t)telegram[i+3]);

      return ret_val / 100.0;
    };
};
