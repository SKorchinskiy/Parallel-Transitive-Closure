namespace ParallelFloyd {
    void parallelFloydWarshall(bool result[2][MAX_VERTICES][MAX_VERTICES], int vertices, int k, int start, int end) {
        const int current_layer = k % 2, prev_layer = (k - 1) % 2;
        for (int i = start; i < end; ++i) {
            for (int j = 0; j < vertices; ++j) {
                result[current_layer][i][j] = result[prev_layer][i][j] || 
                                              result[prev_layer][i][k - 1] && result[prev_layer][k - 1][j];
            }
        }
    }

    void computeTransitiveClosure(const bool closure[MAX_VERTICES][MAX_VERTICES], bool result[2][MAX_VERTICES][MAX_VERTICES], const int vertices, const int threadCount) {
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                result[0][i][j] = result[1][i][j] = closure[i][j];
            }
        }
        
        std::thread threads[threadCount];
        const int workload = vertices / threadCount;

        for (int k = 1; k <= vertices; k++) {
            for (int i = 0; i < threadCount; i++) {
                int start = i * workload;
                int end = (i == threadCount - 1) ? vertices : (i + 1) * workload;
                threads[i] = std::thread(parallelFloydWarshall, std::ref(result), vertices, k, start, end);
            }

            for (int i = 0; i < threadCount; i++) {
                threads[i].join();
            }
        }
    }
}