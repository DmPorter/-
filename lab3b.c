#include "stdlib.h"
#include "malloc.h"
#include "stdio.h"
#include <string.h>

typedef struct stringItem {
    char c;
    struct stringItem *next;
} stringItem;


void deleteList(stringItem *p_begin){
    stringItem *p = p_begin;
    while (p != NULL) {
        stringItem *tmp;
        tmp = p;
        p = p->next;
        free(tmp);
    }
}

int getList(stringItem **pptr) {
    char buf[81], *str;
    stringItem head = {'*', NULL};
    stringItem *last = &head;
    int n, rc = 1;
    do {
        n = scanf("%80[^\n]", buf);
        if (n < 0) {
            deleteList(head.next);
            head.next = NULL;
            rc = 0;
            continue;
        }
        if (n > 0) {
            for (str = buf; *str != '\0'; str++) {
                last->next = (stringItem *) malloc(sizeof(stringItem));
                last = last->next;
                last->c = *str;
            }
            last->next = NULL;
        } else scanf("%*c");
    } while (n > 0);

    *pptr = head.next;
    return rc;
}

void printList(stringItem *char_begin){
    stringItem *p=char_begin;
    printf("'");
    while(p!=NULL){
        printf("%c", p->c);
        p = p->next;
    }
    printf("'");
    printf("\n");
};

struct stringItem* obr(stringItem *st){
    struct stringItem  *tmp;
    struct stringItem  *head = malloc(sizeof(stringItem));

    if (st != NULL)
        while((st != NULL) & (st->c < '0') || (st->c > '9')) {
            st = st->next;
            if (st == NULL)
                return 0;
        }
    else return 0;

    head->c = st->c;
    head->next = NULL;
    st = st->next;
    
    while(st != NULL) {
        while ((st != NULL) & (st->c >= '0') & (st->c <= '9')) {
            tmp = (stringItem *) malloc(sizeof(stringItem));
            tmp->c = st->c;
            st = st->next;
            tmp->next = head;
            head = tmp;

            if (st == NULL )
                return head;
        }
        while((st != NULL) & (st->c < '0') || (st->c > '9') ){
            st = st->next;
            if (st == NULL )
                return head;
        }
        
        tmp = (stringItem *) malloc(sizeof(stringItem));
        tmp->c = ' ';
        tmp->next = head;
        head = tmp;
    }
    return head;
}

int main(){
    struct stringItem *st;
    do{
        getList(&st);
        st = obr(st);
        printList(st);
        deleteList(st);
    } while(1);
}
