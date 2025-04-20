void mmult_opt(float **A, float **B, float **R, int M, int N, int P, int b) {
    for (int ii = 0; ii < M; ii += b) {
        for (int jj = 0; jj < P; jj += b) {
            for (int kk = 0; kk < N; kk += b) {
                for (int i = ii; i < fmin(ii + b, M); ++i) {
                    for (int j = jj; j < fmin(jj + b, P); ++j) {
                        float val = R[i][j];
                        for (int k = kk; k < fmin(kk + b, N); ++k) {
                            val += A[i][k] * B[k][j];
                        }
                        R[i][j] = val;
                    }
                }
            }
        }
    }
}