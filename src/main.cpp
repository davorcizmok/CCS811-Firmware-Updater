#include <Arduino.h>
#include "SdFat.h"
#include "Wire.h"
#include "I2CDevice.h"

#define PIN_WAK 31
#define PIN_RST 32
#define I2C_ADR 0x5A // LOW

const uint8_t CCS811_STATUS = 0x00;
const uint8_t CCS811_MEAS_MODE = 0x01;
const uint8_t CCS811_ALG_RESULT_DATA = 0x02;
const uint8_t CCS811_RAW_DATA = 0x03;
const uint8_t CCS811_ENV_DATA = 0x05;
const uint8_t CCS811_NTC = 0x06;
const uint8_t CCS811_THRESHOLDS = 0x10;
const uint8_t CCS811_BASELINE = 0x11;
const uint8_t CCS811_HW_ID = 0x20;
const uint8_t CCS811_HW_VERSION = 0x21;
const uint8_t CCS811_FW_BOOT_VERSION = 0x23;
const uint8_t CCS811_FW_APP_VERSION = 0x24;
const uint8_t CCS811_ERROR_ID = 0xE0;
const uint8_t CCS811_SW_RESET = 0xFF;

const uint8_t CCS811_BOOTLOADER_APP_ERASE = 0xF1;
const uint8_t CCS811_BOOTLOADER_APP_DATA = 0xF2;
const uint8_t CCS811_BOOTLOADER_APP_VERIFY = 0xF3;
const uint8_t CCS811_BOOTLOADER_APP_START = 0xF4;

SdFatSdio sd;
File file;

void errorHalt()
{
  Serial.println("ERROR HALT!");
  while (1)
  {
  }
}

void setup()
{
  pinMode(PIN_WAK, OUTPUT);
  pinMode(PIN_RST, OUTPUT);

  Serial.begin(115200);
  while (!Serial)
  {
  }

  Serial.print("Checking for Firmware File...");
  if (!sd.begin())
  {
    sd.initErrorHalt("SdFatSdio begin() failed");
  }
  sd.chvol();
  if (!file.open("firmware.bin", O_RDONLY))
  {
    Serial.println("Could not open firmware.bin on the SD card.");
    errorHalt();
  }
  if (file.size() % 8 != 0)
  {
    Serial.print("File Size should be a multiple of 8 bytes, but is ");
    Serial.print(file.size());
    Serial.println(".");
    errorHalt();
  }
  Serial.print(" Size: ");
  Serial.print(file.size());
  Serial.println(" -> OK.");

  Serial.print("Initializing I2C at 0x");
  Serial.print(I2C_ADR, HEX);
  Serial.println("...");

  digitalWrite(PIN_RST, HIGH);
  digitalWrite(PIN_WAK, LOW);
  delay(100);

  I2CDevice ccs = I2CDevice(I2C_ADR, &Wire);
  ccs.begin();

  uint8_t hwId = ccs.readByte(CCS811_HW_ID);
  if (hwId != 0x81)
  {
    Serial.print("Wrong HW Id: ");
    Serial.print(hwId);
    Serial.println("!");
    errorHalt();
  }

  Serial.println("Found CSS811 Device.");
  Serial.println("Starting Bootloader Mode...");

  Serial.print("Cycling RST and WAK...");
  digitalWrite(PIN_WAK, HIGH);
  delay(100);
  digitalWrite(PIN_RST, LOW);
  delay(100);
  digitalWrite(PIN_RST, HIGH);
  delay(100);
  digitalWrite(PIN_WAK, LOW);
  delay(100);
  Serial.println("Done.");

  uint8_t status = ccs.readByte(CCS811_STATUS);
  if ((status & 0x80) != 0)
  {
    Serial.println();
    Serial.print("Device is not in Bootloader Mode (");
    Serial.print(status);
    Serial.println(")!");
    errorHalt();
  }
  Serial.println("Bootloader started successfully.");

  Serial.print("Erasing APP...");
  uint8_t eraseSeq[] = {0xE7, 0xA7, 0xE6, 0x09};
  ccs.write(CCS811_BOOTLOADER_APP_ERASE, eraseSeq, 4);
  delay(500);

  status = ccs.readByte(CCS811_STATUS);
  if ((status & 0x40) != 0x40)
  {
    Serial.println();
    Serial.print("APP erase failed (");
    Serial.print(status);
    Serial.println(")!");
    errorHalt();
  }
  Serial.println(" Done.");

  Serial.println("Copying firmware...");
  uint8_t buffer[8];
  uint16_t blocks = file.size() / 8;
  for (uint16_t i = 0; i < blocks; i++)
  {
    file.read(buffer, 8);
    ccs.write(CCS811_BOOTLOADER_APP_DATA, buffer, 8);
    delay(50);
    if (i % 40 == 0)
    {
      Serial.println();
      Serial.printf("%03u / %03u :", i, blocks);
    }
    else
    {
      Serial.print(".");
    }
  }
  Serial.println();
  Serial.println("Done wrting Firmware.");

  Serial.print("Verifying APP...");
  ccs.write(CCS811_BOOTLOADER_APP_VERIFY, NULL, 0);
  delay(500);
  status = ccs.readByte(CCS811_STATUS);
  if ((status & 0x30) != 0x30)
  {
    Serial.println();
    Serial.print("APP verify failed (");
    Serial.print(status);
    Serial.println(")!");
    errorHalt();
  }
  Serial.println(" Done.");

  Serial.println("Starting App...");
  ccs.write(CCS811_BOOTLOADER_APP_START, NULL, 0);
  status = ccs.readByte(CCS811_STATUS);
  if ((status & 0x90) != 0x90)
  {
    Serial.println();
    Serial.print("APP start failed (");
    Serial.print(status);
    Serial.println(")!");
    errorHalt();
  }

  uint8_t version[2];
  ccs.read(CCS811_FW_APP_VERSION, version, 2);
  Serial.print("App Version is now: ");
  Serial.print(version[0] >> 4);
  Serial.print(".");
  Serial.print(version[0] & 0x0F);
  Serial.print(".");
  Serial.print(version[1]);
  Serial.println(".");

  Serial.println();
  Serial.println("All Done.");
}

void loop()
{
  // put your main code here, to run repeatedly:
}