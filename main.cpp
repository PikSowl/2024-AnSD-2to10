#include <iostream>
#include <fstream>
#include <queue>
#include <algorithm>
#include <vector>

#define intLimit 2147483647
#define matrixSize 10

using std::cin;
using std::cout;
using std::ifstream;
using std::string;
using std::swap;
using std::endl;
using std::queue;
using std::vector;

struct edge {
    int a, b, cost;
};

int readUnGraph(vector<vector<int>>& graph, const string& filename){
    ifstream f(filename);
    int hight = 0, length = 0;
    char h;
    bool newLine = true;
    vector<int> line;

    while (f.peek() != EOF) {
        f >> h;
        switch (h) {
            case '|':
            case '/':
            case '\\':
                newLine = !newLine;
                if (newLine) {
                    graph.push_back(line);
                    hight++;
                    length = 0;
                    line = {};
                }
                break;
            case EOF:
                f.close();
            default: {
                if ('1' == h){
                    line.push_back (length);
                }
                length++;
            }
        }
    }
    return hight;
}

int readEdges(vector<edge>& edges, int& eCount, const string& filename){
    ifstream f(filename);
    int hight = 0, length = 0;
    char h;
    bool newLine = true;
    vector<int> line;

    while (f.peek() != EOF) {
        f >> h;
        switch (h) {
            case '|':
            case '/':
            case '\\':
                newLine = !newLine;
                if (newLine) {
                    hight++;
                    length = 0;
                }
                break;
            case EOF:
                f.close();
            default: {
                if ('0' != h) {
                    edge e{};
                    e.a = hight;
                    e.b = length;
                    e.cost = int(h) - 48;
                    edges.push_back(e);
                    eCount++;
                }
                length++;
            }
        }
    }
    return hight;
}

int readMatrix(int(&matrix)[matrixSize][matrixSize], const string& filename){
    ifstream f(filename);
    int length = 0, hight = 0;
    char h;
    bool newLine = true;

    while (f.peek() != EOF) {
        f >> h;
        switch (h) {
            case '|':
            case '/':
            case '\\':
                newLine = !newLine;
                if (newLine) {
                    hight++;
                    length = 0;
                }
                break;
            default: {
                int temp = int(h) - 48;
                while (f.peek() != ' ') {
                    f >> h;
                    temp = 10 * temp + (int(h) - 48);
                }
                matrix[hight][length] = temp;
                length++;
            }
        }
    }
    return hight;
}

void writeComponents(vector<vector<int>>& components){
    for (int i = 0; components.size() > i; ++i) {
        cout << '(';
        for (int j = 0; components[i].size() > j + 1 ; ++j) {
            cout << components[i][j] + 1 << ", ";
        }
        cout << components[i][components[i].size() - 1] + 1 << ')' << endl;
    }
}

void AtoB(int &source, int &destination, int &hight) {
    cout << "Input source and destination vertex:";
    cin >> source >> destination;
    source--; destination--;
    if (hight <= source || hight <= destination){
        cout << "out of bounds";
        return;
    }
}

void Ato(int &source, int &hight) {
    cout << "Input source vertex:";
    cin >> source;
    source--;
    if (hight <= source){
        cout << "out of bounds";
        return;
    }
}

void lab_2(){
    vector<vector<int>> graph;
    int hight = readUnGraph(graph, "Matrix2.txt");
    int source, destination;
    AtoB(source, destination, hight);
    queue<int> queue;
    queue.push(source);
    bool visited[hight];
    int minPath[hight], ancestor[hight];
    for(int i = 0; hight > i; i++){
        minPath[i] = 0;
        ancestor[i] = 0;
        visited[i] = false;
    }
    visited[source] = true;
    ancestor[source] = -1;
    while (!queue.empty()) {
        int v = queue.front();
        queue.pop();
        for (int i = 0; graph[v].size() > i; ++i) {
            int to = graph[v][i];
            if (!visited[to]) {
                visited[to] = true;
                queue.push (to);
                minPath[to] = minPath[v] + 1;
                ancestor[to] = v;
            }
        }
    }
    if (!visited[destination])
        cout << "No path!" << endl;
    else {
        vector<int> path;
        for (int v=destination; -1 != v; v=ancestor[v])
            path.push_back (v);
        cout << "Path: ";
        for (int i = path.size() - 1; 0 < i; --i)
            cout << path[i] + 1 << " - ";
        cout << path[0] + 1 << endl;
    }

}

void lab_3() {
    vector<vector<int>> graph;
    vector<vector<int>> components;
    vector<int> comp;
    int count = 0;
    int hight = readUnGraph(graph, "Matrix3.txt");
    queue<int> queue;
    queue.push(0);
    bool visited[hight];
    for (int i = 0; hight > i; i++) {
        visited[i] = false;
    }
    visited[0] = true;
    comp.push_back(0);
    while (!queue.empty()) {
        int v = queue.front();
        queue.pop();
        for (int i = 0; graph[v].size() > i; ++i) {
            int to = graph[v][i];
            if (!visited[to]) {
                visited[to] = true;
                queue.push(to);
                comp.push_back(to);
            }
        }
        if (queue.empty()) {
            components.push_back(comp);
            comp = {};
            count++;
            for (int j = 0; hight > j; j++) {
                if (!visited[j]){
                    queue.push(j);
                    j = hight;
                }
            }
        }
    }

    for (auto & component : components)
        std::sort(component.begin(), component.end());


    cout << "Components: " << endl;
    writeComponents(components);
    cout << "Component count: " << count << endl;
}

void dfs1 (int &v, vector<vector<int>>& g, int &hight , vector<bool>& visited, vector<int>& comp) {
    visited[v] = true;
    int temp;
    for (int i = 0; g[v].size() > i; ++i){
        temp = g[v][i];
        if (!visited[temp])
            comp.push_back(temp);
    }
    comp.push_back(v);
}

void dfs2 (int &v, vector<vector<int>>& gr, int &hight , vector<bool>& visited, vector<int>& comp) {
    visited[v] = true;
    comp.push_back(v);
    for (int i = 0; gr[v].size() > i; ++i)
        if (!visited[gr[v][i] ])
            comp.push_back(gr[v][i]);
}
/*
void lab_4(){
    vector<vector<int>> graph;
    vector<vector<int>> components;
    vector<int> comp;
    int count = 0;
    int hight = readUnGraph(graph, "Matrix3.txt");


    vector<int> graph[100000];
    bool used[100000];
    int comp[100000];

    void dfs(int v, int c_num) {    //c_num - номер текущей компоненты связности.
        used[v] = true;
        comp[v] = c_num;

        for (int u: graph[v]) {
            if (!used[u]) {
                dfs(u, c_num);
            }
        }
    }

    int main() {
        //Ввод графа...

        int c_num = 1;  //Номер очередной компоненты.
        //Нумеровать можно как с 0, так и с 1, как вам удобнее.

        for (int i = 0; i < n; i++) {
            if (!used[i]) {     //если мы ещё не посетили эту вершину, обходя одну из предыдущих
                dfs(i, c_num);
                c_num++;
            }
        }

        for (int i = 0; i < n; i++) {
            cout << "Vertex " << i + 1 << " belongs to component " << comp[i] << endl;
        }
    }
}
*/
/*
void lab_5(){
    vector<vector<int>> graph;
    int hight = readUnGraph(graph, "Matrix2.txt");
    vector<vector<int>> components, g, gr;
    vector<int> order, component, gSmal, grSmal;
    vector<bool> visited;

    int con;
    for (int i = 0; hight > i; i++) {
        for (int j = 0; graph[i].size() > j; j++) {
            con = graph[i][j];
            gSmal.push_back(con);
            grSmal.push_back(i);
        }
        g.push_back(gSmal);
        gr.push_back(grSmal);
        gSmal = {};
        grSmal = {};
    }

    visited.assign (hight, false);
    for (int v = 0; hight > v; ++v)
        if (!visited[v])
            dfs1(v, g, hight, visited, order);

    visited.assign (hight, false);
    for (int i = 1; hight > i; ++i) {
        int v = order[hight-1-i];
        if (!visited[v]) {
            dfs2(v, gr, hight, visited, component);
            components.push_back(component);
            component.clear();
        }
    }

    cout << "Strong components: " << endl;
    writeComponents(components);
}
*/
void lab_8(){
    int matrix[matrixSize][matrixSize];
    int hight = readMatrix(matrix, "Matrix8.txt");
    int source, destination;
    Ato(source, hight);
    int minPath[hight];
    bool visited[hight];
    int temp, minIn = 0, min;

    for (int i = 0; hight > i; i++) {
        minPath[i] = intLimit;
        visited[i] = false;
    }
    minPath[source] = 0;

    while (intLimit > minIn) {
        minIn = intLimit;
        min = intLimit;
        for (int i = 0; hight > i; i++) {
            if (!visited[i] && (minPath[i] < min)) {
                min = minPath[i];
                minIn = i;
            }
        }
        if (intLimit != minIn) {
            for (int i = 0; hight > i; i++) {
                if (0 < matrix[minIn][i]) {
                    temp = min + matrix[minIn][i];
                    if (temp < minPath[i])
                        minPath[i] = temp;
                }
            }
            visited[minIn] = true;
        }
    }

    cout << "Shortest path length to vertex: ";
    for (int i = 0; hight > i + 1; i++)
        cout << i+1 << ":" << minPath[i] << ", ";
    cout << hight << ":"<< minPath[hight - 1] << endl;

    for(int dest = 0; hight > dest; dest++){
        if( dest == source) dest++;
        destination = dest;
        int path[hight];
        path[0] = destination + 1;
        int k = 1;
        int weight = minPath[destination];

        while (destination != source){
            for (int i = 0; hight > i; i++)
                if (matrix[i][destination] != 0)
                {
                    temp = weight - matrix[i][destination];
                    if (temp == minPath[i]) {
                        weight = temp;
                        destination = i;
                        path[k] = i + 1;
                        k++;
                    }
                }
        }
        cout << "Shortest path from " << source + 1 << " to " << dest + 1<< ": ";
        for (int i = k - 1; 0 < i; i--)
            cout << path[i] << " - ";
        cout << path[0] << endl;
    }
}

void lab_9(){
    vector<edge> edges;
    int eCount = 0;
    int hight = readEdges(edges, eCount, "Matrix8.txt");
    int source, destination;
    Ato(source, hight);

    vector<int> minPath(hight, intLimit);
    minPath[source] = 0;
    vector<int> before(hight, -1);
    while (true) {
        bool any = false;
        for (int j=0; eCount > j; ++j)
            if (minPath[edges[j].a] < intLimit)
                if (minPath[edges[j].b] > minPath[edges[j].a] + edges[j].cost) {
                    minPath[edges[j].b] = minPath[edges[j].a] + edges[j].cost;
                    before[edges[j].b] = edges[j].a;
                    any = true;
                }
        if (!any)  break;
    }

    for(int dest = 0; hight > dest; dest++) {
        if (dest == source) dest++;
        destination = dest;
        if (minPath[destination] == intLimit)
            cout << "No path from " << source << " to " << destination << ".";
        else {
            vector<int> path;
            for (int now=destination; now!=-1; now=before[now])
                path.push_back(now);

            cout << "Shortest path from " << source + 1 << " to " << dest + 1 << ": ";
            for (int i = path.size() - 1; 0 < i; i--)
                cout << path[i] + 1 << " - ";
            cout << path[0] + 1 << endl;
        }
    }
}

int main() {
    int hight;
    int lab_num;
    cout << "Chose lab number:";
    cin >> lab_num;
    while (true) {
        switch (lab_num) {
            case 2:
                lab_2();
                return 0;
            case 3:
                lab_3();
                return 0;
            case 8:
                lab_8();
                return 0;
            case 9:
                lab_9();
                return 0;
            default:
                cout << "wrong input" << endl << "Chose lab number:";
                cin >> lab_num;
        }
    }
}

