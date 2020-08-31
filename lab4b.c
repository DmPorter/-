#define _CRT_SECURE_NO_WARNINGS
#define SIZE 5
#define RAND 100
#include <stdio.h>
#include <string.h>	
const char* msgs[] = { NULL, "Exit programm", "Generate random table", "Input item", "Delete releases", "Print table", "Find key", "Free table" };

typedef struct Item {
    int key1;     //ключ элемента из 1 - го пространства ключей;
    int key2;     //ключ элемента из 2 - го пространства ключей;
    int offset;
    int len;
}Item;

typedef struct ARR {
    int busy;     //состояние элемента
    Item* arr;    
}ARR;
//структура - таблица
typedef struct Table {
    int size;     //максимальный размер таблицы
    int count;    //текущий размер таблицы
    ARR* arr1;    //указатель на 1 массив ARR
    ARR* arr2;    //указатель на 2 массив ARR
    FILE* fd;
}Table;

void F0_Menu();
int F1_End(Table*, Item*);
int F2_Generate(Table*, Item*);
int F3_Input(Table*, Item*);
int F4_DeleteRelease(Table*, Item*);
int F5_Print(Table*, Item*);
int F6_FindKey(Table*, int*);
int F7_FreeTable(Table*, Item*);


Table* TableCreate(int);
void TableDelete(Table*, Item*);
int TablePrint(Table*, Item*);
int hash(int, int);
void DelItem(Item*);
int ItemPrint(Item*);
int CheckTable(Table*);
int CheckTableFree(Table*);
void DeleteK(Table*, Item*, int, int);
int D_Load(Table*, Item*);
int load(Table*, char*, Item*);
int create(Table*, char* fname, int);
int save(Table*);
char* findInfo1(Table*, int);
char* findInfo2(Table*, int);

void getInt(int*);
char* getstr();


int main() {
    int  menu = -1, ex = 0, flag = 0;
    int(*fun1[]) (Table*, Item*) = { NULL, F1_End, F2_Generate, F3_Input, F4_DeleteRelease, F5_Print, F6_FindKey, F7_FreeTable };
    //Удаленный элемент
    Item* delitem = (Item*)calloc(1, sizeof(Item));
    //Выделяю память под таблицу
    Table* T = TableCreate(SIZE, delitem);
    F0_Menu();
    //выбор пункта меню
    while (ex != 1) {
        menu = -1;
        flag = 0;
        while (menu <= -1 || menu >= 8) {
            if (flag == 1)
                printf("Error!\n");
            printf("Selected action (0 - Menu):");
            getInt(&menu);
            flag = 1;
        }
        if (menu == 0)
            F0_Menu();
        else {
            ex = (*fun1[menu])(T, delitem);
        }
    }
    return 0;
}
void F0_Menu()
{
    printf("\n      MENU\n");
    int i;
    for (i = 1; i < 8; ++i)
        printf("[%d] %s\n", i, msgs[i]);

}int F1_End(Table* T, Item* delitem)
{
    save(T, delitem);
    F7_FreeTable(T, delitem);
    TableDelete(T, delitem);
    return 1;
}

int F2_Generate(Table* T, Item* delitem)
{
    srand(RAND);
    int key1, key2, n, flag1 = 0, j = 0, t, k2, k1;
    char* info;
    int m;
    if (CheckTable(T) == 1)
        return 0;
    printf("\nMax size of table:[%d]", SIZE);
    do {
        printf("\nHow many elements do you want to add?\nEnter:");
        getInt(&n);
        if (n == 0)
            return 0;
        if (n > SIZE || n < 0)
            printf("Error!\n");
        else
            if (n > (SIZE - T->count)) {
                printf("You can only add [%d] characters. Do it?\nYes[1] or No[2]\n", (SIZE - T->count));
                do {
                    printf("Enter:");
                    getInt(&m);
                } while (m <= 1 && m >= 2);
                if (m == 1)
                    n = SIZE - T->count;
                else
                    return 0;
            }
    } while (n > SIZE || n < 0);
    for (int i = 0; i < n; i++) {
        Item* I = (Item*)calloc(1, sizeof(Item));
        info = (char*)calloc(4, sizeof(char));
        for (j = 0; j < 3; j++) {
            info[j] = '0' + rand() % 10;
        }
        j = 0;
        info[3] = '\0';
        //key1
        do {
            key1 = rand() % 1000;
            flag1 = 0;
            k1 = hash(key1, SIZE);
            while (T->arr1[hash(k1, SIZE)].arr != NULL) {
                if ((k1 + 1) >= T->count)
                    break;
                if (T->arr1[hash(k1, SIZE)].arr->key1 == key1) {
                    flag1 = 1;
                }
                k1++;
            }
        } while (flag1 == 1);
        //key2
        do {
            key2 = rand() % 1000;
            flag1 = 0;
            k2 = hash(key2, SIZE);
            while (T->arr2[hash(k2, SIZE)].arr != NULL) {
                if ((k2 + 1) >= T->count)
                    break;
                if (T->arr2[hash(k2, SIZE)].arr->key2 == key2) {
                    flag1 = 1;
                }
                k2++;
            }
        } while (flag1 == 1);
        //Запись информации в таблицу
        I->key1 = key1;
        I->key2 = key2;
        I->len = strlen(info) + 1;
        fseek(T->fd, 0, SEEK_END);
        I->offset = ftell(T->fd);
        fwrite(info, sizeof(char), I->len, T->fd);
        T->count++;
        //Заполнение таблицы
        while (T->arr1[hash(k1, SIZE)].arr != NULL) {
            if (T->arr1[hash(k1, SIZE)].arr == delitem)
                break;
            k1 = k1++;
        }
        T->arr1[hash(k1, SIZE)].arr = I;
        T->arr1[hash(k1, SIZE)].busy = 1;
        j = 0;
        while (T->arr2[hash(k2, SIZE)].arr != NULL) {
            if (T->arr2[hash(k2, SIZE)].arr == delitem)
                break;
            k2 = k2 + 3;
        }
        T->arr2[hash(k2, SIZE)].arr = I;
        T->arr2[hash(k2, SIZE)].busy = 1;
    }
    printf("Success. Added [%d] elements.\n", n);
    T->count = n;
    return 0;
}


int F3_Input(Table* T, Item* delitem)
{
    int key1, key2, n, flag1 = 0, j = 0, k1, k2, m;
    char* info;
    if (CheckTable(T) == 1)
        return 0;
    printf("\nMax size of table:[%d]", SIZE);
    do {
        printf("\nHow many elements do you want to add?\nEnter:");
        getInt(&n);
        if (n == 0)
            return 0;
        if (n > SIZE || n < 0)
            printf("Error!\n");
        else
            if (n > (SIZE - T->count)) {
                printf("You can only add [%d] characters. Do it?\nYes[1]    No[2]\n", (SIZE - T->count));
                do {
                    printf("Enter:");
                    getInt(&m);
                } while (m <= 1 && m >= 2);
                if (m == 1)
                    n = SIZE - T->count;
                else
                    return 0;
            }
    } while (n > SIZE || n < 0);
    for (int i = 0; i < n; i++) {
        Item* I = (Item*)calloc(1, sizeof(Item));
        //info
        printf("Enter information:");
        info = getstr();
        //key1
        flag1 = 0;
        do {
            printf("Enter key1:");
            getInt(&key1);
            flag1 = 0;
            k1 = hash(key1, SIZE);
            while (T->arr1[hash(k1, SIZE)].arr != NULL) {
                if (T->arr1[hash(k1, SIZE)].arr == delitem)
                    break;
                if (T->arr1[hash(k1, SIZE)].arr->key1 == key1) {
                    flag1 = 1;
                    printf("The key is busy.\n");
                }
                k1++;
            }
        } while (flag1 == 1);

        //key2
        do {
            printf("Enter key2:");
            getInt(&key2);
            flag1 = 0;
            k2 = hash(key2, SIZE);
            while (T->arr2[hash(k2, SIZE)].arr != NULL) {
                if (T->arr2[hash(k2, SIZE)].arr == delitem)
                    break;
                if (T->arr2[hash(k2, SIZE)].arr->key2 == key2) {
                    flag1 = 1;
                    printf("The key is busy.\n");
                }
                k2 += 3;
            }
        } while (flag1 == 1);
        //Запись информации в таблицу
        I->key1 = key1;
        I->key2 = key2;
        I->len = strlen(info) + 1;
        fseek(T->fd, 0, SEEK_END);
        I->offset = ftell(T->fd);
        fwrite(info, sizeof(char), I->len, T->fd);
        T->count++;
        while (T->arr1[hash(key1, SIZE)].arr != delitem && T->arr1[hash(k1, SIZE)].arr != NULL)
            k1 = k1++;
        T->arr1[hash(k1, SIZE)].arr = I;
        T->arr1[hash(k1, SIZE)].busy = 1;
        j = 0;
        while (T->arr2[hash(k2, SIZE)].arr != delitem && T->arr2[hash(k2, SIZE)].arr != NULL)
            k2 = k2 + 3;
        T->arr2[hash(k2, SIZE)].arr = I;
        T->arr2[hash(k2, SIZE)].busy = 1;
    }
    printf("Success. Added [%d] elements.\n", n);
    return 0;
}

int F4_DeleteRelease(Table* T, Item* delitem)
{
    int i = 0, res, key2, key, key1;
    if (CheckTableFree(T) == 1)
        return 0;
    printf("\nkey1[1].\nkey2[2].\n");
    do {
        printf("Enter the number (1 or 2):");
        getInt(&key);
    } while (key > 2 || key < 1);
    if (key == 1) {
        key1 = FKey1(T, 1, 0);
        if (key1 == -1) {
            printf("Key is not found.\n");
            return 0;
        }
        key2 = FKey2(T, 0, T->arr1[hash(key1, SIZE)].arr->key2);
    }
    else {
        key2 = FKey2(T, 1, 0);
        if (key2 == -1) {
            printf("Key is not found.\n");
            return 0;
        }
        key1 = FKey1(T, 0, T->arr2[hash(key2, SIZE)].arr->key1);
    }
    DeleteK(T, delitem, key1, key2);
    T->count--;
    printf("The Item was deleted.\n");
    return 0;
}

int F5_Print(Table* T, Item* delitem)
{
    TablePrint(T, delitem);
    return 0;
}

int F6_FindKey(Table* T, int* a)
{
    int i = 0, key, j = 0, k, key2, key1, flagn = 0, choice;
    char* info;
    if (CheckTableFree(T) == 1)
        return 0;
    Item* CopyI = (Item*)malloc(sizeof(Item));
    printf("\nkey1[1].\nkey2[2].\n");
    do {
        printf("Enter the number (1 or 2):");
        getInt(&key);
    } while (key > 2 || key < 1);
    if (key == 1) {
        key1 = FKey1(T, 1, 0);
        if (key1 == -1) {
            printf("Key is not found.\n");
            return 0;
        }
        CopyI->key1 = T->arr1[hash(key1, SIZE)].arr->key1;
        CopyI->key2 = T->arr1[hash(key1, SIZE)].arr->key2;
        CopyI->len = T->arr1[hash(key1, SIZE)].arr->len;
        CopyI->offset = T->arr1[hash(key1, SIZE)].arr->offset;
        info = findInfo1(T, key1);
    }
    else
    {
        key2 = FKey2(T, 1, 0);
        if (key2 == -1) {
            printf("Key is not found.\n");
            return 0;
        }
        CopyI->key1 = T->arr2[hash(key2, SIZE)].arr->key1;
        CopyI->key2 = T->arr2[hash(key2, SIZE)].arr->key2;
        CopyI->len = T->arr2[hash(key2, SIZE)].arr->len;
        CopyI->offset = T->arr2[hash(key2, SIZE)].arr->offset;
        info = findInfo2(T, key2);
    }
    printf("Key1:%d\nKey2:%d\nInfo:%s\n", CopyI->key1, CopyI->key2, info);
    free(CopyI);
    return 0;
}

int F7_FreeTable(Table* T, Item* delitem)
{
    int i = 0, j = 0, key2, key;
    if (CheckTableFree(T) == 1)
        return 0;
    for (i = 0; i < T->size; i++) {
        T->arr1[i].arr = NULL;
        T->arr1[i].busy = 0;
    }
    for (i = 0; i < T->size; i++) {
        T->arr2[i].arr = NULL;
        T->arr2[i].busy = 0;
    }
    printf("The table was cleared. Cleared [%d] elements.\n", i);
    T->count = 0;
    return 0;
}

void getInt(int* a)
{
    int n = 0, flag = 0;
    do
    {
        if (flag == 1) {
            printf("Error!\nRepeat:");
            scanf_s("%*[^\n]");
        }
        n = scanf("%d", a);
        if (*a == -1)
            n = -1;
        flag = 1;
    } while (n <= 0);
    scanf("%*c");
}

char* getstr()
{
    char* ptr = (char*)malloc(1);
    char buf[81];
    int n, len = 0;
    *ptr = '\0';
    do {
        n = scanf("%80[^\n]", buf); // n = scanf_s ("%80[^\n]", buf, 81);
        if (n < 0) {
            free(ptr);
            ptr = NULL;
            continue;
        }
        if (n == 0)
            scanf("%*c");
        else {
            len += strlen(buf);
            ptr = (char*)realloc(ptr, len + 1);
            strcat(ptr, buf);
        }
    } while (n > 0);
    return ptr;
}

Table* TableCreate(int size, Item* delitem)
{
    Table* T;
    if (size <= 0)
        return NULL;
    //выделяем память под структуру таблицы
    T = (Table*)calloc(1, sizeof(Table));
    if (T == NULL)
        return NULL;
    if (D_Load(T, delitem) == 0)
        return 1;
    if (T->arr2 == NULL && T->arr1 == NULL)
        return NULL;
    return T;
}

void DeleteK(Table* T, Item* delitem, int key1, int key2) {
    T->arr1[hash(key1, SIZE)].arr = delitem;
    T->arr1[hash(key1, SIZE)].busy = -1;
    T->arr2[hash(key2, SIZE)].arr = delitem;
    T->arr2[hash(key2, SIZE)].busy = -1;
}

void TableDelete(Table* T, Item* delitem)
{
    int i = 0;
    if (T->count == 0) {
        free(T->arr1);
        free(T->arr2);
        free(T);
    }
    free(delitem);
    printf("Table was deleted.");
}

int hash(int key, int size)
{
    return key = key % size;
};

int CheckTableFree(Table* T)
{
    if (T->count == 0) {
        puts("\nTable is free.");
        return 1;
    }
    return 0;
}

int FKey1(Table* T, int a, int k) {
    int key, key1, n = 0;
    if (a == 1) {
        printf("\nEnter key:");
        getInt(&key);
    }
    else
    {
        key = k;
    }

    int p = hash(key, SIZE);
    while ((n < SIZE) && (T->arr1[hash(p, SIZE)].busy != 0)) {
        if ((T->arr1[hash(p, SIZE)].arr != NULL) && (T->arr1[hash(p, SIZE)].arr->key1 == key)) {
            if (T->arr1[hash(p, SIZE)].busy == 1)
                return p;
        }
        p++;
        n++;
    }
    return -1;
}

int FKey2(Table* T, int a, int k) {
    int key, key2, n = 0;
    if (a == 1) {
        printf("\nEnter key:");
        getInt(&key);
    }
    else
    {
        key = k;
    }
    int p = hash(key, SIZE);
    while ((n < SIZE) && (T->arr2[hash(p, SIZE)].busy != 0)) {
        if ((T->arr2[hash(p, SIZE)].arr != NULL) && (T->arr2[hash(p, SIZE)].arr->key2 == key)) {
            if (T->arr2[hash(p, SIZE)].busy == 1)
                return p;
        }
        p += 3;
        n++;
    }
    return -1;
}

int CheckTable(Table* T)
{
    if (T->count == SIZE) {
        printf("Table is full.\n");
        return 1;
    }
    return 0;
}
int TablePrint(Table* T, Item* delitem)
{
    int i = 0, key;
    printf("\nkey1[1].\nkey2[2].\n");
    do {
        printf("Enter the number (1 or 2):");
        getInt(&key);
    } while (key > 2 || key < 1);
    puts("\nTable:");
    printf("count = %d, size = %d.\n", T->count, T->size);
    if (key == 1) {
        printf("%2s %5s %5s %8s %3s\n", "i", "key1", "key2", "info", "busy1");
        for (i = 0; i < SIZE; i++) {     //проходим по массиву
            printf("%2d", i);          //выводим очередной элемент
            if (T->arr1[i].arr != NULL && T->arr1[i].arr != delitem) {
                printf("%5d %5d  %8s %3d\n", T->arr1[i].arr->key1, T->arr1[i].arr->key2, findInfo1(T, i), T->arr1[i].busy);
            }
            else {
                printf("%5d %5d  (null) %3d\n", delitem->key1, delitem->key2, T->arr1[i].busy);
            }
        }
    }
    else {
        printf("%2s %5s %5s %8s %3s\n", "i", "key1", "key2", "info", "busy2");
        for (i = 0; i < SIZE; i++) {     //проходим по массиву
            printf("%2d", i);          //выводим очередной элемент
            if (T->arr2[i].arr != NULL && T->arr2[i].arr != delitem) {
                printf("%5d %5d  %8s %3d\n", T->arr2[i].arr->key1, T->arr2[i].arr->key2, findInfo2(T, i), T->arr2[i].busy);
            }
            else {
                printf("%5d %5d  (null) %3d\n", delitem->key1, delitem->key2, T->arr2[i].busy);
            }
        }
    }
    return 0;
}
int D_Load(Table* T, Item* delitem)
{
    int SZ;
    char* fname = NULL;
    printf("Enter file name:");
    fname = getstr();
    if (fname == NULL)
        return 0;
    //если такого файла нет, то создаем таблицу
    if (load(T, fname, delitem) == 0) {
        printf("File wasn't found.");
        create(T, fname, SIZE);
    }

    return 1;
}
//считываем информацию из файла(если он существует)    +                  
int load(Table* T, char* fname, Item* delitem)
{
    int k2, k1, a;
    Item* I;
    fopen_s(&(T->fd), fname, "r+b");
    if (T->fd == NULL)
        return 0;
    fread(&T->size, sizeof(int), 1, T->fd);
    T->arr1 = (ARR*)calloc(T->size, sizeof(ARR));
    T->arr2 = (ARR*)calloc(T->size, sizeof(ARR));
    fread(&T->count, sizeof(int), 1, T->fd);
    for (int i = 0; i < T->count; i++) {
        I = (Item*)calloc(1, sizeof(Item));
        fread(I, sizeof(Item), 1, T->fd);
        
        k1 = I->key1;
        a = 0;
        while(T->arr1[hash(k1,SIZE)].arr != NULL && a < T->size)
        {
            k1++;
            a++;
        }
        T->arr1[hash(k1, SIZE)].arr = I;
        T->arr1[hash(k1, SIZE)].busy = 1;

        a = 0;

        k2 = I->key2;

        while (T->arr2[hash(k2, SIZE)].arr != NULL && a < T->size)
        {
            k2 += 3;
            a++;
        }
        T->arr2[hash(k2, SIZE)].arr = I;
        T->arr2[hash(k2, SIZE)].busy = 1;
    }
    printf("File was found.");
    return 1;
}


int create(Table* T, char* fname, int sz)
{
    T->size = sz;
    T->count = 0;
    if (fopen_s(&(T->fd), fname, "w+b") != 0) {
        T->arr1 = NULL;
        return 0;
    }
    T->arr1 = (ARR*)calloc(T->size, sizeof(ARR));
    T->arr2 = (ARR*)calloc(T->size, sizeof(ARR));
    if (T->arr2 == NULL && T->arr1 == NULL)
        return 1;
    fwrite(&T->size, sizeof(int), 1, T->fd);
    fwrite(&T->count, sizeof(int), 1, T->fd);
    fwrite(T->arr1, sizeof(Item), T->size, T->fd);
    return 1;
}
//сохранение таблицы                                   +
int save(Table* T, Item* delitem)
{
    fseek(T->fd, sizeof(int), SEEK_SET);
    fwrite(&T->count, sizeof(int), 1, T->fd);
    for (int i = 0; i < T->size; i++)
        if (T->arr1[i].arr != NULL && T->arr1[i].arr != delitem)
        fwrite(T->arr1[i].arr, sizeof(Item), 1, T->fd);
    fclose(T->fd);
    T->fd = NULL;
    printf("Table was saved.");
    return 1;
}

char* findInfo2(Table* T, int i)
{
    char* info = NULL;
    if (i >= 0) {
        info = (char*)malloc(T->arr2[hash(i, SIZE)].arr->len);
        fseek(T->fd, T->arr2[hash(i, SIZE)].arr->offset, SEEK_SET);
        fread(info, sizeof(char), T->arr2[hash(i, SIZE)].arr->len, T->fd);
    }
    return info;
}

char* findInfo1(Table* T, int i)
{
    char* info = NULL;
    if (i >= 0) {
        info = (char*)malloc(T->arr1[hash(i, SIZE)].arr->len);
        fseek(T->fd, T->arr1[hash(i, SIZE)].arr->offset, SEEK_SET);
        fread(info, sizeof(char), T->arr1[hash(i, SIZE)].arr->len, T->fd);
    }
    return info;
}
