#include <Wire.h>

#define BH1750_ADDR 0x23
#define POWER_ON 0x01
#define RESET 0x07
#define CONT_HIGH_RES_MODE 0x10

void setup() {
  Serial.begin(115200);
  Wire.begin();  // SDA = 21, SCL = 22 by default on ESP32

  // Power on the sensor
  Wire.beginTransmission(BH1750_ADDR);
  Wire.write(POWER_ON);
  if (Wire.endTransmission() != 0) {
    Serial.println("❌ No ACK from BH1750. Check wiring or address!");
    while (true);
  }

  // Reset (optional but good practice)
  Wire.beginTransmission(BH1750_ADDR);
  Wire.write(RESET);
  Wire.endTransmission();

  // Set continuous high-res mode (1 lx resolution)
  Wire.beginTransmission(BH1750_ADDR);
  Wire.write(CONT_HIGH_RES_MODE);
  Wire.endTransmission();

  delay(200); // wait for first measurement
}

void loop() {
  Wire.requestFrom(BH1750_ADDR, 2); // read 2 bytes of data

  if (Wire.available() == 2) {
    uint16_t lux_raw = (Wire.read() << 8) | Wire.read();
    float lux = lux_raw / 1.2; // per datasheet conversion
    Serial.print("💡 Light intensity: ");
    Serial.print(lux);
    Serial.println(" lx");
  } else {
    Serial.println("⚠️ No data received — sensor may not be responding");
  }

  delay(1000);
}

