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
  virtual esphome::optional<std::map<std::string, float>> get_values(std::vector<unsigned char> &telegram) override {
    std::map<std::string, float> ret_val{};

    add_to_map(ret_val, "total_energy_consumption_gj", this->get_current_hca(telegram));
    add_to_map(ret_val, "last_month_energy_consumption_gj", this->get_previous_hca(telegram));
    add_to_map(ret_val, "current_temperature_c", this->get_temperature(telegram));

    if (ret_val.size() > 0) {
      return ret_val;
    }
    else {
      return {};
    }
  };

private:
  esphome::optional<float> get_current_hca(std::vector<unsigned char> &telegram) {
    for (size_t i = 10; i + 6 < telegram.size(); i++) {
      // Szukamy sekwencji 0x0C (DIF: 8-digit BCD) + 0x0D (VIF: HCA)
      if (telegram[i] == 0x0C && telegram[i+1] == 0x0D) {
        return (float)bcd_to_int(telegram, i + 2, 4);
      }
    }
    // Fallback dla starszych ramek Qheat
    if (telegram.size() > 25 && telegram[19] == 0x0C && telegram[20] == 0x0D) {
      return (float)bcd_to_int(telegram, 21, 4);
    }
    return {};
  };

  esphome::optional<float> get_previous_hca(std::vector<unsigned char> &telegram) {
    for (size_t i = 10; i + 6 < telegram.size(); i++) {
      // Szukamy sekwencji 0x4C (Storage 1 BCD) + 0x0D (VIF: HCA)
      if (telegram[i] == 0x4C && telegram[i+1] == 0x0D) {
        return (float)bcd_to_int(telegram, i + 2, 4);
      }
    }
    return {};
  };

  esphome::optional<float> get_temperature(std::vector<unsigned char> &telegram) {
    for (size_t i = 20; i + 6 < telegram.size(); i++) {
      // W ramkach Qundis po markerze 0xCC 0x08 0x0D znajdują się 4 bajty BCD temperatury (w setnych stopnia)
      if (telegram[i] == 0xCC && telegram[i+1] == 0x08 && telegram[i+2] == 0x0D) {
        return ((float)bcd_to_int(telegram, i + 3, 4)) / 100.0f;
      }
    }
    return {};
  };
};
