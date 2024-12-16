#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int permutationsCount = 0;

int longestPalindrome(const string& str) {
    int n = str.length();

    vector<vector<int>> dp(n, vector<int>(n, 0));

    for (int i = 0; i < n; i++) {
        dp[i][i] = 1;
    }

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
            permutationsCount++;
        }
    }

    int maxLength = dp[0][n - 1];
    cout << "Длина наибольшего палиндрома: " << maxLength << endl;

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

    cout << "Наибольший палиндром: ";
    for (int k = 0; k < maxLength; k++) {
        cout << palindrome[k];
    }
    cout << endl;

    return maxLength;
}

int main() {
    string input;
    cout << "Введите строку из заглавных букв латинского алфавита: ";
    cin >> input;

    int result = longestPalindrome(input);

    cout << "Количество переборов: " << permutationsCount << endl;

    return 0;
}
