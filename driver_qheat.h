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

    if (ret_val.size() > 0) {
      return ret_val;
    }
    else {
      return {};
    }
  };

private:
  bool is_valid_bcd4(std::vector<unsigned char> &t, size_t idx) {
    if (idx + 3 >= t.size()) return false;
    for (size_t k = 0; k < 4; k++) {
      if (((t[idx + k] >> 4) > 9) || ((t[idx + k] & 0x0F) > 9)) return false;
    }
    return true;
  }

  esphome::optional<double> get_current_hca(std::vector<unsigned char> &telegram) {
    // 1. Ramka długa (89 bajtów): wartość leży zawsze po sekwencji C0 0D FF FF (np. 38 46 00 00 -> 4638)
    for (size_t i = 20; i + 8 < telegram.size(); i++) {
      if (telegram[i] == 0xC0 && telegram[i+1] == 0x0D && telegram[i+2] == 0xFF && telegram[i+3] == 0xFF) {
        if (is_valid_bcd4(telegram, i + 4)) {
          return (double)bcd_2_int(telegram, i + 4, 4);
        }
      }
    }
    // 2. Ramka standardowa (66 bajtów): wartość leży po 0C 0D, gdzie wszystkie 4 kolejne bajty są poprawnym BCD
    // (w ramce 89-bajtowej po 0C 0D stoi bajt 0xFF, więc is_valid_bcd4 automatycznie go odrzuci!)
    for (size_t i = 18; i + 6 < telegram.size(); i++) {
      if (telegram[i] == 0x0C && telegram[i+1] == 0x0D) {
        if (is_valid_bcd4(telegram, i + 2)) {
          return (double)bcd_2_int(telegram, i + 2, 4);
        }
      }
    }
    return {};
  };
};
