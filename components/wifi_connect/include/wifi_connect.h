#pragma once
#include "esp_err.h"

// En Wokwi, la red siempre se llama así:
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASS ""

void wifi_init(void);