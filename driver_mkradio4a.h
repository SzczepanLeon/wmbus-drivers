/*
  Based on: https://github.com/wmbusmeters/wmbusmeters/blob/master/src/driver_mkradio4a.cc
  Copyright (C) 2017-2022 Fredrik Öhrström (gpl-3.0-or-later)
*/

#pragma once

#include "driver.h"

#include <vector>
#include <string>

struct Mkradio4a: Driver
{
  Mkradio4a(std::string key = "") : Driver(std::string("mkradio4a"), key) {};
  virtual esphome::optional<std::map<std::string, double>> get_values(std::vector<unsigned char> &telegram) override {
    std::map<std::string, double> ret_val{};

    add_to_map(ret_val, "target_water_m3", this->get_4315(telegram));

    if (ret_val.size() > 0) {
      return ret_val;
    }
    else {
      return {};
    }
  };

private:
};