#include <bitset>
#include <iostream>
#include <vector>

int main() {
	//Input
	int n;
	std::cout << "Type size of array (0-64): ";
	std::cin >> n;
	if (n > 64) {
		std::cout << "I told u..\n";
		return 0;
	}

	int arr[n];
	for (int i = 0; i < n; ++i) {
		std::cout << "Type array's element (0-63): ";
		std::cin >> arr[i];
		if (arr[i] > 63 || arr[i] < 0) {
			std::cout << "I told u..\n";
			return 0;
		}
	}
	
	//Sort
	std::vector<unsigned char> bitarr = {0, 0, 0, 0, 0, 0, 0, 0};
	unsigned char mask = 1 << 7;
	for (int i = 0; i < n; ++i) {
		int byte_index = arr[i] / 8;
		int bit_index = arr[i] % 8;
		bitarr[byte_index] = bitarr[byte_index] | (mask >> bit_index);
		std::cout << "Sorting process: ";
		for (int j = 0; j < 8; ++j) {
			std::cout << std::bitset<8> (bitarr[j]);
		}
		std::cout << '\n';
	}
	
	//Sorted sequence output:
	std::cout << "Sorted array: ";
	for (int i = 0; i < 64; ++i) {
		int bit_index = i % 8;
		int byte_index = i / 8;
		if (bitarr[byte_index] & (mask >> bit_index))
			std::cout << i << ' ';
	}
	std::cout << '\n';

	return 0;
}
