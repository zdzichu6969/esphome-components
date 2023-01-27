# Energy Statistics

Gather statistics for (ex. PV):
* today
* yesterday
* week
* month

```yaml
# Example configuration entry
...
external_components:
  - source:
      type: git
      url: https://github.com/zdzichu6969/esphome_custom_components
    components: [ energy_production_statistics ]
    refresh: 0s
...
sensor:
  - platform: "energy_production_statistics"
    total: total
    energy_production_today:
      name: "$name Energy Today"
    energy_production_yesterday:
      name: "$name Energy Yesterday"
    energy_production_week:
      name: "$name Energy Week"
    energy_production_month:
      name: "$name Energy Month"
```

## Configuration variables:
* **id** (*Optional*, [ID](https://esphome.io/guides/configuration-types.html#config-id)): Manually specify the ID used for code generation.
* **total** (**Required**, [ID](https://esphome.io/guides/configuration-types.html#config-id)): The ID of the total power sensor.
* **energy_production_today** (*Optional*, Sensor):
  * Any options from [Sensor](https://esphome.io/components/sensor/index.html#config-sensor).
* **energy_production_yesterday** (*Optional*, Sensor):
  * Any options from [Sensor](https://esphome.io/components/sensor/index.html#config-sensor).
* **energy_production_week** (*Optional*, Sensor):
  * Any options from [Sensor](https://esphome.io/components/sensor/index.html#config-sensor).
* **energy_production_month** (*Optional*, Sensor):
  * Any options from [Sensor](https://esphome.io/components/sensor/index.html#config-sensor).
