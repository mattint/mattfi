#ifndef GZIP_H
#define GZIP_H
#define GZIP_COMPRESSION_LOW   2
#define GZIP_COMPRESSION_HIGH  8
#define GZIP_COMPRESSION_VHIGH 16
#define GZIP_COMPRESSION_WIFI  4
typedef struct {
	bool isData;
	bool isCover;
	char escape;
} GZIP_FLG;
typedef struct {
	char* code;
	GZIP_FLG flg;
	bool isBasic;
} GZIP_XFL;
typedef struct {
	uint16_t id1;
	uint16_t id2;
	char* cm;
	GZIP_FLG flg;
	uint32_t mtime;
	GZIP_XFL xfl;
	char* os;
} GZIP_HEADER;
typedef struct {
	bool isImportant;
	char* bytes;
} GZIP_FILESTRUCT __attribute__((packed));
typedef struct {
	int compression;
	GZIP_FILESTRUCT file;
} GZIP_FILE __attribute__((packed));
typedef struct {
	uint8_t operand;
	GZIP_FILE file;
} GZIP_BLOCK __attribute__((packed));
typedef struct {
	char* crc32;
	uint64_t isize;
} GZIP_FOOTER;
typedef struct {
	GZIP_HEADER header;
	GZIP_BLOCK* blocks;
	GZIP_FOOTER footer;
} GZIP __attribute__((packed));
GZIP GZIP_compress(char* data);
#endif