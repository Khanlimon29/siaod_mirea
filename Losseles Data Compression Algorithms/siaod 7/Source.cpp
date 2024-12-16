#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <conio.h>
#include <queue>
#include <iomanip>
#include <fstream>
#include <map>
#include <bitset>


using namespace std;

string compressRLE(const string& input) {
    string compressed;
    size_t n = input.length();

    for (size_t i = 0; i < n; i++) {
        size_t count = 1;
        while (i < n - 1 && input[i] == input[i + 1]) {
            count++;
            i++;
        }
        compressed += input[i] + to_string(count);
    }

    return compressed;
}

struct Token77 {
    int offset;
    int length;
    char nextChar;

    Token77(int o, int l, char c) : offset(o), length(l), nextChar(c) {}
};

vector<Token77> compressLZ77(const string& input) {
    vector<Token77> result;
    int currentIndex = 0;

    while (currentIndex < input.length()) {
        int maxLength = 0;
        int bestMatchIndex = -1;

        for (int i = 0; i < currentIndex; ++i) {
            int j = 0;
            while (currentIndex + j < input.length() && input[i + j] == input[currentIndex + j]) {
                ++j;
            }

            if (j > maxLength) {
                maxLength = j;
                bestMatchIndex = i;
            }
        }

        if (bestMatchIndex != -1) {
            result.push_back(Token77(currentIndex - bestMatchIndex, maxLength, input[currentIndex + maxLength]));
            currentIndex += maxLength + 1;
        }
        else {
            result.push_back(Token77(0, 0, input[currentIndex]));
            ++currentIndex;
        }
    }

    return result;
}

string decompressLZ77(const vector<Token77>& compressed) {
    string result;

    for (const auto& token : compressed) {
        if (token.length > 0) {
            int start = result.length() - token.offset;
            for (int i = 0; i < token.length; ++i) {
                char nextChar = result[start + i];
                result.push_back(nextChar);
                cout << "Восстановление: добавлен символ '" << nextChar << "' из позиции " << start + i << endl;
            }
        }

        char nextChar = token.nextChar;
        result.push_back(nextChar);
        cout << "Восстановление: добавлен символ '" << nextChar << "'" << endl;
    }

    return result;
}


vector<pair<int, char>> compressLZ78(const string& input) {
    unordered_map<string, int> dictionary;
    vector<pair<int, char>> result;

    int nextCode = 1;
    string current = "";

    for (char c : input) {
        current += c;
        if (dictionary.find(current) == dictionary.end()) {
            dictionary[current] = nextCode++;
            result.emplace_back(dictionary[current.substr(0, current.length() - 1)], current.back());
            current = "";
        }
    }

    if (!current.empty()) {
        result.emplace_back(dictionary[current], '\0');
    }
    return result;
}

string decompressLZ78(const vector<pair<int, char>>& compressed) {
    unordered_map<int, string> dictionary;
    string result;

    int nextCode = 1;

    for (const auto& entry : compressed) {
        int code = entry.first;
        char character = entry.second;

        if (code == 0) {
            result += character;
            dictionary[nextCode++] = string(1, character);
            cout << "Восстановление: добавлен символ '" << character << "'" << endl;
        }
        else {
            string phrase = dictionary[code] + character;
            result += phrase;
            dictionary[nextCode++] = phrase;
            cout << "Восстановление: добавлена фраза '" << phrase << "' с кодом " << code << endl;
        }
    }

    return result;
}

struct ShannonFanoNode {
    char symbol;
    double probability;
    string code;
    ShannonFanoNode* left;
    ShannonFanoNode* right;

    ShannonFanoNode(char sym, double prob) : symbol(sym), probability(prob), left(nullptr), right(nullptr) {}
};

bool compareNodes(const ShannonFanoNode* a, const ShannonFanoNode* b) {
    return a -> probability > b -> probability;
}

void generateShannonFanoCode(ShannonFanoNode* root, string code, unordered_map<char, string>& codes) {
    if (root == nullptr) {
        return;
    }

    if (root -> left == nullptr && root -> right == nullptr) {
        codes[root -> symbol] = code;
    }

    generateShannonFanoCode(root -> left, code + "0", codes);
    generateShannonFanoCode(root -> right, code + "1", codes);
}

void shannonFanoDecompression(const string& compressed, ShannonFanoNode* root, const unordered_map<char, string>& codes) {
    ShannonFanoNode* currentNode = root;

    cout << "\nРаскодированная строка: ";

    for (char bit : compressed) {
        if (bit == '0') {
            currentNode = currentNode -> left;
        }
        else if (bit == '1') {
            currentNode = currentNode -> right;
        }

        if (currentNode -> left == nullptr && currentNode -> right == nullptr) {
            cout << currentNode -> symbol;
            currentNode = root;
        }
    }
}

void visualizePrefixTree(ShannonFanoNode* root, int indent = 0) {
    if (root == nullptr) {
        return;
    }

    visualizePrefixTree(root -> right, indent + 4);

    if (indent > 0) {
        cout << setw(indent) << " ";
    }

    cout << root -> symbol << " (" << root -> probability << ")" << endl;

    visualizePrefixTree(root -> left, indent + 4);
}

void ShanonFanoDisplay(string input) {
    ShannonFanoNode* root;
    unordered_map<char, string> codes;
    {
        unordered_map<char, int> frequency;
        for (char ch : input) {
            frequency[ch]++;
        }

        priority_queue<ShannonFanoNode*, vector<ShannonFanoNode*>, decltype(&compareNodes)> pq(&compareNodes);

        for (const auto& entry : frequency) {
            pq.push(new ShannonFanoNode(entry.first, static_cast<double>(entry.second) / input.size()));
        }

        while (pq.size() > 1) {
            ShannonFanoNode* left = pq.top();
            pq.pop();

            ShannonFanoNode* right = pq.top();
            pq.pop();

            ShannonFanoNode* internalNode = new ShannonFanoNode('\0', left -> probability + right -> probability);
            internalNode -> left = left;
            internalNode -> right = right;

            pq.push(internalNode);
        }

        root = pq.top();
        generateShannonFanoCode(root, "", codes);

        cout << "Шеннон-Фано коды:\n";
        for (const auto& entry : codes) {
            cout << entry.first << ": " << entry.second << endl;
        }
    }
    string compressed;
    for (char ch : input) {
        compressed += codes[ch];
    }
    cout << "\nПрефиксное дерево:\n";
    visualizePrefixTree(root);

    shannonFanoDecompression(compressed, root, codes);
    cout << "\nСжатая строка (Шеннон-Фано): " << compressed << endl;
    double originalBits = input.size() * 8.0;
    double compressedBits = compressed.size();
    double compressionRatio = 100.0 * compressedBits / originalBits ;
    cout << "Коэффициент сжатия: " << fixed << setprecision(2) << compressionRatio << "%" << endl;
}

struct Node {
    char data;
    int frequency;
    Node* left;
    Node* right;

    Node(char data, int frequency) : data(data), frequency(frequency), left(nullptr), right(nullptr) {}
};

struct CompareNodes {
    bool operator()(Node* lhs, Node* rhs) const {
        return lhs -> frequency > rhs -> frequency;
    }
};

void buildFrequencyTable(const string& input, map<char, int>& frequencyTable) {
    for (char ch : input) {
        frequencyTable[ch]++;
    }
}

void buildHuffmanTree(priority_queue<Node*, vector<Node*>, CompareNodes>& pq) {
    while (pq.size() > 1) {
        Node* left = pq.top();
        pq.pop();

        Node* right = pq.top();
        pq.pop();

        Node* internalNode = new Node('-', left -> frequency + right -> frequency);
        internalNode -> left = left;
        internalNode -> right = right;

        pq.push(internalNode);
    }
}

void generateHuffmanCodes(Node* root, string code, map<char, string>& huffmanCodes) {
    if (root == nullptr) {
        return;
    }

    if (root -> data != '$') {
        huffmanCodes[root -> data] = code;
    }

    generateHuffmanCodes(root -> left, code + "0", huffmanCodes);
    generateHuffmanCodes(root -> right, code + "1", huffmanCodes);
}

void printFrequencyTable(const map<char, int>& frequencyTable, int totalLength) {
    cout << "Частота вхождений и вероятности: \n";
    for (const auto& pair : frequencyTable) {
        double probability = static_cast<double>(pair.second) / totalLength;
        cout << pair.first << ": " << pair.second << " (Вероятность: " << fixed << setprecision(4) << probability << ")\n";
    }
    cout << "Алфавит: ";
    for (const auto& pair : frequencyTable) {
        cout << pair.first << " ";
    }
    cout << "\n";
}

void printHuffmanCodes(const map<char, string>& huffmanCodes) {
    cout << "\nКоды Хаффмана\n";
    for (const auto& pair : huffmanCodes) {
        cout << pair.first << ": " << pair.second << "\n";
    }
}

void printHuffmanTree(Node* root, int indent = 0) {
    if (root != nullptr) {
        printHuffmanTree(root -> right, indent + 4);
        cout << setw(indent) << root -> data << "\n";
        printHuffmanTree(root -> left, indent + 4);
    }
}

string encodeHaffman(const string& input, const map<char, string>& huffmanCodes) {
    string encodedString = "";
    for (char ch : input) {
        encodedString += huffmanCodes.at(ch);
    }
    return encodedString;
}

double calculateAverageCodeLength(const map<char, int>& frequencyTable, const map<char, string>& huffmanCodes, string input) {
    double totalBits = 0;
    for (const auto& pair : frequencyTable) {
        totalBits += pair.second * huffmanCodes.at(pair.first).length();
    }
    return totalBits / input.length();
}

double calculateVariance(const map<char, int>& frequencyTable, const map<char, string>& huffmanCodes, double avgCodeLength, string input) {
    double variance = 0;
    for (const auto& pair : frequencyTable) {
        double diff = huffmanCodes.at(pair.first).length() - avgCodeLength;
        variance += pair.second * diff * diff;
    }
    return variance / input.length();
}

string decodeHaffman(const string& encodedString, Node* root) {
    string decodedString = "";
    Node* currentNode = root;

    for (char bit : encodedString) {
        if (bit == '0') {
            currentNode = currentNode -> left;
        }
        else if (bit == '1') {
            currentNode = currentNode -> right;
        }

        if (currentNode -> left == nullptr && currentNode -> right == nullptr) {
            decodedString += currentNode -> data;
            currentNode = root;
        }
    }

    return decodedString;
}

string readFromFile(const string& fileName) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Ошибка при открытии файла" << endl;
        exit(1);
    }

    string content;
    char ch;
    while (file.get(ch)) {
        content += ch;
    }

    file.close();
    return content;
}

void writeBinaryFile(string fileName, string binaryData) {
    ofstream binaryFile(fileName, ios::out | ios::binary);
    if (binaryFile.is_open()) {
        for (size_t i = 0; i < binaryData.length(); i += 8) {
            bitset<8> bits(binaryData.substr(i, 8));
            char byte = static_cast<char>(bits.to_ulong());
            binaryFile.write(&byte, 1);
        }
        binaryFile.close();
    }
    else {
        cout << "Ошибка при открытии файла" << fileName << endl;
    }
}


int main() {
    setlocale(0, "");
    char ch = 0;
    while (ch != 27) {
        system("cls");
        cout << "1. RLE\n2. LZ77\n3. LZ78\n4. Шенон-Фано\n5. Хаффман"
            << "\n\nEsc. Выход из программы";
        ch = _getch();
        int result = 0;
        switch (ch) {
        default:
            break;
        case '1': {
            system("cls");
            string RepeatedText(3000, 'A');
            string CompressedRepeated = compressRLE(RepeatedText);
            cout << "Изначальная строка: " << RepeatedText << endl
                << "Сжатая строка (RLE): " << CompressedRepeated << endl
                << "Коэффициент сжатия: " << fixed << setprecision(2) << (static_cast<double>(CompressedRepeated.length()) / RepeatedText.length()) * 100 << "%" << "\n\n";

            string UniqueText = "0123456789";
            for (char ch = 'A'; ch <= 'Z'; ++ch) {
                UniqueText += ch;
            }
            for (char ch = 'a'; ch <= 'z'; ++ch) {
                UniqueText += ch;
            }
            string CompressedUnique = compressRLE(UniqueText);
            cout << "Изначальная строка: " << UniqueText << endl
                << "Сжатая строка (RLE): " << CompressedUnique << endl
                << "Коэффициент сжатия: " << fixed << setprecision(2) << (static_cast<double>(CompressedUnique.length()) / UniqueText.length()) * 100 << "%" << endl;
            _getch();
            break;
        }
        case '2': {
            system("cls");
            string input = "0100101010010000101";

            vector<Token77> compressed = compressLZ77(input);

            

            cout << "Исходная строка: " << input << endl;
            cout << "Сжатие (LZ77): ";
            for (const auto& token : compressed) {
                cout << "(" << token.offset << "," << token.length << "," << token.nextChar << ")";
            }
            cout << endl;
            string decompressed = decompressLZ77(compressed);
            cout << "Восстановленная строка: " << decompressed << endl;

            float compressionRatio = 100.0 * static_cast<float>(compressed.size()) / static_cast<float>(input.length()) ;
            cout << "Коэффициент сжатия: " << fixed << setprecision(2) << compressionRatio << "%" << endl;

            _getch();
            break;
        }
        case '3': {
            system("cls");
            string original = "lorloraloranranloran";
            vector<pair<int, char>> compressed = compressLZ78(original);

            cout << "Исходный текст: " << original << endl;
            cout << "Сжатие LZ78: ";
            for (const auto& entry : compressed) {
                cout << "(" << entry.first << "," << entry.second << ")";
            }
            cout << endl;
            string decompressed = decompressLZ78(compressed);
            cout << "Распакованный текст: " << decompressed << endl;

            float compressionRatio = 100.0 * compressed.size() / static_cast<float>(original.size()) ;
            cout << "Коэффициент сжатия: " << fixed << setprecision(2) << compressionRatio << "%" << endl;
            _getch();
            break;
        }
        case '4': {
            system("cls");
            string shannonFanoInput = "Ene-bene, riki-taki, Bul`-bul`-bul`, Karaki-hmaki Eus-deus-krasnodeus bac";
            ShanonFanoDisplay(shannonFanoInput);
            _getch();
            break;
        }
        case '5' : {
            system("cls");
            cout << "Выберите файл:\n";
            cout << "1. surname.txt\n";
            cout << "2. text.txt\n";
            cout << "Введите номер файла (1 или 2): ";

            int choice;
            cin >> choice;

            string fileName;
            switch (choice) {
            case 1:
                fileName = "surname.txt";
                break;
            case 2:
                fileName = "text.txt";
                break;
            }

            ofstream outputFile("output.txt");
            if (!outputFile.is_open()) {
                cout << "Ошибка при открытии файла" << endl;
                return 1;
            }

            streambuf* coutbuf = cout.rdbuf(); 
            cout.rdbuf(outputFile.rdbuf());   

            string input = readFromFile(fileName);

            map<char, int> frequencyTable;
            buildFrequencyTable(input, frequencyTable);
            printFrequencyTable(frequencyTable, input.length());

            priority_queue<Node*, vector<Node*>, CompareNodes> pq;
            for (const auto& pair : frequencyTable) {
                pq.push(new Node(pair.first, pair.second));
            }

            buildHuffmanTree(pq);

            map<char, string> huffmanCodes;
            generateHuffmanCodes(pq.top(), "", huffmanCodes);

            printHuffmanCodes(huffmanCodes);

            cout << "\nПрефиксное дерево Хаффмана:\n";
            printHuffmanTree(pq.top());

            string encodedString = encodeHaffman(input, huffmanCodes);
            cout << "\nЗакодированная строка: " << encodedString << "\n";
            writeBinaryFile("encoded.bin", encodedString);
            
            double avgCodeLength = calculateAverageCodeLength(frequencyTable, huffmanCodes, input);
            cout << "\nСредняя длина кода: " << avgCodeLength << " бит\n";

            double variance = calculateVariance(frequencyTable, huffmanCodes, avgCodeLength, input);
            cout << "\nДисперсия: " << variance << " бит^2\n";

            double compressionRatio = 100.0 * ((encodedString.length() / static_cast<double>(input.length()) / 8.0));
            cout << "\nКоэффициент сжатия: " << fixed << setprecision(2) << compressionRatio << "%" << "\n";

            string decodedString = decodeHaffman(encodedString, pq.top());
            cout << "\nРаскодированная строка: " << decodedString << "\n";

            cout.rdbuf(coutbuf);
            outputFile.close();

            cout << "Результат записан в файлы output и encoded";
            _getch();
            break;
        }
        }
    }
    cout << "\nВыход";
    return 0;
}
