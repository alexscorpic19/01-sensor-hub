#include "mpu6050_driver.h"
#include "esp_log.h"

static const char *TAG = "MPU6050";

Mpu6050Driver::Mpu6050Driver(i2c_port_t port) {
    _i2c_port = port;
}

bool Mpu6050Driver::testConnection() {
    uint8_t reg = MPU6050_WHO_AM_I_REG;
    uint8_t data = 0;

    // Escribir dirección de registro
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (MPU6050_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg, true);
    i2c_master_start(cmd); // Restart
    i2c_master_write_byte(cmd, (MPU6050_ADDR << 1) | I2C_MASTER_READ, true);
    i2c_master_read_byte(cmd, &data, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    
    esp_err_t ret = i2c_master_cmd_begin(_i2c_port, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);

    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "WHO_AM_I: 0x%02x", data);
        return (data == 0x68); // 0x68 es el valor correcto por defecto
    } else {
        ESP_LOGE(TAG, "Error leyendo MPU6050");
        return false;
    }
}