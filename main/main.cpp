#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

// Etiqueta para logs
static const char *TAG = "SENSOR_HUB";

extern "C" void app_main(void)
{
    // CONFIGURACIÓN PRO: Desactivar buffer para ver prints inmediatos en Windows
    setvbuf(stdout, NULL, _IONBF, 0);

    ESP_LOGI(TAG, "--- INICIO SISTEMA (WINDOWS) ---");
    ESP_LOGI(TAG, "Hardware: ESP32-S3");

    int i = 0;
    while(1) {
        // Log con formato de Espressif
        ESP_LOGI(TAG, "El sistema funciona correctamente. Ciclo: %d", i++);
        
        // Pausa de 1 segundo
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}