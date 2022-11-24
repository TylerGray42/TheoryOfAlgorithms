#include <iostream>
#include <utility>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

/*
6 7

0 1
0 5
0 4
1 2
1 4
4 3
2 3
*/


bool anyFalse(vector<int> &a) {
    for (int i: a) {
        if (!i) return true;
    }
    return false;
}


typedef struct {
    int start;
    int end;
    int weight;
} Edge;


/**
 * @brief Класс, содержащий ребра графа
 * 
 * @param edges vector<Edge>; ребра графа.
 * @param numOfVertices int; количевство вершин.
 * @param directed bool; true - ориентированный граф, false - неориентированный граф.
 */
class Graph {

    public:
        vector<vector<Edge>> agjList;
    private:
        bool directed;
        vector<int> discovered; // посещенные вершины
        int size; // кол-во вершин


    public: Graph(vector<Edge> const &edges, int numOfVertices, bool directed) : directed(directed), size(numOfVertices) {

            agjList.resize(numOfVertices);
            discovered.resize(numOfVertices);

            for (Edge edge : edges) {
                agjList[edge.start].push_back(edge);
                if (!directed) {
                    Edge ed = {edge.end, edge.start, edge.weight};
                    agjList[edge.end].push_back(ed);
                }
            }
        }

    public: friend ostream& operator<<(ostream &out, const Graph& graph) {
            for (int i = 0; i < graph.size; ++i) {
                out << i << ": ";

                for (Edge j: graph.agjList[i]) {
                    out << j.end << "(" << j.weight << ") ";
                }

                out << endl;
            }

            return out;
        }
    

    /**
     * @brief Заполнение весов ребер графа случайными числами
     * 
     * @param min int минимальное значение веса
     * @param max int максимальное значение веса
     */
    public: void random_weights(int min, int max) {
            srand(time(NULL));
            if (directed) {
                for (int i = 0; i < size; i++) {
                    int neigh = agjList[i].size();
                    for (int j = 0; j < neigh; j++) {
                        agjList[i][j].weight = rand() % (max - min) + min;
                    }
                }
            } else {
                for (int i = 0; i < size; i++) {
                    for (Edge &j : agjList[i]) {
                        if (j.start < j.end) {
                            j.weight = rand() % (max - min) + min;
                            for (Edge &l : agjList[j.end]) {
                                if (l.end == j.start) {
                                    l.weight = j.weight;
                                    break;
                                }
                            }
                        } else continue;
                    }
                }
            }
        }


    /**
     * @brief Необходимо вызывать перед обходом графа.
     */
    public: void init_search() {
            for (int i = 0; i < size; ++i) {
                discovered[i] = 0;
            }
        }
    

    /**
     * @brief Обход графа в ширину.
     * 
     * @param start вершина, с которой начинается обход.
     */
    public: void bfs(int start) {
            queue<int> q;
            int v;
            q.push(start);
            discovered[start] = 1;

            while (!q.empty()) {
                v = q.front();
                q.pop();
                for (Edge neighbor: agjList[v]) {
                    if (!discovered[neighbor.end]) {
                        cout << "(" << v << ", " << neighbor.end << ")\n";
                        q.push(neighbor.end);
                        discovered[neighbor.end] = 1;
                    }
                }
            }
        }
    

    /**
     * @brief Обход графа в глубину.
     * 
     * @param start вершина, с которой начинается обход.
     */
    public: void dfs(int start) {
            if (discovered[start]) return;
            discovered[start] = 1;
            for (Edge neighbor: agjList[start]) {
                if (!discovered[neighbor.end]) {
                    cout << "(" << start << ", " << neighbor.end << ")\n"; 
                    dfs(neighbor.end);
                }

                //discovered[neighbor.end] = 1;
            }
        }

    /**
     * @brief Алгоритм Дейкстры
     * 
     * @param start
     * @param end
     */
    public: void Dijkstra(int start, int end) {
        vector<int> edgeWeights;
        vector<int> shortPath; 
        queue<int> q;
        int v;

        for (int i = 0; i < size; i++) {
            shortPath.push_back(start);
            edgeWeights.push_back(INT_MAX);
        }

        edgeWeights[start] = 0;
        q.push(start);
        while (anyFalse(discovered)) {
            v = q.front();
            q.pop();
            for (Edge &neighbor: agjList[v]) {
                if (edgeWeights[neighbor.end] > (edgeWeights[neighbor.start] + neighbor.weight)) {
                    edgeWeights[neighbor.end] = edgeWeights[neighbor.start] + neighbor.weight;
                    shortPath[neighbor.end] = neighbor.start;
                }
                q.push(neighbor.end);
            }

            discovered[v] = true;
        }

        // cout << "\nPath weight:\n";
        // for (int i = 0; i < edgeWeights.size(); i++) {
        //     cout << "to " << i << " - " << edgeWeights[i] << "; ";
        // }
        // cout << "\b\b  " << endl;

        // cout << "\nPath:\n";
        // for (int i = 0; i < shortPath.size(); i++) {
        //     cout << "from " << shortPath[i] << " to " << i << "; ";
        // }
        // cout << "\b\b  " << endl;

        int step = end; 
        while (step != start) {
            cout << step << " <- ";
            step = shortPath[step];
        }

        cout << step << "\n";
    }


    /**
     * @brief Алгоритм A*. Не для этой задачи.
     */
    public: void A(int start, int end);

};


int main() {

    int numOfVertices, numOfEdges, start, end;
    vector<Edge> edges;

    cout << "Введите кол-во вершин и ребер: ";
    cin >> numOfVertices >> numOfEdges;

    // Вершины необходимо нумеровать начиная с 0
    cout << "Введите вершины прилежащие к ребрам (начало и конец):\n";
    for (int i = 0; i < numOfEdges; i++) {
        cin >> start >> end;
        edges.push_back({start, end, 0});
    }

    Graph graph(edges, numOfVertices, false);
    graph.random_weights(1, 10);
    cout << endl << graph;

    cout << endl << "Обход в ширину:\n";
    graph.init_search();
    graph.bfs(1);

    cout << endl << "Обход в глубину:\n";
    graph.init_search();
    graph.dfs(0);

    cout << endl << "Обход Дейкстры: \n";
    graph.init_search();
    graph.Dijkstra(0, 3);

    return 0;
}