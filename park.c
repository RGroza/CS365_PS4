#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <time.h>

#define MAX_NODES 100005
#define MAX_EDGES 200005
#define MAX_LENGTH 200055

typedef struct Edge {
    int u, v, weight;
} Edge;

// Edge edges[MAX_EDGES];
// long long dist[MAX_NODES];
// long long dp[MAX_NODES][MAX_LENGTH];
Edge *edges;
int n, m, k, p;

long long bellman_ford() {
    // long long dist[n + 1];
    // long long dist[MAX_NODES];
    long long *dist = (long long *) malloc((n + 1) * sizeof(long long));
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

    long long d = dist[n];
    free(dist);

    return d;
}

long long count_paths(int max_length) {
    clock_t before = clock();
    long long **dp = (long long **) malloc((n + 1) * sizeof(long long *));
    // if (dp == NULL) {
    //     perror("Failed to allocate memory for dp array");
    //     exit(EXIT_FAILURE);
    // }

    for (int i = 0; i <= n; i++) {
        dp[i] = (long long *) malloc((max_length + 1) * sizeof(long long));
        // if (dp[i] == NULL) {
        //     perror("Failed to allocate memory for dp row");
        //     exit(EXIT_FAILURE);
        // }
        for (int j = 0; j <= max_length; j++) {
            dp[i][j] = 0;
        }
    }

    // long long dp[MAX_NODES][MAX_LENGTH];
    // for (int i = 0; i <= n; i++) {
    //     for (int j = 0; j <= max_length; j++) {
    //         dp[i][j] = 0;
    //     }
    // }
    dp[1][0] = 1;

    clock_t diff = clock() - before;
    printf("dp alloc: %ld\n", diff * 1000 / CLOCKS_PER_SEC);

    before = clock();

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

    long long paths = 0;
    for (int length = 0; length <= max_length; length++) {
        paths = (paths + dp[n][length]) % p;
    }

    diff = clock() - before;
    printf("dp algo: %ld\n", diff * 1000 / CLOCKS_PER_SEC);

    before = clock();

    for (int i = 0; i <= n; i++) {
        free(dp[i]);
    }
    free(dp);

    diff = clock() - before;
    printf("dp dealloc: %ld\n", diff * 1000 / CLOCKS_PER_SEC);


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

        clock_t before = clock();

        edges = (Edge *) malloc(m * sizeof(Edge));

        for (int i = 0; i < m; i++) {
            int u = fast_read();
            int v = fast_read();
            int w = fast_read();
            edges[i].u = u;
            edges[i].v = v;
            edges[i].weight = w;
        }

        clock_t diff = clock() - before;
        printf("reading edges: %ld\n", diff * 1000 / CLOCKS_PER_SEC);

        before = clock();

        long long d = bellman_ford();

        diff = clock() - before;
        printf("bellman ford: %ld\n", diff * 1000 / CLOCKS_PER_SEC);

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

        free(edges);
    }
    return 0;
}
