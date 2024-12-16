#include <iostream>
#include <queue>
using namespace std;

struct TreeNode {
    char data;
    TreeNode* left;
    TreeNode* right;

    TreeNode(char value) : data(value), left(nullptr), right(nullptr) {}
};

TreeNode* createBalancedTree(int n) {
    if (n <= 0) {
        return nullptr;
    }
    queue<TreeNode*> nodes;
    TreeNode* root = new TreeNode('A');
    nodes.push(root);

    for (int i = 1; i < n; i++) {
        TreeNode* current = nodes.front();
        nodes.pop();

        current->left = new TreeNode('A' + i);
        nodes.push(current->left);

        if (i < n - 1) {
            current->right = new TreeNode('A' + i + 1);
            nodes.push(current->right);
            i++;
        }
    }

    return root;
}

void PrintTreeAntiClock(TreeNode* root, int space = 0) {
    if (!root)
        return;
    int COUNT = 2;
    space += 2;
    PrintTreeAntiClock(root -> right, space);
    for (int i = 2; i < space; ++i)
        cout << "  ";
    cout << root -> data << endl;
    PrintTreeAntiClock(root -> left, space);
}

TreeNode* findLeftmostNode(TreeNode* root) {
    while (root->left) {
        root = root->left;
    }
    return root;
}

int findPathLength(TreeNode* root, char target) {
    int depth = 0;
    while (root) {
        if (root->data == target) {
            return depth;
        }
        if (target < root->data) {
            root = root->left;
        }
        else {
            root = root->right;
        }
        depth++;
    }
    return -1;
}

char findMaxLeafValue(TreeNode* root) {
    if (!root) {
        return '\0';
    }
    if (!root->left && !root->right) {
        return root->data;
    }
    char leftMax = findMaxLeafValue(root->left);
    char rightMax = findMaxLeafValue(root->right);
    return max(root->data, max(leftMax, rightMax));
}

int main() {
    int n;
    cout << "Введите количество узлов: ";
    cin >> n;
    TreeNode* root = createBalancedTree(n);
    int choice;
    char targetValue;
    int pathLength = 1;
    char maxLeafValue = '\0';
    TreeNode* leftmostNode = nullptr;
    PrintTreeAntiClock(root);
    do {
        
        cout << "\nВыберите операцию:" << endl
         << "1. Найти самый левый узел" << endl
         << "2. Найти длину пути до узла с заданным значением" << endl 
         << "3. Найти максимальное значение среди листьев" << endl
         << "0. Выйти" << endl;
        cin >> choice;

        switch (choice) {
        case 1:
            leftmostNode = findLeftmostNode(root);
            cout << "Самый левый узел: " << leftmostNode->data << endl;
            break;
        case 2:
            cout << "Введите значение для поиска длины пути: ";
            cin >> targetValue;
            pathLength = findPathLength(root, targetValue);
            if (pathLength >= 0) {
                cout << "Длина пути до '" << targetValue << "': " << pathLength << endl;
            }
            else {
                cout << "Значение '" << targetValue << "' не найдено в дереве." << endl;
            }
            break;
        case 3:
            maxLeafValue = findMaxLeafValue(root);
            cout << "Максимальное значение среди листьев: " << maxLeafValue << endl;
            break;
        case 0:
            cout << "Выход из программы." << endl;
            break;
        default:
            cout << "Неверный выбор операции. Попробуйте снова." << endl;
        }
    } while (choice != 0);

    return 0;
}
