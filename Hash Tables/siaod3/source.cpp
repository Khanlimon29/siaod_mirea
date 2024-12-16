#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <Windows.h>
#include <conio.h>

using namespace std;

struct Hashtable {
    int key = -1;
    int filepos = -1;
    bool insert = 0;
};

struct Record {
    char PhoneNumber[10];
    char Address[20];
    char FullName[20];
};

int myhash(int key, int size) {
    int hash = key;
    hash = hash * (31 + key);
    hash = hash % size;
    return hash;
}

int myhash2(int key, int size) {
    int hash = key;
    hash = hash * (15 + key);
    hash = hash % size + 5;
    return hash;
}

int rehash(int hash, int secondHash, int size) {
    hash = (hash + secondHash) % size;
    return hash;
}

void BinaryDisplay(const string& binaryFileName) {
    ifstream binaryFile(binaryFileName, ios::in | ios::binary);

    if (!binaryFile.is_open()) {
        cout << "Ошибка при открытии файла" << endl;
        return;
    }
    Record record;
    cout << "  Номер    |    Адрес   |       ФИО\n";
    cout << "----------------------------------------\n";
    while (binaryFile.read(reinterpret_cast<char*>(&record), sizeof(Record))) {
        cout << record.PhoneNumber << "  |  " << record.Address << "  |  " << record.FullName << endl;
    }

    binaryFile.close();
}

void HashInput(int& FileSize, Hashtable*& hashtable, int& TableSize, const string& filename) {
    ifstream file(filename, ios::in | ios::binary);

    if (!file.is_open()) {
        cout << "Ошибка при открытии файла" << endl;
        return;
    }

    file.seekg(0, ios::end);
    int totalRecords = file.tellg() / sizeof(Record);
    file.seekg(0, ios::beg);

    for (int i = 0; i < totalRecords; i++) {
        Record table;
        file.read((char*)&table, sizeof(Record));

        int PhoneNumber = stoi(table.PhoneNumber);

        int hash = myhash(PhoneNumber, TableSize);
        int secondHash = myhash2(PhoneNumber, TableSize);
        while (hashtable[hash].insert)
            hash = rehash(hash, secondHash, TableSize);

        hashtable[hash].key = PhoneNumber;
        hashtable[hash].filepos = i * sizeof(Record);
        hashtable[hash].insert = 1;
    }

    FileSize = totalRecords;

    file.close();
}



void HashDisplay(int FileSize, Hashtable* hashtable, int TableSize) {
    cout << "[hash]:\t    Номер|\tПозиция|\n" << "---------------------------------\n";
    for (int i = 0; i < TableSize; i++) {
        if (hashtable[i].key == -1) printf("[%4d]:\n", i);
        else printf("[%4d]:\t%8d|\t%8d|\n", i, hashtable[i].key, hashtable[i].filepos);
    }
}

int KeyUnique(int key, Hashtable* hashtable, int& TableSize) {
    int originalkey = myhash(key, TableSize);
    int hash = originalkey;
    int secondHash = myhash2(key, TableSize);

    while (hashtable[hash].key != -1) {
        if (hashtable[hash].key == key) {
            key = rehash(originalkey, secondHash, TableSize);
            hash = myhash(key, TableSize);
            secondHash = myhash2(key, TableSize);
        }
        else {
            hash = rehash(hash, secondHash, TableSize);
        }
    }
    return key;
}

void HashResize(Hashtable*& hashtable, int& TableSize) {
    int newTableSize = TableSize * 2;
    Hashtable* newHashtable = new Hashtable[newTableSize];

    for (int i = 0; i < TableSize; i++) {
        if (hashtable[i].insert) {
            int key = hashtable[i].key;
            int hash = myhash(key, newTableSize);
            int secondHash = myhash2(key, newTableSize);

            while (newHashtable[hash].insert) {
                hash = rehash(hash, secondHash, newTableSize);
            }
            newHashtable[hash] = hashtable[i];
        }
    }
    hashtable = newHashtable;
    TableSize = newTableSize;
}

void KeyInsert(int& key, const char* Adress, const char* FullName, Hashtable*& hashtable, int& FileSize, int& TableSize, string currentFile) {
    if (FileSize >= TableSize) HashResize(hashtable, TableSize);
    key = KeyUnique(key, hashtable, TableSize);

    Record table;
    strcpy_s(table.PhoneNumber, to_string(key).c_str());
    strcpy_s(table.Address, Adress);
    strcpy_s(table.FullName, FullName);

    fstream file(currentFile, ios::out | ios::binary | ios::app);
    file.write((char*)&table, sizeof(Record));
    file.close();

    int hash = myhash(key, TableSize);
    int secondHash = myhash2(key, TableSize);

    while (hashtable[hash].insert) {
        if (hashtable[hash].key == key) {
            hashtable[hash].filepos = FileSize * sizeof(Record);
            return;
        }
        hash = rehash(hash, secondHash, TableSize);
    }
    hashtable[hash].key = key;
    hashtable[hash].filepos = FileSize * sizeof(Record);
    hashtable[hash].insert = true;
    FileSize++;
}

void FileRemove(int position, const string& currentFile) {
    const char* originalFile = currentFile.c_str();
    const char* tempFile = "tempfile.txt";
    Record table;

    ifstream inFile(originalFile, ios::in | ios::binary);
    ofstream outFile(tempFile, ios::out | ios::binary);
    if (!inFile || !outFile) return;

    int recordSize = sizeof(Record);
    int FileSize = 0;
    inFile.seekg(0, ios_base::end);
    FileSize = inFile.tellg();
    inFile.seekg(0, ios_base::beg);

    for (int i = 0; i < position; i += recordSize) {
        inFile.read((char*)&table, recordSize);
        outFile.write((char*)&table, recordSize);
    }

    inFile.seekg(recordSize, ios_base::cur);

    while (inFile.read((char*)&table, recordSize))
        outFile.write((char*)&table, recordSize);

    inFile.close();
    outFile.close();

    if (remove(originalFile) != 0) {
        cout << "Не удалось удалить исходный файл." << endl;
        return;
    }

    if (rename(tempFile, originalFile) != 0) {
        cout << "Не удалось переименовать временный файл." << endl;
    }
}

void KeyRemove(int key, Hashtable* hashtable, int TableSize, int& FileSize, string currentFile) {
    int hash = myhash(key, TableSize);
    int secondHash = myhash2(key, TableSize);

    if (hashtable[hash].insert == 0) {
        cout << "Ключ не найден";
        return;
    }
    while (hashtable[hash].key != key) {
        hash = rehash(hash, secondHash, TableSize);
    }

    int removeposition = hashtable[hash].filepos;
    FileRemove(removeposition, currentFile);
    FileSize--;
    hashtable[hash].key = -1;
    hashtable[hash].filepos = -1;
    hashtable[hash].insert = 0;

    for (int i = 0; i < TableSize; i++) {
        if (hashtable[i].filepos > removeposition) {
            hashtable[i].filepos -= sizeof(Record);
        }
    }
}

void KeyFind(int TableSize, Hashtable* hashtable, int keytofind, string currentFile) {
    chrono::time_point<chrono::steady_clock> start, end_time;
    double cpu_ms = 0;

    fstream file(currentFile, ios::in | ios::binary);
    Record table;
    int hash = myhash(keytofind, TableSize);
    int secondHash = myhash2(keytofind, TableSize);

    if (!file.is_open()) {
        cout << "Ошибка при открытии файла" << endl;
        return;
    }

    start = chrono::steady_clock::now();
    while (hashtable[hash].key != keytofind || !hashtable[hash].insert) {
        hash = rehash(hash, secondHash, TableSize);
    }

    file.seekg(hashtable[hash].filepos);
    if (!file.read((char*)&table, sizeof(Record))) {
        cout << "Ошибка при чтении из файла" << endl;
        file.close();
        return;
    }

    end_time = chrono::steady_clock::now();
    cpu_ms = chrono::duration<double, milli>(end_time - start).count();
    cout << "[ ключ]:\t   Имя\t\t   Адрес\t   Затраченное время\n" << "--------------------------------------------------------------------\n";
    cout << table.PhoneNumber << "\t"
        << table.Address << "\t"
        << table.FullName << "\t\t"
        << cpu_ms << "\n";
    file.close();
}



int main() {
    setlocale(0, "");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int FileSize = 0;
    int FileName;
    string currentFile;
    cout << "Введите номер файла (1 - файл из второй работы, 2 - файл с 500 записями): ";
    cin >> FileName;
    if (FileName == 1) currentFile = "filebin.txt";
    else currentFile = "filebin500.txt";

    ifstream fileCheck(currentFile, ios::binary | ios::ate);
    if (fileCheck.is_open()) {
        FileSize = fileCheck.tellg() / sizeof(Record);
        fileCheck.close();
    }

    int TableSize = FileSize;
    Hashtable* hashtable = new Hashtable[TableSize];
    HashInput(FileSize, hashtable, TableSize, currentFile);

    int size = 0;
    int key = -1;
    char Adress[20];
    char FullName[20];
    char ch = 0;
    while (ch != 27) {
        system("cls");
        cout << "Двоичный файл:\n";
        BinaryDisplay(currentFile);
        cout << "\nВыберите действие:\n1. Удалить по ключу\n2. Поиск по ключу\n3. Вставь ключ\n4. Хеш таблица\n\n";
        ch = _getch();
        cin.ignore();
        switch (ch) {
        default:
            break;
        case '1':
            cout << "Введите ключ >> ";
            cin >> key;
            KeyRemove(key, hashtable, TableSize, FileSize, currentFile);
            break;
        case '2':
            cout << "Введите ключ >> ";
            cin >> key;
            system("cls");
            KeyFind(TableSize, hashtable, key, currentFile);
            _getch();
            break;
        case '3':
            cout << "Введите номер для вставки >> ";
            cin >> key;
            cout << "Введите данные для вставки через пробел (адрес и ФИО) >> ";
            cin >> Adress >> FullName;
            KeyInsert(key, Adress, FullName, hashtable, FileSize, TableSize, currentFile);
            break;
        case '4':
            system("cls");
            cout << "Хеш таблица:\n";
            HashDisplay(FileSize, hashtable, TableSize);
            _getch();
            break;
        }
    }
    cout << "\nВыход";
    return 0;
}
