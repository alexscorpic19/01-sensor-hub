#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "esp_log.h"

// Incluimos AMBOS drivers
#include "bme280_driver.h"
#include "mpu6050_driver.h"

static const char *TAG = "MAIN";

// Configuración global del I2C
#define I2C_MASTER_SCL_IO           2     // Pin SCL
#define I2C_MASTER_SDA_IO           1     // Pin SDA
#define I2C_MASTER_NUM              I2C_NUM_0
#define I2C_MASTER_FREQ_HZ          100000

void i2c_master_init() {
    i2c_config_t conf = {};
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
    ESP_LOGI(TAG, "I2C Bus inicializado en SDA:%d SCL:%d", I2C_MASTER_SDA_IO, I2C_MASTER_SCL_IO);
}

extern "C" void app_main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    
    // 1. Iniciar el Bus Compartido
    i2c_master_init();

    // 2. Instanciar sensores pasando el puerto
    Bme280Driver bme(I2C_MASTER_NUM);
    Mpu6050Driver mpu(I2C_MASTER_NUM);

    // 3. Verificar MPU6050 (El que sí se ve en Wokwi)
    if (mpu.testConnection()) {
        ESP_LOGI(TAG, "✅ MPU6050 Online!");
    } else {
        ESP_LOGE(TAG, "❌ MPU6050 No responde");
    }

    // 4. Verificar BME280 (El fantasma visual, pero probamos si responde)
    // (Asegúrate de que bme.init() ya no intente configurar pines, solo lógica interna si la hay)
    
    while(1) {
        // Aquí leeremos ambos en el futuro
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}