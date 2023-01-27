#pragma once

#include "esphome/core/component.h"
#include "esphome/core/preferences.h"

#include "esphome/components/sensor/sensor.h"
#include "esphome/components/time/real_time_clock.h"

namespace esphome {
namespace energy_production_statistics {

using sensor::Sensor;

class EnergyStatistics : public Component {
 public:
  float get_setup_priority() const override { return setup_priority::DATA; }
  void dump_config() override;
  void setup() override;
  void loop() override;

  void set_time(time::RealTimeClock *time) { this->time_ = time; }
  void set_total(Sensor *sensor) { this->total_ = sensor; }

  void set_energy_production_today(Sensor *sensor) { this->energy_production_today_ = sensor; }
  void set_energy_production_yesterday(Sensor *sensor) { this->energy_production_yesterday_ = sensor; }
  void set_energy_production_week(Sensor *sensor) { this->energy_production_week_ = sensor; }
  void set_energy_production_month(Sensor *sensor) { this->energy_production_month_ = sensor; }

 protected:
  ESPPreferenceObject pref_;
  time::RealTimeClock *time_;

  // input sensors
  Sensor *total_{nullptr};

  // exposed sensors
  Sensor *energy_production_today_{nullptr};
  Sensor *energy_production_yesterday_{nullptr};
  Sensor *energy_production_week_{nullptr};
  Sensor *energy_production_month_{nullptr};

  // start day of week configuration
  int energy_production_week_start_day_{2};
  // start day of month configuration
  int energy_production_month_start_day_{1};

  struct energy_production_data_t {
    uint16_t current_day_of_year{0};
    float start_today{NAN};
    float start_yesterday{NAN};
    float start_week{NAN};
    float start_month{NAN};
  } energy_production_;

  void process_(float total);
  void save_();
};  // class EnergyStatistics

}  // namespace energy_production_statistics
}  // namespace esphome
