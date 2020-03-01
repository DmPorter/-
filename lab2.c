#include<stdio.h>
#include<stdlib.h>
#include<locale.h>
#include<malloc.h>

#define SEED 2

typedef struct line {
    int n;
    int* arr;
} line;

typedef struct matrix {
    int m;
    line* pt;
} matrix;

void input(struct matrix* matrix) {
    int n;
    int m = matrix->m;

    line* m_ptr = matrix->pt;
    int* ptr;

    do {
        getInt(&m);
        if (m <= 0)
            printf_s("Error! \n");
    } while (m <= 0);

    matrix->m = m;
    matrix->pt = (line*)calloc(m, sizeof(line));

    for (int i = 0; i < m; ++i) {
        printf("Input quantity of elements in %d row\n", i + 1);
        do {
            getInt(&n);
            if (n <= 0)
                printf_s("Error! \n");
        } while (n <= 0);

        (*matrix).pt[i].n = n;  
        (*matrix).pt[i].arr = (int*)calloc(n, sizeof(int));

        ptr = (*matrix).pt[i].arr;

        for (int j = 0; j < n; j++) {

            *(ptr++) = rand() % 21 - 10 ;
        }
    }
}


int getInt(int* a)
{
    int n;
    do
    {
        n = scanf_s("%d", a);
        if (n == 0)
        {
            printf_s("Error!\n");
            scanf_s("%*[^\n]");
        }
    } while (n == 0);
    return n < 0 ? 0 : 1;

}


void printm(matrix* rm) {
    int i, j;

    line* m_ptr = rm->pt;
    int* ptr;

    for (i = 0; i < rm->m; i++) {
        ptr = m_ptr->arr;
        for (j = 0; j < m_ptr->n; j++) {
            printf("%d ", *(ptr++));
        }
        m_ptr++;
        printf("\n");
    }
    printf("\n");
}


void printl(line* a, int l) {

    int *ptr =  a->arr;

    for (int i = 0; i < l; i++)
        printf_s("%d\t", *(ptr++));
}

void armean(struct matrix* matrix, line* a)
{
    int m, n, mean = 0;
    int i = 0;
    float sum;

    line *m_ptr = matrix->pt;
    int* ptr;

    int* ptrl = a->arr;


    for (int j = 0; j < matrix->m; j++) {

        ptr = (m_ptr)->arr;

        m = matrix->m;
        n = (m_ptr)->n;
        sum = 0;

        for (i = 0; i < n; i++) {
            sum += (float)*(ptr++);
             //printf_s("123    %f\n", sum);
        }

        sum /= n;
         printf_s("%f\n", sum);
        ptr = m_ptr->arr;

        mean = 0;
        for (i = 0; i < n; ++i) {
            if (((float)*(ptr)) > sum)
                mean += *(ptr);

            ++ptr;
        }
        
        *(ptrl++) = mean;

        m_ptr++;
    }
};

void destroym(struct matrix* matrix, int j) {
    line* m_ptr = matrix->pt;
    int* ptr;

    for (int i = 0; i < j; i++) {
        ptr = m_ptr->arr;
        free(ptr);
        m_ptr++;
    }

        
}

void destroyl(line* a) {
    int* ptr = a->arr;
    free(ptr);
}

int main(){
    int m;
    struct matrix matrix;
    struct line a;
    int h = 0;

    srand(SEED);

    printf_s("Input quatity of rows\n");
    input(&matrix); 

    printf_s("It's your matrix: \n");
    printm(&matrix);

    int l = 0;
    
    l = matrix.m;

    a.arr = (int)calloc(l, sizeof(int));

    armean(&matrix, &a);
     

    printf_s("It's your vector:\n");
    printl(&a, l);

    destroym(&matrix, matrix.m);
    destroyl(&a);

    return 0;
}

