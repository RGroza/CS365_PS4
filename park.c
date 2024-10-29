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

int n, m, k, p;
Edge edges[MAX_EDGES];
long long dist[MAX_NODES];

typedef struct EdgeNode {
    int to, weight;
    struct EdgeNode *next;
} EdgeNode;

typedef struct {
    int node, cost;
} HeapNode;

typedef struct {
    EdgeNode *head;
} AdjList;

AdjList graph[MAX_NODES];
HeapNode heap[MAX_EDGES];
int heapSize = 0;

void push(int node, int cost) {
    heap[++heapSize].node = node;
    heap[heapSize].cost = cost;
    int i = heapSize;

    while (i > 1 && heap[i].cost < heap[i / 2].cost) {
        HeapNode tmp = heap[i];
        heap[i] = heap[i / 2];
        heap[i / 2] = tmp;
        i /= 2;
    }
}

HeapNode pop() {
    HeapNode top = heap[1];
    heap[1] = heap[heapSize--];
    int i = 1;

    while (2 * i <= heapSize) {
        int child = 2 * i;
        if (child + 1 <= heapSize && heap[child + 1].cost < heap[child].cost)
            child++;
        if (heap[i].cost <= heap[child].cost)
            break;
        HeapNode tmp = heap[i];
        heap[i] = heap[child];
        heap[child] = tmp;
        i = child;
    }

    return top;
}

void add_edge(int u, int v, int w) {
    EdgeNode *edge = (EdgeNode *) malloc(sizeof(EdgeNode));
    if (!edge) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    edge->to = v;
    edge->weight = w;
    edge->next = graph[u].head;
    graph[u].head = edge;
}

void free_graph() {
    for (int v = 0; v <= n; v++) {
        EdgeNode *head = graph[v].head;
        EdgeNode *prev;
        while (head != NULL) {
            prev = head;
            head = head->next;
            free(prev);
        }
        graph[v].head = NULL;
    }
}

long long dijkstra() {
    for (int i = 1; i <= n; i++) {
        dist[i] = LLONG_MAX;
    }

    heapSize = 0;
    dist[1] = 0;
    push(1, 0);

    while (heapSize > 0) {
        HeapNode u = pop();

        if (u.cost > dist[u.node]){
            continue;
        }

        for (EdgeNode *edge = graph[u.node].head; edge; edge = edge->next) {
            int v = edge->to;
            int weight = edge->weight;

            if (dist[u.node] + weight < dist[v]) {
                dist[v] = dist[u.node] + weight;
                push(v, dist[v]);
            }
        }
    }

    return dist[n];
}

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
    long long **dp = (long long **) malloc((n + 1) * sizeof(long long *));
    for (int i = 0; i <= n; i++) {
        dp[i] = (long long *) malloc((max_length + 1) * sizeof(long long));
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

    long long paths = 0;
    for (int length = 0; length <= max_length; length++) {
        paths = (paths + dp[n][length]) % p;
    }

    for (int i = 0; i <= n; i++) {
        free(dp[i]);
    }
    free(dp);

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
            add_edge(u, v, w);
        }

        long long d = dijkstra();

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

        free_graph();
    }
    return 0;
}
