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
#define EUC 2
#define ANK_POINT "12"
#define KANJI_POINT "12"
#define ANK_XGAP "0"
#define ANK_YGAP "0"
#define KANJI_XGAP "0"
#define KANJI_YGAP "0"

main(argc, argv)
    int  argc;
    char *argv[];
{
    unsigned char buf[MAXBUF+1], str[MAXBUF+1];
    int  char_sw = 0 /* ANK */;
    int  line = 0, col, i, j, maxline = 0;
    char ankpoint[5], kanjipoint[5];
    char ank_xgap[5], ank_ygap[5];
    char kanji_xgap[5], kanji_ygap[5];

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

    if (getenv("PROV_ANK_POINT") == (char *)NULL){
       strncpy(ankpoint, ANK_POINT, (sizeof ankpoint) - 1);
    }else{
       strncpy(ankpoint, getenv("PROV_ANK_POINT"), (sizeof ankpoint) - 1);
    }
    ankpoint[(sizeof ankpoint) - 1] = '\0';

    if (getenv("PROV_KANJI_POINT") == (char *)NULL){
       strncpy(kanjipoint, KANJI_POINT, (sizeof kanjipoint) - 1);
    }else{
       strncpy(kanjipoint, getenv("PROV_KANJI_POINT"), (sizeof kanjipoint) - 1);
    }
    kanjipoint[(sizeof kanjipoint) - 1] = '\0';

    if (getenv("PROV_ANK_XGAP") == (char *)NULL){
       strncpy(ank_xgap, ANK_XGAP, (sizeof ank_xgap) - 1);
    }else{
       strncpy(ank_xgap, getenv("PROV_ANK_XGAP"), (sizeof ank_xgap) - 1);
    }
    ank_xgap[(sizeof ank_xgap) - 1] = '\0';

    if (getenv("PROV_ANK_YGAP") == (char *)NULL){
       strncpy(ank_ygap, ANK_YGAP, (sizeof ank_ygap) - 1);
    }else{
       strncpy(ank_ygap, getenv("PROV_ANK_YGAP"), (sizeof ank_ygap) - 1);
    }
    ank_ygap[(sizeof ank_ygap) - 1] = '\0';

    if (getenv("PROV_KANJI_XGAP") == (char *)NULL){
       strncpy(kanji_xgap, KANJI_XGAP, (sizeof kanji_xgap) - 1);
    }else{
       strncpy(kanji_xgap, getenv("PROV_KANJI_XGAP"), (sizeof kanji_xgap) - 1);
    }
    kanji_xgap[(sizeof kanji_xgap) - 1] = '\0';

    if (getenv("PROV_KANJI_YGAP") == (char *)NULL){
       strncpy(kanji_ygap, KANJI_YGAP, (sizeof kanji_ygap) - 1);
    }else{
       strncpy(kanji_ygap, getenv("PROV_KANJI_YGAP"), (sizeof kanji_ygap) - 1);
    }
    kanji_ygap[(sizeof kanji_ygap) - 1] = '\0';

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
                    printf("%s FONT_NORMAL setChar\n", ankpoint);
                    printf("/SYS_drawText_Xgap %s def\n", ank_xgap);
                    printf("/SYS_drawText_Ygap %s def\n", ank_ygap);
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
                    printf("%s KFONT_RYUMIN setKanjiChar\n", kanjipoint);
                    printf("/SYS_drawText_Xgap %s def\n", kanji_xgap);
                    printf("/SYS_drawText_Ygap %s def\n", kanji_ygap);
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
