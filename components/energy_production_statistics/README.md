# Energy production statistics

Component calculating production deltas from a cumulative energy sensor.
It exposes:
* today
* yesterday
* week
* month

## What was fixed

* `yesterday` no longer reports multi-day deltas after skipped days, reboots, or missing time sync.
* year rollover is handled correctly.
* preferences are saved only when the day changes, instead of on every sensor update.
* output sensors use `measurement` state class instead of `total_increasing`.
* removed broken Python code path referencing non-existent `set_power()`.

## Example configuration

```yaml
external_components:
  - source:
      type: git
      url: https://github.com/zdzichu6969/esphome-components
    components: [ energy_production_statistics ]
    refresh: 0s

sensor:
  - platform: energy_production_statistics
    time_id: sntp_time1
    total: esp_sofar_energy_total
    energy_production_today:
      name: "$name Energy Today"
    energy_production_yesterday:
      name: "$name Energy Yesterday"
    energy_production_week:
      name: "$name Energy Week"
    energy_production_month:
      name: "$name Energy Month"
```

## Notes

* `total` must be a cumulative lifetime energy sensor in kWh.
* `time_id` is required and should point to a valid ESPHome time component.
* If the device misses one or more full days, `yesterday` is published as `NaN` until the next valid day transition. This avoids reporting incorrect multi-day production as yesterday's value.
