#include <iostream>
#include "lab1.h"
namespace lab1 {

	struct Matrix* input()
	{
		const char* pr = "";
		//struct Matrix Matr = new struct Martix;
		//struct Matrix buff;
		//Matrix Matr = new Matrix(0, 0, nullptr);
		//auto* Matr = new Matrix[]; 
		Matrix buff = Matrix();

		unsigned int cntEl;
		do {
			std::cout << pr << std::endl;
			std::cout << "Enter number of lines: --> ";
			pr = "You are wrong; repeat please!";
			if (getNum(Matr->m) < 0)
				return nullptr;
		} while (Matr->m < 1);

		do {
			std::cout << pr << std::endl;
			std::cout << "Enter number of items: --> ";
			pr = "You are wrong; repeat please!";
			if (getNum(Matr->n) < 0)
				return nullptr;
		} while (Matr->n < 1);

		do {
			std::cout << pr << std::endl;
			std::cout << "How many elements do you want to enter? --> ";
			pr = "You are wrong; repeat please!";
			if (getNum(cntEl) < 0)
				return nullptr;
		} while (Matr->m < 1);
		try{
			Matr->a = new Item[cntEl];
		}
		catch (std::bad_alloc & ba) {

		}
		for (int l = 0; l < cntEl; l++)
		{
			do {
				std::cout << pr << std::endl;
				std::cout << "i --> ";
				pr = "You are wrong; repeat please!";
				if (getNum(Matr->a[l].i) < 0)
					return nullptr;
			} while (Matr->m < 1);

			do {
				std::cout << pr << std::endl;
				std::cout << "j --> ";
				pr = "You are wrong; repeat please!";
				if (getNum(Matr->a[l].j) < 0)
					return nullptr;
			} while (Matr->m < 1);

			do {
				std::cout << pr << std::endl;
				std::cout << "a --> ";
				pr = "You are wrong; repeat please!";
				if (getNum(Matr->a[l].a) < 0)
					return nullptr;
			} while (Matr->m < 1);
		}
		return Matr;
	}
	
}

namespace lab1 {
	struct Item {
		int i;
		int j;
		double* a;
	};

	struct Martix {
		int n;
		int m;
		Item* a;
	};
	template <class T>
	int getNum(T& a)
	{
		std::cin >> a;
		if (!std::cin.good())
			return -1;
		return 1;
	}

	int comp(double a) {
		if (a > 0)
			return 1;
		else return 0;
	}
	inline int isgreater(double a, double b)
	{
		return a > b;
	}
	inline int isless(double a, double b)
	{
		return a < b;
	}
	struct Matrix* input();
}
