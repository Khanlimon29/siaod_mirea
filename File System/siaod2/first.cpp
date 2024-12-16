#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <conio.h>

using namespace std;


void CreateFile(const string& filename) {
    ofstream outFile(filename);
    fstream file(filename, ios::out);

    if (!file.is_open()) {
        cout << "Не удалось создать файл: " << filename;
        return;
    }
    cout << "Файл успешно создан: " << filename;

    for (int i = 1; i <= 23; ++i) {
        outFile << i << " ";
        if (i % 10 == 0) { outFile << endl; }
    }
    outFile.close();
}

void DisplayFile(const string& filename) {
    ifstream inFile(filename);
    fstream file(filename, ios::in);
    if (!inFile) {
        cout << "Не удалось открыть файл: " << filename;
        return;
    }
    string line;
    while (getline(inFile, line)) {
        cout << line << endl;
    }
    inFile.close();
}

void NewEndLine(const string& filename, const string& newLine) {
    ofstream outFile(filename, ios_base::app);
    if (!outFile) {
        cout << "Не удалось открыть файл для добавления: " << filename;
        return;
    }

    outFile << "\n" << newLine;
    cout << "Новая строка успешно добавлена в файл: " << filename;
    outFile.close();
}

int ReadNumberByIndex(const string& filename, int index) {
    ifstream inFile(filename);
    if (!inFile) {
        cout << "Не удалось открыть файл: " << filename;
        return -1;
    }

    int value;
    int currentIndex = 0;

    while (inFile >> value) {
        currentIndex++;
        if (currentIndex == index) {
            inFile.close();
            return value;
        }
    }

    inFile.close();
    cout << "Число с указанным порядковым номером не найдено.";
    return -1;
}

int CountNumbers(const string& filename) {
    ifstream inFile(filename);
    if (!inFile) {
        cout << "Не удалось открыть файл: " << filename;
        return -1;
    }

    int count = 0;
    int value;
    while (inFile >> value) {
        count++;
    }

    inFile.close();
    return count;
}

void NewFirstLine(const string& filename, const string& newLine) {
    ifstream FileName(filename);
    if (!FileName) {
        cout << "Не удалось открыть файл: " << filename << endl;
        return;
    }

    string TempFileName = "newline" + filename;
    ofstream TempFile(TempFileName);
    if (!TempFile) {
        cout << "Не удалось создать временный файл." << endl;
        FileName.close();
        return;
    }

    TempFile << newLine << endl;
    string line;
    while (getline(FileName, line)) {
        TempFile << line << endl;
    }

    FileName.close();
    TempFile.close();

    system("cls");
    DisplayFile(TempFileName);
    if (remove(filename.c_str()) != 0) {
        cout << "Не удалось удалить исходный файл." << endl;
        return;
    }

    if (rename(TempFileName.c_str(), filename.c_str()) != 0) {
        cout << "Не удалось переименовать временный файл." << endl;
    }
}

int main() {
    string filename;
    cout << "Введите имя файла: ";
    cin >> filename;
    CreateFile(filename);
    string newLine;
    char ch = 0;
    while (ch != 27) {
        system("cls");
        cout << "Содержимое файла:\n";
        DisplayFile(filename);
        cout << "\nВыберите действие:\n"
             << "1. Создать файл с числами\n"
             << "2. Добавить новую строку в конец файла\n"               
             << "3. Прочитать число по порядковому номеру\n"
             << "4. Определить количество чисел в файле\n"
             << "5. Добавить новую строку перед первой строкой\n"
             << "\nEsc: Выход";
        ch = _getch();
        switch (ch)
        {
        default:
            break;
        case '1':
        {
            system("cls");
            CreateFile(filename);
            _getch();
            break;
        }
        case '2':
        {
            system("cls"); 
            cin.ignore();
            cout << "Введите новую строку: ";
            string NewLine;
            cin >> NewLine;
            NewEndLine(filename, NewLine);
            _getch();
            break;
        }
        case '3':
        {
            system("cls");
            int index;
            cout << "Введите порядковый номер числа: ";
            cin >> index;
            int number = ReadNumberByIndex(filename, index);
            if (number != -1) {
                cout << "Число с порядковым номером " << index << " равно " << number << endl;
            }
            _getch();
            break;
        }
        case '4':
        {
            system("cls");
            int count = CountNumbers(filename);
            if (count != -1) {
                cout << "Количество чисел в файле: " << count << endl;
            }
            _getch();
            break;
        }
        case '5':
        {
            system("cls");
            cin.ignore();
            cout << "Введите новую строку: ";
            string NewLine;
            cin >> NewLine;
            NewFirstLine(filename, NewLine);
            _getch();
            break;
        }
        }
    }
    cout << "\nВыход";
    return 0;
}