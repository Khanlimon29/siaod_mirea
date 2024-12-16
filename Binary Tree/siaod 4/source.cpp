#include <iostream>
#include <queue>
using namespace std;

struct TreeNode {
    char data;
    TreeNode* left;
    TreeNode* right;
    TreeNode(char value) : data(value), left(nullptr), right(nullptr) {}
};

TreeNode* CreateTree(int n) {
    if (n <= 0) {
        return nullptr;
    }
    queue<TreeNode*> nodes;
    TreeNode* root = new TreeNode('A');
    nodes.push(root);

    for (int i = 1; i < n; i++) {
        TreeNode* current = nodes.front();
        nodes.pop();

        current -> left = new TreeNode('A' + i);
        nodes.push(current -> left);

        if (i < n - 1) {
            current -> right = new TreeNode('A' + i + 1);
            nodes.push(current -> right);
            i++;
        }
    }
    return root;
}

void PrintTreeAntiClock(TreeNode* root, int space = 0) {
    if (!root)
        return;
    space += 2;
    PrintTreeAntiClock(root -> right, space);

    for (int i = 2; i < space; ++i) cout << "  ";

    cout << root -> data << endl;
    PrintTreeAntiClock(root -> left, space);
}

TreeNode* findLeftmostNode(TreeNode* root) {
    while (root -> left) {
        root = root -> left;
    }
    return root;
}

int findPathLength(TreeNode* root, char target) {
    if (!root) {
        return -1;
    }
    if (root -> data == target) {
        return 0;
    }
    int leftPathLength = findPathLength(root -> left, target);
    int rightPathLength = findPathLength(root -> right, target);

    if (leftPathLength == -1 && rightPathLength == -1) {
        return -1;
    }
    else return 1 + max(leftPathLength, rightPathLength);
}

char findMaxLeafValue(TreeNode* root) {
    if (!root) {
        return '\0';
    }
    if (!root -> left && !root -> right) {
        return root -> data;
    }
    char leftMax = findMaxLeafValue(root -> left);
    char rightMax = findMaxLeafValue(root -> right);
    return max(root -> data, max(leftMax, rightMax));
}

int main() {
    int n = 0;
    cout << "Задайте количество узлов в дерево: ";
    cin >> n;

    TreeNode* root = CreateTree(n);
    PrintTreeAntiClock(root);

    TreeNode* leftmostNode = findLeftmostNode(root);
    cout << "Самый левый узел: " << leftmostNode->data << endl;

    cout << "Введите значения узла для поиска длины пути: ";
    char temp;
    cin >> temp;
    char targetValue = temp;
    int pathLength = findPathLength(root, targetValue);
    if (pathLength >= 0) {
        cout << "Длина пути до '" << targetValue << "': " << pathLength << endl;
    }
    else {
        cout << "Значение '" << targetValue << "' не найдено в дереве." << endl;
    }

    char maxLeafValue = findMaxLeafValue(root);
    cout << "Максимальное значение среди листьев: " << maxLeafValue << endl;

    return 0;
}
