
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

char *skipSpace(char *buf, int* n)
{
    int k = strspn(buf, " \t");
    *n = k;
    return buf + k;
}

int beginword(char* buf, int k){
    char bw;
    bw = *(buf);
    for (int i = 0; i < k; i++)
        *(buf--);
    return bw;
}

int endword(char* buf){
    int  l = 0;
    while (*(buf) != ' '){
        *(buf++);
        l++;
        printf("%c\n", *buf);
    }
    *(buf--);
    int ew = *(buf);
    for(int i = 0; i < l ; i++)
        *(buf--);
    return ew;
}

char *obr(char **buf, char *newbuf, int k, char bw, char ew){
    int l = strlen(*buf);
    int n = 0;
    do{
        if (**buf != ' '){
            *newbuf++ = **buf;
            n++;
        }
    }while(*(*buf)++ != ew);
    *newbuf++ = ' ';
    int m = n;
    for( int i = m - 1 ; i < l ; i++){
        if (**buf == bw){
            do{
                if (**buf != ' '){
                    *newbuf++ = **buf;
                    n++;
                    i++;
                }
            }while(*(*buf)++ != ew);
            i--;
            *newbuf++ = ' ';
            *(*buf)--;
            n++;
        }
        *(*buf)++;
    }
    *newbuf--;
    *newbuf++ = '\0';
    for(int i = 0; i < n + 1 ; i++)
        *newbuf--;
    return newbuf;
    for(int i = 0; i < l; i++)
        *(*buf)--;
    return newbuf;
}

int main(){
    char *buf, *newbuf;
    int n;
    int k = 0;
    do{
        printf("Please enter line\n");
        buf=getstr();
        buf=skipSpace(buf, &k);

        if (*buf == NULL){
            printf("Error!\n");
            return  0;
        }
        k = 0;
        char bw;
        bw = beginword(buf, k);
        char ew;
        ew = endword(buf);
        newbuf =  (char *) malloc(2 * strlen(buf));
        newbuf = obr(&buf, newbuf, k, bw, ew);
        printf("Your line: '%s'\n", newbuf);
        free(buf);
        free(newbuf);
        printf("\n");

    }while (10);
}
