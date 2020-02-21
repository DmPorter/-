#include<stdio.h>
#include<locale.h>

#define MAXL 1000

int getInt(int* a);
void input(int n, int a[]);
void out(int n, int a[]);
void  get(int n, int a[], int b[], int kv[]);
int  obr(int ch, int* sum1);
void sort(int n, int a[]);

int main() {
	setlocale(LC_ALL, "Rus");
	int a[MAXL], b[MAXL], kv[MAXL];
	printf_s("Доброго времени суток, введите количество членов последовательности (не более %d)\n", MAXL-1);
	int n;
	do {
		getInt(&n);
	if (n >= MAXL)
		printf_s("Error! Введите число меньше %d ", MAXL - 1, "\n");
	} while (n >= MAXL);
	printf_s("Введите члены последовательности \n");
	input(n, a);
	get(n, a, b, kv);
	printf_s("Последовательность количества цифр в числе: \n");
	out(n, kv);
	printf_s("Последоветельность сумм произведений четных и нечетныз мест: \n");
	out(n, b);
	sort(n, kv);
	printf_s("Отсортированная последовательность количества цифр в числе: \n");
	out(n, kv);
	sort(n, b);
	printf_s("Последоветельность сумм произведений четных и нечетныз мест: \n");
	out(n, b);
	return 0;
}


int getInt(int* a)
{
	int n;
	do
	{
		n = scanf_s("%d", a);
		if ((n == 0) | (*a <= 0))
		{
			printf_s("Error!\n");
			scanf_s("%*[^\n]");
		}
	} while ((n == 0) | (*a <= 0));
	return n < 0 ? 0 : 1;

}

void input(int n, int a[])
{
	int i;
	for (i = 0; i < n; ++i)
	{
		getInt(&a[i]);
	}

}

void out(int n, int a[])
{
	int i;
	for (i = 0; i < n; ++i)
	printf_s("%d\t", a[i]);
	printf_s("\n");
}

void  get(int n, int a[], int b[], int kv[]) {
	int i, s3 = 0;
	for (i = 0; i < n; ++i) {
		kv[i] = obr(a[i], &s3);
		b[i] = s3;
		
	}
	printf_s("\n");
}

int  obr(int ch, int* sum1) {
	int d = 0, p = 0, s2, s1;
	s2 = 0;
	s1 = 0;
	do {
		d = ch % 10;
		ch /= 10;
		++p;
		if ((p & 1) == 1) {
			s1 += d;
		}
		else s2 += d;

	} while (ch > 0);
	*sum1 = s1 * s2;
	return p;
}

void sort(int n, int a[]) {
	int tmp;
	_Bool noSwap;

	for (int i = n - 1; i >= 0; i--)
	{
		noSwap = 1;
		for (int j = 0; j < i; j++)
		{
			if (a[j] > a[j + 1])
			{
				tmp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = tmp;
				noSwap = 0;
			}
		}
		if (noSwap == 1)
			break;
	}
}
