#include "LTM.hpp"

LTM_GPS_frame::LTM_GPS_frame()
{
	this->valid = false;
	this->buff[0] = LIGHTTELEMETRY_START1;
	this->buff[1] = LIGHTTELEMETRY_START2;
	this->buff[2] = LIGHTTELEMETRY_GFRAME;
}

bool LTM_GPS_frame::update(double lat, double lon, uint8_t speed, int32_t alt, uint8_t sats, uint8_t fix)
{
	if(lat < -90 || lat > 90) {
		return false;
	}

	if(lon < -180 || lon > 180){
		return false;
	}

	uint32_t lat_mult = lat * 1E7;
	buff[3] = (uint8_t)(lat_mult>>0);
	buff[4] = (uint8_t)(lat_mult>>8);
	buff[5] = (uint8_t)(lat_mult>>16);
	buff[6] = (uint8_t)(lat_mult>>24);

	uint32_t lon_mult = lon * 1E7;
	buff[7] = (uint8_t)(lon_mult>>0);
	buff[8] = (uint8_t)(lon_mult>>8);
	buff[9] = (uint8_t)(lon_mult>>16);
	buff[10] = (uint8_t)(lon_mult>>24);

	buff[11] = speed;

	uint32_t alt_cm = alt * 100;
	buff[12] = (uint8_t)(alt_cm>>0);
	buff[13] = (uint8_t)(alt_cm>>8);
	buff[14] = (uint8_t)(alt_cm>>16);
	buff[15] = (uint8_t)(alt_cm>>24);

	buff[16] = (sats << 2) | (0x03 & fix);
	
	uint8_t checksum = 0;
	for (uint16_t i = 3; i < LIGHTTELEMETRY_GFRAMELENGTH-1; i++)
	{
		checksum ^= buff[i];
	}
	buff[17] = checksum;
	return true;
}

String LTM_GPS_frame::toString()
{
	return String(buff);
}

uint8_t* LTM_GPS_frame::toCharBuff()
{
	return (uint8_t*)buff;
}