/*
 *  ps + overlay concatenate
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define MAXBUF 4096

#define OFF 0
#define ON  1

main(argc, argv)
    int  argc;
    char *argv[];
{
    initialize(argc, argv);
    mainloop(argc, argv);
    terminate(argc, argv);
    exit(0);
}

initialize(argc, argv)
    int  argc;
    char **argv;
{
    FILE *mc_fp;
    char buf[MAXBUF+1];

    if (argc == 2){
        ShowUsage();
        exit(0);
    }

    if ((mc_fp = fopen(argv[1], "r")) == NULL){
        fprintf(stderr, "macro file(%s) can't open\n", argv[1]);
        exit(errno);
    }

    while (fgets(buf, MAXBUF, mc_fp) != NULL){
        fputs(buf, stdout);
    }

    fclose(mc_fp);

    return 0;
}

mainloop(argc, argv)
    int  argc;
    char **argv;
{
    FILE *ov_fp, *ps_fp;
    char buf[MAXBUF+1];
    int  ov_sw;
    int  i;

    if ((ov_fp = fopen(argv[2], "r")) == NULL){
        fprintf(stderr, "overlay file(%s) can't open\n", argv[2]);
        exit(errno);
    }

    if (argc < 4){
        ov_sw = ON;
	while (fgets(buf, MAXBUF, stdin) != NULL){
	    if (ov_sw == ON){
                overlay(ov_fp);
                ov_sw = OFF;
	    }

            fputs(buf, stdout);

	    if (strcmp(buf, "showpage\n") == 0){
                ov_sw = ON;
	    }
	}
    }else{
        for (i = 3; i < argc; i++){
            if ((ps_fp = fopen(argv[i], "r")) == NULL){
                fprintf(stderr, "data file(%s) can't open\n", argv[i]);
                exit(errno);
            }
            ov_sw = ON;
	    while (fgets(buf, MAXBUF, ps_fp) != NULL){
	        if (ov_sw == ON){
                    overlay(ov_fp);
                    ov_sw = OFF;
	        }

                fputs(buf, stdout);

	        if (strcmp(buf, "showpage\n") == 0){
                    ov_sw = ON;
	        }
	    }
            fclose(ps_fp);
        }
        fclose(ov_fp);
    }

    return 0;
}


terminate(argc, argv)
    int  argc;
    char **argv;
{
    return 0;
}

overlay(ov_fp)
    FILE *ov_fp;
{
    char buf[MAXBUF+1];

    fseek(ov_fp, 0, SEEK_SET);
    while (fgets(buf, MAXBUF, ov_fp) != NULL){
        fputs(buf, stdout);
    }
    return 0;
}

ShowUsage()
{
    fprintf(stderr, "Usage: ovps HEADER.ps OVERLAY.ps [FILE.ps] ...\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "  -h    show usage\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "overlay HEADER.ps and OVERLAY.ps on FILE.ps (or stdin) and\n");
    fprintf(stderr, "output to stdout\n");

    return 0;
}
