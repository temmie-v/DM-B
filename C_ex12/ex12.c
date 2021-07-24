#include <stdio.h>
#define rep(i, n) for (int i = 0; i < (int)(n); i++)

void printS(const int s) { // like ...  5(101) -> {1,3}, 3(011) -> {1,2}
    int c = 0, sp = 3;
    rep(i, 3) {
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

void solve(const int wlist, const int r[3][3]) {
    printf(" W : %d\n", wlist);
    rep(i, 3) {
        printf("R%d : [", i + 1);
        rep(j, 3) {
            printf(j ? ",%c" : "%c", 'a' + r[i][j]);
        }
        printf("]\n");
    }

    int w[1 << 3];
    rep(i, 1 << 3) {
        w[i] = (wlist >> i) & 1;
    }
    rep(i, 27) {
        int p[3] = {1 + i / 9, 1 + (i % 9) / 3, 1 + i % 3};
        printf("(P1 P2 P3) = (%1d %1d %1d)\n    W_C(P):\n", p[0], p[1], p[2]);
        int Acp[3] = {0}, SxIsS[1 << 3] = {0}, barWcp[1 << 3] = {0}, barAcp[3] = {0};
        rep(j, 3) { // i = a, b, c
            rep(k, 1 << 3) { // S = {}, 1, 2, 12, 3, 13, 23, 123
                if (w[k]) {
                    int Sx = 0;
                    rep(l, 3) { // 1, 2, 3
                        if ((k >> l) & 1) {
                            rep(m, p[l]) {
                                if (r[l][m] == j) {
                                    Sx += 1 << l;
                                }
                            }
                        }
                    }
                    rep(l, 1 << 3) {
                        if (w[l] && Sx == l) {
                            printf("        ");
                            printS(k);
                            printf(" ... x = %c, Sx = ", 'a' + j);
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
        rep(j, 1 << 3) { // S = {}, 1, 2, 12, 3, 13, 23, 123
            if (SxIsS[j]) {
                rep(k, 3) { // x = a, b, c
                    if ((SxIsS[j] >> k) & 1) {
                        int flag = 1;
                        rep(l, 3) { // i in S
                            if ((j >> l) & 1) {
                                rep(m, 3) {
                                    rep(n, 3) {
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
    int R[3][3] = {{0, 1, 2}, {1, 2, 0}, {2, 0, 1}};
    solve(128, R); // W = {123}
    solve(232, R); // W = {12,13,23,123}

    R[2][1] = 1;
    R[2][2] = 0;
    solve(128, R);
    solve(232, R);
    return 0;
}
