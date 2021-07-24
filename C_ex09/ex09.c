#include <stdio.h>
#include <stdlib.h>
#define rep(i, n) for(int i = 0; i < (int)(n); i++)
#define N 3 // number of players (fixed)
#define A 3 // number of alternatives (fixed)
int compare(const void *a, const void *b) {
    return *(int *)a < *(int *)b ? -1 : *(int *)a > *(int *)b ? 1 : 0;
}
void sort(int *a, int n) { qsort(a, n, sizeof(int), compare); } // sort function

int Wlist[11] = {170, 204, 240, 168, 200, 224, 136, 192, 160, 128, 232};
// a list of W in the given order

void printW(const int w[1 << N]) { // for printing W
    printf("{");
    int g = 0, sp = 12;
    rep(i, 1 << N) {
        if (w[i]) {
            g++;
            if (g != 1) {
                printf(",");
                sp--;
            }
            rep(j, N) {
                if ((i >> j) & 1) {
                    printf("%d", 1 + j);
                    sp--;
                }
            }
        }
    }
    printf("}");
    rep(i, sp) printf(" ");
}

void printCore(const int core) { // for printing Core
    printf("{");
    int g = 0, sp = A;
    rep(i, A) {
        if ((core >> i) & 1) {
            printf(g++ ? ",%c" : "%c", 'a' + i);
            sp--;
        }
    }
    if (sp == A) sp--;
    printf(g ? "}" : " }");
    rep(i, sp * 2) printf(" ");
    printf("|");
}

void printPref(int a[1 + 11 * 36]) { 
    // for printing combinations of simple games and preferences
    // a[0] = count, a[1~] = 11 * pref + W
    sort(a + 1, a[0]);
    rep(i, a[0]) {
        printf("Pref-%2d, W=", a[1 + i] / 11);
        int w[1 << N];
        rep(j, 1 << N) w[j] = (Wlist[a[1 + i] % 11] >> j) & 1;
        printW(w);
        printf("\n");
    }
}

int Core(const int w[1 << N], const int dm[A]) { // decide Core from W and DM
    // dm[j]:[R] ... 0:[abc], 1:[acb], 2:[bac], 3:[bca], 4:[cab], 5:[cba]
    int x, core[A] = {1, 1, 1};
    int dom[A]; // dom[3] = {a Dom? b, b Dom? c, c Dom? a}, 
    rep(i, 1 << N) {
        x = 0;
        rep(j, A) dom[j] = 0;
        if (w[i] == 1) { // each S in W
            rep(j, A) {
                if ((i >> j) & 1) { // each player in S
                    x--;
                    dom[0] += (dm[j] < 2 || dm[j] == 4) ? 1 : -1;
                    // a Ri b in dm[j] = {0, 1, 4}
                    dom[1] -= (dm[j] > 3 || dm[j] == 1) ? 1 : -1;
                    // b Ri c in dm[j] = {0, 2, 3}
                    dom[2] += (dm[j] > 2) ? 1 : -1;
                    // c Ri a in dm[j] = {3, 4, 5}
                }
            }
            rep(j, A) {
                if (dom[j] == x || dom[(j + 2) % 3] == -x) {
                    // if x Ri y for all i in S, x Dom y
                    // and if S exists such that x Dom y, y is not Core
                    core[j]--;
                }
            }
        }
    }
    int ret = 7;
    rep(j, A) {
        if (core[j] < 1) {
            ret -= (1 << j);
            // return 3bit number: 'c is Core?'-'b is Core?'-'a is Core?'
            // like: 5 = 101(2) means 'Core(C) = {c, a}'
        }
    }
    return ret;
}

void solve() {
    int corelist[2][1 + 11 * 36] = {{0}, {0}};
    // store some combinations of simple games and preferences
    // corelist[0] ... {count, combinations whose Core is empty}
    // corelist[1] ... {count, combinations whose Core is A}

    rep(dm2, 6) {
        printf("                 |");
        rep(pref, 6) printf((pref < 5) ? "Pref-%2d|" : "Pref-%2d|\n", pref + dm2 * 6);
        rep(i, 11) {
            printf("%2d.", i + 1);
            int w[1 << N];
            rep(j, 1 << N) w[j] = (Wlist[i] >> j) & 1;
            printW(w);
            printf("|");
            rep(dm3, 6) {
                int dm[A] = { 0, dm2, dm3 }; // DMs' preference
                int ans = Core(w, dm);
                if (ans == 0) { // Core is empty
                    corelist[0][++corelist[0][0]] = i + 11 * (dm2 * 6 + dm3);
                }
                else if (ans == 7) { // Core is A
                    corelist[1][++corelist[1][0]] = i + 11 * (dm2 * 6 + dm3);
                }
                printCore(ans);
            }
            printf("\n");
        }
        printf("\n");
    }

    printf("\nCases that the core is empty:\n");
    printPref(corelist[0]);
    printf("\nCases that the core is A:\n");
    printPref(corelist[1]);
}

int main() {
    rep(i, N) printf(i ? ",%d" : " N = {%d", i + 1);
    printf("}\n");
    rep(i, A) printf(i ? ",%c" : " A = {%c", i + 'a');
    printf("}\n\n");
    solve();
    return 0;
}
