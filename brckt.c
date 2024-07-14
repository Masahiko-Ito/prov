/*
 *  bracket field of csv
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DELIM '\t'
#define BUFMAX 4096

int ShowUsage();

int main(argc, argv)
	int argc;
	char *argv[];
{
	int ch, delim = DELIM, i;
	unsigned char buf[BUFMAX + 1];

	if (1 < argc) {
		if (strcmp(argv[1], "-h") == 0
		    || strcmp(argv[1], "--help") == 0) {
			ShowUsage();
			exit(0);
		}
		if (strcmp(argv[1], "-d") == 0) {
			delim = argv[2][0];
		}
	}

	for (;;) {
		for (i = 0; (ch = getchar()) != EOF && i < BUFMAX; i++) {
			if (ch == delim || ch == '\n') {
				break;
			} else {
				switch (ch) {
				case '\\':
					buf[i] = ch;
					i++;
					if (i < BUFMAX) {
						buf[i] = '\\';
					}
					break;
				case '(':
				case ')':
					buf[i] = '\\';
					i++;
					if (i < BUFMAX) {
						buf[i] = ch;
					}
					break;
				default:
					buf[i] = ch;
					break;
				}
			}
		}
		buf[i] = '\0';
		if (ch == EOF && i == 0) {
			break;
		}
		if (buf[0] & 0x80) {
			printf("<%s>%c", buf, ch);
		} else {
			printf("(%s)%c", buf, ch);
		}
	}
}

int ShowUsage()
{
	fprintf(stderr, "Usage: brckt [OPTION]\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "  -h        show usage\n");
	fprintf(stderr, "  -d delim  default is TAB\n");
	fprintf(stderr, "\n");
	fprintf(stderr,
		"bracket field of csv data from stdin to stdout\n");

	return 0;
}
