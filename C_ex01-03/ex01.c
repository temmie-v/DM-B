#include <stdio.h>
#define rep(i, n) for(int i = 0; i < (int)(n); i++)
#define N 3 // number of player (less than 5)

int isSimple(const int w[1 << N]) {
	// must not include empty set, must include N
	if (w[0] == 1 || w[(1 << N) - 1] != 1) {
		return 0;
	}
	rep(s, 1 << N) {
		rep(t, 1 << N) {
			// if S¼T and S¸W then W must include T
			if (s < t && (s & t) == s && w[s] == 1 && w[t] != 1) {
				return 0;
			}
		}
	}
	return 1;
}

int isProper(const int w[1 << N]) {
	rep(s, 1 << N) {
		// if S¸W then W must not include N-S
		if (w[s] == 1 && w[(1 << N) - 1 - s] == 1) {
			return 0;
		}
	}
	return 1;
}

void printW(const int w[1 << N]) { // for printing W
	printf("{");
	int g = 0;
	rep(i, 1 << N) {
		if (w[i] == 1) {
			g++;
			if (g != 1) {
				printf(", ");
			}
			printf("{");
			int n = 0;
			rep(j, N) {
				if ((i >> j) % 2 == 1) {
					n++;
					if (n != 1) {
						printf(",");
					}
					printf("%d", 1 + j);
				}
			}
			printf("}");
		}
	}
	printf("}\n");
}

void solve() {
	int ans = 0;
	rep(i, 1 << (1 << N)) {
		int w[1 << N];
		rep(j, 1 << N) {
			w[j] = (i >> j) % 2;
		}
		if (isSimple(w) && isProper(w)) {
			printf("%2d : ", ++ans);
			printW(w);
		}
	}
}

int main() {
	rep(i, N) printf(i ? ",%d" : " N = {%d", i + 1);
	puts("}");
	solve();
	return 0;
}
