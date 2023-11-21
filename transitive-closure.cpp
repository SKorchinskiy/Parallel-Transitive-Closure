#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <shared_mutex>
#include <mutex>
#include <chrono>
#include <random>
#include <vector>

#define MAX_VERTICES 2500

#include "parallel-bfs.h"
#include "parallel-floyd.h"
#include "ord-floyd.h"

auto useParallelBFS(bool closure[MAX_VERTICES][MAX_VERTICES], int vertices, int threadCount) {
    bool newClosure[vertices][vertices];
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            newClosure[i][j] = closure[i][j];
        }
    }
    auto start = std::chrono::high_resolution_clock::now();
    ParallelBFS::computeTransitiveClosureBFS(newClosure, vertices, threadCount);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << duration << " ms (parallel bfs), threads: " << threadCount << '\n';
}

void useParallelFloyd(bool closure[MAX_VERTICES][MAX_VERTICES], int vertices, int threadCount) {
    bool newClosure[vertices][vertices];
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            newClosure[i][j] = closure[i][j];
        }
    }
    auto start = std::chrono::high_resolution_clock::now();
    ParallelFloyd::computeTransitiveClosure(newClosure, vertices, threadCount);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << duration << " ms (parallel floyd) threads: " << threadCount << '\n';
}

int main() {
    int vertices = MAX_VERTICES;
    bool closure[MAX_VERTICES][MAX_VERTICES];
    
    { // generate random binary matrix
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 1);
        int count = 0;
        for (int i = 0; i < vertices; ++i) {
            for (int j = 0; j < vertices; ++j) {
                closure[i][j] = 0;

                if (i == j) closure[i][i] = 1;
                else closure[i][j] = dis(gen);
                
                count += closure[i][j];
            }
        }
        std::cout << "Number of generated edges: " << count << '\n';
    }

    int threadCount = 8;
    useParallelFloyd(closure, vertices, threadCount);
    useParallelBFS(closure, vertices, threadCount);

    return 0;
}
