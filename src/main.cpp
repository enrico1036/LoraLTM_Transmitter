#include <Arduino.h>
#include <TinyGPS++.h>
#include "Lora/lora.hpp"
#include "LTM.hpp"

#define TRANSMIT_PERIOD 5000

HardwareSerial gpsSerial(2);
TinyGPSPlus gps;
LTM_GPS_frame ltm_gps_frame;

uint32_t timer = 0;

void setup() {
  gpsSerial.begin(9600);
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  LORA_setup();

  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  while (gpsSerial.available() > 0)
  {
    gps.encode(gpsSerial.read());
  }


  if(millis() - timer > TRANSMIT_PERIOD)
  {
    timer = millis();
    if(gps.location.isValid() && gps.speed.isValid() && gps.satellites.isValid() && gps.altitude.isValid() && gps.hdop.isValid())
      {
        if(ltm_gps_frame.update(gps.location.lat(), gps.location.lng(), gps.speed.mps(), gps.altitude.meters(), gps.satellites.value(), gps.hdop.value()))
        {
          Serial.println(ltm_gps_frame.toString());
        }
      }
      digitalWrite(LED_BUILTIN, HIGH);
      LoRa.beginPacket();
      LoRa.write(ltm_gps_frame.toCharBuff(), ltm_gps_frame.getSize());
      LoRa.endPacket();
      digitalWrite(LED_BUILTIN, LOW);
  }
}