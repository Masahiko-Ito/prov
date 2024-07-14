/*
 *  UTF-8 to HEX string converter
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int ShowUsage();

int main(argc, argv)
	int argc;
	char *argv[];
{
	int ch, oldch = ' ';

	if (1 < argc) {
		if (strcmp(argv[1], "-h") == 0
		    || strcmp(argv[1], "--help") == 0) {
			ShowUsage();
			exit(0);
		}
	}

	while ((ch = getchar()) != EOF) {
		if (ch & 0x80) {
			if (oldch & 0x80) {
				putchar(' ');
			}
			printf("%02x", ch);
			oldch = ch;
		} else {
			putchar(ch);
			oldch = ch;
		}
	}
}

int ShowUsage()
{
	fprintf(stderr, "Usage: k2c [OPTION]\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "  -h    show usage\n");
	fprintf(stderr, "\n");
	fprintf(stderr,
		"convert UTF-8 to HEX string from stdin and output to stdout\n");

	return 0;
}
