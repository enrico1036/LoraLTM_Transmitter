#include <stdint.h>
#include <WString.h>

#define LIGHTTELEMETRY_START1 0x24 //$ HEADER '$'
#define LIGHTTELEMETRY_START2 0x54 //T  HEADER 'T'
#define LIGHTTELEMETRY_GFRAME 0x47 //G GPS + Baro altitude data ( Lat, Lon, Speed, Alt, Sats, Sat fix)
#define LIGHTTELEMETRY_AFRAME 0x41 //A Attitude data ( Roll,Pitch, Heading )
#define LIGHTTELEMETRY_SFRAME 0x53 //S Sensors/Status data ( VBat, Consumed current, Rssi, Airspeed, Arm status, Failsafe status, Flight mode )
#define LIGHTTELEMETRY_OFRAME 0x4F //O  Origin data (home lon, home lat, homefix)
#define LIGHTTELEMETRY_NFRAME 0x4E //N  NAVIGATION data
#define LIGHTTELEMETRY_XFRAME 0x58 //X  EXTRA FRAME

#define LIGHTTELEMETRY_GFRAMELENGTH 18
#define LIGHTTELEMETRY_AFRAMELENGTH 10
#define LIGHTTELEMETRY_SFRAMELENGTH 11
#define LIGHTTELEMETRY_OFRAMELENGTH 18
#define LIGHTTELEMETRY_NFRAMELENGTH 10
#define LIGHTTELEMETRY_XFRAMELENGTH 10

class LTM_GPS_frame {
private:
	char buff[LIGHTTELEMETRY_GFRAMELENGTH];
	bool valid;

public:
	LTM_GPS_frame();
	bool update(double lat, double lon, uint8_t speed, int32_t alt, uint8_t sats, uint8_t fix);
	bool isValid() {return valid;}
	size_t getSize() {return sizeof(buff);}
	String toString();
	uint8_t* toCharBuff();
};