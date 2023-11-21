#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <shared_mutex>
#include <mutex>

namespace ParallelBFS {
    std::shared_mutex mtx[MAX_VERTICES][MAX_VERTICES];
    bool visited[MAX_VERTICES][MAX_VERTICES];
    
    void parallelBFS(bool closure[MAX_VERTICES][MAX_VERTICES], int vertices, int start, int end) {
    
    std::queue<int> q;
    while (start < end) {
        visited[start][start] = true;
        q.push(start);

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            for (int i = 0; i < vertices; ++i) {
                if (!visited[start][i] && closure[current][i]) {
                    mtx[start][i].lock();
                    closure[start][i] = true;
                    visited[start][i] = true;
                    mtx[start][i].unlock();
                    q.push(i);
                }
            }
        }

        start++;
    }
}

    void computeTransitiveClosureBFS(bool closure[MAX_VERTICES][MAX_VERTICES], int vertices, int threadCount) {
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++){
                visited[i][j] = 0;
            }
        }

        std::thread threads[threadCount];
        int workload = vertices / threadCount;

        for (int i = 0; i < threadCount; ++i) {
            int start = i * workload;
            int end = (i == threadCount - 1) ? vertices : (i + 1) * workload;
            threads[i] = std::thread(parallelBFS, std::ref(closure), vertices, start, end);
        }

        for (int i = 0; i < threadCount; ++i) {
            threads[i].join();
        }
    }
}