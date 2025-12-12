#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <sstream>
#include <queue>
#include <bitset>
#include <cmath>
#include <cctype>

using namespace std;

// === UTF-8 Helper: разбить строку на UTF-8 символы ===
vector<string> utf8_chars(const string& s) {
    vector<string> chars;
    for (size_t i = 0; i < s.size(); ) {
        unsigned char c = static_cast<unsigned char>(s[i]);
        size_t len;
        if ((c & 0x80) == 0) {
            len = 1;
        } else if ((c & 0xE0) == 0xC0) {
            len = 2;
        } else if ((c & 0xF0) == 0xE0) {
            len = 3;
        } else if ((c & 0xF8) == 0xF0) {
            len = 4;
        } else {
            len = 1; // invalid, skip
        }
        if (i + len > s.size()) len = 1;
        chars.push_back(s.substr(i, len));
        i += len;
    }
    return chars;
}

// === Узел дерева Хаффмана (работает со строками) ===
struct HuffmanNode {
    string character;  // UTF-8 символ как строка
    int frequency;
    string code;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(const string& ch, int freq)
        : character(ch), frequency(freq), left(nullptr), right(nullptr) {}

    bool operator>(const HuffmanNode& other) const {
        return frequency > other.frequency;
    }
};

struct CompareNode {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->frequency > b->frequency;
    }
};

// === Класс Хаффман-кодирования ===
class HuffmanCoding {
private:
    unordered_map<string, string> codes;        // символ (UTF-8) -> код
    unordered_map<string, string> reverseCodes; // код -> символ
    HuffmanNode* root;

    void generateCodes(HuffmanNode* node, const string& currentCode) {
        if (!node) return;
        if (!node->character.empty()) {
            node->code = currentCode;
            codes[node->character] = currentCode;
            reverseCodes[currentCode] = node->character;
        }
        generateCodes(node->left, currentCode + "0");
        generateCodes(node->right, currentCode + "1");
    }

    void deleteTree(HuffmanNode* node) {
        if (node) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

public:
    HuffmanCoding() : root(nullptr) {}
    ~HuffmanCoding() { deleteTree(root); }

    string compress(const string& data) {
        if (data.empty()) return "";

        auto chars = utf8_chars(data);

        // Частоты
        unordered_map<string, int> frequency;
        for (const string& ch : chars) {
            frequency[ch]++;
        }

        // Мин-куча
        priority_queue<HuffmanNode*, vector<HuffmanNode*>, CompareNode> minHeap;
        for (const auto& pair : frequency) {
            minHeap.push(new HuffmanNode(pair.first, pair.second));
        }

        // Построение дерева
        while (minHeap.size() > 1) {
            HuffmanNode* left = minHeap.top(); minHeap.pop();
            HuffmanNode* right = minHeap.top(); minHeap.pop();

            auto* internal = new HuffmanNode("", left->frequency + right->frequency);
            internal->left = left;
            internal->right = right;
            minHeap.push(internal);
        }

        root = minHeap.top();
        minHeap.pop();

        // Генерация кодов
        generateCodes(root, "");

        // Кодирование
        stringstream compressed;
        for (const string& ch : chars) {
            compressed << codes[ch];
        }
        return compressed.str();
    }

    string decompress(const string& compressedData) {
        string currentCode, result;
        for (char bit : compressedData) {
            currentCode += bit;
            auto it = reverseCodes.find(currentCode);
            if (it != reverseCodes.end()) {
                result += it->second;
                currentCode.clear();
            }
        }
        return result;
    }

    unordered_map<string, string> getCodes() const { return codes; }
    HuffmanNode* getRoot() const { return root; }

    void printTree(HuffmanNode* node, int indent = 0, const string& prefix = "") {
        if (!node) return;

        cout << string(indent * 4, ' ') << prefix;
        if (!node->character.empty()) {
            string display = node->character;
            if (display == " ") {
                display = "<пробел>";
            } else if (display == "\n") {
                display = "<новая_строка>";
            } else if (display == "\t") {
                display = "<табуляция>";
            }
            cout << "'" << display << "' (freq: " << node->frequency << ", code: " << node->code << ")";
        } else {
            cout << "internal (freq: " << node->frequency << ")";
        }
        cout << endl;

        printTree(node->left, indent + 1, "left: ");
        printTree(node->right, indent + 1, "right: ");
    }
};

// === Статистика ===
void calculateHuffmanStatistics(const string& text, const unordered_map<string, string>& codes) {
    if (text.empty() || codes.empty()) return;

    auto chars = utf8_chars(text);
    unordered_map<string, int> frequency;
    for (const string& ch : chars) {
        frequency[ch]++;
    }
    int totalChars = chars.size();

    // Размер в ASCII (8 бит на байт — НЕ точно, но для сравнения)
    int originalAsciiSize = text.size() * 8; // исходные байты × 8
    int compressedSize = 0;
    for (const string& ch : chars) {
        compressedSize += codes.at(ch).length();
    }

    double ratioAscii = (1.0 - static_cast<double>(compressedSize) / originalAsciiSize) * 100.0;

    // Равномерный код
    int alphabetSize = codes.size();
    int uniformLen = (alphabetSize > 1) ? static_cast<int>(ceil(log2(alphabetSize))) : 1;
    int uniformSize = totalChars * uniformLen;
    double ratioUniform = (1.0 - static_cast<double>(compressedSize) / uniformSize) * 100.0;

    cout << "Коэффициенты сжатия:\n";
    cout << "Относительно исходных байтов (×8 бит): " << ratioAscii << "%\n";
    cout << "Относительно равномерного кода (" << uniformLen << " бит/символ): " << ratioUniform << "%\n";

    // Средняя длина кода
    double avgLen = 0.0;
    for (const auto& p : frequency) {
        double prob = static_cast<double>(p.second) / totalChars;
        avgLen += codes.at(p.first).length() * prob;
    }

    // Дисперсия
    double variance = 0.0;
    for (const auto& p : frequency) {
        double prob = static_cast<double>(p.second) / totalChars;
        double len = codes.at(p.first).length();
        variance += pow(len - avgLen, 2) * prob;
    }

    cout << "Средняя длина кода: " << avgLen << " бит\n";
    cout << "Дисперсия: " << variance << "\n";
}

// === Сохранение сжатых данных как бинарный файл ===
void saveAsBinary(const string& compressedText) {
    ofstream file("compressed.bin", ios::binary);
    if (!file.is_open()) return;

    string bits = compressedText;
    int padding = (8 - (bits.size() % 8)) % 8;
    bits += string(padding, '0');

    for (size_t i = 0; i < bits.size(); i += 8) {
        bitset<8> byte(bits.substr(i, 8));
        unsigned char c = static_cast<unsigned char>(byte.to_ulong());
        file.write(reinterpret_cast<char*>(&c), sizeof(c));
    }
    file.close();
}

// === Сохранение словаря кодов ===
void saveCodeDictionary(const unordered_map<string, string>& codes) {
    ofstream dict("huffman_dict.txt"); // UTF-8 by default on Linux
    for (const auto& p : codes) {
        dict << "Символ: ";
        if (p.first == " ") {
            dict << "<пробел>";
        } else if (p.first == "\n") {
            dict << "<новая_строка>";
        } else if (p.first == "\t") {
            dict << "<табуляция>";
        } else {
            dict << p.first; // полный UTF-8 символ
        }
        dict << " | Код: " << p.second << "\n";
    }
    dict.close();
}

// === Основная функция ===
int main() {
    string testText = "ооофывфычсясывлафыводйцулдсямлчс ьлывалфвы счмдлйлбБ фдзвфюВ, фалфывс, АвфвывфвЮБ фвафбчясоцфв ЮБ,,. фывя";

    HuffmanCoding huffman;
    string compressed = huffman.compress(testText);
    string decompressed = huffman.decompress(compressed);

    cout << "Оригинал: " << testText << "\n\n";
    cout << "Декодировано: " << decompressed << "\n\n";

    // Статистика
    calculateHuffmanStatistics(testText, huffman.getCodes());

    // Дерево
    cout << "\nДерево Хаффмана:\n";
    huffman.printTree(huffman.getRoot());

    // Сохранение
    saveAsBinary(compressed);
    saveCodeDictionary(huffman.getCodes());

    ofstream textFile("text.txt");
    textFile << testText;
    textFile.close();

    cout << "\nФайлы сохранены:\n";
    cout << "- text.txt (исходный текст)\n";
    cout << "- compressed.bin (бинарное сжатие)\n";
    cout << "- huffman_dict.txt (словарь с читаемыми русскими символами)\n";

    return 0;
}
