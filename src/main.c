#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define SKIPR_WRITESIZE (3 * 512)
#define SKIPR_MAX_SEEK (1474560 / (SKIPR_WRITESIZE))
#define SKIPR_MID ((SKIPR_MAX_SEEK) / 2)

int last_offset = 0;

void die(char *s)
{
	perror(s);
	exit(1);
}

void read_and_seek(int fd, int rel_offset)
{
	char data[SKIPR_WRITESIZE];
	int real_offset;

	memset(data, 0, SKIPR_WRITESIZE);

	if (last_offset < SKIPR_MID)
		real_offset = last_offset + rel_offset;
	else
		real_offset = last_offset - rel_offset;

	if ((real_offset < 0) || (real_offset > SKIPR_MAX_SEEK)) {
		fprintf(
				stderr,
				"Relatev offset %d (last %d, result %d) is too high.\n"
				"If you want to play safe, use offsets <= %d.\n",
				rel_offset, last_offset, real_offset, SKIPR_MID
		);
		return;
	}

	printf("%5d => %d\n", rel_offset, real_offset);

	if (lseek(fd, real_offset * SKIPR_WRITESIZE, SEEK_SET) == -1)
		die("lseek");
	if (write(fd, data, sizeof(data)) == -1)
		die("write");
	if (fsync(fd) == -1)
		die("fsync");

	last_offset = real_offset;
}

int main(int argc, char **argv)
{
	char *device;
	int fd, curarg;

	if (argc < 1) {
		fputs("Usage: skipr <device> <off1> ...\n", stderr);
		exit(1);
	}
	else if (argc < 2) {
		printf(
			"writesize %d bytes -> maximum delay = %d\n",
			SKIPR_WRITESIZE, SKIPR_MID
		);
		exit(0);
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
