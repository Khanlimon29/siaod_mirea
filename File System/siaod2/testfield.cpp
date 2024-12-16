#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <conio.h>

using namespace std;


struct Record {
    char phoneNumber[10];
    char address[20];
    char fullName[20];
};

int findRecordPosition(const vector<Record>& records, const string& key) {
    auto it = find_if(records.begin(), records.end(),
        [key](const Record& record) { return key == record.phoneNumber; });

    if (it != records.end()) {
        return distance(records.begin(), it);
    }
    else {
        return -1;
    }
}

void convertTextToBinary(const string& textFileName, const string& binaryFileName) {
    ifstream textFile(textFileName);
    ofstream binaryFile(binaryFileName, ios::out | ios::binary);

    if (!textFile.is_open() || !binaryFile.is_open()) {
        cout << "Ошибка при открытии файла" << endl;
        return;
    }

    Record record;
    while (textFile >> record.phoneNumber >> record.address >> record.fullName) {
        binaryFile.write(reinterpret_cast<char*>(&record), sizeof(Record));
    }
    cout << "\nКонвертация прошла успешно\nНажмите на любую кнопку чтобы продолжить";
    textFile.close();
    binaryFile.close();
}

void convertBinaryToText(const string& binaryFileName, const string& textFileName) {
    ifstream binaryFile(binaryFileName, ios::in | ios::binary);
    ofstream textFile(textFileName);

    if (!binaryFile.is_open() || !textFile.is_open()) {
        cout << "Ошибка при открытии файла" << endl;
        return;
    }

    Record record;
    while (binaryFile.read(reinterpret_cast<char*>(&record), sizeof(Record))) {
        textFile << record.phoneNumber << " " << record.address << " " << record.fullName << endl;
    }
    cout << "\nКонвертация прошла успешно\nНажмите на любую кнопку чтобы продолжить";
    binaryFile.close();
    textFile.close();
}

void displayAllRecords(const string& binaryFileName) {
    ifstream binaryFile(binaryFileName, ios::in | ios::binary);

    if (!binaryFile.is_open()) {
        cout << "Ошибка при открытии файла" << endl;
        return;
    }

    Record record;
    cout << "  Номер    |    Адрес   |       ФИО\n";
    cout << "----------------------------------------\n";
    while (binaryFile.read(reinterpret_cast<char*>(&record), sizeof(Record))) {
        cout << record.phoneNumber << "  |  " << record.address << "  |  " << record.fullName << endl;
    }

    binaryFile.close();
}

void DisplayStream(const string& Filename) {
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

void accessRecordByNumber(const string& binaryFileName, int recordNumber) {
    ifstream binaryFile(binaryFileName, ios::in | ios::binary);

    if (!binaryFile.is_open()) {
        cout << "Ошибка при открытии файла" << endl;
        return;
    }

    Record record;
    binaryFile.seekg(recordNumber * sizeof(Record), ios::beg);
    binaryFile.read(reinterpret_cast<char*>(&record), sizeof(Record));

    cout << "Номер: " << record.phoneNumber << ", Адрес: " << record.address << ", ФИО: " << record.fullName << endl;

    binaryFile.close();
}

void deleteRecordByKey(string binaryFileName, const string& key) {
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

    int position = findRecordPosition(records, key);
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

void createTextFileFromPrefix(const string& binaryFileName, const string& prefix) {
    
    ifstream binaryFile(binaryFileName, ios::in | ios::binary);
    ofstream textFile("prefix_" + prefix + ".txt");

    if (!binaryFile.is_open() || !textFile.is_open()) {
        cout << "Ошибка при открытии файла" << endl;
        return;
    }

    Record record;
    while (binaryFile.read(reinterpret_cast<char*>(&record), sizeof(Record))) {
        string phoneNumberStr(record.phoneNumber, 3);
        if (phoneNumberStr == prefix) {
            textFile << record.phoneNumber << " " << record.fullName << endl;
        }
    }

    binaryFile.close();
    textFile.close();
}

void deleteRecordsByPrefix(string binaryFileName, const string& prefix) {
    fstream binaryFile(binaryFileName, ios::in | ios::out | ios::binary);

    if (!binaryFile.is_open()) {
        cout << "Ошибка при открытии файла" << endl;
        return;
    }

    vector<Record> records;
    Record record;
    while (binaryFile.read(reinterpret_cast<char*>(&record), sizeof(Record))) {
        string phoneNumberStr(record.phoneNumber, 1);
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
    string binaryFileName = "filebin2.txt";
    string textFileName;
    cout << "Введите название файла: ";
    cin >> textFileName;
    ifstream fileExistsCheck(textFileName);
    if (!fileExistsCheck) {
        cout << "Файла не существует";
        return 1;
    }
    convertTextToBinary(textFileName, binaryFileName);

    char ch = 0;
    while (ch != 27) {
        system("cls");
        cout << "1. Преобразование данных из текстового файла в двоичный файл\n";
        cout << "2. Преобразование данных из двоичного файла в текстовый\n";
        cout << "3. Вывод всех записей двоичного файла\n";
        cout << "4. Доступ к записи по ее порядковому номеру в файле\n";
        cout << "5. Удаление записи с заданным значением ключа (номер телефона)\n";
        cout << "6. Сформировать текстовый файл из фамилий владельцев, чьи номера начинаются с введенных первых трех цифр\n";
        cout << "7. Вывод файла с фамилиями владельцев, чьи номера начинаются с введенных первых трех цифр (тест 6 пункта)\n";
        cout << "8. Удалить сведения о владельцах телефонов, которые начинаются с заданной цифры\n";
        cout << "\nEsc. Выход\n";
        ch = _getch();
        switch (ch)
        {
        default:
            break;
        case '1':
        {
            convertBinaryToText(binaryFileName, textFileName);
            _getch();
            break;
        }

        case '2':
        {
            convertTextToBinary(textFileName, binaryFileName);
            _getch();
            break;
        }

        case '3':
        {
            system("cls");
            displayAllRecords(binaryFileName);
            _getch();
            break;
        }

        case '4':
        {
            system("cls");
            int recordNumber;
            cout << "Введите порядковый номер: ";
            cin >> recordNumber;
            accessRecordByNumber(binaryFileName, recordNumber - 1);
            _getch();
            break;
        }

        case '5':
        {
            system("cls");
            string key;
            cout << "Введите номер для удаления: ";
            cin >> key;
            deleteRecordByKey(binaryFileName, key);
            _getch();
            break;
        }

        case '6':
        {
            system("cls");
            string prefix;
            cout << "Введите первые 3 цифры номера: ";
            cin >> prefix;
            if (99 < stoi(prefix) < 1000) {
                createTextFileFromPrefix(binaryFileName, prefix);
                cout << "Файл с фамилиями успешно создан" << endl;
            }
            else cout << "Некорректно введённые данные" << endl;
            _getch();
            break;
        }

        case '7':
        {
            system("cls");
            string PrefFile;
            cout << "Введите первые три цифры номера: ";
            cin >> PrefFile;
            PrefFile = "prefix_" + PrefFile + ".txt";
            DisplayStream(PrefFile);
            _getch();
            break;
        }

        case '8':
        {
            system("cls");
            string prefixToDelete;
            cout << "Введите первую цифру номера для удаления: ";
            cin >> prefixToDelete;
            if (0 < stoi(prefixToDelete) < 10) {
                deleteRecordsByPrefix(binaryFileName, prefixToDelete);
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