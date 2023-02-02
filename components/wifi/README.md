# esphome_wifi
Custom Esphome wifi component.

The sorting of WiFi AP's with the same SSID name is in this custom component no longer based on priority. This makes a WiFi rescan, example below, to pick the AP with the strongest signal, regardless of priority.


# Example configuration entry.
...
external_components:
  - source:
      type: git
      url: https://github.com/zdzichu6969/esphome-components
    components: [ wifi ]
    refresh: 0s
...



<pre>
time:
  - platform: homeassistant
    id: homeassistant_time
    on_time:
      - seconds: 0
        minutes: /5
        then:
          - lambda: |-
              if (wifi::global_wifi_component->wifi_rssi() < -70) 
              {
                WiFi.disconnect();
                wifi::global_wifi_component->start_scanning();
              }
</pre>