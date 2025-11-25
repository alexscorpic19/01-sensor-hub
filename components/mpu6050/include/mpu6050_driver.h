#pragma once
#include "driver/i2c.h"

// Dirección del MPU6050 (Si pin AD0 es Low = 0x68)
#define MPU6050_ADDR 0x68
#define MPU6050_WHO_AM_I_REG 0x75

class Mpu6050Driver {
private:
    i2c_port_t _i2c_port; // Guardamos qué puerto usa

public:
    // Constructor recibe el puerto I2C ya iniciado
    Mpu6050Driver(i2c_port_t port);
    
    // Verifica conexión
    bool testConnection();
};