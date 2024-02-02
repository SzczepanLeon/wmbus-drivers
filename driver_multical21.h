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
  Iperl(std::string key = "") : Driver(std::string("multical21"), key) {};
  virtual esphome::optional<std::map<std::string, double>> get_values(std::vector<unsigned char> &telegram) override {
    std::map<std::string, double> ret_val{};

    add_to_map(ret_val, "total_m3", this->get_0413(telegram));
    add_to_map(ret_val, "target_m3", this->get_4413(telegram));
    // add_to_map(ret_val, "flow_temperature_c", this->get_615B(telegram));
    // add_to_map(ret_val, "external_temperature_c", this->get_6167(telegram));

    if (ret_val.size() > 0) {
      return ret_val;
    }
    else {
      return {};
    }
  };

private:
};