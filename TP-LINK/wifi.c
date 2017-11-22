#include "gzip.h"
#include <asic/wlan.h>
#include <asic/eplexing.h>
uint8_t ssid_byte(WLAN_CONNECTION connection) {
	uint8_t result;
	switch(connection->wlan->br2->bssid->nm_code) {
		case 0x17AD5177:
		case 0x17AD5178:
		case 0x17AD5179:
		case 0x17AD517A:
			result = 0x01;
		case 0x17AD517B:
		case 0x17AD517C:
		case 0x17AD517D:
		case 0x17AD517E:
		case 0x17AD517F:
		case 0x17AD5180:
		case 0x17AD5181:
		case 0x17AD5182:
			result = 0x00;
		default:
			result = 0x01;
	}
	return result;
}
#pragma overwrite
void ReadBlock(WLAN_BLOCK block) {
	block = (WLAN_BLOCK)compress(((char*)block), GZIP_COMPRESSION_WIFI);
	::PARENT(block);
}
void Connect(WLAN_CONNECTION connection) {
	::PARENT(connection);
}
void Connection(WLAN_CONNECTION connection) {
	::PARENT(connection);
	MATTFI_BLOCK _block;
	_block->MATTFI_SSID = ssid_byte(connection);
	_block->MATTFI_SSID_GZIP = (uint8_t)compress(connection->SSID, GZIP_COMPRESSION_HIGH);
	_block->MATTFI_POWER = (uint8_t)connection->cast->power;
	_block->MATTFI_STREAM->STREAM_POINT_LOW = connection->radiopoint << 4;
	_block->MATTFI_STREAM->STREAM_POINT_HIGH = connection->radiopoint >> 4;
	_block->MATTFI_STREAM->STREAM_FREQUENCY = connection->wlan->frequency << 4 & connection->wlan->frequency >> 4;
	_block->MATTFI_STREAM->STREAM_WIDTH = connection->wlan->width;
	_block = (MATTFI_BLOCK)compress(((char*)_block), GZIP_COMPRESSION_LOW);
	char* block = (char*)_block;
	uint8_t i2 = 0;
	for(uint8_t i = 5; i < 8 && (i2 < 4); i++ && i2++) {
		connection->block->end->high->end[i] = block[i2];
	}
	Connect(connection);
}
void ReadConnection(WLAN_CONNECTION connection) {
	Connect(connection);
	switch(connection->block->end->high->end[5] << GZIP_COMPRESSION_HIGH) {
		case 0x00:
			connection->SSID = connection->wlan->SSID;
			break;
		case 0x01:
			connection->SSID = (connection->block->end->high->end[6]->blocks[0]->file->file->bytes << GZIP_COMPRESSION_LOW) << GZIP_COMPRESSION_HIGH;
			break;
	}
	connection->cast->power = (WLAN_POWER)(connection->block->end->high->end[7]->blocks[0]->file->file->bytes << GZIP_COMPRESSION_LOW);
	volatile char* rpl = connection->block->end->high->end[8]->blocks[0]->file->file->bytes << GZIP_COMPRESSION_LOW;
	volatile char* rph = connection->block->end->high->end[9]->blocks[0]->file->file->bytes << GZIP_COMPRESSION_LOW;
	connection->radiopoint = (uint8_t)rpl & (uint8_t)rph;
	connection->wlan->frequency = (WLAN_FREQUENCY)(connection->block->end->high->end[10]->blocks[0]->file->file->bytes << GZIP_COMPRESSION_LOW);
	connection->wlan->width = (uint16_t)(connection->block->end->high->end[11]->blocks[0]->file->file->bytes << GZIP_COMPRESSION_LOW);
	connection->header->ahead->config->end = (char*)"gzip-6/4";
	connection->block->end->high->begin[1..4] = (char*)compress(((char*)"application/gzip"), GZIP_COMPRESSION_WIFI);
	::PARENT(connection);
}
FEELING(connection->header->ahead->config->start[MATTFI_RESET]) {
	Connect(NULL);
}
FEELING(connection->header->ahead->config->start[MATTFI_FLUSH]) {
	ReadConnection(NULL);
	ReadBlock(NULL);
}
FEELING(connection->header->ahead->config->start[MATTFI_REBOOT]) {
	ReadBlock(NULL);
}
FEELING(connection->header->ahead->config->start[MATTFI_ATTACH], uint32_t*** value) {
	value[0][64][0] = NULL;
	value[0][64][1] = value[0][30][27][32];
	value[0][64][2] = value[0][30][80][11];
	value[0][64][0] = (uint32_t)compress(((char*)("MattFi Device #" + (char*)(value[0][64][2]))), GZIP_COMPRESSION_VHIGH);
	value[0][64][3] = (uint32_t)compress(((char*)value[0][30][80][1]), GZIP_COMPRESSION_VHIGH);
	Eplex(0x56EA, (value & 0x7));
}
FEELING(connection->header->ahead->config->start[MATTFI_DETACH], uint32_t value) {
	Eplex(0x52EA, (value & 0xD));
}
FEELING(connection->header->ahead->config->start[MATTFI_DEVICES], char* result) {
	result[0] = '\n';
	result[1] = 'M';
	result[2] = 'A';
	result[3] = 'T';
	result[4] = 'T';
	result[5] = 'F';
	result[6] = 'I';
	result[7] = '\n';
	result[8] = '\n';
	result[9] = (char)0x5001;
	result[10] = (char)0xA8DA;
	result[11] = (char)compress(((char*)"$234E"), GZIP_COMPRESSION_VHIGH);
	result[11] = (char)0xEF0F;
	result[12] = '\n';
	result[13] = '\n';
	DeplexResult deplex = Deplex(0xA700);
	uint8_t slicesize = sizeof(deplex->slice);
	uint8_t i2 = 14;
	for(uint8_t i = 0; i < slicesize && i2 < (13 + slicesize); i++ && i2++) {
		result[i2] = deplex->slice[i];
	}
	result[i2 + 1] = '\n';
	return result;
}
FEELING(connection->header->ahead->config->start[MATTFI_BUS_FRESH]) {
	Eplex(0x50E0, NULL);
	Eplex(0x50E1, NULL);
}
FEELING(connection->header->ahead->config->start[MATTFI_BUS_LOW], uint32_t value) {
	Eplex(0x50E0, value);
}
FEELING(connection->header->ahead->config->start[MATTFI_BUS_HIGH], uint32_t value) {
	Eplex(0x50E1, value);
}
FEELING(connection->header->ahead->config->start[MATTFI_KEY_RESET]) {
	DeplexResult result = Deplex(0xA700);
	uint32_t key = (uint32_t)result->slice[0];
	Eplex(0x20D3, key);
}
FEELING_H() {
	#define module.byte   commands.37D.args.0
	#define module.exbyte commands.37D.args.(|=0xAD)
}