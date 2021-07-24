#include <stdio.h>
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define N 3 // number of players (fixed)

void printS(const int s) { // like ...  5(101) -> {1,3}, 3(011) -> {1,2}
    int c = 0, sp = N;
    rep(i, N) {
        if ((s >> i) & 1) {
            sp--;
            printf(c++ ? ",%d" : "{%d", 1 + i);
        }
    }
    printf("}");
    rep(i, sp) {
        printf("  ");
    }
}

void solve(const int wlist, const int r[N][N]) {
    printf(" W : %d\n", wlist);
    rep(i, N) {
        printf("R%d : [", i + 1);
        rep(j, N) {
            printf(j ? ",%c" : "%c", 'a' + r[i][j]);
        }
        printf("]\n");
    }

    int w[1 << N];
    rep(i, 1 << N) {
        w[i] = (wlist >> i) & 1;
    }
    rep(i, N * N * N) {
        int p[N] = {1 + i / (N * N), 1 + (i % (N * N)) / N, 1 + i % N};
        printf("(P1 P2 P3) = (%1d %1d %1d)\n    W_C(P):\n", p[0], p[1], p[2]); // |N| = 3
        int Acp[N] = {0}, SxIsS[1 << N] = {0}, barWcp[1 << N] = {0}, barAcp[N] = {0};
        rep(j, N) { // i = a, b, c
            rep(k, 1 << N) {
                if (w[k]) {
                    int Sx = 0;
                    rep(l, N) { // 1, 2, 3
                        if ((k >> l) & 1) {
                            rep(m, p[l]) {
                                if (r[l][m] == j) {
                                    Sx += 1 << l;
                                }
                            }
                        }
                    }
                    rep(l, 1 << N) {
                        if (w[l] && Sx == l) {
                            printf("        ");
                            printS(k);
                            printf(" ... S%c = ", 'a' + j);
                            printS(Sx);
                            printf("\n");
                            Acp[j]++;
                            if (k == Sx) {
                                SxIsS[k] += 1 << j;
                            }
                        }
                    }
                }
            }
        }

        printf("    -W_C(P):\n");
        rep(j, 1 << N) {
            if (SxIsS[j]) {
                rep(k, N) { // k: x in A
                    if ((SxIsS[j] >> k) & 1) {
                        int flag = 1;
                        rep(l, N) { // l: i in S
                            if ((j >> l) & 1) {
                                rep(m, N) {
                                    rep(n, N) {
                                        if (r[l][m] != k 
                                            && r[l][n] == k 
                                            && m < n 
                                            && Acp[r[l][m]] != 0) {
                                            flag--;
                                        }
                                    }
                                }
                            }
                        }
                        if (flag == 1) {
                            printf("        *(in x = %c) ", 'a' + k);
                            printS(j);
                            printf("\n");
                        }
                    }
                }
            }
        }
    }
    printf("\n\n");
}

int main() {
    int R[N][N] = {{0, 1, 2}, {1, 2, 0}, {2, 0, 1}};
    solve(128, R); // W = {123}
    solve(232, R); // W = {12,13,23,123}

    R[2][1] = 1;
    R[2][2] = 0;
    solve(128, R);
    solve(232, R);
    return 0;
}
