#ifndef I2CHELPER_h
#define I2CHELPER_h

#include "Wire.h"

class I2CDevice
{
  private:
    uint8_t _address = 0xFF;

    TwoWire *_wire;

  public:
    I2CDevice(uint8_t address, TwoWire *wire)
    {
        _address = address;
        _wire = wire;
    };

    void begin() { _wire->begin(); }

    void writeByte(uint8_t reg, uint8_t value);
    uint8_t readByte(uint8_t reg);
    uint16_t readUInt16(uint8_t reg);
    uint32_t readUInt24(uint8_t reg);
    int16_t readInt16(uint8_t reg);
    uint16_t readUInt16_LE(uint8_t reg); // little endian
    int16_t readInt16_LE(uint8_t reg);   // little endian

    void read(uint8_t reg, uint8_t *buf, uint8_t num);
    void write(uint8_t reg, uint8_t *buf, uint8_t num);
};

#endif