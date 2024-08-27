/*
  Based on: https://github.com/wmbusmeters/wmbusmeters/blob/master/src/driver_flowiq2200.cc
  Copyright (C) 2017-2022 Fredrik Öhrström (gpl-3.0-or-later)
*/

#pragma once
 
#include "driver.h"

#include <vector>
#include <string>

struct Flowiq2200: Driver
{
  Flowiq2200(std::string key = "") : Driver(std::string("flowiq2200"), key) {};
  virtual esphome::optional<std::map<std::string, double>> get_values(std::vector<unsigned char> &telegram) override {
    std::map<std::string, double> ret_val{};

    add_to_map(ret_val, "total_water_m3", this->get_total_water_m3(telegram));
    add_to_map(ret_val, "target_water_m3", this->get_target_water_m3(telegram));
    add_to_map(ret_val, "status", this->get_status(telegram));
    add_to_map(ret_val, "volume_flow_lh", this->get_volume_flow_lh(telegram));
    add_to_map(ret_val, "min_flow_lh", this->get_min_flow_lh(telegram));
    add_to_map(ret_val, "max_flow_lh", this->get_max_flow_lh(telegram));

    if (ret_val.size() > 0) {
      return ret_val;
    }
    else {
      return {};
    }
  };

//                             X         X     X                             X      X
//           CRC             status    prev   maxF    06FF1B    date T1 T2 flow   total
// 0001     1718 1920212223 24252627 28293031 3233 343536373839 4041 42 43 4445 46474849 50
// 3244 ... FDC3 7905099CE6 00000000 7CE20300 7C05 06600006D000 0132 0B 11 0000 79E70300 0F
// see https://github.com/SzczepanLeon/esphome-components/issues/86

private:
  esphome::optional<double> get_total_water_m3(std::vector<unsigned char> &telegram) {
    esphome::optional<double> ret_val{};
    uint8_t l_field = telegram[0];
    uint8_t tpl_ci_field = telegram[19];

    if (tpl_ci_field == 0x78) {
      ret_val = this->get_0413(telegram);
    }
    else if ((tpl_ci_field == 0x79) && (l_field > 49)) {
       uint16_t signature = ((uint16_t)telegram[20] << 8) | telegram[21];
       ESP_LOGVV(TAG, "Signature of message is: '%X'", signature); 
       if (signature == 0xF3A9) {
         uint32_t usage{0};
         uint8_t i = 28;
         usage = (((uint32_t)telegram[i+3] << 24) | ((uint32_t)telegram[i+2] << 16) |
                  ((uint32_t)telegram[i+1] << 8)  | ((uint32_t)telegram[i+0]));
         ret_val = usage / 1000.0;
         ESP_LOGVV(TAG, "Found total_water with '%d'->'%f'", usage, ret_val.value());
        }
    }
    return ret_val;
  };

  esphome::optional<double> get_target_water_m3(std::vector<unsigned char> &telegram) {
    esphome::optional<double> ret_val{};
    uint8_t l_field = telegram[0];
    uint8_t tpl_ci_field = telegram[19];
    
    if (tpl_ci_field == 0x78) {
      ret_val = this->get_4413(telegram);
    }
    else if ((tpl_ci_field == 0x79) && (l_field > 49)) {
      uint16_t signature = ((uint16_t)telegram[20] << 8) | telegram[21];
      ESP_LOGVV(TAG, "Signature of message is: '%X'", signature); 
      if (signature == 0xF3A9) {
        uint32_t usage{0};
        uint8_t i = 32;
        usage = (((uint32_t)telegram[i+3] << 24) | ((uint32_t)telegram[i+2] << 16) |
                 ((uint32_t)telegram[i+1] << 8)  | ((uint32_t)telegram[i+0]));
        ret_val = usage / 1000.0;
        ESP_LOGVV(TAG, "Found target_water with '%d'->'%f'", usage, ret_val.value());
      }
    }
    return ret_val;
  };

  esphome::optional<double> get_status(std::vector<unsigned char> &telegram) {
    esphome::optional<double> ret_val{};
    uint8_t l_field = telegram[0];
    uint8_t tpl_ci_field = telegram[19];
    
    if (tpl_ci_field == 0x78) {
      ret_val = this->get_04FF23(telegram);  
    }
    else if ((tpl_ci_field == 0x79) && (l_field > 49)) {
      uint16_t signature = ((uint16_t)telegram[20] << 8) | telegram[21];
      ESP_LOGVV(TAG, "Signature of message is: '%X'", signature); 
      if (signature == 0xF3A9) { 
        uint32_t status{0};
        uint8_t i = 24;
        status = (((uint32_t)telegram[i+3] << 24) | ((uint32_t)telegram[i+2] << 16) |
                  ((uint32_t)telegram[i+1] << 8)  | ((uint32_t)telegram[i+0]));
        ret_val = (double)status;
        ESP_LOGVV(TAG, "Found status with '%08X'", status);
      }
    }
    return ret_val;
  };

  esphome::optional<double> get_volume_flow_lh(std::vector<unsigned char> &telegram) {
    esphome::optional<double> ret_val{};
    uint8_t l_field = telegram[0];
    uint8_t tpl_ci_field = telegram[19];
    
    if (tpl_ci_field == 0x78) {
      ret_val = this->get_023B(telegram);  
    }
    else if ((tpl_ci_field == 0x79) && (l_field > 49)) {
      uint16_t signature = ((uint16_t)telegram[20] << 8) | telegram[21];
      ESP_LOGVV(TAG, "Signature of message is: '%X'", signature); 
      if (signature == 0xF3A9) {
        uint32_t flow{0};
        uint8_t i = 22;
        flow = (((uint32_t)telegram[i+1] << 8)  | ((uint32_t)telegram[i+0]));
        ret_val = (double)flow;
        ESP_LOGVV(TAG, "Found volume_flow with '%d'->'%f'", flow, ret_val.value());
      }
    }
    return ret_val;
  };

  esphome::optional<double> get_min_flow_lh(std::vector<unsigned char> &telegram) {
    esphome::optional<double> ret_val{};
    uint8_t l_field = telegram[0];
    uint8_t tpl_ci_field = telegram[19];

    if (tpl_ci_field == 0x78) {
      ret_val = this->get_523B(telegram);  
    }
    else if ((tpl_ci_field == 0x79) && (l_field > 49)) {
      uint16_t signature = ((uint16_t)telegram[20] << 8) | telegram[21];
      ESP_LOGVV(TAG, "Signature of message is: '%X'", signature); 
      if (signature == 0xF3A9) {   
        uint32_t flow{0};
        uint8_t i = 27;
        flow = (((uint32_t)telegram[i+1] << 8)  | ((uint32_t)telegram[i+0]));
        ret_val = (double)flow;
        ESP_LOGVV(TAG, "Found min_flow with '%d'->'%f'", flow, ret_val.value());
      }
    }
    return ret_val;
  };
  esphome::optional<double> get_max_flow_lh(std::vector<unsigned char> &telegram) {
    esphome::optional<double> ret_val{};
    uint8_t l_field = telegram[0];
    uint8_t tpl_ci_field = telegram[19];

    if (tpl_ci_field == 0x78) {
      ret_val = this->get_523B(telegram);  
    }
    else if ((tpl_ci_field == 0x79) && (l_field > 49)) {
      uint16_t signature = ((uint16_t)telegram[20] << 8) | telegram[21];
      ESP_LOGVV(TAG, "Signature of message is: '%X'", signature); 
      if (signature == 0xF3A9) {   
        uint32_t flow{0};
        uint8_t i = 40;
        flow = (((uint32_t)telegram[i+1] << 8)  | ((uint32_t)telegram[i+0]));
        ret_val = (double)flow;
        ESP_LOGVV(TAG, "Found max_flow with '%d'->'%f'", flow, ret_val.value());
      }
    }
    return ret_val;
  };

};
