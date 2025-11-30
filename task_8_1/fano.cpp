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

using namespace std;

// Задание 1
struct Symbol {
    char character;   // сам символ
    double frequency; // его частота в тексте
    string code;      // код, который присваиваем ему

    Symbol(char ch, double freq) : character(ch), frequency(freq) {}
};

// Сравнивает узлы по частоте (для сортировки)
bool compareSymbols(Symbol& a, Symbol& b) {
    return a.frequency > b.frequency;
}

// Присвоение символам кодов
void shannonFanoSplit(vector<Symbol>& symbols, int start, int end, string currentCode) {
    if (start > end) return;

    // Если остался один символ - присваиваем ему код
    if (start == end) {
        symbols[start].code = currentCode;
        return;
    }

    // Вычисляем общую частоту для текущего отрезка
    int totalFrequency = 0;
    for (int i = start; i <= end; i++) {
        totalFrequency += symbols[i].frequency;
    }

    // Ищем точку разделения - где сумма частот максимально близка к половине
    int currentSum = 0;
    int splitIndex = start;

    for (int i = start; i <= end; i++) {
        currentSum += symbols[i].frequency;

        // Если текущая сумма больше или равна половине общей частоты
        if (currentSum * 2 >= totalFrequency) {
            // Выбираем лучшее разделение
            int diff1 = abs(2 * currentSum - totalFrequency);
            int diff2 = abs(2 * (currentSum - symbols[i].frequency) - totalFrequency);

            splitIndex = (diff1 < diff2) ? i : i - 1;
            break;
        }
    }

    // Рекурсивно обрабатываем левую и правую части
    shannonFanoSplit(symbols, start, splitIndex, currentCode + "0");
    shannonFanoSplit(symbols, splitIndex + 1, end, currentCode + "1");
}

// Основная функция сжатия по Шеннону-Фано
string compressShannonFano(const string& data) {
    if (data.empty()) {
        return "";
    }
    // Подсчет количества одинаковых символов
    unordered_map<char, int> frequencyMap;
    for (char ch : data) {
        frequencyMap[ch]++;
    }
    // Создаем вектор символов
    vector<Symbol> symbols;
    for (const auto& pair : frequencyMap) {
        symbols.push_back(Symbol(pair.first, pair.second));
    }
    // Сортируем символы по убыванию частоты
    sort(symbols.begin(), symbols.end(), compareSymbols);
    // Генерируем коды Шеннона-Фано
    shannonFanoSplit(symbols, 0, symbols.size() - 1, "");
    // Создаем таблицу кодов для быстрого доступа
    unordered_map<char, string> codeTable;
    for (const auto& symbol : symbols) {
        codeTable[symbol.character] = symbol.code;
    }

    // Кодируем исходный текст
    stringstream compressedText;
    for (char ch : data) {
        compressedText << codeTable[ch];
    }

    // Сохраняем сжатые данные
    ofstream compressedFile("compressed_text.txt", ios::binary);
    compressedFile << compressedText.str();
    compressedFile.close();

    // Сохраняем таблицу кодов
    ofstream dictFile("dict.txt");
    dictFile.imbue(locale("ru_RU.UTF-8"));

    for (const auto& symbol : symbols) {
        dictFile << "Символ: ";

        // Обработка специальных символов
        if (symbol.character == ' ') {
            dictFile << "<пробел>";
        }
        else if (symbol.character == '\n') {
            dictFile << "<новая_строка>";
        }
        else if (symbol.character == '\t') {
            dictFile << "<табуляция>";
        }
        else {
            dictFile << symbol.character;
        }

        dictFile << " | Код: " << symbol.code << "\n";
    }
    dictFile.close();

    return compressedText.str();
}

// Функция для расшифровки
string decompressShannonFano(const string& compressedData, const unordered_map<string, char>& codeTable) {
    string currentCode;
    string result;


    for (char bit : compressedData) {
        currentCode += bit;
        if (codeTable.find(currentCode) != codeTable.end()) {
            result += codeTable.at(currentCode);
            currentCode.clear();
        }
    }

    return result;
}

// Функция для загрузки таблицы кодов из файла
unordered_map<string, char> loadCodeTable(const string& filename) {
    unordered_map<string, char> codeTable;
    ifstream dictFile(filename);
    dictFile.imbue(locale("ru_RU.UTF-8"));

    string line;
    while (getline(dictFile, line)) {
        // Пропуск заголовков
        if (line.find("Символ:") != string::npos) {
            size_t codePos = line.find("| Код: ");
            if (codePos != string::npos) {
                // Извлечение кода
                string code = line.substr(codePos + 7);

                // Извлечение символа
                size_t symbolStart = line.find("Символ: ") + 8;
                size_t symbolEnd = line.find(" |", symbolStart);
                string symbolStr = line.substr(symbolStart, symbolEnd - symbolStart);

                char symbol;
                if (symbolStr == "<пробел>") {
                    symbol = ' ';
                }
                else if (symbolStr == "<новая_строка>") {
                    symbol = '\n';
                }
                else if (symbolStr == "<табуляция>") {
                    symbol = '\t';
                }
                else {
                    symbol = symbolStr[0];
                }

                codeTable[code] = symbol;
            }
        }
    }

    dictFile.close();
    return codeTable;
}

int main() {
    cout << "Enter the text to compress: ";
    string text;
    getline(cin, text);
    string compressed = compressShannonFano(text);
    cout << "Data saved to file compressed_text.txt" << endl;
    cout << "Dictionary saved to file dict.txt" << endl;

    unordered_map<string, char> codeTable = loadCodeTable("dict.txt");

    ifstream compressedFile("compressed_text.txt", ios::binary);
    string compressedData;
    compressedFile >> compressedData;
    compressedFile.close();

    string decompressed = decompressShannonFano(compressedData, codeTable);
    cout << decompressed;
    
    return 0;
}


