#pragma once

#include "esp_err.h"

class Bme280Driver {
private:
    int _sda_pin;
    int _scl_pin;
    float _simulated_temp; // Variable temporal para simulación

public:
    // Constructor
    Bme280Driver(int sda, int scl);

    // Método para inicializar el sensor
    esp_err_t init();

    // Método para leer temperatura
    float readTemperature();
};