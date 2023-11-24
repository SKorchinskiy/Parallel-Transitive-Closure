namespace OrdFloyd {
    void computeTransitiveClosure(const bool closure[MAX_VERTICES][MAX_VERTICES], bool result[MAX_VERTICES][MAX_VERTICES], const int vertices) {
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                result[i][j] = closure[i][j];
            }
        }
        for (int k = 0; k < vertices; k++) {
            for (int i = 0; i < vertices; i++) {
                for (int j = 0; j < vertices; j++) {
                    result[i][j] = result[i][j] || result[i][k] && result[k][j];
                }
            }
        }
    }
}