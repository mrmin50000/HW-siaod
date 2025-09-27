#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> split(std::string &s) {
	std::vector<std::string> res;
	std::string word;
	for (int i = 0; i < s.length(); ++i) {
		if (s[i] != ' ')
			word += s[i];
		else if (word != "") {
			res.push_back(word);
			word = "";
		}
	}
	if (word != "") {
		res.push_back(word);
	}
	return res;
}

bool identicalCharacters(std::string &word) {
	if (word.empty())
		return false;
	return tolower(word[0]) == tolower(word[word.length() - 1]);
}

std::pair<std::string, int> findMax(std::vector<std::string> &sentence) {
	int maxLength = 0;
	std::string maxWord;
	for (int i = 0; i < sentence.size(); ++i) {
		if (identicalCharacters(sentence[i]) && sentence[i].length() > maxLength) {
			maxLength = sentence[i].length();
			maxWord = sentence[i];
		}
	}

	return std::pair(maxWord, maxLength);
}

int main() {
	std::string sentence;
	std::cout << "Enter a sentence: ";
	std::getline(std::cin, sentence);
	std::vector<std::string> arr = split(sentence);
	std::pair<std::string, int> answer = findMax(arr);
	if (answer.first.empty()) {
		std::cout << "Not found\n";
	}
	else {
		std::cout << "Word: " << answer.first << "\nLength: " << answer.second << '\n';
	}
	return 0;
}
