#include <Wire.h>

#define AHT10_ADDR 0x38
#define AHT10_INIT_CMD 0xBE
#define AHT10_MEASURE_CMD 0xAC

void setup() {
  Serial.begin(115200);
  Wire.begin();

  Wire.beginTransmission(0x38);
  Wire.write(0xBA);
  Wire.endTransmission();
  delay(20);

  Serial.println("success!");

  // --- Step 1: Initialize the sensor ---
  Wire.beginTransmission(AHT10_ADDR);
  Wire.write(AHT10_INIT_CMD);
  Wire.write(0x08);  // calibration enable
  Wire.write(0x00);
  if (Wire.endTransmission() != 0) {
    Serial.println("❌ No ACK from AHT10. Check wiring or address!");
    while (true);
  }
  delay(500); // give it time to calibrate

  Serial.println("✅ AHT10 initialized. Starting measurement...");

  // --- Step 2: Trigger measurement ---
  Wire.beginTransmission(AHT10_ADDR);
  Wire.write(AHT10_MEASURE_CMD);
  Wire.write(0x33);
  Wire.write(0x00);
  Wire.endTransmission();
  delay(100); // wait for conversion

  // --- Step 3: Read data (6 bytes) ---
  Wire.requestFrom(AHT10_ADDR, 6);
  if (Wire.available() == 6) {
    uint8_t data[6];
    for (int i = 0; i < 6; i++) data[i] = Wire.read();

    // --- Step 4: Convert raw data ---
    uint32_t rawHumidity = ((uint32_t)(data[1]) << 12) | ((uint32_t)(data[2]) << 4) | ((data[3] >> 4) & 0x0F);
    uint32_t rawTemp = (((uint32_t)(data[3] & 0x0F)) << 16) | ((uint32_t)(data[4]) << 8) | (data[5]);

    float humidity = (rawHumidity * 100.0) / 1048576.0;
    float temperature = ((rawTemp * 200.0) / 1048576.0) - 50.0;

    Serial.print("🌡 Temperature: ");
    Serial.print(temperature, 1);
    Serial.print(" °C  |  💧 Humidity: ");
    Serial.print(humidity, 1);
    Serial.println(" %");
  } else {
    Serial.println("⚠️ No data received from AHT10!");
  }

  delay(2000);
}

void loop() {
  
}
