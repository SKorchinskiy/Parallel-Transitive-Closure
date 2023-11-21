#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>

namespace ParallelFloyd {
    std::mutex mtx[MAX_VERTICES][MAX_VERTICES];

    void parallelFloydWarshall(bool closure[MAX_VERTICES][MAX_VERTICES], int vertices, int k, int start, int end) {
        for (int i = start; i < end; ++i) {
            for (int j = 0; j < vertices; ++j) {
                mtx[i][j].lock();
                if (i != j && !closure[i][j] && closure[i][k] && closure[k][j]) {
                    closure[i][j] = true;
                }
                mtx[i][j].unlock();
            }
        }
    }

    void computeTransitiveClosure(bool closure[MAX_VERTICES][MAX_VERTICES], int vertices, int threadCount) {
        std::thread threads[threadCount];
        int workload = vertices / threadCount;
        
        for (int k = 0; k < vertices; k++) {
            for (int i = 0; i < threadCount; ++i) {
                int start = i * workload;
                int end = (i == threadCount - 1) ? vertices : (i + 1) * workload;
                threads[i] = std::thread(parallelFloydWarshall, closure, vertices, k, start, end);
            }

            for (int i = 0; i < threadCount; ++i) {
                threads[i].join();
            }
        }
    }
}