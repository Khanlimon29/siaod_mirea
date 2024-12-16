#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <bitset>

using namespace std;

struct HuffmanNode {
    char data;
    unsigned frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char data, unsigned frequency) : data(data), frequency(frequency), left(nullptr), right(nullptr) {}
};

struct CompareNodes {
    bool operator()(HuffmanNode* left, HuffmanNode* right) {
        return left->frequency > right->frequency;
    }
};

HuffmanNode* buildHuffmanTree(unordered_map<char, unsigned>& frequencies) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, CompareNodes> minHeap;

    for (auto& pair : frequencies) {
        minHeap.push(new HuffmanNode(pair.first, pair.second));
    }

    while (minHeap.size() != 1) {
        HuffmanNode* left = minHeap.top();
        minHeap.pop();

        HuffmanNode* right = minHeap.top();
        minHeap.pop();

        HuffmanNode* internalNode = new HuffmanNode('$', left->frequency + right->frequency);
        internalNode->left = left;
        internalNode->right = right;

        minHeap.push(internalNode);
    }

    return minHeap.top();
}

void buildHuffmanCodes(HuffmanNode* root, string code, unordered_map<char, string>& huffmanCodes) {
    if (!root)
        return;

    if (root->data != '$') {
        huffmanCodes[root->data] = code;
    }

    buildHuffmanCodes(root->left, code + "0", huffmanCodes);
    buildHuffmanCodes(root->right, code + "1", huffmanCodes);
}

string encodeText(string text, unordered_map<char, string>& huffmanCodes) {
    string encodedText = "";
    for (char character : text) {
        encodedText += huffmanCodes[character];
    }
    return encodedText;
}

string decodeText(HuffmanNode* root, string encodedText) {
    string decodedText = "";
    HuffmanNode* currentNode = root;
    for (char bit : encodedText) {
        if (bit == '0') {
            currentNode = currentNode->left;
        }
        else {
            currentNode = currentNode->right;
        }

        if (currentNode->data != '$') {
            decodedText += currentNode->data;
            currentNode = root;
        }
    }
    return decodedText;
}

void writeBinaryFile(string fileName, string binaryData) {
    ofstream binaryFile(fileName, ios::out | ios::binary);
    if (binaryFile.is_open()) {
        // Запись битов в файл
        for (size_t i = 0; i < binaryData.length(); i += 8) {
            bitset<8> bits(binaryData.substr(i, 8));
            char byte = static_cast<char>(bits.to_ulong());
            binaryFile.write(&byte, 1);
        }
        binaryFile.close();
    }
    else {
        cerr << "Unable to open binary file: " << fileName << endl;
    }
}

int main() {
    string text = "hello, world!";

    unordered_map<char, unsigned> frequencies;
    for (char character : text) {
        frequencies[character]++;
    }

    HuffmanNode* huffmanTreeRoot = buildHuffmanTree(frequencies);

    unordered_map<char, string> huffmanCodes;
    buildHuffmanCodes(huffmanTreeRoot, "", huffmanCodes);

    string encodedText = encodeText(text, huffmanCodes);
    string decodedText = decodeText(huffmanTreeRoot, encodedText);

    cout << "Original text: " << text << endl;
    cout << "Encoded text: " << encodedText << endl;
    cout << "Decoded text: " << decodedText << endl;

    writeBinaryFile("encoded.bin", encodedText);

    return 0;
}
