#include "esphome/core/log.h"
#include "esphome/core/hal.h"
#include "energy_production_statistics.h"

namespace esphome {
namespace energy_production_statistics {

static const char *const TAG = "energy_production_statistics";
static const char *const GAP = "  ";

void EnergyStatistics::dump_config() {
  ESP_LOGCONFIG(TAG, "Energy statistics sensors");
  if (this->energy_production_today_) {
    LOG_SENSOR(GAP, "Energy Today", this->energy_production_today_);
  }
  if (this->energy_production_yesterday_) {
    LOG_SENSOR(GAP, "Energy Yesterday", this->energy_production_yesterday_);
  }
  if (this->energy_production_week_) {
    LOG_SENSOR(GAP, "Energy Week", this->energy_production_week_);
  }
  if (this->energy_production_month_) {
    LOG_SENSOR(GAP, "Energy Month", this->energy_production_month_);
  }
}

void EnergyStatistics::setup() {
  if (this->total_ != nullptr) {
    this->total_->add_on_state_callback([this](float state) { this->process_(state); });
  }

  this->pref_ = global_preferences->make_preference<energy_production_data_t>(fnv1_hash(TAG));

  energy_production_data_t loaded{};
  if (this->pref_.load(&loaded)) {
    this->energy_production_ = loaded;
  }

  const auto total = this->total_ != nullptr ? this->total_->get_state() : NAN;
  if (!std::isnan(total)) {
    this->process_(total);
  }
}

bool EnergyStatistics::is_same_day_(const ESPTime &t) const {
  return t.year == this->energy_production_.current_year &&
         t.day_of_year == this->energy_production_.current_day_of_year;
}

bool EnergyStatistics::is_next_day_(const ESPTime &t) const {
  if (this->energy_production_.current_year == 0) {
    return false;
  }

  if (t.year == this->energy_production_.current_year) {
    return t.day_of_year == static_cast<int>(this->energy_production_.current_day_of_year) + 1;
  }

  return t.year == static_cast<int>(this->energy_production_.current_year) + 1 && t.day_of_year == 1;
}

void EnergyStatistics::loop() {
  if (this->time_ == nullptr || this->total_ == nullptr) {
    return;
  }

  const auto t = this->time_->now();
  if (!t.is_valid()) {
    // time is not sync yet
    return;
  }

  const auto total = this->total_->get_state();
  if (std::isnan(total)) {
    // total is not published yet
    return;
  }

  if (this->energy_production_.current_year == 0) {
    this->energy_production_.current_year = t.year;
    this->energy_production_.current_day_of_year = t.day_of_year;
    this->energy_production_.start_today = total;

    if (std::isnan(this->energy_production_.start_week)) {
      this->energy_production_.start_week = total;
    }
    if (std::isnan(this->energy_production_.start_month)) {
      this->energy_production_.start_month = total;
    }

    this->save_();
    this->process_(total);
    return;
  }

  if (this->is_same_day_(t)) {
    // nothing to do
    return;
  }

  const bool next_day = this->is_next_day_(t);
  const float completed_day = total - this->energy_production_.start_today;

  if (next_day && !std::isnan(this->energy_production_.start_today) && completed_day >= 0.0f) {
    this->energy_production_.yesterday_value = completed_day;
  } else {
    // We skipped one or more days or the counter moved backwards, so a reliable
    // "yesterday" value cannot be reconstructed from the cumulative total.
    this->energy_production_.yesterday_value = NAN;
  }

  this->energy_production_.start_today = total;

  // At specified day of week we start a new week calculation.
  if (t.day_of_week == this->energy_production_week_start_day_) {
    this->energy_production_.start_week = total;
  }

  // At configured start day of month we start a new month calculation.
  if (t.day_of_month == this->energy_production_month_start_day_) {
    this->energy_production_.start_month = total;
  }

  this->energy_production_.current_year = t.year;
  this->energy_production_.current_day_of_year = t.day_of_year;

  this->save_();
  this->process_(total);
}

void EnergyStatistics::process_(float total) {
  if (this->energy_production_today_ && !std::isnan(this->energy_production_.start_today)) {
    const float value = total - this->energy_production_.start_today;
    this->energy_production_today_->publish_state(value >= 0.0f ? value : NAN);
  }

  if (this->energy_production_yesterday_) {
    this->energy_production_yesterday_->publish_state(this->energy_production_.yesterday_value);
  }

  if (this->energy_production_week_ && !std::isnan(this->energy_production_.start_week)) {
    const float value = total - this->energy_production_.start_week;
    this->energy_production_week_->publish_state(value >= 0.0f ? value : NAN);
  }

  if (this->energy_production_month_ && !std::isnan(this->energy_production_.start_month)) {
    const float value = total - this->energy_production_.start_month;
    this->energy_production_month_->publish_state(value >= 0.0f ? value : NAN);
  }
}

void EnergyStatistics::save_() { this->pref_.save(&(this->energy_production_)); }

}  // namespace energy_production_statistics
}  // namespace esphome
