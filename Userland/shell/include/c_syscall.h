#include <stdint.h>
#define STDIN  0
#define STDOUT 1
#define STDERR 2

typedef struct{
	uint8_t sec,min,hour,day,month;
	uint32_t year;
} TIME;

int64_t write(uint64_t fd,const uint8_t * buf, uint64_t count );
int64_t read(uint64_t fd, const uint8_t * buf, uint8_t count);
TIME * time();
