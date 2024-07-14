/*
 *  a part of convert text to ps
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXLINE 66
#define MAXBUF 4096
#define ANK 1
#define UTF8 2
#define ANK_POINT "12"
#define KANJI_POINT "12"
#define ANK_XGAP "0"
#define ANK_YGAP "0"
#define KANJI_XGAP "2.4"
#define KANJI_YGAP "0"

int ShowUsage();

int main(argc, argv)
	int argc;
	char *argv[];
{
	unsigned char buf[MAXBUF + 1], str[MAXBUF + 1];
	int char_sw = 0 /* ANK */ ;
	int line = 0, col, col_tmp, i, j, maxline = 0;
	char ankpoint[5], kanjipoint[5];
	char ank_xgap[5], ank_ygap[5];
	char kanji_xgap[5], kanji_ygap[5];

	if (argc == 2) {
		if (strcmp(argv[1], "-h") == 0
		    || strcmp(argv[1], "--help") == 0) {
			ShowUsage();
			exit(0);
		}
	}
	if (argc == 3) {
		if (strcmp(argv[1], "-l") == 0) {
			if (argv[2] != NULL) {
				maxline = atoi(argv[2]);
			}
		}
	}
	if (maxline == 0) {
		maxline = MAXLINE;
	}

	if (getenv("PROV2_ANK_POINT") == (char *) NULL) {
		strncpy(ankpoint, ANK_POINT, (sizeof ankpoint) - 1);
	} else {
		strncpy(ankpoint, getenv("PROV2_ANK_POINT"),
			(sizeof ankpoint) - 1);
	}
	ankpoint[(sizeof ankpoint) - 1] = '\0';

	if (getenv("PROV2_KANJI_POINT") == (char *) NULL) {
		strncpy(kanjipoint, KANJI_POINT, (sizeof kanjipoint) - 1);
	} else {
		strncpy(kanjipoint, getenv("PROV2_KANJI_POINT"),
			(sizeof kanjipoint) - 1);
	}
	kanjipoint[(sizeof kanjipoint) - 1] = '\0';

	if (getenv("PROV2_ANK_XGAP") == (char *) NULL) {
		strncpy(ank_xgap, ANK_XGAP, (sizeof ank_xgap) - 1);
	} else {
		strncpy(ank_xgap, getenv("PROV2_ANK_XGAP"),
			(sizeof ank_xgap) - 1);
	}
	ank_xgap[(sizeof ank_xgap) - 1] = '\0';

	if (getenv("PROV2_ANK_YGAP") == (char *) NULL) {
		strncpy(ank_ygap, ANK_YGAP, (sizeof ank_ygap) - 1);
	} else {
		strncpy(ank_ygap, getenv("PROV2_ANK_YGAP"),
			(sizeof ank_ygap) - 1);
	}
	ank_ygap[(sizeof ank_ygap) - 1] = '\0';

	if (getenv("PROV2_KANJI_XGAP") == (char *) NULL) {
		strncpy(kanji_xgap, KANJI_XGAP, (sizeof kanji_xgap) - 1);
	} else {
		strncpy(kanji_xgap, getenv("PROV2_KANJI_XGAP"),
			(sizeof kanji_xgap) - 1);
	}
	kanji_xgap[(sizeof kanji_xgap) - 1] = '\0';

	if (getenv("PROV2_KANJI_YGAP") == (char *) NULL) {
		strncpy(kanji_ygap, KANJI_YGAP, (sizeof kanji_ygap) - 1);
	} else {
		strncpy(kanji_ygap, getenv("PROV2_KANJI_YGAP"),
			(sizeof kanji_ygap) - 1);
	}
	kanji_ygap[(sizeof kanji_ygap) - 1] = '\0';

	while (fgets(buf, MAXBUF, stdin) != (char *) NULL) {
		col = 0;
		for (i = 0; i <= strlen(buf); /* do nothing */ ) {
			if (buf[i] == ' ') {
				i++;
				col++;
			} else if (isprint(buf[i])) {
				col_tmp = 0;
				for (j = 0; isprint(buf[i]) && j < MAXBUF;
				     i++, j++) {
					switch (buf[i]) {
					case '\\':
						str[j] = buf[i];
						j++;
						if (j < MAXBUF) {
							str[j] = '\\';
						}
						break;
					case '(':
					case ')':
						str[j] = '\\';
						j++;
						if (j < MAXBUF) {
							str[j] = buf[i];
						}
						break;
					default:
						str[j] = buf[i];
						break;
					}
					col_tmp++;
				}
				str[j] = '\0';
				if (char_sw != ANK) {
					printf("%s FONT_NORMAL setChar\n",
					       ankpoint);
					printf
					    ("/SYS_drawText_Xgap %s def\n",
					     ank_xgap);
					printf
					    ("/SYS_drawText_Ygap %s def\n",
					     ank_ygap);
					char_sw = ANK;
				}
				printf("%d %d (%s) drawText\n", col, line,
				       str);
				col += col_tmp;
			} else if (buf[i] & 0x80) {
				col_tmp = 0;
				str[0] = '\0';
				for (j = strlen(str); buf[i] & 0x80;
				     i++, j = strlen(str)) {
					sprintf(str + j, "%2x", buf[i]);
					if ((buf[i] & 0xc0) == 0xc0) {
						col_tmp += 2;
					}
				}
				if (char_sw != UTF8) {
					printf
					    ("%s KFONT_RYUMIN setKanjiChar\n",
					     kanjipoint);
					printf
					    ("/SYS_drawText_Xgap %s def\n",
					     kanji_xgap);
					printf
					    ("/SYS_drawText_Ygap %s def\n",
					     kanji_ygap);
					char_sw = UTF8;
				}
				printf("%d %d <%s> drawText\n", col, line,
				       str);
				col += col_tmp;
			} else if (buf[i] == '\f') {
				printf("showpage\n");
				line = 0;
				i++;
				char_sw = 0;
			} else {
				i++;
			}
		}
		line++;
		if (line >= maxline) {
			printf("showpage\n");
			line = 0;
			char_sw = 0;
		}
	}
	if (line != 0) {
		printf("showpage\n");
		char_sw = 0;
	}
}

int ShowUsage()
{
	fprintf(stderr, "Usage: t2p [OPTION]\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "  -l maxline    maxline per page\n");
	fprintf(stderr, "  -h            show usage\n");
	fprintf(stderr, "\n");
	fprintf(stderr,
		"a part of converting plain text to postscript from stdin\n");
	fprintf(stderr, "and output to stdout\n");

	return 0;
}
