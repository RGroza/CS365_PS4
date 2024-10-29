#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_VERTICES 100005
#define MAX_EDGES 200005

typedef struct Edge {
    int u, v, w;
} Edge;

typedef struct EdgeNode {
    int v, w;
    struct EdgeNode *next;
} EdgeNode;

typedef struct {
    int node, w;
} HeapNode;

int n, m, k, p;
long long dist[MAX_VERTICES];
Edge edges[MAX_EDGES];
EdgeNode *graph[MAX_VERTICES];
HeapNode heap[MAX_EDGES];
int heap_size = 0;

void push(int node, int w) {
    heap[++heap_size].node = node;
    heap[heap_size].w = w;
    int i = heap_size;

    while (i > 1 && heap[i].w < heap[i / 2].w) {
        HeapNode tmp = heap[i];
        heap[i] = heap[i / 2];
        heap[i / 2] = tmp;
        i /= 2;
    }
}

HeapNode pop() {
    HeapNode top = heap[1];
    heap[1] = heap[heap_size--];
    int i = 1;

    while (2 * i <= heap_size) {
        int child = 2 * i;

        if (child + 1 <= heap_size && heap[child + 1].w < heap[child].w) {
            child++;
        }
        if (heap[i].w <= heap[child].w) {
            break;
        }

        HeapNode tmp = heap[i];
        heap[i] = heap[child];
        heap[child] = tmp;
        i = child;
    }

    return top;
}

void add_edge(int u, int v, int w) {
    EdgeNode *edge = (EdgeNode *) malloc(sizeof(EdgeNode));
    edge->v = v;
    edge->w = w;
    edge->next = graph[u];
    graph[u] = edge;
}

void free_graph() {
    for (int v = 0; v <= n; v++) {
        EdgeNode *head = graph[v];
        EdgeNode *prev;
        while (head != NULL) {
            prev = head;
            head = head->next;
            free(prev);
        }
        graph[v] = NULL;
    }
}

long long dijkstra() {
    for (int i = 1; i <= n; i++) {
        dist[i] = LLONG_MAX;
    }

    heap_size = 0;
    dist[1] = 0;
    push(1, 0);

    while (heap_size > 0) {
        HeapNode u = pop();

        if (u.w > dist[u.node]){
            continue;
        }

        for (EdgeNode *edge = graph[u.node]; edge; edge = edge->next) {
            int v = edge->v;
            int w = edge->w;

            if (dist[u.node] + w < dist[v]) {
                dist[v] = dist[u.node] + w;
                push(v, dist[v]);
            }
        }
    }

    return dist[n];
}

long long count_paths(int max_length) {
    long long **P = (long long **) malloc((n + 1) * sizeof(long long *));
    for (int i = 0; i <= n; i++) {
        P[i] = (long long *) malloc((max_length + 1) * sizeof(long long));
        for (int j = 0; j <= max_length; j++) {
            P[i][j] = 0;
        }
    }

    P[1][0] = 1;

    for (int length = 0; length < max_length; length++) {
        for (int j = 0; j < m; j++) {
            int u = edges[j].u;
            int v = edges[j].v;
            int w = edges[j].w;

            if (length + w <= max_length) {
                P[v][length + w] = (P[v][length + w] + P[u][length]) % p;
            }
        }
    }

    long long paths = 0;
    for (int length = 0; length <= max_length; length++) {
        paths = (paths + P[n][length]) % p;
    }

    for (int i = 0; i <= n; i++) {
        free(P[i]);
    }
    free(P);

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
            edges[i].w = w;
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
