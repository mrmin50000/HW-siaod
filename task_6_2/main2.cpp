#include <iostream>
#include <vector>
#include <string>

std::vector<int> prefixFunction(std::string &pattern) {
	int n = pattern.length();
	std::vector<int> res(n, 0);
	int k = 0;

	for (int i = 1; i < n; ++i) {
		while (k > 0 && pattern[k] != pattern[i]) {
			k = res[k - 1];
		}
		if (pattern[k] == pattern[i]) {
			++k;
		}
		res[i] = k;
	}
	return res;
}

int KMP(std::string &text, std::string &pattern) {
	if (pattern.empty())
		return -1;

	int n = text.length();
	int m = pattern.length();
	if (m > n)
		return -1;

	std::vector<int> pi = prefixFunction(pattern);
	int k = 0;
	int lastOccurrence = -1;

	for (int i = 0; i < n; ++i) {
		while (k > 0 && pattern[k] != text[i]) {
			k = pi[k - 1];
		}
		if (pattern[k] == text[i]) {
			++k;
		}
		if (k == m) {
			lastOccurrence = i - m + 1;
			k = pi[k - 1];
		}
	}
	return lastOccurrence;
}

int main() {
    std::string text, pattern;

    std::cout << "Text: ";
    std::getline(std::cin, text);

    std::cout << "Pattern: ";
    std::getline(std::cin, pattern);

    int result = KMP(text, pattern);

    if (result != -1) {
        std::cout << "Index of last: " << result << '\n';
    } else {
        std::cout << "not found\n";
    }

    return 0;
}
