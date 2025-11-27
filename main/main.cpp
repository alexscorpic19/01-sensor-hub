#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "mqtt_client.h" // Cliente MQTT nativo
#include "wifi_connect.h" // Nuestro componente nuevo
#include "esp_crt_bundle.h"

static const char *TAG = "MAIN";

// --- 🔐 TUS CREDENCIALES HIVEMQ (DÍA 6) ---
#define MQTT_BROKER_URL "mqtts://8ba1b11b052d48ce8b5129ddd69562eb.s1.eu.hivemq.cloud" // Pon tu URL aquí (con mqtts://)
#define MQTT_USERNAME   "sensor_hub_admin"                            // Tu usuario
#define MQTT_PASSWORD   "Callefalsa03061005*"                                  // Tu contraseña

// Certificado Raíz de Let's Encrypt (Necesario para HiveMQ)
// Certificado Raíz ISRG Root X1 (Usado por HiveMQ y Let's Encrypt)

// (He recortado el certificado por espacio, pero para Wokwi a veces no es estricto.
// Si falla, te pasaré el completo).

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t)event_data;
    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "✅ MQTT CONECTADO A HIVEMQ!");
            // Publicar un mensaje de prueba
            esp_mqtt_client_publish(event->client, "sensor/test", "Hola Jhon desde Wokwi!", 0, 1, 0);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGE(TAG, "❌ MQTT Desconectado");
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGE(TAG, "MQTT Error");
            break;
        default:
            break;
    }
}

static void mqtt_app_start(void) {
    esp_mqtt_client_config_t mqtt_cfg = {};
    mqtt_cfg.broker.address.uri = MQTT_BROKER_URL;
    
    // --- CAMBIO MÁGICO ---
    // Usamos el "Llavero" interno del ESP32 en lugar de un texto pegado
    mqtt_cfg.broker.verification.crt_bundle_attach = esp_crt_bundle_attach; 
    
    mqtt_cfg.credentials.username = MQTT_USERNAME;
    mqtt_cfg.credentials.authentication.password = MQTT_PASSWORD;

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, (esp_mqtt_event_id_t)ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}

extern "C" void app_main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    
    ESP_LOGI(TAG, "--- INICIANDO DÍA 7: CONEXIÓN CLOUD ---");

    // 1. Iniciar WiFi
    wifi_init();

    // 2. Esperar un poco a que conecte (manera simple)
    vTaskDelay(pdMS_TO_TICKS(5000));

    // 3. Iniciar MQTT
    mqtt_app_start();

    while(1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}