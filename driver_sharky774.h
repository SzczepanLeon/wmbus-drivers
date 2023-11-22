/*
  Based on: https://github.com/wmbusmeters/wmbusmeters/blob/master/src/driver_sharky774.cc
  Copyright (C) 2017-2022 Fredrik Öhrström (gpl-3.0-or-later)
*/

#pragma once

#include "driver.h"

#include <vector>
#include <string>

struct Sharky774 : Driver
{
  Sharky774(std::string key = "") : Driver(std::string("sharky774"), key){};
  virtual esphome::optional<std::map<std::string, float>> get_values(std::vector<unsigned char> &telegram) override {
    std::map<std::string, float> ret_val{};

    add_to_map(ret_val, "total_energy_consumption_gj", this->get_total_energy_consumption_gj(telegram));
    add_to_map(ret_val, "power_kw", this->get_0C2B(telegram));
    add_to_map(ret_val, "total_volume_m3", this->get_0C13(telegram));
    add_to_map(ret_val, "volume_flow_l3h", this->get_0B3B(telegram));
    add_to_map(ret_val, "flow_temperature_c", this->get_0A5A(telegram));
    add_to_map(ret_val, "return_temperature_c", this->get_0A5E(telegram));
    add_to_map(ret_val, "operating_time_d", this->get_0B26(telegram));

    if (ret_val.size() > 0)
    {
      return ret_val;
    }
    else
    {
      return {};
    }
  };

private:
  esphome::optional<float> get_total_energy_consumption_gj(std::vector<unsigned char> &telegram)
  {
    esphome::optional<float> ret_val{};
    uint32_t usage = 0;
    size_t i = 11;
    uint32_t total_register = 0x0C0E;
    while (i < telegram.size())
    {
      uint32_t c = (((uint32_t)telegram[i + 0] << 8) | ((uint32_t)telegram[i + 1]));
      if (c == total_register)
      {
        i += 2;
        usage = bcd_2_int(telegram, i, 4);
        ret_val = usage / 1000.0;
        break;
      }
      i++;
    }
    return ret_val;
  };
};