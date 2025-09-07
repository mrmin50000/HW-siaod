#include <iostream>
#include <bitset>

int main() {
	unsigned int x = 25;
	const int n = sizeof(int)*8; //=32 - количество разрядов в числе типа int
	unsigned maska = (1 << n - 1); //1 в старщем бите 32-разрядной сетки
	std::cout << "Начальный вид маски: " << std::bitset<n> (maska) << '\n';
	std::cout << "Результат: ";
	for (int i = 1; i <= n; ++i) { //32 раза - по количеству разрядов
		std::cout << ((x & maska) >> (n - i)); 
		maska = maska >> 1; //смещение 1 в маске на разряд вправо
	}
	std::cout << '\n';
	return 0;
}
