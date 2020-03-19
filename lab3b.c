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


int length(stringItem *ptr){
    int n = 0;
    while (ptr != NULL){
        n++;
        ptr = ptr->next;
    }
    return n;
}

struct stringItem* obr(stringItem *st){
    struct stringItem  *tmp;
    struct stringItem  *head = malloc(sizeof(stringItem)), *head2 = malloc(sizeof(stringItem));
    int i = 0;


    if (st != NULL)
        while((st != NULL) & (st->c == ' ')) {
            st = st->next;
            if (st == NULL)
            {
                printf("Error!\n");
                return 0;
            }
    }
    else
    {
        printf("Error!\n");
        return 0;
    }

    head->c = st->c;
    head->next = NULL;
    st = st->next;

    while(st != NULL) {
        while ((st != NULL) & (st->c != ' ')) {
            tmp = (stringItem *) malloc(sizeof(stringItem));
            tmp->c = st->c;
            st = st->next;
            tmp->next = head;
            head = tmp;

            if (st == NULL )
                return head;
        }

        if (st == NULL )
            return head;
        while((st != NULL) & (st->c == ' ') ){
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
        printList(obr(st));
        deleteList(st);
    } while(1);
    return 0;
}
