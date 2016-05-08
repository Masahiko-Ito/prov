BEGIN{
    DEFFILE = "";
    CSVFILE = "";
    FS = "\t";
    curpoint = 0;
    curchar = "";
}
{
    if (DEFFILE == ""){
        DEFFILE = FILENAME;
        i = 0;
    }
    if (DEFFILE != FILENAME && CSVFILE == ""){
        CSVFILE = FILENAME;
        in_recno = 0;
        maxdef = i;
        i = 1;
    }
    if (CSVFILE == ""){
        i++;
        recno[i] = $1;
        field[i] = $2;
        point[i] = $3;
        x[i] = $4;
        y[i] = $5;
    }else{
        in_recno++;
        for (/* do nothing */; i <= maxdef; i++){
            if (in_recno == recno[i]){
                if ($field[i] ~ /^\(.*\)$/){
                    if (curpoint != point[i] || curchar != "CHAR"){
                        printf("%d FONT_NORMAL setChar\n", point[i]);
                        curpoint = point[i];
                        curchar = "CHAR";
                    }
                }else{
                    if (curpoint != point[i] || curchar != "KANJI"){
                        printf("%d KFONT_RYUMIN setKanjiChar\n", point[i]);
                        curpoint = point[i];
                        curchar = "KANJI";
                    }
                }
                printf("%s %s %s drawText\n", x[i], y[i], $field[i]);
            }else{
                break;
            }
        }
        if (maxdef < i){
            printf("showpage\n");
            i = 1;
            in_recno = 0;
            curpoint = 0;
            curchar = "";
        }
    }
}
END{
    if (in_recno != 0){
        printf("showpage\n")
    }
}
