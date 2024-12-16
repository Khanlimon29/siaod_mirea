#include <iostream>
#include <string>
#include <conio.h>
#include <iomanip>

using namespace std;

string nToBin(int n) {
	int nDec = n;
	string nBin;
	while (n) {
		if ((n % 2) == 1) nBin = "1" + nBin;
		else nBin = "0" + nBin;
		n /= 2;
	}
	return nBin + " " + to_string(nDec);
}

int EvenTo1(int n) {
	unsigned int mask = 0x555; // 10101010101
	return n | mask;
}

int BitsTo0(int n) {
	unsigned int mask = ~((1 << 7) | (1 << 9) | (1 << 11));
	return n & mask;
}

int mult16(int n) {
	return n << 4;
}

int div16(int n) {
	return n >> 4;
}

int nTo0Mask(int n, int bit) {
	unsigned int mask = 1 << bit;
	return n & ~mask;
}

int main() {
	setlocale(0, "");
	const int n = 0xAB1;
	int num = 0;
	char ch = 0;
	while (ch != 27) {
		system("cls");
		cout << "Константа: " << n
			 << "\n1. Установить все чётные биты в значение 1\n2. Обнулить 7-ой, 9-ый и 11-ый биты\n"
			 <<"3. Умножить число на 16\n4. Поделить число на 16\n"
			 <<"5. Обнулить n-ый бит, используя маску 1 (единица в младшем разряде)\n\nEsc. Выход";
		ch = _getch();
		switch (ch)
		{
		default:
			break;
		case '1':
			system("cls");
			cout << nToBin(n) << endl;
			cout << nToBin(EvenTo1(n)) << endl;			
			_getch();
			break;
		case '2':
			system("cls");
			int num;
			cout << "Введите число: ";
			cin >> num;
			cout << nToBin(num) << endl;
			cout << nToBin(BitsTo0(num)) << endl;
			_getch();
			break;
		case '3':
			system("cls");
			cout << "Введите число: ";
			cin >> num;
			cout << setw(49) << nToBin(num) << endl << setw(50) << nToBin(mult16(num));			
			_getch();
			break;
		case '4':
			system("cls");
			cout << "Введите число: ";
			cin >> num;
			cout << setw(50) << nToBin(num) << endl << setw(49) << nToBin(div16(num));			
			_getch();
			break;
		case '5':
			system("cls");
			int bit;
			cout << "Введите число: ";
			cin >> num;
			cout << "Введите номер бита: ";
			cin >> bit;
			cout << setw(49) << nToBin(num) << endl << setw(49) << nToBin(nTo0Mask(num,bit));		
			_getch();
			break;
		}
	}
	return 0;
}