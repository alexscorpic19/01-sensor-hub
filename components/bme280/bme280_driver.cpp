#include "bme280_driver.h"
#include "esp_log.h"

static const char *TAG = "BME280_DRIVER";

// Constructor: Solo guardamos el puerto
Bme280Driver::Bme280Driver(i2c_port_t port) {
    _i2c_port = port;
}

// Init: Ya no configura hardware (lo hace el main), solo lógica del sensor
esp_err_t Bme280Driver::init() {
    ESP_LOGI(TAG, "Driver BME280 instanciado en puerto %d", _i2c_port);
    return ESP_OK;
}

esp_err_t Bme280Driver::read_register(uint8_t reg_addr, uint8_t *data, size_t len) {
    // Aquí usamos _i2c_port en lugar de la constante I2C_NUM_0
    return i2c_master_write_read_device(_i2c_port, 
                                        BME280_SENSOR_ADDR, 
                                        &reg_addr, 1, 
                                        data, len, 
                                        1000 / portTICK_PERIOD_MS);
}

uint8_t Bme280Driver::read_chip_id() {
    uint8_t id = 0;
    esp_err_t err = read_register(BME280_CHIP_ID_REG, &id, 1);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Chip ID: 0x%X", id);
    }
    return id;
}