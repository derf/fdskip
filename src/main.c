#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define SKIPR_WRITESIZE 512
#define SKIPR_MAX_SEEK (1474560 / (SKIPR_WRITESIZE))

void die(char *s)
{
	perror(s);
	exit(1);
}

void read_and_seek(int fd, int offset)
{
	char data[] = "discarded";

	if (lseek(fd, offset * SKIPR_WRITESIZE, SEEK_SET) == -1)
		die("lseek");
	if (write(fd, data, sizeof(data)) == -1)
		die("write");
	if (fsync(fd) == -1)
		die("fsync");
}

int main(int argc, char **argv)
{
	char *device;
	int fd, curarg;

	if (argc < 2) {
		fputs("Usage: skipr <device> <off1> ...\n", stderr);
		exit(1);
	}

	device = argv[1];

	if ((fd = open(device, O_WRONLY)) == -1)
		die("open");

	for (curarg = 2; curarg < argc; curarg++) {
		read_and_seek(fd, atoi(argv[curarg]));
	}

	close(fd);
	return 0;
}
