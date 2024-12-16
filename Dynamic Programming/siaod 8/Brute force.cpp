#include <iostream>
#include <string>
#include <algorithm>
int permutationsCount = 0;
using namespace std;

bool isPalindrome(const string& s) {
    int left = 0;
    int right = s.length() - 1;

    while (left < right) {
        if (s[left] != s[right]) {
            return false;
        }
        left++;
        right--;
    }

    return true;
}

string findLargestPalindrome(const string& input) {
    string largestPalindrome;

    for (int i = 0; i < (1 << input.length()); i++) {
        string candidate;

        for (int j = 0; j < input.length(); j++) {
            if ((i & (1 << j)) != 0) {
                candidate += input[j];
            }
        }

        permutationsCount++;

        if (isPalindrome(candidate) && candidate.length() > largestPalindrome.length()) {
            largestPalindrome = candidate;
        }
    }

    return largestPalindrome;
}

int main() {
    string input;
    cout << "Введите строку из заглавных букв латинского алфавита: ";
    cin >> input;

    string result = findLargestPalindrome(input);

    cout << "Длина наибольшего палиндрома: " << result.length() << endl;
    cout << "Наибольший палиндром: " << result << endl;
    cout << "Количество переборов: " << permutationsCount << endl;

    return 0;
}
