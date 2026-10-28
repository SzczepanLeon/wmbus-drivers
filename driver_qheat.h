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
  esphome::optional<double> get_current_hca(std::vector<unsigned char> &telegram) {
    // Dekodujemy wyłącznie pełną ramkę (66 bajtów, CI=0x72 na bajcie 10)
    // Wartość bieżąca (np. 4573 j.) znajduje się na bajtach 23..26 tuż po markerze 0x0C 0x0D (bajty 21..22)
    if (telegram.size() >= 60 && telegram[10] == 0x72) {
      if (telegram[21] == 0x0C && telegram[22] == 0x0D) {
        return (double)bcd_2_int(telegram, 23, 4);
      }
    }
    // Krótkie ramki kompaktowe są ignorowane (zwracamy puste {}), dzięki czemu nie wskakuje 356665 j.
    return {};
  };

  esphome::optional<double> get_previous_hca(std::vector<unsigned char> &telegram) {
    if (telegram.size() >= 60 && telegram[10] == 0x72) {
      if (telegram[27] == 0x4C && telegram[28] == 0x0D) {
        return (double)bcd_2_int(telegram, 29, 4);
      }
    }
    return {};
  };

  esphome::optional<double> get_temperature(std::vector<unsigned char> &telegram) {
    if (telegram.size() >= 60 && telegram[10] == 0x72) {
      if (telegram[37] == 0xCC && telegram[38] == 0x08 && telegram[39] == 0x0D) {
        return ((double)bcd_2_int(telegram, 40, 4)) / 100.0;
      }
    }
    return {};
  };
};
