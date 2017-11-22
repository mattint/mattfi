#include "gzip.h"
#include <sys/time.h>
GZIP GZIP_compress(char* data, int power) {
	struct timeval time;
	gettimeofday(&time, NULL);
	uint8_t count = sizeof(data) / 16;
	uint16_t id2 = data[8] << 4;
	uint8_t width = sizeof(data);
	GZIP gzip;
	gzip->header->id1 = 0x0000;
	gzip->header->id2 = id2;
	gzip->header->cm[0] = 'Z';
	gzip->header->cm[1] = 'L';
	gzip->header->cm[2] = 'I';
	gzip->header->cm[3] = 'B';
	gzip->header->flg->isData = true;
	gzip->header->flg->isCover = false;
	gzip->header->flg->escape = '~';	
	gzip->header->mtime = time.tv_usec;
	gzip->header->xfl->code[0] = '!';
	gzip->header->xfl->code[1] = '~';
	gzip->header->xfl->flg->isData = false;
	gzip->header->xfl->flg->isCover = true;
	gzip->header->xfl->flg->escape = NULL;
	gzip->header->xfl->isBasic = false;
	gzip->header->os[0] = 'U';
	gzip->header->os[1] = 'N';
	gzip->header->os[2] = 'I';
	gzip->header->os[3] = 'X';
	for(uint16_t i = 0; i < count; i++) {
		gzip->blocks[i]->operand = 0xE4;
		gzip->blocks[i]->file->compression = power >> 2;
		gzip->blocks[i]->file->file->isImportant = true;
		for(uint16_t ii = 0; i < width; i++) {
			gzip->blocks->[i]->file->file->bytes[ii] = data[ii] << power;
		}
	}
	gzip->footer->crc32[0] = NULL;
	for(uint16_t i = 1; i < (sizeof(id2) - 1); i++) {
		gzip->footer->crc32[i] = (char)(id2[i] >> (32 << power));
	}
	gzip->footer->isize = (uint64_t)(i & 0x0000 & 0x0000);
	return gzip;
}