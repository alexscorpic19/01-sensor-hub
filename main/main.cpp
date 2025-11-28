#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "cJSON.h"
#include "mqtt_client.h"
#include "esp_crt_bundle.h"
#include "esp_random.h"

// --- TUS COMPONENTES ---
#include "wifi_connect.h"
#include "mpu6050_driver.h"
#include "bme280_driver.h" // <--- ¡VUELVE EL BME280!

static const char *TAG = "SENSOR_HUB";

// --- CONFIGURACIÓN I2C ---
#define I2C_MASTER_SCL_IO 2
#define I2C_MASTER_SDA_IO 1
#define I2C_MASTER_NUM    I2C_NUM_0

// --- CREDENCIALES HIVEMQ ---
#define MQTT_BROKER_URL "mqtts://8ba1b11b052d48ce8b5129ddd69562eb.s1.eu.hivemq.cloud"
#define MQTT_USERNAME   "sensor_hub_admin"
#define MQTT_PASSWORD   "Callefalsa03061005*"

esp_mqtt_client_handle_t client = NULL;

// 1. INICIALIZAR EL BUS I2C (Compartido)
void i2c_master_init() {
    i2c_config_t conf = {};
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 100000;
    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
    ESP_LOGI(TAG, "Bus I2C iniciado en SDA:%d SCL:%d", I2C_MASTER_SDA_IO, I2C_MASTER_SCL_IO);
}

// 2. MANEJADOR MQTT
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    if (event_id == MQTT_EVENT_CONNECTED) {
        ESP_LOGI(TAG, "✅ MQTT CONECTADO");
    } else if (event_id == MQTT_EVENT_DISCONNECTED) {
        ESP_LOGW(TAG, "⚠️ MQTT Desconectado");
    }
}

// 3. INICIAR MQTT
static void mqtt_app_start(void) {
    esp_mqtt_client_config_t mqtt_cfg = {};
    mqtt_cfg.broker.address.uri = MQTT_BROKER_URL;
    mqtt_cfg.broker.verification.crt_bundle_attach = esp_crt_bundle_attach; 
    mqtt_cfg.credentials.username = MQTT_USERNAME;
    mqtt_cfg.credentials.authentication.password = MQTT_PASSWORD;

    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, (esp_mqtt_event_id_t)ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}

// 4. TAREA DE TELEMETRÍA (Leyendo DOS sensores)
void telemetry_task(void *pvParameters) {
    // Instanciar drivers pasando el puerto I2C compartido
    Mpu6050Driver mpu(I2C_MASTER_NUM);
    Bme280Driver bme(I2C_MASTER_NUM);

    // Verificación inicial
    bool mpu_ok = mpu.testConnection();
    uint8_t bme_id = bme.read_chip_id();
    bool bme_ok = (bme_id == 0x60) || (bme_id == 0x58); // 0x60 es BME, 0x58 es BMP

    ESP_LOGI(TAG, "Estado Sensores -> MPU6050: %s | BME280: %s (ID: 0x%X)", 
             mpu_ok ? "OK" : "FAIL", 
             bme_ok ? "OK" : "FAIL", bme_id);

    // Variables para datos
    float acc_x = 0, acc_y = 0, acc_z = 0;
    float temp = 0, hum = 0;

    while(1) {
        // LEER SENSORES (Aquí irían las lecturas reales de registros)
        // Por ahora simulamos variación sobre una base lógica
        
        // MPU6050 (Simulado por ahora hasta implementar lectura completa de registros)
        acc_x = (esp_random() % 100) / 100.0; // Random 0.0 to 1.0
        
        // BME280 (Usamos el método readTemperature que creamos el Día 4)
        // Si no lo implementamos full I2C aun, usará la lógica mock del driver.
        // OJO: Si tu driver BME280_driver.cpp tiene la lógica mock, funcionará.
        // Si tiene lógica I2C real y el sensor falla en Wokwi, devolverá 0 o error.
        // Para este ejemplo, asumiremos que devuelve un valor flotante.
        // temp = bme.readTemperature(); // Descomenta si tu driver tiene este método público
        temp = 24.5 + ((esp_random() % 10) / 10.0); // Simulación temporal si falla driver
        hum = 60.0 + ((esp_random() % 5) / 10.0);

        if (client != NULL) {
            // --- CREAR JSON ROBUSTO ---
            cJSON *root = cJSON_CreateObject();
            cJSON_AddStringToObject(root, "device_id", "SENSOR_HUB_001");
            
            cJSON *sensors = cJSON_CreateObject();
            
            // Datos Ambientales
            cJSON *env = cJSON_CreateObject();
            cJSON_AddNumberToObject(env, "temperature", temp);
            cJSON_AddNumberToObject(env, "humidity", hum);
            cJSON_AddStringToObject(env, "status", bme_ok ? "online" : "offline");
            cJSON_AddItemToObject(sensors, "environmental", env);

            // Datos Inerciales
            cJSON *imu = cJSON_CreateObject();
            cJSON_AddNumberToObject(imu, "acc_x", acc_x);
            cJSON_AddStringToObject(imu, "status", mpu_ok ? "online" : "offline");
            cJSON_AddItemToObject(sensors, "inertial", imu);

            cJSON_AddItemToObject(root, "data", sensors);

            // Enviar
            char *payload = cJSON_Print(root);
            esp_mqtt_client_publish(client, "sensor/telemetry", payload, 0, 1, 0);
            ESP_LOGI(TAG, "JSON Enviado: %s", payload);

            // Limpiar memoria
            cJSON_Delete(root);
            free(payload);
        }

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

extern "C" void app_main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    
    i2c_master_init();
    wifi_init();
    
    // Esperar un poco a que el WiFi estabilice
    vTaskDelay(pdMS_TO_TICKS(3000));
    
    mqtt_app_start();

    xTaskCreate(telemetry_task, "telemetry_task", 4096, NULL, 5, NULL);
}