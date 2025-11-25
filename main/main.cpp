#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

// INCLUIMOS NUESTRO NUEVO DRIVER
#include "bme280_driver.h"

static const char *TAG = "MAIN_APP";

extern "C" void app_main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    ESP_LOGI(TAG, "--- INICIANDO SISTEMA MODULAR ---");

    // 1. Instanciamos el objeto (pines I2C virtuales: SDA=1, SCL=2)
    Bme280Driver sensor(1, 2);

    // 2. Inicializamos el sensor
    sensor.init();

    int i = 0;
    while(1) {
        // 3. Usamos el método del objeto para obtener el dato
        float temp = sensor.readTemperature();

        ESP_LOGI(TAG, "Ciclo: %d | Temperatura Sala de Servidores: %.2f °C", i++, temp);
        
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}