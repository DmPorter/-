#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

char *getstr() {
    char *ptr = (char *) malloc(1);
    char buf[81];
    int n, len = 0;
    *ptr = '\0';

    do {
        n = scanf("%80[^\n]", buf);
        if (n < 0) {
            free(ptr);
            ptr = NULL;
            continue;
        }
        if (n == 0) scanf("%*c");
        else {
            len += strlen(buf);
            ptr = (char *) realloc(ptr, len + 1);
            strncat(ptr, buf, len);
        }

    } while (n > 0);
    return ptr;
}

char *skipSpace(char *buf)
{
    int k = strspn(buf, " \t");
    return buf + k;
}

char *skipWord(char *buf){
    while(*buf != NULL) {
        if ((*buf != '1') && (*buf != '3') && (*buf != '5') && (*buf != '7') && (*buf != '9') && (*buf != '0') && (*buf != '2') && (*buf != '4') && (*buf != '6') && (*buf != '8') && (*buf != ' ')) {
            printf("Error!\n");
            return 0;
        } else{
            *buf++;
        }


    }
    return 1;
}

char *obr(char *buf, char **newbuf1, char** newbuf2){
    int i = 0, n = 0;
    while(*buf != NULL) {
        if ((*buf == '1') || (*buf == '3') || (*buf == '5') || (*buf == '7') || (*buf == '9')) {
            *(*newbuf1)++ = *buf;
            i++;
        }
        else if ((*buf == '0') || (*buf == '2') || (*buf == '4') || (*buf == '6') || (*buf == '8')){
            *(*newbuf2)++ = *buf;
            n++;
        }
    *buf++;
        if(*buf == ' ') {
            buf = skipSpace(buf);
                if (*buf != NULL){
                    *(*newbuf1)++ = ' ';
                    n++;
                    *(*newbuf2)++ = ' ';
                    i++;
                }
        }
    }
    *(*newbuf1)++ = NULL;
    *(*newbuf2)++ = NULL;
    for(int l = 0; l < n + 1; l++)
        *(*newbuf2)--;
    for(int l = 0; l < i + 1; l++)
        *(*newbuf1)--;
}

int main(){
    char *buf, *newbuf1, *newbuf2;

    do{
        printf("Please enter line\n");
        buf=getstr();
        buf=skipSpace(buf);

        while (*buf == NULL){
            printf("Please re-enter line\n");
            buf=getstr();
            buf = skipSpace(buf);
        }

        while(skipWord(buf) == 0){
            printf("Please re-enter line\n");
            buf=getstr();
            buf=skipSpace(buf);
            while (*buf == NULL){
                printf("Please re-enter line\n");
                buf=getstr();
                buf = skipSpace(buf);
            }
        }



        newbuf1 =  (char *) malloc(strlen(buf));
        newbuf2 =  (char *) malloc(strlen(buf));

        obr(buf, &newbuf1, &newbuf2);

        printf("Your line: '%s'\n", newbuf1);
        printf("Your line: '%s'\n", newbuf2);

        free(buf);
        free(newbuf1);
        printf("\n");

    }while (10);
}
