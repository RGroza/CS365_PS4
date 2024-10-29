#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 10000
#define MAX_EDGES 20000
#define MAX_LENGTH 10050

typedef struct Edge {
    int u, v, weight;
} Edge;

Edge edges[MAX_EDGES];
long long dist[MAX_NODES];
long long dp[MAX_NODES][MAX_LENGTH];
int n, m, k, p;

long long bellman_ford() {
    for (int i = 1; i <= n; i++) {
        dist[i] = LLONG_MAX;
    }
    dist[1] = 0;

    for (int i = 1; i <= n - 1; i++) {
        for (int j = 0; j < m; j++) {
            int u = edges[j].u;
            int v = edges[j].v;
            int weight = edges[j].weight;

            if (dist[u] != LLONG_MAX && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
            }
        }
    }
    return dist[n];
}

long long count_paths(int max_length) {
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j <= max_length; j++) {
            dp[i][j] = 0;
        }
    }
    dp[1][0] = 1;

    for (int length = 0; length < max_length; length++) {
        for (int j = 0; j < m; j++) {
            int u = edges[j].u;
            int v = edges[j].v;
            int weight = edges[j].weight;

            if (length + weight <= max_length) {
                dp[v][length + weight] = (dp[v][length + weight] + dp[u][length]) % p;
            }
        }
    }

    int paths = 0;
    for (int length = 0; length <= max_length; length++) {
        paths = (paths + dp[n][length]) % p;
    }

    return paths;
}

int fast_read() {
    int x = 0, w = 1;
    char ch = 0;
    while (ch < '0' || ch > '9') {  
        if (ch == '-') w = -1;     
        ch = getchar();           
    }
    while (ch >= '0' && ch <= '9') { 
        x = x * 10 + (ch - '0'); 
        ch = getchar();  
    }
    return x * w; 
}

int main() {
    for (int t = fast_read(); t > 0; t--) {
        n = fast_read();
        m = fast_read();
        k = fast_read();
        p = fast_read();

        for (int i = 0; i < m; i++) {
            int u = fast_read();
            int v = fast_read();
            int w = fast_read();
            edges[i].u = u;
            edges[i].v = v;
            edges[i].weight = w;
        }

        long long d = bellman_ford();

        if (d == LLONG_MAX) {
            printf("-1\n");
        } else {
            long long result = count_paths(d + k);
            if (result == 0) {
                printf("-1\n");
            } else {
                printf("%lld\n", result % p);
            }
        }
    }
    return 0;
}
