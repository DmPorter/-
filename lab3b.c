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

struct stringItem* deleteZERO(stringItem *st){
    while(st != NULL){
        if (st->c == '0')
            st = st->next;
        else return st;
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

struct stringItem* add(stringItem *st, int *i){
    struct stringItem  *tmp;
    struct stringItem  *head = malloc(sizeof(stringItem));
    int n = 0;
    if (st != NULL)
        while((st != NULL) & (st->c < '0') || (st->c > '9')) {
            st = st->next;
            n++;
            if (st == NULL){
                *i = n;
                return 0;
            }
        }
    else return 0;

        n++;
        head->c = st->c;
        head->next = NULL;
        st = st->next;

    if (st == NULL){
        *i = n;
        return head;
    }

    while ((st != NULL) & (st->c >= '0') & (st->c <= '9')) {
        tmp = (stringItem *) malloc(sizeof(stringItem));
        tmp->c = st->c;
        st = st->next;
        tmp->next = head;
        head = tmp;
        n++;
        *i = n;
        if (st == NULL )
            return head;
    }
    while((st != NULL) & (st->c < '0') || (st->c > '9') ){
        st = st->next;
        n++;
        *i = n;
        if (st == NULL )
            return head;
    }
    *i = n;
    return head;
}

stringItem* getLast(stringItem *head) {
    if (head == NULL) {
        return NULL;
    }
    while (head->next) {
        head = head->next;
    }
    return head;
}


void pushBack(stringItem *head, char value) {
    stringItem *last = getLast(head);
    stringItem *tmp = (stringItem*) malloc(sizeof(stringItem));
    tmp->c = value;
    tmp->next = NULL;
    last->next = tmp;
}

struct stringItem* obr(stringItem *st){
    struct stringItem  *tmp;
    int i = 0;
    struct stringItem  *head;

    tmp = deleteZERO(add(st, &i));
    for(int l = 0; l < i; l++){
        st = st->next;
    }

    if(st != NULL) {
        pushBack(tmp, ' ');
        head = deleteZERO(add(st, &i));
    } else return tmp;

    while(st != NULL)
    {
        pushBack(tmp, head->c);
        if(head->next == NULL)
        {
            for(int l = 0; l < i; l++){
                st = st->next;
            }
            if(st != NULL){
                pushBack(tmp, ' ');
                head = add(st, &i);
            } else return tmp;

        }
        else head = head->next;
    }
}


int main(){
    struct stringItem *st, *head;
    do{
        getList(&st);
        head = obr(st);
        printList(head);
        deleteList(st);
    } while(1);
}
