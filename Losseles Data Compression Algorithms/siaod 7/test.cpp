#include <iostream>
#include <queue>
#include <map>
#include <vector>
#include <iomanip>

using namespace std;

struct Node {
    char data;
    int frequency;
    Node* left;
    Node* right;

    Node(char data, int frequency) : data(data), frequency(frequency), left(nullptr), right(nullptr) {}
};

struct CompareNodes {
    bool operator()(Node* lhs, Node* rhs) const {
        return lhs->frequency > rhs->frequency;
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

        Node* internalNode = new Node('-', left->frequency + right->frequency);
        internalNode->left = left;
        internalNode->right = right;

        pq.push(internalNode);
    }
}

void generateHuffmanCodes(Node* root, string code, map<char, string>& huffmanCodes) {
    if (root == nullptr) {
        return;
    }

    if (root->data != '$') {
        huffmanCodes[root->data] = code;
    }

    generateHuffmanCodes(root->left, code + "0", huffmanCodes);
    generateHuffmanCodes(root->right, code + "1", huffmanCodes);
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
        printHuffmanTree(root->right, indent + 4);
        cout << setw(indent) << root->data << "\n";
        printHuffmanTree(root->left, indent + 4);
    }
}

string encodeString(const string& input, const map<char, string>& huffmanCodes) {
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

string decodeString(const string& encodedString, Node* root) {
    string decodedString = "";
    Node* currentNode = root;

    for (char bit : encodedString) {
        if (bit == '0') {
            currentNode = currentNode->left;
        }
        else if (bit == '1') {
            currentNode = currentNode->right;
        }

        if (currentNode->left == nullptr && currentNode->right == nullptr) {
            decodedString += currentNode->data;
            currentNode = root;
        }
    }

    return decodedString;
}


int main() {
    string input = "Akhmetov A.S.";

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

    string encodedString = encodeString(input, huffmanCodes);
    cout << "\nЗакодированная строка: " << encodedString << "\n";

    double avgCodeLength = calculateAverageCodeLength(frequencyTable, huffmanCodes, input);
    cout << "\nСредняя длина кода: " << avgCodeLength << " бит\n";

    double variance = calculateVariance(frequencyTable, huffmanCodes, avgCodeLength, input);
    cout << "\nДисперсия: " << variance << " бит^2\n";

    double compressionRatio = 8.0 * input.length() / encodedString.length();
    cout << "\nКоэффициент сжатия: " << compressionRatio << "\n";

    string decodedString = decodeString(encodedString, pq.top());
    cout << "\nРаскодированная строка: " << decodedString << "\n";

    return 0;
}
