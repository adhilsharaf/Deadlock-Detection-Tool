#include <iostream>
#include <vector>
#include <stack>
using namespace std;

class DeadlockDetector {
    int processes;
    int resources;
    vector<vector<int>> allocation;
    vector<vector<int>> request;
    vector<vector<int>> graph;
    vector<bool> visited;
    vector<bool> recStack;

public:
    DeadlockDetector(int p, int r) {
        processes = p;
        resources = r;
        allocation.resize(p, vector<int>(r));
        request.resize(p, vector<int>(r));
        graph.resize(p);
        visited.resize(p, false);
        recStack.resize(p, false);
    }

    void inputMatrices() {
        cout << "\nEnter Allocation Matrix (" << processes << "x" << resources << "):\n";
        for (int i = 0; i < processes; i++)
            for (int j = 0; j < resources; j++)
                cin >> allocation[i][j];

        cout << "\nEnter Request Matrix (" << processes << "x" << resources << "):\n";
        for (int i = 0; i < processes; i++)
            for (int j = 0; j < resources; j++)
                cin >> request[i][j];
    }

    void buildGraph() {
        for (int i = 0; i < processes; i++) {
            for (int j = 0; j < resources; j++) {
                if (request[i][j] == 1) {
                    for (int k = 0; k < processes; k++) {
                        if (allocation[k][j] == 1) {
                            graph[i].push_back(k);
                        }
                    }
                }
            }
        }
    }

    bool dfs(int node) {
        visited[node] = true;
        recStack[node] = true;

        for (int adj : graph[node]) {
            if (!visited[adj] && dfs(adj))
                return true;
            else if (recStack[adj])
                return true;
        }

        recStack[node] = false;
        return false;
    }

    bool detectDeadlock() {
        for (int i = 0; i < processes; i++) {
            if (!visited[i]) {
                if (dfs(i))
                    return true;
            }
        }
        return false;
    }

    void displayGraph() {
        cout << "\nWait-For Graph (WFG):\n";
        for (int i = 0; i < processes; i++) {
            cout << "P" << i << " -> ";
            for (int adj : graph[i])
                cout << "P" << adj << " ";
            cout << endl;
        }
    }
};

int main() {
    int p, r;

    cout << "Enter Number of Processes: ";
    cin >> p;
    cout << "Enter Number of Resources: ";
    cin >> r;

    DeadlockDetector d(p, r);

    d.inputMatrices();
    d.buildGraph();

    cout << "\nConstructed Wait-For Graph:";
    d.displayGraph();

    if (d.detectDeadlock())
        cout << "\n⚠ DEADLOCK DETECTED! Circular wait exists.\n";
    else
        cout << "\n✔ No Deadlock Detected.\n";
}