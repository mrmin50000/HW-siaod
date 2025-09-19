#include <algorithm>
#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <endian.h>
#include <ios>
#include <utility>
#include <chrono>

int genRandNumber(int min, int max) {
	std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(min, max);
	return dist(gen);
}

std::string genRandString(int len) {
	std::string str;
	static const std::string alphabet = "ABCDRFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	for (int i = 0; i < len; ++i) {
		str += alphabet[genRandNumber(0, alphabet.length() - 1)];
	}
	return str;
}

std::string readFromOffset(int offset, std::ifstream &file) {
	file.seekg(offset);
	char a[62];
	file.read(a, 62);
	std::string res = a;
	return res;
}

std::string getFioAndAddress(std::vector<std::pair<int, int>> table, int billNumber, std::ifstream &file) {
	int left = 0, right = table.size(), mid;
	while (left <= right) {
		mid = (left + right) / 2;
		if (table[mid].first == billNumber)
			return readFromOffset(table[mid].second, file);
		else if (table[mid].first > billNumber)
			right = mid - 1;
		else
			left = mid + 1;
	}	
	return "not found";
}


int main() {
	std::string result;
	std::ofstream inputFile("file.bin", std::ios::binary);
	int billNumberInput, n, offset, yo;
	std::vector<std::pair<int, int>> table;
	std::cin >> n;
	std::cin >> yo;
	for (int i = 0; i < n; ++i) {
		if (i == n / 8)
			billNumberInput = htobe32(8888888);
		else
			billNumberInput = htobe32(genRandNumber(1000000, 9999999));
		std::string fioInput = genRandString(20);
		std::string adressInput = genRandString(40);
		result = fioInput + ' ' + adressInput + '\n';
		inputFile.write((char *)&billNumberInput, sizeof(int));
		inputFile.write(result.c_str(), result.length());
	}
	inputFile.close();
	
	std::ifstream outputFile("file.bin", std::ios::binary);
	int billNumber = 0;
	char data[62];
	while (outputFile.read((char *)&billNumber, sizeof(int))) {
		int i = be32toh(billNumber);
		offset = outputFile.tellg();
		outputFile.read(data, 62);
		table.push_back(std::pair(i, offset));
	}
	outputFile.clear();

	auto start = std::chrono::high_resolution_clock::now();
	std::sort(table.begin(), table.end());
	std::cout << getFioAndAddress(table, yo, outputFile) << '\n';
	outputFile.close();

	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> total = end - start;
	std::cout << "Total time: " << total.count() << "sec\n";

	return 0;
}
