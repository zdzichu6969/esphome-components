# `ESPHome` components

[![License][license-shield]][license]

[license-shield]: https://img.shields.io/static/v1?label=License&message=MIT&color=orange&logo=license
[license]: https://opensource.org/licenses/MIT

A collection of my ESPHome components.

To use this repository you should confugure it inside your yaml-configuration:
```yaml
external_components:
  - source: github://zdzichu6969/esphome_custom_components
```

## [Energy Production Statistics (PV)](components/energy_production_statistics)
Gather energy production statistics.

## [Energy Production Statistics](components/energy_statistics)
Gather energy statistics. For energy meters.

## [Startup Sensor](components/startup/)
Uptime/Startup sensor based on timestamp.

## [Backup](components/backup/)
Backup config yaml.

## [Energy Monitoring](components/energy_monitoring)

## [Energy Tarrifs](components/energy_tarrifs)
Calculate tarrifs from energy meter.

## [Crash info](components/crash_info)
Show last crash log.

## [Heapmon](components/heapmon)
Show heapsize entity.