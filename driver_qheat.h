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
    // Odczyt dla ramek C1 Qundis (szukamy markera 0x0C 0x0D i bierzemy kolejne bajty)
    for (size_t i = 10; i + 5 < telegram.size(); i++) {
      if (telegram[i] == 0x0C && telegram[i+1] == 0x0D) {
        double val = (double)bcd_2_int(telegram, i + 2, 3);
        if (val > 0 && val < 100000) return val;
      }
    }
    return {};
  };

  esphome::optional<double> get_previous_hca(std::vector<unsigned char> &telegram) {
    for (size_t i = 15; i + 5 < telegram.size(); i++) {
      if (telegram[i] == 0x4C && telegram[i+1] == 0x0D) {
        double val = (double)bcd_2_int(telegram, i + 2, 3);
        if (val >= 0 && val < 100000) return val;
      }
    }
    return {};
  };

  esphome::optional<double> get_temperature(std::vector<unsigned char> &telegram) {
    for (size_t i = 20; i + 6 < telegram.size(); i++) {
      if (telegram[i] == 0x5E && telegram[i+1] == 0x39) { // Marker temperatury w C1 Qundis
        double val = (double)bcd_2_int(telegram, i + 2, 2);
        return val / 10.0;
      }
    }
    return {};
  };
};
