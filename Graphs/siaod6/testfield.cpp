#include <iostream>
#include <climits>
#include <conio.h>
#include <vector>
#include <stack>
#include <algorithm>
#include <set>
using namespace std;

class Graph {
private:
    int** AdjMatrix;
    int NumVertices;

    int* parent;
    int* key;

public:
    Graph(int NumVertices) {
        this -> NumVertices = NumVertices;
        AdjMatrix = new int* [NumVertices];
        for (int i = 0; i < NumVertices; i++) {
            AdjMatrix[i] = new int[NumVertices];
            for (int j = 0; j < NumVertices; j++) AdjMatrix[i][j] = 0;
        }
        parent = new int[NumVertices];
        key = new int[NumVertices];
        for (int i = 0; i < NumVertices; ++i) {
            parent[i] = -1; // Инициализация всех вершин как не принадлежащих остовному дереву
            key[i] = INT_MAX; // Установка бесконечного веса для всех вершин
        }
    }



    void AddEdge(int i, int j, int weight, bool direct) {
        if (i >= 0 && i < NumVertices && j >= 0 && j < NumVertices) {

            if (direct == 0) AdjMatrix[i][j] = weight;
            else AdjMatrix[i][j] = AdjMatrix[j][i] = weight;           

            cout << "Грань успешно добавлена" << endl;
            parent[j] = i; // Устанавливаем вершину j в качестве родительской для вершины i
            key[j] = weight; // Устанавливаем вес ребра между i и j
        }

        else cout << "Некоректно введённые данные" << endl;
    }

    void AddNode() {
        int** newMatrix = new int* [NumVertices + 1];
        for (int i = 0; i <= NumVertices; i++) {
            newMatrix[i] = new int[NumVertices + 1];
            for (int j = 0; j <= NumVertices; j++) newMatrix[i][j] = 0;
        }

        for (int i = 0; i < NumVertices; i++) {
            for (int j = 0; j < NumVertices; j++) newMatrix[i][j] = AdjMatrix[i][j];    
        }

        delete[] AdjMatrix;
        AdjMatrix = newMatrix;
        NumVertices++;

        cout << "Узел успешно добавлен" << endl;
    }

    void DisplayGraph() {
        cout << "   ";
        for (int i = 0; i < NumVertices; i++) cout << i << "\t";

        cout << "\n";
        for (int i = 0; i < NumVertices; i++) {
            cout << i << ": ";
            for (int j = 0; j < NumVertices; j++) {

                if (AdjMatrix[i][j] == 0) cout << "0\t";                
                else cout << AdjMatrix[i][j] << "\t";
                
            }
            cout << "\n";
        }
    }
    
    void CleanUp() {
        for (int i = 0; i < NumVertices; i++) {
            AdjMatrix[i] = new int[NumVertices];
            for (int j = 0; j < NumVertices; j++)  AdjMatrix[i][j] = 0;
        }
        cout << "Граф успешно очищен";
    }

  /////////////////////////////////////////////////////////////////////////
    
    vector<int> FindEulerianCycle(int startVertex) {
        vector<int> eulerianCycle;

        // Копирование матрицы смежности, чтобы не изменять исходную матрицу
        int** tempMatrix = new int* [NumVertices];
        for (int i = 0; i < NumVertices; ++i) {
            tempMatrix[i] = new int[NumVertices];
            for (int j = 0; j < NumVertices; ++j) {
                tempMatrix[i][j] = AdjMatrix[i][j];
            }
        }

        stack<int> stack;
        stack.push(startVertex);

        while (!stack.empty()) {
            int u = stack.top();
            int v;

            for (v = 0; v < NumVertices; ++v) {
                if (tempMatrix[u][v] > 0) {
                    break;
                }
            }

            if (v == NumVertices) {
                // Если все смежные ребра были удалены
                eulerianCycle.push_back(u);
                stack.pop();
            }
            else {
                // Иначе добавляем ребро в цикл и удаляем его из графа
                stack.push(v);
                tempMatrix[u][v] = tempMatrix[v][u] = 0;
            }
        }

        // Освобождение временной матрицы
        for (int i = 0; i < NumVertices; ++i) {
            delete[] tempMatrix[i];
        }
        delete[] tempMatrix;

        return eulerianCycle;
    }

    vector<int> FindEulerianCycleWithStart() {
        for (int startVertex = 0; startVertex < NumVertices; ++startVertex) {
            vector<int> eulerianCycle = FindEulerianCycle(startVertex);
            if (eulerianCycle.front() == eulerianCycle.back() && eulerianCycle.size() != 1) {
                cout << "Эйлеров цикл найден, начальная вершина: " << startVertex << endl;
                for (int vertex : eulerianCycle) cout << vertex << " ";
                return eulerianCycle;
            }
        }
        cout << "Эйлеров цикл не найден" << endl;
        return vector<int>();
    }
    //////////////////////////////////////////////////////////
    void PrimMST() {
        vector<int> parent(NumVertices, -1);
        vector<int> key(NumVertices, INT_MAX);
        vector<bool> inMST(NumVertices, false);

        key[0] = 0;

        for (int count = 0; count < NumVertices - 1; ++count) {
            int u = -1;

            for (int v = 0; v < NumVertices; ++v) {
                if (!inMST[v] && (u == -1 || key[v] < key[u]))
                    u = v;
            }

            inMST[u] = true;

            for (int v = 0; v < NumVertices; ++v) {
                if (AdjMatrix[u][v] && !inMST[v] && AdjMatrix[u][v] < key[v]) {
                    parent[v] = u;
                    key[v] = AdjMatrix[u][v];
                }
            }
        }

        displayMST(parent);
    }

    // Add this method to the Graph class
    void displayMST(const vector<int>& parent) {
        vector<pair<int, pair<int, int>>> sortedEdges; // Pair of (Weight, (Vertex1, Vertex2))

        for (int i = 1; i < NumVertices; ++i) {
            sortedEdges.push_back({ AdjMatrix[i][parent[i]], {parent[i], i} });
        }

        // Sort the edges based on the first digit of vertices
        sort(sortedEdges.begin(), sortedEdges.end(), [](const auto& a, const auto& b) {
            return a.second.first % 10 < b.second.first % 10;
            });

        cout << "Edge\tWeight\n";
        for (const auto& edge : sortedEdges) {
            cout << edge.second.first << " - " << edge.second.second << "\t" << edge.first << "\n";
        }
    }
};

int main() {
    int NumNodes;
    cout << "Введите количество узлов в графе: ";
    cin >> NumNodes;
    Graph graph(NumNodes);
    char ch = 0;
    while (ch != 27) {
        system("cls");
        
        graph.DisplayGraph();
        cout << "\n1. Добавить узел\n2. Добавить грань\n3. Очистить граф\n"
            <<"4. Вывод эйлерова цикла\n5. Алгоритм Прима\n"
            <<"8. Ввод тестового значения 1(Эйлеров цикл есть)\n9. Ввод тестового значения 2(Эйлеров цикл отсутсвует)\n"
            <<"0. Ввод тестового значения 3(Алгоритм Прима)\n\nEsc.Выход из программы";

        ch = _getch();
        int result = 0;
        switch (ch) {
            default:
                break;
            case '1':
                system("cls");
                graph.AddNode();
                _getch();
                break;
            case '2': {
                int node1, node2, weight;
                bool direct;
                cout << "\n\nВведите номер вершины 1: ";
                cin >> node1;
                cout << "Введите номер вершины 2: ";
                cin >> node2;
                if (node1 == node2) {cout << "Ошибочно введённые данные, попробуйте ещё раз"; _getch(); break;}
                cout << "Введите вес грани: ";
                cin >> weight;
                cout << "Грань двунаправленная? (1 – да, 0 – нет): ";
                cin >> direct;
                graph.AddEdge(node1, node2, weight, direct);
                _getch();
                break;
            }
            case '3':{
                system("cls");
                graph.CleanUp();
                _getch();
                break;
            }
            case '4': {
                system("cls");
                vector<int> eulerianCycle = graph.FindEulerianCycleWithStart();
                _getch();
                break;
            }

            case '5': {
                system("cls");
                graph.PrimMST();
                _getch();
                break;
            }
            case '8':
                system("cls");
                graph.AddEdge(1, 2, 1, 0);
                graph.AddEdge(2, 3, 1, 0);
                graph.AddEdge(3, 4, 1, 0);
                graph.AddEdge(4, 1, 1, 1);
                _getch();
                break;
            case '9':
                system("cls");
                graph.AddEdge(1, 2, 1, 0);
                graph.AddEdge(1, 3, 1, 0);
                graph.AddEdge(3, 4, 1, 0);
                graph.AddEdge(1, 0, 1, 0);
                _getch();
                break;
            case '0':
                system("cls");
                graph.AddEdge(0, 1, 7, 1);
                graph.AddEdge(0, 3, 5, 1);
                graph.AddEdge(1, 2, 8, 1);
                graph.AddEdge(1, 3, 9, 1);                
                graph.AddEdge(1, 4, 7, 1);
                graph.AddEdge(2, 4, 5, 1);
                graph.AddEdge(3, 4, 15, 1);
                graph.AddEdge(3, 5, 6, 1);
                graph.AddEdge(4, 5, 8, 1);
                graph.AddEdge(4, 6, 9, 1);
                graph.AddEdge(5, 6, 11, 1);
                _getch();
                break;
            case 'q':
                system("cls");
                graph.AddEdge(0, 1, 3, 1);
                graph.AddEdge(0, 2, 4, 1);
                graph.AddEdge(0, 4, 1, 1);
                graph.AddEdge(1, 2, 5, 1);
                graph.AddEdge(2, 3, 2, 1);
                graph.AddEdge(2, 4, 6, 1);
                graph.AddEdge(3, 4, 7, 1);
                _getch();
                break;
            case 'w':
                system("cls");
                graph.AddEdge(0, 1, 9, 1);
                graph.AddEdge(0, 2, 75, 1);
                graph.AddEdge(1, 2, 95, 1);
                graph.AddEdge(1, 3, 19, 1);
                graph.AddEdge(1, 4, 42, 1);
                graph.AddEdge(2, 3, 51, 1);
                graph.AddEdge(2, 4, 66, 1);
                graph.AddEdge(3, 4, 31, 1);
                _getch();
                break;
        }

    }
    cout << "\nВыход";
    return 0;    
}