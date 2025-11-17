#include <Wire.h>

void setup() {
  Wire.begin();  // Para ESP32: Wire.begin(SDA, SCL);
  Serial.begin(115200);
  while (!Serial);  // Espera porta serial iniciar (opcional)

  Serial.println("Scanning for I2C devices...");

  byte count = 0;

  // Testa os endereços de 1 a 127
  for (byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Dispositivo I2C encontrado no endereço 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println();
      count++;
    } 
    
    else if (error == 4) {
      Serial.print("Erro desconhecido no endereço 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }

  if (count == 0)
    Serial.println("Nenhum dispositivo I2C encontrado.");
  else
    Serial.println("Scan concluído.");
}

void loop() {
  // Nada no loop
}
