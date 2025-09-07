#include <iostream>
#include <bitset>

int main() {
	unsigned char x = 222;
	std::cout << "Previous x: " << std::bitset<8> (x);
	unsigned char maska = 1;
	x = x & (~(maska<<4));
	std::cout << "\nCurrent x:  " << std::bitset<8> (x) <<"\nResult: " << (unsigned int) x << '\n';
}
