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

int rehash(int hash, int size) {
    hash++;
    hash = hash % size;
    return hash;
}


void DisplayBinary(const string& binaryFileName) {
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

void inputHT(int FILESIZE, Hashtable* hashtable, int TABLESIZE) {
    fstream file("filebin.txt", ios::in | ios::binary);
    file.seekg(0);
    for (int i = 0; i < FILESIZE; i++) {
        Record table;
        file.read((char*)&table, sizeof(Record));

        int PhoneNumber = stoi(table.PhoneNumber);

        int hash = myhash(PhoneNumber, TABLESIZE);
        while (hashtable[hash].key != -1)
            hash = rehash(hash, TABLESIZE);

        hashtable[hash].key = PhoneNumber;
        hashtable[hash].filepos = i * sizeof(Record);
        hashtable[hash].insert = 1;
    }
    file.close();
}


void outputHT(int FILESIZE, Hashtable* hashtable, int TABLESIZE) {
    cout << "[hash]:\t    Номер|\tПоизиция|\n" << "---------------------------------\n";
    for (int i = 0; i < TABLESIZE; i++) {
        if (hashtable[i].key == -1) printf("[%4d]:\n", i);
        else printf("[%4d]:\t%8d|\t%8d|\n", i, hashtable[i].key, hashtable[i].filepos);
    }
}

int uniqkey(int key, Hashtable* hashtable, int& TABLESIZE) {
    int originalkey = myhash(key, TABLESIZE);
    int hash = originalkey;
    while (hashtable[hash].key != -1) {
        if (hashtable[hash].key == key) {
            key++;
            hash = myhash(key, TABLESIZE);
        }
        else hash = rehash(hash, TABLESIZE);
    }
    return key;
}

void resizeHashtable(Hashtable*& hashtable, int& TABLESIZE) {
    int newTableSize = TABLESIZE * 2;
    Hashtable* newHashtable = new Hashtable[newTableSize];
    for (int i = 0; i < TABLESIZE; i++) {
        if (hashtable[i].insert) {
            int key = hashtable[i].key;
            int hash = myhash(key, newTableSize);
            while (newHashtable[hash].insert) {
                hash = rehash(hash, newTableSize);
            }
            newHashtable[hash] = hashtable[i];
        }
    }
    hashtable = newHashtable;
    TABLESIZE = newTableSize;
}

void insertKey(int& key, const char* data1, const char* data2, Hashtable*& hashtable, int& FILESIZE, int& TABLESIZE) {
    if (FILESIZE >= TABLESIZE) resizeHashtable(hashtable, TABLESIZE);
    key = uniqkey(key, hashtable, TABLESIZE);
    
    // Create a new Record with the provided data
    Record table;
    strcpy_s(table.PhoneNumber, to_string(key).c_str());
    strcpy_s(table.Address, data1);
    strcpy_s(table.FullName, data2);

    // Write the Record to the binary file
    fstream file("filebin.txt", ios::out | ios::binary | ios::app);
    file.write((char*)&table, sizeof(Record));
    file.close();

    // Update the hashtable
    int hash = myhash(key, TABLESIZE);
    while (hashtable[hash].insert) {
        if (hashtable[hash].key == key) {
            hashtable[hash].filepos = FILESIZE * sizeof(Record);
            return;
        }
        hash = rehash(hash, TABLESIZE);
    }
    hashtable[hash].key = key;
    hashtable[hash].filepos = FILESIZE * sizeof(Record);
    hashtable[hash].insert = true;
    FILESIZE++;
}
 

void fileremove(int position) {
    const char* originalFile = "filebin.txt";
    const char* tempFile = "tempfile.txt";
    Record table;
    ifstream inFile(originalFile, ios::in | ios::binary);
    ofstream outFile(tempFile, ios::out | ios::binary);
    if (!inFile || !outFile) return;
    int recordSize = sizeof(Record);
    int fileSize = 0;
    inFile.seekg(0, ios_base::end);
    fileSize = inFile.tellg();
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
    remove(originalFile);
    rename(tempFile, originalFile);
}

void remove(int key, Hashtable* hashtable, int TABLESIZE, int& FILESIZE) {
    int hash = myhash(key, TABLESIZE);
        if (hashtable[hash].insert == 0) {
            cout << "Ключ не найден";
            return;
        }
    while (hashtable[hash].key != key) {

        hash = rehash(hash, TABLESIZE);
    }

    int removeposition = hashtable[hash].filepos;
    fileremove(removeposition);
    FILESIZE--;
    hashtable[hash].key = -1;
    hashtable[hash].filepos = -1;
    hashtable[hash].insert = 0;

    for (int i = 0; i < TABLESIZE; i++) {
        if (hashtable[i].filepos > removeposition) {
            hashtable[i].filepos -= sizeof(Record);
        }
    }
}

void findekey(int TABLESIZE, Hashtable* hashtable, int keytofind) {
    chrono::time_point<chrono::steady_clock> start, end_time;
    double cpu_ms = 0;
    fstream file("filebin.txt", ios::in | ios::binary);
    Record table;
    int hash = myhash(keytofind, TABLESIZE);
    if (hashtable[hash].key != keytofind || !hashtable[hash].insert) {
        cout << "Ключ не найден";
        return;
    }
    start = chrono::steady_clock::now();
    while (hashtable[hash].key != keytofind) hash = rehash(hash, TABLESIZE);
    file.seekg(hashtable[hash].filepos);    
    file.read((char*)&table, sizeof(Record));
    end_time = chrono::steady_clock::now();
    cpu_ms = chrono::duration<double, milli>(end_time - start).count();
    cout << "[ ключ]:\t   Имя\t\t   Адрес\t   Затраченное время\n" << "--------------------------------------------------------------------\n";
    cout << table.PhoneNumber << "\t" << table.Address << "\t" << table.FullName << "\t\t" << cpu_ms << "\n";
    file.close();
}

int main() {
    setlocale(0, "");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int FILESIZE = 0;
    ifstream fileCheck("filebin.txt", ios::binary | ios::ate);
    if (fileCheck.is_open()) {
        FILESIZE = fileCheck.tellg() / sizeof(Record);
        fileCheck.close();
    }

    int TABLESIZE = FILESIZE;
    Hashtable* hashtable = new Hashtable[TABLESIZE];
    inputHT(FILESIZE, hashtable, TABLESIZE);

    int key = -1;
    char Adress[40] = "";
    char FullName[40] = "";
    char ch = 0;
    while (ch != 27) {
        system("cls");
        cout << "Двоичный файл:\n";
        DisplayBinary("filebin.txt");
        cout << "\nВыберите действие:\n1. Удалить по ключу\n2. Поиск по ключу\n3. Вставь ключ\n4. Хеш таблица\n\n";
        ch = _getch();
        switch (ch)
        {
        default:
            break;
        case '1':
            //cin.ignore();
            cout << "Введите ключ >> ";
            cin >> key;
            remove(key, hashtable, TABLESIZE, FILESIZE);
            break;
        case '2':
           // cin.ignore();
            cout << "Введите ключ >> ";
            cin >> key;
            system("cls");
            findekey(TABLESIZE, hashtable, key);
            _getch();
            break;
        case '3':
           // cin.ignore();
            cout << "Введите номер для вставки >> ";
            cin >> key;
            cout << "Введите данные для вставки через пробел (адрес и ФИО) >> ";
            cin >> Adress >> FullName;
            insertKey(key, Adress, FullName, hashtable, FILESIZE, TABLESIZE);
            break;
        case '4':
            system("cls");
            cout << "Хеш таблица:\n";
            outputHT(FILESIZE, hashtable, TABLESIZE);
            _getch();
            break;
        }
    }
    cout << "\nВыход";
    return 0;
}