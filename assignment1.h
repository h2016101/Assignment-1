
#include <linux/ioctl.h>

int MAGIC_NUMBER;
#define MAGIC_NUMBER 12
#define READ_RANDOM _IOR(MAGIC_NUMBER, 0, char *)
#define READWRITE_RANDOM _IOWR(MAGIC_NUMBER, 2, char *)
