#include <stdio.h>

// misc/linice-kdb/bin-2.4/test/int3.c
#define INT(_x) __asm__ __volatile__("int %0" :: "g" (_x))

int main(int argc, char *argv[])
{
	printf("Hello %s\n", "world");
	INT(3);
	printf("Bye %s\n", "dude");
	return 0;
}
