import json
import time
import paho.mqtt.client as mqtt
import ssl

# --- 🔐 TUS CREDENCIALES (IGUAL QUE EN EL ESP32) ---
BROKER_URL = "8ba1b11b052d48ce8b5129ddd69562eb.s1.eu.hivemq.cloud" # SIN mqtts://, solo el dominio
BROKER_PORT = 8883
USERNAME = "sensor_hub_admin"
PASSWORD = "Callefalsa03061005*"
TOPIC = "sensor/telemetry"

# Colores para la terminal (Para que se vea Pro)
class Colors:
    HEADER = '\033[95m'
    BLUE = '\033[94m'
    GREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print(f"{Colors.GREEN}[SISTEMA] Conectado exitosamente a HiveMQ Cloud!{Colors.ENDC}")
        client.subscribe(TOPIC)
        print(f"{Colors.BLUE}[SISTEMA] Escuchando tópico: {TOPIC}{Colors.ENDC}")
    else:
        print(f"{Colors.FAIL}[ERROR] Falló la conexión. Código: {rc}{Colors.ENDC}")

def on_message(client, userdata, msg):
    try:
        # 1. Decodificar el mensaje (Bytes -> String -> JSON Object)
        payload_str = msg.payload.decode('utf-8')
        data = json.loads(payload_str)
        
        # 2. Extraer datos específicos
        device = data.get("device_id", "Unknown")
        env = data.get("data", {}).get("environmental", {})
        imu = data.get("data", {}).get("inertial", {})
        
        temp = env.get("temperature", 0)
        acc_x = imu.get("acc_x", 0)
        status_bme = env.get("status", "unknown")

        # 3. Visualización en consola
        print("-" * 50)
        print(f"📡 DISPOSITIVO: {Colors.HEADER}{device}{Colors.ENDC}")
        
        # Alerta visual si la temperatura es alta (simulada)
        temp_color = Colors.GREEN
        if temp > 30.0: temp_color = Colors.FAIL # Rojo si > 30°C
        
        print(f"🌡️  Temperatura: {temp_color}{temp:.2f} °C{Colors.ENDC} (Sensor: {status_bme})")
        print(f"⚖️  Aceleración X: {Colors.BLUE}{acc_x:.2f}{Colors.ENDC}")
        
        # Alerta de lógica de negocio
        if status_bme == "offline":
            print(f"{Colors.WARNING}⚠️  ALERTA: Sensor Ambiental desconectado{Colors.ENDC}")

    except Exception as e:
        print(f"{Colors.FAIL}[ERROR] No se pudo procesar el JSON: {e}{Colors.ENDC}")

# --- CONFIGURACIÓN CLIENTE ---
client = mqtt.Client()
client.username_pw_set(USERNAME, PASSWORD)

# Configuración TLS (Obligatorio para HiveMQ)
# Esto le dice a Python que use el certificado seguro del sistema Windows
client.tls_set(cert_reqs=ssl.CERT_REQUIRED, tls_version=ssl.PROTOCOL_TLS)

# Callbacks
client.on_connect = on_connect
client.on_message = on_message

print(f"{Colors.HEADER}--- MONITOR IOT INICIADO ---{Colors.ENDC}")
print("Conectando a la nube...")

# Conexión y Bucle
try:
    client.connect(BROKER_URL, BROKER_PORT, 60)
    client.loop_forever() # Mantiene el script corriendo escuchando
except KeyboardInterrupt:
    print("\n[SISTEMA] Monitor detenido.")
except Exception as e:
    print(f"\n{Colors.FAIL}[FATAL] Error de conexión: {e}{Colors.ENDC}")