#ifndef WIFI_H
#define WIFI_H
#define MATTFI_RESET      0
#define MATTFI_FLUSH      1
#define MATTFI_REBOOT     2
#define MATTFI_ATTACH     3
#define MATTFI_DETACH     4
#define MATTFI_DEVICES    5
#define MATTFI_BUS_FRESH  6
#define MATTFI_BUS_LOW    7
#define MATTFI_BUS_HIGH   8
#define MATTFI_KEY_RESET  9
typedef struct {
	uint8_t STREAM_POINT_LOW;
	uint8_t STREAM_POINT_HIGH;
	uint16_t STREAM_FREQUENCY;
	uint16_t STREAM_WIDTH;
} WIFI_STREAM;
typedef struct {
	uint8_t MATTFI_SSID;
	char* MATTFI_SSID_GZIP;
	uint8_t MATTFI_POWER;
	WIFI_STREAM MATTFI_STREAM;
} MATTFI_BLOCK;
#endif