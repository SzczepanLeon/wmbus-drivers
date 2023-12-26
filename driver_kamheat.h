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
    add_to_map(ret_val, "total_forward_energy_m3c", this->get_forward_energy_m3c(telegram));
    add_to_map(ret_val, "total_return_energy_m3c", this->get_return_energy_m3c(telegram));
    add_to_map(ret_val, "total_volume_m3", this->get_total_volume_m3(telegram));
    add_to_map(ret_val, "current_flow_m3h", this->get_volume_flow_m3h(telegram));
    add_to_map(ret_val, "current_temperature_inlet_c", this->get_temperature_inlet_C(telegram));
    add_to_map(ret_val, "current_temperature_outlet_c", this->get_temperature_outlet_C(telegram));

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

  esphome::optional<float> get_forward_energy_m3c(std::vector<unsigned char> &telegram) {
      esphome::optional<float> ret_val{};
      size_t i = 19;

      ret_val = (((uint32_t)telegram[i+13] << 32) + (uint32_t)telegram[i+12] << 16) + (uint32_t)telegram[i+11] << 8) + (uint32_t)telegram[i+10]);

      return ret_val ;
    };

  esphome::optional<float> get_return_energy_m3c(std::vector<unsigned char> &telegram) {
      esphome::optional<float> ret_val{};
      size_t i = 19;

      ret_val = (((uint32_t)telegram[i+20] << 32) + (uint32_t)telegram[i+19] << 16) + (uint32_t)telegram[i+18] << 8) + (uint32_t)telegram[i+17]);

      return ret_val ;
    };

  esphome::optional<float> get_total_volume_m3(std::vector<unsigned char> &telegram) {
      esphome::optional<float> ret_val{};
      size_t i = 19;

      ret_val = (((uint32_t)telegram[i+26] << 32) + (uint32_t)telegram[i+25] << 16) + (uint32_t)telegram[i+24] << 8) + (uint32_t)telegram[i+23]);

      return ret_val / 100.0 ;
    };

  esphome::optional<float> get_volume_flow_m3h(std::vector<unsigned char> &telegram) {
      esphome::optional<float> ret_val{};
      size_t i = 19;

      ret_val = (((uint32_t)telegram[i+37] << 32) + (uint32_t)telegram[i+36] << 16) + (uint32_t)telegram[i+35] << 8) + (uint32_t)telegram[i+34]);

      return ret_val / 1000.0;
    };

  esphome::optional<float> get_temperature_inlet_C(std::vector<unsigned char> &telegram) {
      esphome::optional<float> ret_val{};
      size_t i = 19;

      ret_val = (((uint32_t)telegram[i+41] << 8) + (uint32_t)telegram[i+40]);

      return ret_val / 100.0 ;
    };

  esphome::optional<float> get_temperature_outlet_C(std::vector<unsigned char> &telegram) {
      esphome::optional<float> ret_val{};
      size_t i = 19;

      ret_val = (((uint32_t)telegram[i+45] << 8) + (uint32_t)telegram[i+44]);

      return ret_val / 100.0 ;
    };

};
