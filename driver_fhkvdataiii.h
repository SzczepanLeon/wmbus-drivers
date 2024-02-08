/*
  Based on: https://github.com/wmbusmeters/wmbusmeters/blob/master/src/driver_fhkvdataiii.cc
  Copyright (C) 2019-2022 Fredrik Öhrström (gpl-3.0-or-later)
*/

#pragma once

#include "driver.h"

#include <vector>
#include <string>

struct FhkvdataIII: Driver
{
  FhkvdataIII(std::string key = "") : Driver(std::string("fhkvdataiii"), key) {};
  virtual esphome::optional<std::map<std::string, double>> get_values(std::vector<unsigned char> &telegram) override {
    std::map<std::string, double> ret_val{};

    add_to_map(ret_val, "current_hca", this->get_current_hca(telegram));
    add_to_map(ret_val, "previous_hca", this->get_previous_hca(telegram));
    add_to_map(ret_val, "temp_room_c", this->get_temp_room_c(telegram));
    add_to_map(ret_val, "temp_radiator_c", this->get_temp_radiator_c(telegram));

    if (ret_val.size() > 0) {
      return ret_val;
    }
    else {
      return {};
    }
  };

private:
  esphome::optional<double> get_current_hca(std::vector<unsigned char> &telegram) {
    esphome::optional<double> ret_val{};
    size_t i = 11;

    ret_val = (((uint32_t)telegram[i+8] << 8) + (uint32_t)telegram[i+7]);
    ESP_LOGVV(TAG, "Found current_hca with '%f'", ret_val.value());

    return ret_val;
  };

  esphome::optional<double> get_previous_hca(std::vector<unsigned char> &telegram) {
    esphome::optional<double> ret_val{};
    size_t i = 11;

    ret_val = (((uint32_t)telegram[i+4] << 8) + (uint32_t)telegram[i+3]);
    ESP_LOGVV(TAG, "Found previous_hca with '%f'", ret_val.value());

    return ret_val;
  };

  esphome::optional<double> get_temp_room_c(std::vector<unsigned char> &telegram) {
    esphome::optional<double> ret_val{};
    size_t i = 20;
    if((uint32_t)telegram[8] == 0x94) { // dll_version
      i++;
    }

    ret_val = (((uint32_t)telegram[i+1] << 8) + (uint32_t)telegram[i])/100.0;
    ESP_LOGVV(TAG, "Found temp_room with '%f'", ret_val.value());

    return ret_val;
  };

  esphome::optional<double> get_temp_radiator_c(std::vector<unsigned char> &telegram) {
    esphome::optional<double> ret_val{};
    size_t i = 22;
    if((uint32_t)telegram[8] == 0x94) { // dll_version
      i++;
    }

    ret_val = (((uint32_t)telegram[i+1] << 8) + (uint32_t)telegram[i])/100.0;
    ESP_LOGVV(TAG, "Found temp_radiator with '%f'", ret_val.value());

    return ret_val;
  };
};