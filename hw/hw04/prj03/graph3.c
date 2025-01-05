#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXV 100
#define INF INT_MAX

typedef struct EdgeNode {
    int adjvex;
    int weight;
    struct EdgeNode* next;
} EdgeNode;

typedef struct {
    int data;
    EdgeNode* firstedge;
} VertexNode, AdjList[MAXV];

typedef struct {
    AdjList adjList;
    int numVertexes;
    int numEdges;
} Graph;

int visited[MAXV];

void InitGraph(Graph* G, int vertexNum)
{
    G->numVertexes = vertexNum;
    G->numEdges = 0;
    for (int i = 1; i <= vertexNum; i++) {
        G->adjList[i].data = i;
        G->adjList[i].firstedge = NULL;
    }
}

void AddEdge(Graph* G, int u, int v, int w)
{
    EdgeNode* e = (EdgeNode*)malloc(sizeof(EdgeNode));
    e->adjvex = v;
    e->weight = w;
    e->next = G->adjList[u].firstedge;
    G->adjList[u].firstedge = e;

    EdgeNode* e2 = (EdgeNode*)malloc(sizeof(EdgeNode));
    e2->adjvex = u;
    e2->weight = w;
    e2->next = G->adjList[v].firstedge;
    G->adjList[v].firstedge = e2;

    G->numEdges++;
}

void CreateGraph(Graph* G)
{
    InitGraph(G, 8);
    AddEdge(G, 1, 2, 24);
    AddEdge(G, 1, 3, 4);
    AddEdge(G, 2, 4, 23);
    AddEdge(G, 2, 6, 18);
    AddEdge(G, 2, 7, 9);
    AddEdge(G, 3, 4, 6);
    AddEdge(G, 3, 5, 16);
    AddEdge(G, 4, 5, 8);
    AddEdge(G, 4, 6, 5);
    AddEdge(G, 5, 6, 10);
    AddEdge(G, 5, 8, 21);
    AddEdge(G, 6, 7, 11);
    AddEdge(G, 6, 8, 14);
    AddEdge(G, 7, 8, 7);
}

int dfs_edge_count = 0;

void DFSUtil(Graph* G, int v)
{
    visited[v] = 1;
    printf("%d ", v);
    EdgeNode* p = G->adjList[v].firstedge;
    while (p) {
        if (!visited[p->adjvex]) {
            dfs_edge_count++;
            DFSUtil(G, p->adjvex);
        }
        p = p->next;
    }
}

void DFS(Graph* G, int start)
{
    for (int i = 1; i <= G->numVertexes; i++)
        visited[i] = 0;
    dfs_edge_count = 0;
    printf("DFS序列(从%d开始): ", start);
    DFSUtil(G, start);
    printf("\n遍历的边数为: %d\n", dfs_edge_count);
}

void BFS(Graph* G, int start)
{
    for (int i = 1; i <= G->numVertexes; i++)
        visited[i] = 0;
    int queue[MAXV];
    int front = 0, rear = 0;
    visited[start] = 1;
    queue[rear++] = start;
    int bfs_edge_count = 0;
    printf("BFS序列(从%d开始): ", start);
    while (front != rear) {
        int u = queue[front++];
        printf("%d ", u);
        EdgeNode* p = G->adjList[u].firstedge;
        while (p) {
            if (!visited[p->adjvex]) {
                visited[p->adjvex] = 1;
                queue[rear++] = p->adjvex;
                bfs_edge_count++;
            }
            p = p->next;
        }
    }
    printf("\n遍历的边数为: %d\n", bfs_edge_count);
}

void PrimMST(Graph* G, int start)
{
    int minWeight[MAXV];
    int inMST[MAXV];
    int parent[MAXV];
    for (int i = 1; i <= G->numVertexes; i++) {
        minWeight[i] = INF;
        inMST[i] = 0;
        parent[i] = -1;
    }
    minWeight[start] = 0;
    for (int i = 1; i <= G->numVertexes; i++) {
        int u = -1;
        int minw = INF;
        for (int v = 1; v <= G->numVertexes; v++) {
            if (!inMST[v] && minWeight[v] < minw) {
                minw = minWeight[v];
                u = v;
            }
        }
        inMST[u] = 1;
        EdgeNode* p = G->adjList[u].firstedge;
        while (p) {
            int v = p->adjvex;
            if (!inMST[v] && p->weight < minWeight[v]) {
                minWeight[v] = p->weight;
                parent[v] = u;
            }
            p = p->next;
        }
    }
    printf("Prim最小生成树的边及权值:\n");
    int totalWeight = 0;
    for (int i = 1; i <= G->numVertexes; i++) {
        if (i != start && parent[i] != -1) {
            printf("(%d, %d) = %d\n", parent[i], i, minWeight[i]);
            totalWeight += minWeight[i];
        }
    }
    printf("MST总权重: %d\n", totalWeight);
}

typedef struct {
    int u, v;
    int w;
} Edge;

int Find(int* parent, int x)
{
    while (parent[x] != x)
        x = parent[x];
    return x;
}

void Union(int* parent, int* rank, int x, int y)
{
    int rx = Find(parent, x);
    int ry = Find(parent, y);
    if (rank[rx] > rank[ry])
        parent[ry] = rx;
    else if (rank[rx] < rank[ry])
        parent[rx] = ry;
    else {
        parent[ry] = rx;
        rank[rx]++;
    }
}

int edgeCompare(const void* a, const void* b)
{
    return ((Edge*)a)->w - ((Edge*)b)->w;
}

void KruskalMST(Graph* G)
{
    int E = G->numEdges;
    Edge* edges = (Edge*)malloc(sizeof(Edge) * E);
    int index = 0;
    for (int i = 1; i <= G->numVertexes; i++) {
        EdgeNode* p = G->adjList[i].firstedge;
        while (p) {
            if (i < p->adjvex) {
                edges[index].u = i;
                edges[index].v = p->adjvex;
                edges[index].w = p->weight;
                index++;
            }
            p = p->next;
        }
    }
    qsort(edges, E, sizeof(Edge), edgeCompare);
    int parent[MAXV], rankArr[MAXV];
    for (int i = 1; i <= G->numVertexes; i++) {
        parent[i] = i;
        rankArr[i] = 0;
    }
    printf("Kruskal最小生成树的边及权值:\n");
    int totalWeight = 0;
    int edgeCount = 0;
    for (int i = 0; i < E; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        int ru = Find(parent, u);
        int rv = Find(parent, v);
        if (ru != rv) {
            Union(parent, rankArr, ru, rv);
            printf("(%d, %d) = %d\n", u, v, edges[i].w);
            totalWeight += edges[i].w;
            edgeCount++;
            if (edgeCount == G->numVertexes - 1)
                break;
        }
    }
    printf("MST总权重: %d\n", totalWeight);
    free(edges);
}

void Dijkstra(Graph* G, int start, int end)
{
    int dist[MAXV];
    int prev[MAXV];
    int S[MAXV];
    for (int i = 1; i <= G->numVertexes; i++) {
        dist[i] = INF;
        prev[i] = -1;
        S[i] = 0;
    }
    dist[start] = 0;
    for (int i = 1; i <= G->numVertexes; i++) {
        int u = -1;
        int mind = INF;
        for (int j = 1; j <= G->numVertexes; j++) {
            if (!S[j] && dist[j] < mind) {
                mind = dist[j];
                u = j;
            }
        }
        if (u == -1)
            break;
        S[u] = 1;
        EdgeNode* p = G->adjList[u].firstedge;
        while (p) {
            int v = p->adjvex;
            if (!S[v] && dist[u] != INF && dist[u] + p->weight < dist[v]) {
                dist[v] = dist[u] + p->weight;
                prev[v] = u;
            }
            p = p->next;
        }
    }
    if (dist[end] == INF) {
        printf("从%d到%d不存在路径\n", start, end);
    } else {
        printf("从%d到%d的最短路径长度为: %d\n", start, end, dist[end]);
        int path[MAXV];
        int count = 0;
        for (int v = end; v != -1; v = prev[v]) {
            path[count++] = v;
        }
        printf("路径为: ");
        for (int i = count - 1; i >= 0; i--)
            printf("%d ", path[i]);
        printf("\n");
    }
}

int main()
{
    Graph G;
    CreateGraph(&G);
    DFS(&G, 1);
    BFS(&G, 1);
    PrimMST(&G, 1);
    KruskalMST(&G);
    Dijkstra(&G, 1, 7);
    return 0;
}
