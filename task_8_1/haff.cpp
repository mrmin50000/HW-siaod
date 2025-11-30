#include <clocale>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <sstream>
#include <queue>
#include <bitset>
#include <math.h>
using namespace std;


// Задание 2
// Структура узла дерева Хаффмана
struct HuffmanNode {
    char character;
    int frequency;
    string code;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char ch, int freq) : character(ch), frequency(freq),
        left(nullptr), right(nullptr) {}

    // Для priority_queue - сравнение по частоте (меньшая частота имеет высший приоритет)
    bool operator>(const HuffmanNode& other) const {
        return frequency > other.frequency;
    }
};

// Для сравнения указателей в priority_queue
struct CompareNode {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->frequency > b->frequency; // меньшие частоты - первые
    }
};

class HuffmanCoding {
private:
    unordered_map<char, string> codes;        // Коды для символов
    unordered_map<string, char> reverseCodes; // Символы по кодам
    HuffmanNode* root;                        // Корень дерева

    // Рекурсивная генерация кодов
    void generateCodes(HuffmanNode* node, string currentCode) {
        if (!node) return;
        if (node->character != '\0') {
            node->code = currentCode;
            codes[node->character] = currentCode;
            reverseCodes[currentCode] = node->character;
        }
        generateCodes(node->left, currentCode + "0");
        generateCodes(node->right, currentCode + "1");
    }
    // Рекурсивное удаление дерева
    void deleteTree(HuffmanNode* node) {
        if (node) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

public:
    HuffmanCoding() : root(nullptr) {}
    ~HuffmanCoding() {
        deleteTree(root);
    }

    // Основная функция сжатия
    string compress(const string& data) {
        if (data.empty()) return "";


        // Подсчет частот символов
        unordered_map<char, int> frequency;
        for (char ch : data) {
            frequency[ch]++;
        }

        // Создание начальных узлов (листьев)
        priority_queue<HuffmanNode*, vector<HuffmanNode*>, CompareNode> minHeap;
        for (auto pair : frequency) {
            minHeap.push(new HuffmanNode(pair.first, pair.second));
        }

        // Построение дерева Хаффмана
        while (minHeap.size() > 1) {
            // Извлекаем два узла с наименьшими частотами
            HuffmanNode* left = minHeap.top();
            minHeap.pop();

            HuffmanNode* right = minHeap.top();
            minHeap.pop();

            // Создаем новый внутренний узел
            HuffmanNode* internalNode = new HuffmanNode('\0', left->frequency + right->frequency);
            internalNode->left = left;
            internalNode->right = right;

            minHeap.push(internalNode);
        }

        // Последний узел в куче - корень дерева
        root = minHeap.top();
        minHeap.pop();

        // Генерация кодов
        generateCodes(root, "");

        // Кодирование текста
        stringstream compressedText;
        for (char ch : data) {
            compressedText << codes[ch];
        }

        return compressedText.str();
    }

    // Функция восстановления текста
    string decompress(const string& compressedData) {
        string currentCode;
        string result;

        for (char bit : compressedData) {
            currentCode += bit;
            if (reverseCodes.find(currentCode) != reverseCodes.end()) {
                result += reverseCodes[currentCode];
                currentCode.clear();
            }
        }

        return result;
    }

    // Геттеры
    unordered_map<char, string> getCodes() const { return codes; }
    HuffmanNode* getRoot() const { return root; }

    // Визуализация дерева
    void printTree(HuffmanNode* node, int indent = 0, const string& prefix = "") {
        if (!node) return;

        cout << string(indent * 4, ' ') << prefix;
        if (node->character != '\0') {
            string charDisplay = (node->character == ' ') ? "<пробел>" : string(1, node->character);
            cout << "'" << charDisplay << "' (freq: " << node->frequency << ", code: " << node->code << ")";
        }
        else {
            cout << "internal (freq: " << node->frequency << ")";
        }
        cout << endl;

        printTree(node->left, indent + 1, "left: ");
        printTree(node->right, indent + 1, "right: ");
    }
};

void calculateHuffmanStatistics(const string& text, const unordered_map<char, string>& codes) {
    if (text.empty() || codes.empty()) return;
    // Подсчет частот
    unordered_map<char, int> frequency;
    for (char ch : text) {
        frequency[ch]++;
    }
    int totalChars = text.length();

    // Коэффициенты сжатия
    int originalAsciiSize = text.length() * 8;
    int compressedSize = 0;
    for (char ch : text) {
        compressedSize += codes.at(ch).length();
    }
    double compressionRatioAscii = (1.0 - static_cast<double>(compressedSize) / originalAsciiSize) * 100.0;
    int alphabetSize = codes.size();
    int uniformCodeLength = ceil(log2(alphabetSize));
    int originalUniformSize = text.length() * uniformCodeLength;
    double compressionRatioUniform = (1.0 - static_cast<double>(compressedSize) / originalUniformSize) * 100.0;

    cout << "Коэффициенты сжатия: \n";
    cout << "Относительно ASCII (8 бит/символ): " << compressionRatioAscii << "%\n";
    cout << "Относительно равномерного кода (" << uniformCodeLength << " бит/символ): " << compressionRatioUniform << "%\n";

    // Средняя длина кода
    double avgCodeLength = 0.0;
    for (const auto& pair : frequency) {
        double probability = static_cast<double>(pair.second) / totalChars;
        avgCodeLength += codes.at(pair.first).length() * probability;
    }


    // Дисперсия
    double variance = 0.0;
    for (const auto& pair : frequency) {
        double probability = static_cast<double>(pair.second) / totalChars;
        double codeLength = codes.at(pair.first).length();
        variance += pow(codeLength - avgCodeLength, 2) * probability;
    }
    cout << "Средняя длина кода : " << avgCodeLength << " бит\n";
    cout << "Дисперсия: " << variance << "\n";\
}

// Функция сохранения сжатого текста
void saveAsBinary(const string& compressedText, const unordered_map<char, string>& codes) {
    ofstream file("compressed.bin", ios::binary);

    string bits = compressedText;
    int padding = 8 - bits.length() % 8;
    if (padding != 8) bits += string(padding, '0');

    for (size_t i = 0; i < bits.length(); i += 8) {
        string byteStr = bits.substr(i, 8);
        unsigned char byte = bitset<8>(byteStr).to_ulong();
        file.write(reinterpret_cast<char*>(&byte), 1);
    }

    file.close();
}

int main() {
    string testText = "ооофывфычсясывлафыводйцулдсямлчс ьлывалфвы счмдлйлбБ фдзвфюВ, фалфывс, АвфвывфвЮБ фвафбчясоцфв ЮБ,,. фывя";

    HuffmanCoding huffman;
    string compressed = huffman.compress(testText);
    string decompressed = huffman.decompress(compressed);

    cout << "Закодированный текст: " << compressed << endl;
    cout << "Декодированный текст: " << decompressed << endl;

    // Статистика 
    calculateHuffmanStatistics(testText, huffman.getCodes());

    // Визуализация дерева
    cout << "\n";
    huffman.printTree(huffman.getRoot());

    // Сжатие файла
    saveAsBinary(compressed, huffman.getCodes());
    // Добавление Исходного текста в файл
    ofstream textFile("text.txt");
    textFile << testText;
    textFile.close();
    return 0;
}

