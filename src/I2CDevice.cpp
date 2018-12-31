#include "I2CDevice.h"

void I2CDevice::writeByte(uint8_t reg, uint8_t value)
{
    _wire->beginTransmission(_address);
    _wire->write(reg);
    _wire->write(value);
    _wire->endTransmission();
}

uint8_t I2CDevice::readByte(uint8_t reg)
{
    _wire->beginTransmission(_address);
    _wire->write(reg);
    _wire->endTransmission();
    _wire->requestFrom(_address, (uint8_t)1);
    return _wire->read();
}

uint16_t I2CDevice::readUInt16(uint8_t reg)
{
    _wire->beginTransmission(_address);
    _wire->write(reg);
    _wire->endTransmission();
    _wire->requestFrom(_address, (uint8_t)2);
    return (_wire->read() << 8) | _wire->read();
}

uint32_t I2CDevice::readUInt24(uint8_t reg)
{
    uint32_t value;
    _wire->beginTransmission(_address);
    _wire->write(reg);
    _wire->endTransmission();
    _wire->requestFrom(_address, (uint8_t)3);

    value = _wire->read();
    value <<= 8;
    value |= _wire->read();
    value <<= 8;
    value |= _wire->read();

    return value;
}

int16_t I2CDevice::readInt16(uint8_t reg)
{
    return (int16_t)readUInt16(reg);
}

uint16_t I2CDevice::readUInt16_LE(uint8_t reg)
{
    _wire->beginTransmission(_address);
    _wire->write(reg);
    _wire->endTransmission();
    _wire->requestFrom(_address, (uint8_t)2);
    return _wire->read() | (_wire->read() << 8);
}

int16_t I2CDevice::readInt16_LE(uint8_t reg)
{
    return (int16_t)readUInt16_LE(reg);
}

void I2CDevice::read(uint8_t reg, uint8_t *buf, uint8_t num)
{
    Wire.beginTransmission(_address);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(_address, num);

    for (int i = 0; i < num; i++)
    {
        buf[i] = Wire.read();
    }
}

void I2CDevice::write(uint8_t reg, uint8_t *buf, uint8_t num)
{
    Wire.beginTransmission((uint8_t)_address);
    Wire.write((uint8_t)reg);
    Wire.write((uint8_t *)buf, num);
    Wire.endTransmission();
}