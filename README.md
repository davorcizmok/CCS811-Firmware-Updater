# CCS811 Firmware Updater

This small programm updates the Firmware of the AMS CCS811 Gas Sensor.
Tested with a Teensy 3.5 but should work with any Arduino compatible (and an SDCard).
Place the Firmware an the SDCard, Wire RESET and WAKE Pins of the Sensor to two digitals Pins of your device an configure them in the Script (`PIN_RST` and `PIN_WAK`).

See also:
* [AMS ApplicationeNote: CCS811 Downloading new Application Firmware](https://ams.com/documents/20143/36005/CCS811_AN000371_2-00.pdf/64d84c3a-ec49-8e6f-bdb6-0de19e2058e8)
* [AMS CCS811 Datasheet](https://ams.com/documents/20143/36005/CCS811_DS000459_6-00.pdf/c7091525-c7e5-37ac-eedb-b6c6828b0dcf)
* [AMS Tools / Firmware download](https://ams.com/ccs811#tab/tools)
