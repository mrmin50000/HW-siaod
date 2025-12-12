#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <sstream>
#include <cctype>

using namespace std;

// Helper: Split UTF-8 string into individual UTF-8 character strings
vector<string> utf8_chars(const string& s) {
    vector<string> chars;
    for (size_t i = 0; i < s.size(); ) {
        unsigned char c = static_cast<unsigned char>(s[i]);
        size_t len;
        if ((c & 0x80) == 0) {
            len = 1; // ASCII
        } else if ((c & 0xE0) == 0xC0) {
            len = 2;
        } else if ((c & 0xF0) == 0xE0) {
            len = 3;
        } else if ((c & 0xF8) == 0xF0) {
            len = 4;
        } else {
            len = 1; // Invalid UTF-8: skip 1 byte
        }

        if (i + len > s.size()) {
            len = 1;
        }

        chars.push_back(s.substr(i, len));
        i += len;
    }
    return chars;
}

// Symbol now stores a full UTF-8 character as string
struct Symbol {
    string character;   // UTF-8 character (1-4 bytes)
    double frequency;   // frequency count
    string code;        // Shannon-Fano code

    Symbol(const string& ch, double freq) : character(ch), frequency(freq) {}
};

// Comparator: sort by frequency descending
bool compareSymbols(const Symbol& a, const Symbol& b) {
    return a.frequency > b.frequency;
}

// Recursive Shannon-Fano splitting
void shannonFanoSplit(vector<Symbol>& symbols, int start, int end, string currentCode) {
    if (start > end) return;
    if (start == end) {
        symbols[start].code = currentCode;
        return;
    }

    // Total frequency in segment
    double totalFrequency = 0;
    for (int i = start; i <= end; ++i) {
        totalFrequency += symbols[i].frequency;
    }

    double currentSum = 0;
    int splitIndex = start;

    for (int i = start; i <= end; ++i) {
        currentSum += symbols[i].frequency;
        if (currentSum * 2 >= totalFrequency) {
            double diff1 = abs(2 * currentSum - totalFrequency);
            double diff2 = abs(2 * (currentSum - symbols[i].frequency) - totalFrequency);
            splitIndex = (diff1 < diff2) ? i : i - 1;
            break;
        }
    }

    // Recurse left (0) and right (1)
    shannonFanoSplit(symbols, start, splitIndex, currentCode + "0");
    shannonFanoSplit(symbols, splitIndex + 1, end, currentCode + "1");
}

// Main compression function
string compressShannonFano(const string& data) {
    if (data.empty()) return "";

    // Step 1: Extract UTF-8 characters
    auto chars = utf8_chars(data);

    // Step 2: Count frequencies
    unordered_map<string, int> frequencyMap;
    for (const string& ch : chars) {
        frequencyMap[ch]++;
    }

    // Step 3: Build symbol list
    vector<Symbol> symbols;
    for (const auto& pair : frequencyMap) {
        symbols.emplace_back(pair.first, pair.second);
    }

    // Step 4: Sort by frequency (descending)
    sort(symbols.begin(), symbols.end(), compareSymbols);

    // Step 5: Generate codes
    shannonFanoSplit(symbols, 0, symbols.size() - 1, "");

    // Step 6: Build encoding table
    unordered_map<string, string> codeTable;
    for (const auto& sym : symbols) {
        codeTable[sym.character] = sym.code;
    }

    // Step 7: Encode input
    stringstream compressedStream;
    for (const string& ch : chars) {
        compressedStream << codeTable[ch];
    }
    string compressed = compressedStream.str();

    // Step 8: Save compressed data (as text of 0s/1s)
    ofstream compFile("compressed_text.txt");
    compFile << compressed;
    compFile.close();

    // Step 9: Save dictionary (human-readable UTF-8)
    ofstream dictFile("dict.txt"); // Linux defaults to UTF-8
    for (const auto& sym : symbols) {
        dictFile << "Символ: ";

        if (sym.character == " ") {
            dictFile << "<пробел>";
        } else if (sym.character == "\n") {
            dictFile << "<новая_строка>";
        } else if (sym.character == "\t") {
            dictFile << "<табуляция>";
        } else {
            dictFile << sym.character; // Full UTF-8 character
        }

        dictFile << " | Код: " << sym.code << "\n";
    }
    dictFile.close();

    return compressed;
}

// Decompression: from bitstring + code table
string decompressShannonFano(const string& compressedData,
                             const unordered_map<string, string>& codeTable) {
    // Build reverse lookup: code -> character
    unordered_map<string, string> reverseTable;
    for (const auto& kv : codeTable) {
        reverseTable[kv.second] = kv.first;
    }

    string currentCode;
    string result;
    for (char bit : compressedData) {
        currentCode += bit;
        auto it = reverseTable.find(currentCode);
        if (it != reverseTable.end()) {
            result += it->second;
            currentCode.clear();
        }
    }
    return result;
}

// Load dictionary from file
unordered_map<string, string> loadCodeTable(const string& filename) {
    unordered_map<string, string> codeTable; // code -> character
    ifstream dictFile(filename);
    if (!dictFile.is_open()) {
        cerr << "Error: Cannot open " << filename << endl;
        return codeTable;
    }

    string line;
    while (getline(dictFile, line)) {
        if (line.find("Символ:") == string::npos) continue;

        size_t codePos = line.find(" | Код: ");
        if (codePos == string::npos) continue;

        string code = line.substr(codePos + 8); // " | Код: " is 8 chars

        size_t start = line.find("Символ: ") + 8;
        size_t end = line.find(" |", start);
        if (end == string::npos) continue;

        string symbolStr = line.substr(start, end - start);

        string character;
        if (symbolStr == "<пробел>") {
            character = " ";
        } else if (symbolStr == "<новая_строка>") {
            character = "\n";
        } else if (symbolStr == "<табуляция>") {
            character = "\t";
        } else {
            character = symbolStr; // UTF-8 string
        }

        codeTable[code] = character;
    }
    dictFile.close();
    return codeTable;
}

// Main function
int main() {
    cout << "Enter the text to compress: ";
    string text;
    getline(cin, text);

    string compressed = compressShannonFano(text);
    cout << "Data saved to file compressed_text.txt" << endl;
    cout << "Dictionary saved to file dict.txt" << endl;

    // Load code table
    auto reverseTable = loadCodeTable("dict.txt");

    // Read compressed data
    ifstream compFile("compressed_text.txt");
    string compressedData;
    getline(compFile, compressedData); // It's a single line of 0s/1s
    compFile.close();

    // Decompress
    string decompressed = decompressShannonFano(compressedData, reverseTable);
    cout << "\nDecompressed text:\n" << decompressed << endl;

    return 0;
}
