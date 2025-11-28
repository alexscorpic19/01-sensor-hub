# 🏭 Industrial IoT Sensor Hub (01-sensor-hub)

**High-Performance Data Acquisition System with ESP32-S3 & FreeRTOS.**

This project demonstrates a complete **End-to-End IoT solution**, bridging the gap between embedded hardware (C++) and modern web visualization (React).

## 🖥️ Real-Time Dashboard Interface

![Dashboard Preview](docs/img/dashboard-preview.png)

*   **Communication:** Connects directly to HiveMQ Cloud via **Secure WebSockets (WSS)**.
*   **Latency:** Real-time updates (<100ms) from the ESP32-S3.
*   **Data Visualization:** Displays synchronized environmental (BME280) and inertial (MPU6050) sensor data.
*   **Protocol:** Decodes JSON payloads directly in the browser.

---

## 📦 Hardware Components

| Component | Description |
|-----------|-------------|
| **ESP32-S3** | Dual-core microcontroller (Xtensa LX7) with Wi-Fi + BLE 5.0 |
| **BME280** | Precision environmental sensor (Temp, Humidity, Pressure) via I2C |
| **MPU6050** | 6-axis Motion Tracking device (Accelerometer + Gyroscope) via I2C |
| **HiveMQ Cloud** | Enterprise MQTT Broker for secure telemetry routing (TLS) |

---

## 📂 Project Structure (ESP-IDF Standard)

The project follows the official **Espressif IoT Development Framework** modular architecture:

```text
01-sensor-hub/
├── components/        # Modular Drivers (C++ Classes)
│   ├── bme280/        # I2C Driver for Environmental Sensor
│   ├── mpu6050/       # I2C Driver for IMU
│   └── wifi_connect/  # WiFi Station Manager
├── main/              # Application Entry Point (FreeRTOS Tasks)
├── sensor-dashboard/  # React Frontend (Vite + MQTT.js)
├── tools/             # Python Scripts for data ingestion testing
└── docs/              # Documentation & assets

🔧 Setup Instructions

1. Firmware Deployment (ESP32)

1.Prerequisite: VS Code with Espressif IDF Extension (v5.x).
2.Open the project root in VS Code.
3.Configure Wi-Fi credentials in components/wifi_connect/include/wifi_connect.h.
4.Configure MQTT credentials in main/main.cpp.
5.Click the Build icon (Cylinder) to compile.
6.Click Flash (Lightning) to upload to the board.

2. Dashboard Setup (React)

1.Navigate to the frontend folder:
cd sensor-dashboard

2.Install dependencies:
npm install

3.Start development server:
npm run dev

4.Access dashboard at http://localhost:5173.

3. HiveMQ Configuration

1.Create a free HiveMQ Cloud cluster.
2.Create credentials (User/Password) in "Access Management".
3.Update App.jsx (Frontend) and main.cpp (Firmware) with your cluster URL.

📊 Data Flow Architecture
1.Sensor Acquisition: BME280 & MPU6050 collect raw data via I2C Bus (Shared).
2.Edge Processing: ESP32-S3 aggregates readings and serializes to JSON using cJSON.
3.Cloud Routing: Secure MQTT (TLS 1.2) publish to HiveMQ Cloud Broker.
4.Real-Time Display: React Frontend subscribes to MQTT topic via WebSockets (WSS) and renders data instantly.

📜 License
MIT License - Open Source


