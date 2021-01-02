#include <Arduino.h>
#include <TinyGPS++.h>
#include <LoRa.h>
#include "LTM.hpp"

#define TRANSMIT_PERIOD 5000

#define LORA_FREQ 868E6
#define LORA_TX_POWER 20
#define LORA_BANDWIDTH 62.5E3
#define LORA_CODING_RATE 5
#define LORA_SPREADING_FACTOR 12

HardwareSerial gpsSerial(2);
TinyGPSPlus gps;
LTM_GPS_frame ltm_gps_frame;

uint32_t timer = 0;

void setup() {
  gpsSerial.begin(9600);
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);
	LoRa.begin(LORA_FREQ);
	LoRa.sleep();
	LoRa.enableCrc();
	LoRa.setTxPower(LORA_TX_POWER, PA_OUTPUT_PA_BOOST_PIN);
	LoRa.setSignalBandwidth(LORA_BANDWIDTH);
	LoRa.setCodingRate4(LORA_CODING_RATE);
  LoRa.setSpreadingFactor(LORA_SPREADING_FACTOR);
	LoRa.idle();

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