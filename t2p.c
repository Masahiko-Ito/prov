/*
 *  a part of convert text to ps
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXLINE 66
#define MAXBUF 4096
#define ANK 1
#define EUC 2

main(argc, argv)
    int  argc;
    char *argv[];
{
    unsigned char buf[MAXBUF+1], str[MAXBUF+1];
    int  char_sw = 0 /* ANK */;
    int  line = 0, col, i, j, maxline = 0;

    if (argc == 2){
	if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0){
            ShowUsage();
            exit(0);
	}
    }
    if (argc == 3){
	if (strcmp(argv[1], "-l") == 0){
	    if (argv[2] != NULL){
                maxline = atoi(argv[2]);
	    }
	}
    }
    if (maxline == 0){
        maxline = MAXLINE;
    }

    while (fgets(buf, MAXBUF, stdin) != (char)NULL){
        for (i = 0; i <= strlen(buf); /* do nothing */){
            if (buf[i] == ' '){
                i++;
            }else if (isprint(buf[i])){
                col = i;
                for (j = 0; isprint(buf[i]) && j < MAXBUF; i++, j++){
		    switch (buf[i]){
		      case '\\':
                        str[j] = buf[i];
			j++;
			if (j < MAXBUF){
                            str[j] = '\\';
			}
                        break;
		      case '(':
		      case ')':
                        str[j] = '\\';
			j++;
			if (j < MAXBUF){
                            str[j] = buf[i];
			}
                        break;
		      default:
                        str[j] = buf[i];
                        break;
		    }
                }
                str[j] = '\0';
                if (char_sw != ANK){
                    printf("12 FONT_NORMAL setChar\n");
                    char_sw = ANK;
                }
                printf("%d %d (%s) drawText\n", col, line, str);
            }else if (buf[i] & 0x80){
                col = i;
                sprintf(str, "%2x%2x", (buf[i] & 0x7f), (buf[i + 1] & 0x7f));
                for (j = strlen(str), i += 2; buf[i] & 0x80; i += 2, j = strlen(str)){
                    sprintf(str + j, " %2x%2x", (buf[i] & 0x7f), (buf[i + 1] & 0x7f));
                }
                if (char_sw != EUC){
                    printf("14 KFONT_RYUMIN setKanjiChar\n");
                    char_sw = EUC;
                }
                printf("%d %d <%s> drawText\n", col, line, str);
            }else if (buf[i] == '\f'){
                printf("showpage\n");
                line = 0;
                i++;
                char_sw = 0;
            }else{
                i++;
            }
        }
        line++;
        if (line >= maxline){
            printf("showpage\n");
            line = 0;
            char_sw = 0;
        }
    }
    if (line != 0){
        printf("showpage\n");
        char_sw = 0;
    }
}

ShowUsage()
{
    fprintf(stderr, "Usage: t2p [OPTION]\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "  -l maxline    maxline per page\n");
    fprintf(stderr, "  -h            show usage\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "a part of converting plain text to postscript from stdin\n");
    fprintf(stderr, "and output to stdout\n");

    return 0;
}
