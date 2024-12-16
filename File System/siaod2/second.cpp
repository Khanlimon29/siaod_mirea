#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <conio.h>

using namespace std;


struct Record {
    char PhoneNumber[10];
    char Address[20];
    char FullName[20];
};

int FindRecordPosition(const vector<Record>& records, const string& key) {
    auto it = find_if(records.begin(), records.end(),
        [key](const Record& record) { return key == record.PhoneNumber; });

    if (it != records.end()) {
        return distance(records.begin(), it);
    }
    else {
        return -1;
    }
}

void TextToBinary(const string& FileName, const string& binaryFileName) {
    ifstream textFile(FileName);
    ofstream binaryFile(binaryFileName, ios::out | ios::binary);

    if (!textFile.is_open() || !binaryFile.is_open()) {
        cout << "Ошибка при открытии файла" << endl;
        return;
    }

    Record record;
    while (textFile >> record.PhoneNumber >> record.Address >> record.FullName) {
        binaryFile.write(reinterpret_cast<char*>(&record), sizeof(Record));
    }
    cout << "\nКонвертация прошла успешно\nНажмите на любую кнопку чтобы продолжить";
    textFile.close();
    binaryFile.close();
}

void BinaryToText(const string& binaryFileName, const string& FileName) {
    ifstream binaryFile(binaryFileName, ios::in | ios::binary);
    ofstream textFile(FileName);

    if (!binaryFile.is_open() || !textFile.is_open()) {
        cout << "Ошибка при открытии файла" << endl;
        return;
    }

    Record record;
    while (binaryFile.read(reinterpret_cast<char*>(&record), sizeof(Record))) {
        textFile << record.PhoneNumber << " " << record.Address << " " << record.FullName << endl;
    }
    cout << "\nКонвертация прошла успешно\nНажмите на любую кнопку чтобы продолжить";
    binaryFile.close();
    textFile.close();
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

void DisplayFile(const string& Filename) {
    ifstream inputFile(Filename);
    if (!inputFile.is_open()) {
        cout << "Не удалось открыть файл" << endl;
        return;
    }

    string line;
    while (getline(inputFile, line)) {
        cout << line << endl;
    }
    inputFile.close();
}

void RecordByNumber(const string& binaryFileName, int recordNumber) {
    ifstream binaryFile(binaryFileName, ios::in | ios::binary);

    if (!binaryFile.is_open()) {
        cout << "Ошибка при открытии файла" << endl;
        return;
    }

    Record record;
    binaryFile.seekg(recordNumber * sizeof(Record), ios::beg);
    binaryFile.read(reinterpret_cast<char*>(&record), sizeof(Record));

    cout << "Номер: " << record.PhoneNumber << ", Адрес: " << record.Address << ", ФИО: " << record.FullName << endl;

    binaryFile.close();
}

void DeleteByKey(string binaryFileName, const string& key) {
    fstream binaryFile(binaryFileName, ios::in | ios::out | ios::binary);

    if (!binaryFile.is_open()) {
        cout << "Ошибка при открытии файла" << endl;
        return;
    }

    vector<Record> records;
    Record record;
    while (binaryFile.read(reinterpret_cast<char*>(&record), sizeof(Record))) {
        records.push_back(record);
    }

    int position = FindRecordPosition(records, key);
    if (position != -1) {
        swap(records[position], records.back());
        records.pop_back();

        binaryFile.close();

        binaryFile.open(binaryFileName, ios::out | ios::binary);

        for (const auto& r : records) {
            binaryFile.write(reinterpret_cast<const char*>(&r), sizeof(Record));
        }

        cout << "Номер " << key << " удален" << endl;
    }
    else {
        cout << "Номер " << key << " не найден" << endl;
    }

    binaryFile.close();
}

void PrefixFile(const string& binaryFileName, const string& prefix) {

    ifstream binaryFile(binaryFileName, ios::in | ios::binary);
    ofstream textFile("prefix_" + prefix + ".txt");

    if (!binaryFile.is_open() || !textFile.is_open()) {
        cout << "Ошибка при открытии файла" << endl;
        return;
    }

    Record record;
    while (binaryFile.read(reinterpret_cast<char*>(&record), sizeof(Record))) {
        string phoneNumberStr(record.PhoneNumber, 3);
        if (phoneNumberStr == prefix) {
            textFile << record.PhoneNumber << " " << record.FullName << endl;
        }
    }

    binaryFile.close();
    textFile.close();
}

void DeleteByPrefix(string binaryFileName, const string& prefix) {
    fstream binaryFile(binaryFileName, ios::in | ios::out | ios::binary);

    if (!binaryFile.is_open()) {
        cout << "Ошибка при открытии файла" << endl;
        return;
    }

    vector<Record> records;
    Record record;
    while (binaryFile.read(reinterpret_cast<char*>(&record), sizeof(Record))) {
        string phoneNumberStr(record.PhoneNumber, 1);
        if (phoneNumberStr != prefix) {
            records.push_back(record);
        }
    }

    binaryFile.close();

    binaryFile.open(binaryFileName, ios::out | ios::binary);

    for (const auto& r : records) {
        binaryFile.write(reinterpret_cast<const char*>(&r), sizeof(Record));
    }

    binaryFile.close();
}

int main() {
    string BinaryFileName = "filebin2.txt";
    string FileName;
    cout << "Введите название файла: ";
    cin >> FileName;
    ifstream fileExistsCheck(FileName);
    if (!fileExistsCheck) {
        cout << "Файла не существует";
        return 1;
    }
    TextToBinary(FileName, BinaryFileName);
    char ch = 0;
    while (ch != 27) {
        system("cls");
        cout << "Содержимое двоичного файла:\n";
        DisplayBinary(BinaryFileName);
        cout << "\nВыберите действие:\n"
             << "1. Преобразование данных из текстового файла в двоичный\n"
             << "2. Преобразование данных из двоичного файла в текстовый\n"
             << "3. Доступ к записи по ее порядковому номеру в файле\n"
             << "4. Удаление записи с заданным значением ключа (номер телефона)\n"
             << "5. Сформировать текстовый файл из фамилий владельцев, чьи номера начинаются с введенных первых трех цифр\n"
             << "6. Вывод файла с фамилиями владельцев, чьи номера начинаются с введенных первых трех цифр (тест 6 пункта)\n"
             << "7. Удалить сведения о владельцах телефонов, которые начинаются с заданной цифры\n"
             << "\nEsc. Выход\n";
        ch = _getch();
        switch (ch)
        {
        default:
            break;
        case '1':
        {
            BinaryToText(BinaryFileName, FileName);
            _getch();
            break;
        }

        case '2':
        {
            TextToBinary(FileName, BinaryFileName);
            _getch();
            break;
        }

        case '3':
        {
            system("cls");
            cin.ignore();
            int recordNumber;
            cout << "Введите порядковый номер: ";
            cin >> recordNumber;
            RecordByNumber(BinaryFileName, recordNumber - 1);
            _getch();
            break;
        }

        case '4':
        {
            system("cls");
            cin.ignore();
            string key;
            cout << "Введите номер для удаления: ";
            cin >> key;
            DeleteByKey(BinaryFileName, key);
            _getch();
            break;
        }

        case '5':
        {
            system("cls");
            cin.ignore();
            string prefix;
            cout << "Введите первые 3 цифры номера: ";
            cin >> prefix;
            if (99 < stoi(prefix) < 1000) {
                PrefixFile(BinaryFileName, prefix);
                cout << "Файл с фамилиями успешно создан" << endl;
            }
            else cout << "Некорректно введённые данные" << endl;
            _getch();
            break;
        }

        case '6':
        {
            system("cls");
            cin.ignore();
            string PrefFile;
            cout << "Введите первые три цифры номера: ";
            cin >> PrefFile;
            PrefFile = "prefix_" + PrefFile + ".txt";
            DisplayFile(PrefFile);
            _getch();
            break;
        }

        case '7':
        {
            system("cls");
            cin.ignore();
            string prefixToDelete;
            cout << "Введите первую цифру номера для удаления: ";
            cin >> prefixToDelete;
            if (0 < stoi(prefixToDelete) < 10) {
                DeleteByPrefix(BinaryFileName, prefixToDelete);
                cout << "Номер(а) начинающиеся с " << prefixToDelete << " были удалены" << endl;
            }
            else cout << "Некорректно введённые данные" << endl;
            _getch();
            break;
        }
        }
    }
    cout << "\nВыход";
    return 0;
}