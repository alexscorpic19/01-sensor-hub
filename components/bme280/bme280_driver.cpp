#include "bme280_driver.h"
#include "esp_log.h"

static const char *TAG = "BME280_DRIVER";

// Implementación del Constructor
Bme280Driver::Bme280Driver(int sda, int scl) {
    _sda_pin = sda;
    _scl_pin = scl;
    _simulated_temp = 20.0f; // Temperatura inicial
}

// Implementación de init
esp_err_t Bme280Driver::init() {
    ESP_LOGI(TAG, "Inicializando BME280 en pines SDA:%d SCL:%d", _sda_pin, _scl_pin);
    // Mañana aquí pondremos la configuración real de I2C
    return ESP_OK;
}

// Implementación de lectura
float Bme280Driver::readTemperature() {
    // Simulamos que la temperatura sube un poco en cada lectura
    _simulated_temp += 0.5f;
    
    // Reiniciamos si sube mucho
    if (_simulated_temp > 40.0f) _simulated_temp = 20.0f;

    ESP_LOGD(TAG, "Leyendo sensor... Valor: %.2f", _simulated_temp);
    return _simulated_temp;
}