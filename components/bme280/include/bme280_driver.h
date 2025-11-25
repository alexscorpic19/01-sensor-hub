#pragma once

#include "esp_err.h"
#include "driver/i2c.h"

// Direcciones del BME280
#define BME280_SENSOR_ADDR  0x77
#define BME280_CHIP_ID_REG  0xD0

class Bme280Driver {
private:
    // YA NO guardamos pines (sda/scl), guardamos el PUERTO I2C
    i2c_port_t _i2c_port;

    // Función interna
    esp_err_t read_register(uint8_t reg_addr, uint8_t *data, size_t len);

public:
    // El constructor solo recibe el puerto (ej: I2C_NUM_0)
    Bme280Driver(i2c_port_t port);
    
    // init ya no configura pines, pero lo dejamos por si queremos resetear el sensor luego
    esp_err_t init();
    
    uint8_t read_chip_id();
};