import { useState, useEffect } from 'react'
import mqtt from 'mqtt'
import './App.css'

// --- 🔐 TUS CREDENCIALES HIVEMQ ---
const MQTT_HOST = "8ba1b11b052d48ce8b5129ddd69562eb.s1.eu.hivemq.cloud"; // Tu Host SIN mqtts://
const MQTT_PORT = 8884; // Puerto WebSocket Seguro (WSS)
const MQTT_USER = "sensor_hub_admin";
const MQTT_PASS = "Callefalsa03061005*";
const MQTT_TOPIC = "sensor/telemetry";

function App() {
  const [status, setStatus] = useState("Desconectado 🔴");
  const [data, setData] = useState(null);
  const [lastUpdate, setLastUpdate] = useState("-");

  useEffect(() => {
    console.log("Intentando conectar a MQTT...");

    // Configuración de conexión segura WSS
    const client = mqtt.connect(`wss://${MQTT_HOST}:${MQTT_PORT}/mqtt`, {
      username: MQTT_USER,
      password: MQTT_PASS,
      clientId: `web_client_${Math.random().toString(16).substring(2, 8)}`,
      keepalive: 60,
      protocolId: 'MQTT',
      protocolVersion: 4,
      clean: true,
      reconnectPeriod: 1000,
      connectTimeout: 30 * 1000,
    });

    client.on('connect', () => {
      console.log('✅ Conectado a HiveMQ!');
      setStatus("En Línea 🟢");
      client.subscribe(MQTT_TOPIC, (err) => {
        if (!err) console.log(`Suscrito a ${MQTT_TOPIC}`);
      });
    });

    client.on('message', (topic, message) => {
      try {
        const json = JSON.parse(message.toString());
        console.log("Dato recibido:", json);
        setData(json);
        setLastUpdate(new Date().toLocaleTimeString());
      } catch (e) {
        console.error("Error parseando JSON", e);
      }
    });

    client.on('error', (err) => {
      console.error('Connection error: ', err);
      setStatus("Error de Conexión ⚠️");
    });

    return () => {
      if (client) client.end(); // Limpieza al cerrar
    };
  }, []);

  // Valores por defecto si no hay datos
  const temp = data?.data?.environmental?.temperature?.toFixed(2) || "--";
  const hum = data?.data?.environmental?.humidity?.toFixed(2) || "--";
  const accX = data?.data?.inertial?.acc_x?.toFixed(2) || "0";
  const deviceId = data?.device_id || "Esperando datos...";

  return (
    <div className="dashboard-container">
      <h1>🎛️ IoT Sensor Hub</h1>
      <p className="device-id">ID: {deviceId}</p>
      <div className={`status-bar ${status.includes("En Línea") ? "online" : "offline"}`}>
        Estado: {status} <br />
        <small>Última actualización: {lastUpdate}</small>
      </div>

      <div className="cards-grid">
        {/* Tarjeta Temperatura */}
        <div className="card">
          <h2>🌡️ Temperatura</h2>
          <div className="value">{temp} °C</div>
        </div>

        {/* Tarjeta Humedad */}
        <div className="card">
          <h2>💧 Humedad</h2>
          <div className="value">{hum} %</div>
        </div>

        {/* Tarjeta Acelerómetro */}
        <div className="card">
          <h2>⚖️ Aceleración X</h2>
          <div className="value">{accX} g</div>
          {/* Barra visual simple */}
          <div className="progress-bg">
            <div
              className="progress-bar"
              style={{ width: `${Math.min(Math.abs(parseFloat(accX)) * 100, 100)}%` }}
            ></div>
          </div>
        </div>
      </div>
    </div>
  )
}

export default App