#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int longestPalindrome(const string& str, long long& count) {
    int n = str.length();

    // Создаем двумерный массив для хранения результатов подзадач
    vector<vector<int>> dp(n, vector<int>(n, 0));

    // Каждая подстрока длины 1 является палиндромом
    for (int i = 0; i < n; i++) {
        dp[i][i] = 1;
    }

    // Заполняем массив для подстрок длины 2 и более
    for (int len = 2; len <= n; len++) {
        for (int i = 0; i <= n - len; i++) {
            int j = i + len - 1;

            if (str[i] == str[j] && len == 2) {
                dp[i][j] = 2;
            }
            else if (str[i] == str[j]) {
                dp[i][j] = dp[i + 1][j - 1] + 2;
            }
            else {
                dp[i][j] = max(dp[i][j - 1], dp[i + 1][j]);
            }

            // Увеличиваем счетчик переборов
            count++;
        }
    }

    // Длина наибольшего палиндрома
    int maxLength = dp[0][n - 1];
    cout << "Length of Longest Palindrome: " << maxLength << endl;

    // Восстановление палиндрома
    vector<char> palindrome(maxLength);
    int i = 0, j = n - 1, index = 0;

    while (i <= j) {
        if (str[i] == str[j]) {
            palindrome[index] = str[i];
            palindrome[maxLength - index - 1] = str[j];
            i++;
            j--;
            index++;
        }
        else if (dp[i][j - 1] > dp[i + 1][j]) {
            j--;
        }
        else {
            i++;
        }
    }

    // Вывод наибольшего палиндрома
    cout << "Longest Palindrome: ";
    for (int k = 0; k < maxLength; k++) {
        cout << palindrome[k];
    }
    cout << endl;

    return maxLength;
}

int main() {
    string input;
    cout << "Enter a string of uppercase letters: ";
    cin >> input;

    long long count = 0;
    int result = longestPalindrome(input, count);

    cout << "Number of iterations: " << count << endl;

    return 0;
}
