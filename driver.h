#pragma once

#include <vector>
#include <string>
#include <map>
#include "esphome/core/log.h"
#include "esphome/core/optional.h"

static const char *TAG = "drivers";

using namespace esphome;

struct Driver
{
  virtual ~Driver() = default; 

  public:
    virtual esphome::optional<std::map<std::string, double>> get_values(std::vector<unsigned char> &telegram) = 0;

    std::string get_name() { return this->driver_type_; };
    std::string get_key() { return this->key_; };

  protected:
    Driver(std::string driver_type, std::string key = "") :
          driver_type_(driver_type),
          key_(key) {
      ESP_LOGVV(TAG, "Added driver '%s' with key '%s'",
              this->driver_type_.c_str(), this->key_.c_str());
    };

    void add_to_map(std::map<std::string, double> &values,
                    std::string name,
                    esphome::optional<double> value) {
      if (value.has_value()) {
        ESP_LOGVV(TAG, "Value added to map");
        values[name] = *value;
      }
    };

    uint32_t bcd_2_int(const std::vector<unsigned char> &telegram, size_t start, size_t length) {
      ESP_LOGVV(TAG, "Converting BCD to INT");
      uint32_t result{0};
      uint16_t l_half{0};
      uint16_t h_half{0};
      uint32_t factor{1};
      uint8_t i{0};
      while (i < length) {
        h_half = (telegram[start + i] & 0xF0) >> 4;
        l_half = telegram[start + i] & 0x0F;
        result += ((h_half * 10) + l_half) * factor;
        factor *= 100;
        i++;
      }
      return result;
    };

  // /
    esphome::optional<double> get_022D(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t usage = 0;
      size_t i = 11;
      uint32_t total_register = 0x022D;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t)telegram[i+0] << 8) | ((uint32_t)telegram[i+1]));
        if (c == total_register) {
          i += 2;
          usage = ((uint32_t)telegram[i+1] << 8)  | ((uint32_t)telegram[i+0]);
          // in kW
          ret_val = (double)usage/10.0;
          ESP_LOGVV(TAG, "Found register '022D' with '%d'->'%f'", usage, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };

    esphome::optional<double> get_023B(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t usage = 0;
      size_t i = 11;
      uint32_t total_register = 0x023B;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t)telegram[i+0] << 8) | ((uint32_t)telegram[i+1]));
        if (c == total_register) {
          i += 2;
          usage = ((uint32_t)telegram[i+1] << 8)  | ((uint32_t)telegram[i+0]);
          // in l/h
          ret_val = (double)usage;
          ESP_LOGVV(TAG, "Found register '023B' with '%d'->'%f'", usage, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };

    esphome::optional<double> get_0259(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t usage = 0;
      size_t i = 11;
      uint32_t total_register = 0x0259;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t)telegram[i+0] << 8) | ((uint32_t)telegram[i+1]));
        if (c == total_register) {
          i += 2;
          usage = ((uint32_t)telegram[i+1] << 8)  | ((uint32_t)telegram[i+0]);
          // in °C
          ret_val = (double)usage / 100.0;
          ESP_LOGVV(TAG, "Found register '0259' with '%d'->'%f'", usage, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };

    esphome::optional<double> get_025D(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t usage = 0;
      size_t i = 11;
      uint32_t total_register = 0x025D;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t)telegram[i+0] << 8) | ((uint32_t)telegram[i+1]));
        if (c == total_register) {
          i += 2;
          usage = ((uint32_t)telegram[i+1] << 8)  | ((uint32_t)telegram[i+0]);
          // in °C
          ret_val = (double)usage / 100.0;
          ESP_LOGVV(TAG, "Found register '025D' with '%d'->'%f'", usage, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };

    esphome::optional<double> get_523B(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t usage = 0;
      size_t i = 11;
      uint32_t total_register = 0x523B;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t)telegram[i+0] << 8) | ((uint32_t)telegram[i+1]));
        if (c == total_register) {
          i += 2;
          usage = ((uint32_t)telegram[i+1] << 8)  | ((uint32_t)telegram[i+0]);
          // in l/h
          ret_val = (double)usage;
          ESP_LOGVV(TAG, "Found register '523B' with '%d'->'%f'", usage, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };

    esphome::optional<double> get_043B(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t usage = 0;
      size_t i = 11;
      uint32_t total_register = 0x043B;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t)telegram[i+0] << 8) | ((uint32_t)telegram[i+1]));
        if (c == total_register) {
          i += 2;
          usage = ((uint32_t)telegram[i+3] << 24) | ((uint32_t)telegram[i+2] << 16) |
                  ((uint32_t)telegram[i+1] << 8)  | ((uint32_t)telegram[i+0]);
          // in l/h
          ret_val = (double)usage;
          ESP_LOGVV(TAG, "Found register '043B' with '%d'->'%f'", usage, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };

    esphome::optional<double> get_0405(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t usage = 0;
      size_t i = 11;
      uint32_t total_register = 0x0405;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t)telegram[i+0] << 8) | ((uint32_t)telegram[i+1]));
        if (c == total_register) {
          i += 2;
          usage = ((uint32_t)telegram[i+3] << 24) | ((uint32_t)telegram[i+2] << 16) |
                  ((uint32_t)telegram[i+1] << 8)  | ((uint32_t)telegram[i+0]);
          // in kWh
          ret_val = usage / 10.0;
          ESP_LOGVV(TAG, "Found register '0405' with '%d'->'%f'", usage, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };

    esphome::optional<double> get_0406(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t usage = 0;
      size_t i = 11;
      uint32_t total_register = 0x0406;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t)telegram[i+0] << 8) | ((uint32_t)telegram[i+1]));
        if (c == total_register) {
          i += 2;
          usage = ((uint32_t)telegram[i+3] << 24) | ((uint32_t)telegram[i+2] << 16) |
                  ((uint32_t)telegram[i+1] << 8)  | ((uint32_t)telegram[i+0]);
          // in kWh
          ret_val = usage / 1.0;
          ESP_LOGVV(TAG, "Found register '0406' with '%d'->'%f'", usage, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };

    esphome::optional<double> get_0414(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t usage = 0;
      size_t i = 11;
      uint32_t total_register = 0x0414;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t)telegram[i+0] << 8) | ((uint32_t)telegram[i+1]));
        if (c == total_register) {
          i += 2;
          usage = ((uint32_t)telegram[i+3] << 24) | ((uint32_t)telegram[i+2] << 16) |
                  ((uint32_t)telegram[i+1] << 8)  | ((uint32_t)telegram[i+0]);
          ret_val = usage / 100.0;
          ESP_LOGVV(TAG, "Found register '0414' with '%d'->'%f'", usage, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };

    esphome::optional<double> get_0413(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t usage = 0;
      size_t i = 11;
      uint32_t total_register = 0x0413;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t)telegram[i+0] << 8) | ((uint32_t)telegram[i+1]));
        if (c == total_register) {
          i += 2;
          usage = ((uint32_t)telegram[i+3] << 24) | ((uint32_t)telegram[i+2] << 16) |
                  ((uint32_t)telegram[i+1] << 8)  | ((uint32_t)telegram[i+0]);
          ret_val = usage / 1000.0;
          ESP_LOGVV(TAG, "Found register '0413' with '%d'->'%f'", usage, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };

    esphome::optional<double> get_4413(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t usage = 0;
      size_t i = 11;
      uint32_t total_register = 0x4413;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t)telegram[i+0] << 8) | ((uint32_t)telegram[i+1]));
        if (c == total_register) {
          i += 2;
          usage = ((uint32_t)telegram[i+3] << 24) | ((uint32_t)telegram[i+2] << 16) |
                  ((uint32_t)telegram[i+1] << 8)  | ((uint32_t)telegram[i+0]);
          ret_val = usage / 1000.0;
          ESP_LOGVV(TAG, "Found register '4413' with '%d'->'%f'", usage, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };

    esphome::optional<double> get_615B(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t temperature = 0;
      size_t i = 11;
      uint32_t temperature_register = 0x615B;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t) telegram[i + 0] << 8) | ((uint32_t) telegram[i + 1]));
        if (c == temperature_register) {
          i += 2;
          temperature = ((uint32_t) telegram[i + 0]);
          // in celcius degrees
          ret_val = temperature / 1.0;
          ESP_LOGVV(TAG, "Found register '615B' with '%d'->'%f'", temperature, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };

    esphome::optional<double> get_6167(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t temperature = 0;
      size_t i = 11;
      uint32_t temperature_register = 0x6167;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t) telegram[i + 0] << 8) | ((uint32_t) telegram[i + 1]));
        if (c == temperature_register) {
          i += 2;
          temperature = ((uint32_t) telegram[i + 0]);
          // in celcius degrees
          ret_val = temperature / 1.0;
          ESP_LOGVV(TAG, "Found register '6167' with '%d'->'%f'", temperature, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };

    esphome::optional<double> get_040F(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t usage = 0;
      size_t i = 11;
      uint32_t total_register = 0x040F;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t)telegram[i+0] << 8) | ((uint32_t)telegram[i+1]));
        if (c == total_register) {
          i += 2;
          usage = ((uint32_t)telegram[i+3] << 24) | ((uint32_t)telegram[i+2] << 16) |
                  ((uint32_t)telegram[i+1] << 8)  | ((uint32_t)telegram[i+0]);
          // in kWh
          ret_val = usage / 3600000.0;
          ESP_LOGVV(TAG, "Found register '040F' with '%d'->'%f'", usage, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };

    esphome::optional<double> get_0C0E(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t usage = 0;
      size_t i = 11;
      uint32_t total_register = 0x0C0E;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t)telegram[i+0] << 8) | ((uint32_t)telegram[i+1]));
        if (c == total_register) {
          i += 2;
          usage = bcd_2_int(telegram, i, 4);
          // in kWh
          ret_val = usage / 3.6;
          ESP_LOGVV(TAG, "Found register '0C0E' with '%d'->'%f'", usage, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };


    esphome::optional<double> get_0C09(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t usage = 0;
      size_t i = 11;
      uint32_t total_register = 0x0C09;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t)telegram[i+0] << 8) | ((uint32_t)telegram[i+1]));
        if (c == total_register) {
          i += 2;
          usage = bcd_2_int(telegram, i, 4);
          // in kWh
          ret_val = usage / 360000.0;
          ESP_LOGVV(TAG, "Found register '0C09' with '%d'->'%f'", usage, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };

    esphome::optional<double> get_0C0D(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t usage = 0;
      size_t i = 11;
      uint32_t total_register = 0x0C0D;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t)telegram[i+0] << 8) | ((uint32_t)telegram[i+1]));
        if (c == total_register) {
          i += 2;
          usage = bcd_2_int(telegram, i, 4);
          // in kWh
          ret_val = usage / 36.00000;
          ESP_LOGVV(TAG, "Found register '0C0D' with '%d'->'%f'", usage, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };

    esphome::optional<double> get_0C03(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t usage = 0;
      size_t i = 11;
      uint32_t total_register = 0x0C03;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t)telegram[i+0] << 8) | ((uint32_t)telegram[i+1]));
        if (c == total_register) {
          i += 2;
          usage = bcd_2_int(telegram, i, 4);
          // in kWh
          ret_val = usage / 1000.0;
          ESP_LOGVV(TAG, "Found register '0C03' with '%d'->'%f'", usage, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };

    esphome::optional<double> get_0C05(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t usage = 0;
      size_t i = 11;
      uint32_t total_register = 0x0C05;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t)telegram[i+0] << 8) | ((uint32_t)telegram[i+1]));
        if (c == total_register) {
          i += 2;
          usage = bcd_2_int(telegram, i, 4);
          // in kWh
          ret_val = usage / 10.0;
          ESP_LOGVV(TAG, "Found register '0C05' with '%d'->'%f'", usage, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };

    esphome::optional<double> get_0C06(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t usage = 0;
      size_t i = 11;
      uint32_t total_register = 0x0C06;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t)telegram[i+0] << 8) | ((uint32_t)telegram[i+1]));
        if (c == total_register) {
          i += 2;
          usage = bcd_2_int(telegram, i, 4);
          // in kWh
          ret_val = usage / 1.0;
          ESP_LOGVV(TAG, "Found register '0C06' with '%d'->'%f'", usage, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };

    esphome::optional<double> get_0C13(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t usage = 0;
      size_t i = 11;
      uint32_t total_register = 0x0C13;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t)telegram[i+0] << 8) | ((uint32_t)telegram[i+1]));
        if (c == total_register) {
          i += 2;
          usage = bcd_2_int(telegram, i, 4);
          ret_val = usage / 1000.0;
          ESP_LOGVV(TAG, "Found register '0C13' with '%d'->'%f'", usage, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };

    esphome::optional<double> get_0C0A(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t usage = 0;
      size_t i = 11;
      uint32_t total_register = 0x0C0A;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t)telegram[i+0] << 8) | ((uint32_t)telegram[i+1]));
        if (c == total_register) {
          i += 2;
          usage = bcd_2_int(telegram, i, 4);
          // in kWh
          ret_val = usage / 36000.0;
          ESP_LOGVV(TAG, "Found register '0C0A' with '%d'->'%f'", usage, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };

    esphome::optional<double> get_0E0A(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t usage = 0;
      size_t i = 11;
      uint32_t total_register = 0x0E0A;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t)telegram[i+0] << 8) | ((uint32_t)telegram[i+1]));
        if (c == total_register) {
          i += 2;
          usage = bcd_2_int(telegram, i, 6);
          // in kWh
          ret_val = usage / 36000.0;
          ESP_LOGVV(TAG, "Found register '0E0A' with '%d'->'%f'", usage, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };

    esphome::optional<double> get_0E01(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t usage = 0;
      size_t i = 11;
      uint32_t total_register = 0x0E01;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t)telegram[i+0] << 8) | ((uint32_t)telegram[i+1]));
        if (c == total_register) {
          i += 2;
          usage = bcd_2_int(telegram, i, 6);
          // in kWh
          ret_val = usage / 100000.0;
          ESP_LOGVV(TAG, "Found register '0E01' with '%d'->'%f'", usage, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };

    esphome::optional<double> get_0A2D(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t usage = 0;
      size_t i = 11;
      uint32_t total_register = 0x0A2D;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t)telegram[i+0] << 8) | ((uint32_t)telegram[i+1]));
        if (c == total_register) {
          i += 2;
          usage = bcd_2_int(telegram, i, 2);
          // in kW
          ret_val = usage / 10.0;
          ESP_LOGVV(TAG, "Found register '0A2D' with '%d'->'%f'", usage, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };

    esphome::optional<double> get_0A5A(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t usage = 0;
      size_t i = 11;
      uint32_t total_register = 0x0A5A;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t)telegram[i+0] << 8) | ((uint32_t)telegram[i+1]));
        if (c == total_register) {
          i += 2;
          usage = bcd_2_int(telegram, i, 2);
          // in C
          ret_val = usage / 10.0;
          ESP_LOGVV(TAG, "Found register '0A5A' with '%d'->'%f'", usage, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };

    esphome::optional<double> get_0A5E(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t usage = 0;
      size_t i = 11;
      uint32_t total_register = 0x0A5E;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t)telegram[i+0] << 8) | ((uint32_t)telegram[i+1]));
        if (c == total_register) {
          i += 2;
          usage = bcd_2_int(telegram, i, 2);
          // in C
          ret_val = usage / 10.0;
          ESP_LOGVV(TAG, "Found register '0A5E' with '%d'->'%f'", usage, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };

    esphome::optional<double> get_04FF23(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      size_t i = 11;
      uint32_t status_register = 0x04FF23;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t)telegram[i+0] << 16) | ((uint32_t)telegram[i+1] << 8) | ((uint32_t)telegram[i+2]));
        if (c == status_register) {
          i += 3;
          uint32_t alarms = ((uint32_t)telegram[i+3] << 24) | ((uint32_t)telegram[i+2] << 16) |
                            ((uint32_t)telegram[i+1] << 8)  | ((uint32_t)telegram[i+0]);
          ret_val = (double)alarms;
          ESP_LOGV(TAG, "Found register '04FF23' with '%08X'", alarms);
          break;
        }
        i++;
      }
      return ret_val;
    };

    esphome::optional<double> get_0C943A(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t usage = 0;
      size_t i = 11;
      uint32_t total_register = 0x0C943A;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t)telegram[i+0] << 16) | ((uint32_t)telegram[i+1] << 8) | ((uint32_t)telegram[i+2]));
        if (c == total_register) {
          i += 3;
          usage = bcd_2_int(telegram, i, 4);
          ret_val = usage / 100.0;
          ESP_LOGVV(TAG, "Found register '0C943A' with '%d'->'%f'", usage, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };

    esphome::optional<double> get_0C2B(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t usage = 0;
      size_t i = 11;
      uint32_t total_register = 0x0C2B;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t)telegram[i + 0] << 8) | ((uint32_t)telegram[i + 1]));
        if (c == total_register) {
          i += 2;
          usage = bcd_2_int(telegram, i, 4);
          // in kW
          ret_val = usage / 1000.0;
          ESP_LOGVV(TAG, "Found register '0C2B' with '%d'->'%f'", usage, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };

    esphome::optional<double> get_0B2D(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t usage = 0;
      size_t i = 11;
      uint32_t total_register = 0x0B2D;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t)telegram[i + 0] << 8) | ((uint32_t)telegram[i + 1]));
        if (c == total_register) {
          i += 2;
          usage = bcd_2_int(telegram, i, 3);
          // in kW
          ret_val = usage / 10.0;
          ESP_LOGVV(TAG, "Found register '0B2D' with '%d'->'%f'", usage, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };

    esphome::optional<double> get_0B3B(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t usage = 0;
      size_t i = 11;
      uint32_t total_register = 0x0B3B;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t)telegram[i + 0] << 8) | ((uint32_t)telegram[i + 1]));
        if (c == total_register) {
          i += 2;
          usage = bcd_2_int(telegram, i, 3);
          // in l/h
          ret_val = (double)usage;
          ESP_LOGVV(TAG, "Found register '0B3B' with '%d'->'%f'", usage, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };

    esphome::optional<double> get_0B26(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t usage = 0;
      size_t i = 11;
      uint32_t total_register = 0x0B26;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t)telegram[i + 0] << 8) | ((uint32_t)telegram[i + 1]));
        if (c == total_register) {
          i += 2;
          usage = bcd_2_int(telegram, i, 3);
          // in day
          ret_val = usage / 24.0;
          ESP_LOGVV(TAG, "Found register '0B26' with '%d'->'%f'", usage, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };

    esphome::optional<double> get_4315(std::vector<unsigned char> &telegram) {
      esphome::optional<double> ret_val{};
      uint32_t usage = 0;
      size_t i = 11;
      uint32_t total_register = 0x4315;
      while (i < telegram.size()) {
        uint32_t c = (((uint32_t)telegram[i + 0] << 8) | ((uint32_t)telegram[i + 1]));
        if (c == total_register) {
          i += 2;
          usage = (((uint32_t)telegram[i+2] << 16) | ((uint32_t)telegram[i+1] << 8)  | ((uint32_t)telegram[i+0]));
          // in m3
          ret_val = usage / 1000.0;
          ESP_LOGVV(TAG, "Found register '4315' with '%d'->'%f'", usage, ret_val.value());
          break;
        }
        i++;
      }
      return ret_val;
    };

  private:
    Driver();
    std::string driver_type_;
    std::string key_;
};
