/*
  Based on: https://github.com/wmbusmeters/wmbusmeters/blob/master/src/driver_rfmtx1.cc
  Copyright (C) 2019-2022 Fredrik Öhrström (gpl-3.0-or-later)
*/

#pragma once

#include "driver.h"

#include <vector>
#include <string>

struct Rfmtx1: Driver
{
  Rfmtx1(std::string key = "") : Driver(std::string("rfmtx1"), key) {};
  virtual esphome::optional<std::map<std::string, double>> get_values(std::vector<unsigned char> &telegram) override {
    std::map<std::string, double> ret_val{};

    add_to_map(ret_val, "total_water_m3", this->get_total_water_m3(telegram));

    if (ret_val.size() > 0) {
      return ret_val;
    }
    else {
      return {};
    }
  };

private:
  esphome::optional<double> get_total_water_m3(std::vector<unsigned char> &telegram) {
    esphome::optional<double> ret_val{};
    uint32_t tpl_cfg = (((uint32_t)telegram[14] << 8) | ((uint32_t)telegram[13]));
    ESP_LOGVV(TAG, "tpl_cfg = [0x%04X]", tpl_cfg);
    if (tpl_cfg == 0x1006) {
      unsigned char decoded_total[6];

      for (int i = 0; i < 6; ++i) {
        decoded_total[i] = (unsigned char)(telegram[0xf + i] ^ telegram[0xb] ^ decode_vectors_[telegram[0xb] & 0x0f][i]);
        ESP_LOGVV(TAG, "decoded_total[%d] = %d", i, decoded_total[i]);
      }

      uint32_t total = bcd_2_int(std::vector<unsigned char>(decoded_total + 2, decoded_total + 6), 0, 4);

      ret_val = total / 1000.0;
      ESP_LOGVV(TAG, "Found total_water with '%d'->'%f'", total, ret_val.value());
    }

    return ret_val;
  };

  unsigned char decode_vectors_[16][6] = {{ 117, 150, 122,  16,  26,  10 }, {  91, 127, 112,  19,  34,  19 },
                                          { 179,  24, 185,  11, 142, 153 }, { 142, 125, 121,   7,  74,  22 },
                                          { 181, 145,   7, 154, 203, 105 }, { 184, 163,  50, 161,  57,  14 },
                                          { 189, 128, 156, 126,  96, 153 }, {  39,  92, 180, 196, 128, 163 },
                                          {  48, 208,  10, 206,  25,   3 }, { 194,  76, 240,   5, 165, 134 },
                                          {  84,  75,  22, 152,  17,  94 }, {  75, 238,  12, 201, 125, 162 },
                                          { 135, 202,  74,  72, 228,  31 }, { 196, 135, 119,  46, 138, 232 },
                                          { 227,  48, 189, 120,  87, 140 }, { 164, 154,  57, 111,  40,   5 }
                                         };

};