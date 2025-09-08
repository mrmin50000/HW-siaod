#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>

int main() {

	auto start = std::chrono::high_resolution_clock::now();

	//Input
	std::ofstream inFile("input.txt");
	
	if (inFile.is_open()) {
		for (int i = 9999999; i >= 0; --i)
			inFile << i << '\n';
		inFile.close();
	}
	else 
		std::cout << "Error opening file\n";
	int n = 10000000;
	
	std::ifstream outFile("input.txt");
	std::vector<int> arr;
	std::vector<unsigned char> bitarr;

	if (outFile.is_open()) {
		std::string line;
		while (getline(outFile, line)) {
			arr.push_back(stoi(line));
		}
		outFile.close();
	}
	else
		std::cout << "Error opening file\n";

	for (int i = 0; i < n / 8; ++i)
		bitarr.push_back(0);
	//Sort
	unsigned char mask = 1 << 7;
	for (int i = 0; i < n; ++i) {
		int byte_index = arr[i] / 8;
		int bit_index = arr[i] % 8;
		bitarr[byte_index] = bitarr[byte_index] | (mask >> bit_index);
	}
	
	//Sorted sequence output:
	std::ofstream InFile("output.txt");
	for (int i = 0; i < n; ++i) {
		int bit_index = i % 8;
		int byte_index = i / 8;
		if (bitarr[byte_index] & (mask >> bit_index)) {
			InFile << i << '\n';
		}
        }

	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> total = end - start;
	std::cout << "Total time: " << total.count() << "sec\n";
	std::cout << "RAM volume: " << bitarr.size() / 1024 << "Kb\n";

	return 0;
}
