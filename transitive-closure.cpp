#include <iostream>
#include <thread>
#include <chrono>
#include <random>

#define MAX_VERTICES 1500

#include "parallel-floyd.h"
#include "ord-floyd.h"

void useOrdinaryFloyd(const bool closure[MAX_VERTICES][MAX_VERTICES], const int vertices) {
    auto start = std::chrono::high_resolution_clock::now();
    bool result[MAX_VERTICES][MAX_VERTICES];
    OrdFloyd::computeTransitiveClosure(std::ref(closure), std::ref(result), vertices);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << duration << " ms (ordinary floyd)" << '\n';
}

void useParallelFloyd(const bool closure[MAX_VERTICES][MAX_VERTICES], const int vertices, int threadCount) {
    auto start = std::chrono::high_resolution_clock::now();
    bool result[2][MAX_VERTICES][MAX_VERTICES];
    ParallelFloyd::computeTransitiveClosure(std::ref(closure), std::ref(result), vertices, threadCount);
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
    useOrdinaryFloyd(closure, vertices);
    useParallelFloyd(closure, vertices, threadCount);

    return 0;
}
