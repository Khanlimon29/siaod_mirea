#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
#include <iomanip>
#include <conio.h>
#include <chrono>

using namespace std;

struct BinaryNode {
    int value = -1;
    int pos = -1;
    int rotations = 0;
    BinaryNode* left = nullptr;
    BinaryNode* right = nullptr;

    BinaryNode(int value = -1, int pos = -1, BinaryNode* left = nullptr, BinaryNode* right = nullptr) : value(value), pos(pos), left(left), right(right), rotations(0) {}

    void print(int space = 0) {
        if (!this)
            return;
        space += 5;
        if (right) right->print(space);

        for (int i = 5; i < space; ++i) cout << " ";
        cout << value << endl;
        if (left) left->print(space);
    }
};

void collectRotationData(const BinaryNode* node, int& totalRotations, int& totalKeysInserted) {
    if (node) {
        totalRotations += node->rotations;
        totalKeysInserted++;
        collectRotationData(node->left, totalRotations, totalKeysInserted);
        collectRotationData(node->right, totalRotations, totalKeysInserted);
    }
}

struct BinaryTree {
private:
    BinaryNode* root;

public:
    BinaryTree()
        : root(nullptr) {}

    const BinaryNode* getRoot() const {
        return root;
    }

    void print() {
        if (root) root->print();
    }

    void add(int value, int pos) {
        root = insert(root, value, pos);
    }

    BinaryNode* insert(BinaryNode* node, int value, int pos) {
        if (node == nullptr) {
            return new BinaryNode(value, pos);
        }

        if (value < node->value) {
            node->left = insert(node->left, value, pos);
            node->rotations++;
        }
        else if (value > node->value) {
            node->right = insert(node->right, value, pos);
            node->rotations++;
        }
        else {
            return node;
        }

        return node;
    }


    int search(int value) {
        return searchValue(root, value);
    }

    int searchValue(BinaryNode* node, int value) {
        if (node == nullptr) {
            return -1;
        }
        if (value < node->value) {
            return searchValue(node->left, value);
        }
        else if (value > node->value) {
            return searchValue(node->right, value);
        }
        else {
            return node->pos;
        }
    }

    void remove(int value) {
        root = removeNode(root, value);
    }

    BinaryNode* removeNode(BinaryNode* node, int value) {
        if (node == nullptr) {
            return node;
        }

        if (value < node->value) {
            node->left = removeNode(node->left, value);
        }
        else if (value > node->value) {
            node->right = removeNode(node->right, value);
        }
        else {
            if (node->left == nullptr) {
                BinaryNode* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr) {
                BinaryNode* temp = node->left;
                delete node;
                return temp;
            }
            BinaryNode* temp = minValueNode(node->right);
            node->value = temp->value;
            node->right = removeNode(node->right, temp->value);
        }

        return node;
    }

    BinaryNode* minValueNode(BinaryNode* node) {
        BinaryNode* current = node;
        while (current && current->left) {
            current = current->left;
        }
        return current;
    }
};

struct BalanceNode {
    int value = -1;
    int pos = -1;
    int factor = 0;
    BalanceNode* left = nullptr;
    BalanceNode* right = nullptr;
    BalanceNode(int value = -1, int pos = -1, BalanceNode* left = nullptr, BalanceNode* right = nullptr)
        : value(value), pos(pos), left(left), right(right) {}

    void print(int space = 0) {
        if (!this)
            return;
        space += 5;
        if (right) right->print(space);

        for (int i = 5; i < space; ++i) cout << " ";
        cout << value << " (" << factor << ")" << endl;
        if (left) left->print(space);
    }
};

struct BalanceTree {
private:
    BalanceNode* root;

    int getHeight(BalanceNode* node) {
        if (node == nullptr) {
            return 0;
        }
        return 1 + max(getHeight(node->left), getHeight(node->right));
    }

    void updateBalance(BalanceNode* node) {
        if (node == nullptr) {
            return;
        }
        node->factor = getHeight(node->right) - getHeight(node->left);
        updateBalance(node->left);
        updateBalance(node->right);
    }

    BalanceNode* balance(BalanceNode* node) {
        if (node == nullptr) {
            return nullptr;
        }
        updateBalance(node);

        if (node->factor > 1) {
            if (node->right->factor < 0) {
                node->right = rotateRight(node->right);
            }
            return rotateLeft(node);
        }
        if (node->factor < -1) {
            if (node->left->factor > 0) {
                node->left = rotateLeft(node->left);
            }
            return rotateRight(node);
        }
        return node;
    }

    BalanceNode* rotateRight(BalanceNode* y) {
        BalanceNode* x = y->left;
        BalanceNode* T2 = x->right;
        x->right = y;
        y->left = T2;
        return x;
    }

    BalanceNode* rotateLeft(BalanceNode* x) {
        BalanceNode* y = x->right;
        BalanceNode* T2 = y->left;
        y->left = x;
        x->right = T2;
        return y;
    }

    BalanceNode* minValueNode(BalanceNode* node) {
        BalanceNode* current = node;
        while (current && current->left) {
            current = current->left;
        }
        return current;
    }

public:
    BalanceTree()
        : root(nullptr) {}

    void print() {
        if (root) root->print();
    }

    void add(int value, int pos) {
        root = insert(root, value, pos);
    }

    BalanceNode* insert(BalanceNode* node, int value, int pos) {
        if (node == nullptr) {
            return new BalanceNode(value, pos);
        }

        if (value < node->value) {
            node->left = insert(node->left, value, pos);
        }
        else if (value > node->value) {
            node->right = insert(node->right, value, pos);
        }
        else {
            return node;
        }

        return balance(node);
    }

    int search(int value) {
        return searchValue(root, value);
    }

    int searchValue(BalanceNode* node, int value) {
        if (node == nullptr) {
            return -1;
        }
        if (value < node->value) {
            return searchValue(node->left, value);
        }
        else if (value > node->value) {
            return searchValue(node->right, value);
        }
        else {
            return node->pos;
        }
    }
    void remove(int value) {
        root = removeNode(root, value);
        root = balance(root);
    }

    BalanceNode* removeNode(BalanceNode* node, int value) {
        if (node == nullptr) {
            return node;
        }

        if (value < node->value) {
            node->left = removeNode(node->left, value);
        }
        else if (value > node->value) {
            node->right = removeNode(node->right, value);
        }
        else {
            if (node->left == nullptr) {
                BalanceNode* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr) {
                BalanceNode* temp = node->left;
                delete node;
                return temp;
            }
            BalanceNode* temp = minValueNode(node->right);
            node->value = temp->value;
            node->right = removeNode(node->right, temp->value);
        }

        return node;
    }
};

struct Table {
    int key;
    char Adress[40];
};

void textToBinary(fstream& textfile) {
    fstream file("Table.txt", ios::in);
    ofstream binaryfile("binaryfile.txt", ios::out | ios::binary);
    if (!file.is_open() || !binaryfile.is_open()) return;
    Table table;
    while (file >> table.key) {
        file >> table.Adress;
        binaryfile.write(reinterpret_cast<char*>(&table), sizeof(Table));
        textfile << table.key << " " << table.Adress << endl;
        if (file.fail()) break;
    }
    file.close();
    binaryfile.close();
}

void BinOut(int number) {
    cout << "     [ключ]:\t        Адрес|\n" << "==============================\n";
    fstream binaryfile("binaryfile.txt", ios::in | ios::binary);
    Table table;

    if (number != -1) {
        binaryfile.seekg((number - 1) * sizeof(Table), ios::beg);
        if (binaryfile.read(reinterpret_cast<char*>(&table), sizeof(Table))) {
            printf("[%9d]:\t%13s|\n", table.key, table.Adress);
        }
        else cout << "Ключ не найден";

    }
    else {
        while (binaryfile.read(reinterpret_cast<char*>(&table), sizeof(Table)))
            printf("[%9d]:\t%13s|\n", table.key, table.Adress);
    }
    binaryfile.close();
}

bool LinSearch(int key) {
    fstream binaryfile("binaryfile.txt", ios::in | ios::binary);
    if (!binaryfile.is_open()) {
        cout << "Ошибка при открытии файла" << endl;
        return false;
    }
    Table table;
    bool found = false;
    while (binaryfile.read(reinterpret_cast<char*>(&table), sizeof(Table))) {
        if (table.key == key) {
            cout << "Двоичный файл:\n" << "    [ключ]:\t        Адрес|\n" << "==============================\n";
            printf("[%9d]:\t%13s|\n", table.key, table.Adress);
            found = true;
            break;
        }
    }
    if (!found) cout << "Нет записи с текущим ключем\n";
    binaryfile.close();
    return found;
}

bool AddressSearch(streampos faddress) {
    fstream binaryfile("binaryfile.txt", ios::in, ios::binary);
    if (!binaryfile.is_open()) return false;
    binaryfile.seekg(faddress);
    Table table;
    if (binaryfile.read(reinterpret_cast<char*>(&table), sizeof(Table))) {
        cout << "Двоичный файл:\n" << "     [ключ]:\t        Адрес|\n" << "==============================\n";
        printf("[%9d]:\t%13s|\n", table.key, table.Adress);
        binaryfile.close();
        return true;
    }
    else {
        cout << "Нет строки по текущему адрессу\n";
        binaryfile.close();
        return false;
    }
}

void AddRecord(const Table& newRecord, fstream& textfile) {
    fstream binaryfile("binaryfile.txt", ios::binary | ios::app);
    if (!binaryfile.is_open()) return;
    binaryfile.write(reinterpret_cast<const char*>(&newRecord), sizeof(Table));
    textfile << newRecord.key << " " << newRecord.Adress << endl;
    binaryfile.close();
}

void RemoveRecord(int key) {
    fstream binaryfile("binaryfile.txt", ios::in | ios::binary);
    fstream tempFile("tempfile.txt", ios::out | ios::binary);

    if (!binaryfile.is_open() || !tempFile.is_open()) {
        cout << "Не удалось открыть файл " << endl;
        return;
    }

    Table table;
    bool found = false;

    while (binaryfile.read(reinterpret_cast<char*>(&table), sizeof(Table))) {
        if (table.key != key) {
            tempFile.write(reinterpret_cast<const char*>(&table), sizeof(Table));
        }
        else {
            found = true;
        }
    }

    binaryfile.close();
    tempFile.close();

    if (!found) {
        cout << "Ключ не найден " << endl;
        remove("tempfile.txt");
        return;
    }

    if (remove("binaryfile.txt") == 0) {
        if (rename("tempfile.txt", "binaryfile.txt") != 0) {
            cout << "Ошибка при переименовании временного файла." << endl;
        }
        else {
            cout << "Файл успешно удален и переименован." << endl;
        }
    }
    else {
        cout << "Ошибка при удалении исходного файла." << endl;
    }
}

void displayAverageRotations(const BinaryTree& binaryTree) {
    int totalRotations = 0;
    int totalKeysInserted = 0;
    collectRotationData(binaryTree.getRoot(), totalRotations, totalKeysInserted);
    if (totalKeysInserted > 0) {
        double averageRotations = static_cast<double>(totalRotations) / totalKeysInserted;
        cout << "Среднее число выполненных поворотов: " << averageRotations << endl;
    }
    else {
        cout << "Ошибка" << endl;
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(0, "");
    Table table;
    BinaryTree binaryTree;
    BalanceTree balanceTree;
    Table reader;
    fstream textfile("Table.txt", ios::in | ios::out);
    textToBinary(textfile);
    fstream binaryfile("binaryfile.txt", ios::in | ios::binary);
    chrono::time_point<chrono::steady_clock> start, end_time;
    double cpu_ms = 0;
    int val, pos = 0;
    char ch = 0;
    int result;
    int totalRotations = 0;
    int totalKeysInserted = 0;
    while (binaryfile.read(reinterpret_cast<char*>(&table), sizeof(Table))) {
        binaryTree.add(table.key, pos);
        balanceTree.add(table.key, pos);
        pos++;
    }
    while (ch != 27) {
        system("cls");
        cout << "Бинарное дерево:\n";
        binaryTree.print();
        cout << "\nСбалансированное дерево:\n";
        displayAverageRotations(binaryTree);
        balanceTree.print();
        cout << "\nТаблица из файла:\n";
        BinOut(-1);
        cout << "\n1. Вывод записи по известному адресу в файле \n"
            << "2. Добавить элемент в деревья и таблицу \n"
            << "3. Поиск по ключу в бинарном дереве\n"
            << "4. Поиск по ключу в сбалансированном дереве\n"
            << "5. Поиск по ключу в бинарном файле по ключу (линейный поиск)\n"
            << "6. Удаление элемента по ключу\n"
            << "\nEsc Выход\n";
        ch = _getch();
        int result = 0;
        switch (ch) {
        default:
            break;
        case '1':
            cout << "Введите номер записи: ";
            int LineNumber;
            cin >> LineNumber;
            BinOut(LineNumber);
            _getch();
            break;
        case '2':
            system("cls");
            cout << "Введите новый ключ >> ";
            cin >> val;
            cout << "Введите данные >> ";
            cin >> table.Adress;
            binaryTree.add(val, pos);
            balanceTree.add(val, pos);
            Table newRecord;
            newRecord.key = val;
            strcpy(newRecord.Adress, table.Adress);
            AddRecord(newRecord, textfile);
            pos++;
            break;
        case '3':
            cout << "Введите ключ >> ";
            cin >> val;
            start = chrono::steady_clock::now();
            result = binaryTree.search(val);
            if (result == -1) {
                cout << "Нет записи с текущим ключем в бинарном дереве\n";
            }
            else {
                AddressSearch(sizeof(Table) * result);
            }
            end_time = chrono::steady_clock::now();
            cpu_ms = chrono::duration<double, milli>(end_time - start).count();
            cout << "Время поиска: " << cpu_ms << "ms";
            _getch();
            break;
        case '4':
            cout << "Введите ключ >> ";
            cin >> val;
            start = chrono::steady_clock::now();
            result = balanceTree.search(val);
            if (result == -1) {
                cout << "Нет записи с текущим ключем в сбалансированном дереве\n";
            }
            else {
                AddressSearch(sizeof(Table) * result);
            }
            end_time = chrono::steady_clock::now();
            cpu_ms = chrono::duration<double, milli>(end_time - start).count();
            cout << "Время поиска: " << cpu_ms << "ms";
            _getch();
            break;
        case '5':
            cout << "Введите ключ >> ";
            cin >> val;
            start = chrono::steady_clock::now();
            result = LinSearch(val);
            if (result == -1) {
                cout << "Нет записи с текущим ключем в файле\n";
            }
            end_time = chrono::steady_clock::now();
            cpu_ms = chrono::duration<double, milli>(end_time - start).count();
            cout << "Время поиска: " << cpu_ms << "ms";
            _getch();
            break;
        case '6':
            cout << "Введите ключ для удаления >> ";
            cin >> val;
            start = chrono::steady_clock::now();
            binaryTree.remove(val);
            balanceTree.remove(val);
            RemoveRecord(val);
            end_time = chrono::steady_clock::now();
            cpu_ms = chrono::duration<double, milli>(end_time - start).count();
            cout << "Запись удалена. Время выполнения: " << cpu_ms << "ms";
            _getch();
            break;
        }
    }
    cout << "Выход";
    return 0;
}