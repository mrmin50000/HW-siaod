#include <algorithm>
#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <endian.h>
#include <ios>
#include <utility>
#include <chrono>
#include <unordered_map>

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

std::string getFioAndAddress(std::unordered_map<int, std::pair<int, std::string>> map, int billNumber) {

	int left = 0, right = map.size(), mid;
	while (left <= right) {
		mid = (left + right) / 2;
		if (map[mid].first == billNumber)
			return map[mid].second;
		else if (map[mid].first > billNumber)
			right = mid - 1;
		else
			left = mid + 1;
	}	
	return "not found";
}

int main() {
	std::string result;
	std::ofstream inputFile("file.bin", std::ios::binary);
	int billNumberInput, n;
	std::unordered_map<int, std::pair<int, std::string>> map;
	std::vector<std::pair<int, std::string>> forMap;
	std::cin >> n;
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
	
	auto start = std::chrono::high_resolution_clock::now();
	std::ifstream outputFile("file.bin", std::ios::binary);
	int billNumber;
	std::string data;
	while (outputFile.read((char *)&billNumber, sizeof(int))) {
		int i = be32toh(billNumber);
		std::getline(outputFile, data);
		forMap.push_back(std::pair(i, data));
	}
	outputFile.close();
	
	std::sort(forMap.begin(), forMap.end());
	for (int i = 0;i < forMap.size(); ++i) {
		map[i] = forMap[i];
	}

	std::cout << getFioAndAddress(map, 8888888) << '\n';

	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> total = end - start;
	std::cout << "Total time: " << total.count() << "sec\n";

	return 0;
}
