#include <Arduino.h>
#include <TinyGPS++.h>


HardwareSerial gpsSerial(2);
TinyGPSPlus gps;

void setup() {
  gpsSerial.begin(9600);
  Serial.begin(115200);
}

void loop() {
  while (gpsSerial.available() > 0)
  {
    if (gps.encode(gpsSerial.read()))
    {
      Serial.print(F("Sats=")); Serial.print(gps.satellites.value());Serial.print(F(" "));
      if(gps.location.isValid())
      {
        Serial.print(gps.location.lat(), 6);
        Serial.print(F(","));
        Serial.print(gps.location.lng(), 6);
      }
      Serial.println();
    }
  }
}