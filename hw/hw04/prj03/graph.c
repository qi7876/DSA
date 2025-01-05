#include <alloca.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 8

typedef struct EdgeNode {
    int adjvex;
    int weight;
    struct EdgeNode* next;
} EdgeNode;

typedef struct VertexNode {
    int data;
    struct EdgeNode* firstEdge;
} VertexNode, AdjList[MAX_VERTICES];

typedef struct Graph {
    AdjList adjList;
    int numVertices, numEdges;
} Graph;

void createGraph(Graph* G)
{
    int i, j, k, w;

    G->numVertices = MAX_VERTICES;
    G->numEdges = 14;

    for (i = 0; i < G->numVertices; i++) {
        G->adjList[i].data = i + 1;
        G->adjList[i].firstEdge = NULL;
    }

    int edges[][3] = {
        { 1, 2, 24 },
        { 1, 3, 4 },
        { 2, 4, 23 },
        { 2, 6, 18 },
        { 2, 7, 9 },
        { 3, 4, 6 },
        { 3, 5, 16 },
        { 4, 5, 8 },
        { 4, 6, 5 },
        { 5, 6, 10 },
        { 5, 8, 21 },
        { 6, 7, 11 },
        { 6, 8, 14 },
        { 7, 8, 7 }
    };

    for (k = 0; k < G->numEdges; k++) {
        i = edges[k][0] - 1;
        j = edges[k][1] - 1;
        w = edges[k][2];

        EdgeNode* newNode = (EdgeNode*)malloc(sizeof(EdgeNode));
        newNode->adjvex = j;
        newNode->weight = w;
        newNode->next = G->adjList[i].firstEdge;
        G->adjList[i].firstEdge = newNode;

        newNode = (EdgeNode*)malloc(sizeof(EdgeNode));
        newNode->adjvex = i;
        newNode->weight = w;
        newNode->next = G->adjList[j].firstEdge;
        G->adjList[j].firstEdge = newNode;
    }
}

int visited[MAX_VERTICES];
int edgeCountDFS = 0;

void DFS(Graph* G, int i)
{
    EdgeNode* p;
    visited[i] = 1;
    printf("City %d ", G->adjList[i].data);

    p = G->adjList[i].firstEdge;
    while (p) {
        if (!visited[p->adjvex]) {
            edgeCountDFS++;
            DFS(G, p->adjvex);
        }
        p = p->next;
    }
}

void DFSTraverse(Graph* G)
{
    int i;
    edgeCountDFS = 0;
    for (i = 0; i < G->numVertices; i++) {
        visited[i] = 0;
    }
    for (i = 0; i < G->numVertices; i++) {
        if (!visited[i]) {
            DFS(G, i);
        }
    }
    printf("\nThe Number of Edges(DFS): %d\n", edgeCountDFS / 2);
}

int edgeCountBFS = 0;

void BFSTraverse(Graph* G)
{
    int i, j;
    EdgeNode* p;
    int queue[MAX_VERTICES];
    int front = 0, rear = 0;
    edgeCountBFS = 0;

    for (i = 0; i < G->numVertices; i++) {
        visited[i] = 0;
    }

    for (i = 0; i < G->numVertices; i++) {
        if (!visited[i]) {
            visited[i] = 1;
            printf("City %d ", G->adjList[i].data);
            queue[rear++] = i;

            while (front != rear) {
                j = queue[front++];
                p = G->adjList[j].firstEdge;
                while (p) {
                    if (!visited[p->adjvex]) {
                        edgeCountBFS++;
                        visited[p->adjvex] = 1;
                        printf("City %d ", G->adjList[p->adjvex].data);
                        queue[rear++] = p->adjvex;
                    }
                    p = p->next;
                }
            }
        }
    }
    printf("\nThe Number of Edges(BFS): %d\n", edgeCountBFS / 2);
}

/**
 * @brief Prim Part.
 */

#define INFINITY 65535

void Prim(Graph* G)
{
    int min, i, j, k;
    int adjvex[MAX_VERTICES];
    int lowcost[MAX_VERTICES];
    lowcost[0] = 0;
    adjvex[0] = 0;
    for (i = 1; i < G->numVertices; i++) {
        lowcost[i] = INFINITY;
        adjvex[i] = 0;
    }
    EdgeNode* p = G->adjList[0].firstEdge;
    while (p) {
        lowcost[p->adjvex] = p->weight;
        p = p->next;
    }

    for (i = 1; i < G->numVertices; i++) {
        min = INFINITY;
        j = 1;
        k = 0;
        while (j < G->numVertices) {
            if (lowcost[j] != 0 && lowcost[j] < min) {
                min = lowcost[j];
                k = j;
            }
            j++;
        }
        printf("(City %d, City %d) %d\n", G->adjList[adjvex[k]].data, G->adjList[k].data, lowcost[k]);
        lowcost[k] = 0;

        p = G->adjList[k].firstEdge;
        while (p) {
            if (lowcost[p->adjvex] != 0 && p->weight < lowcost[p->adjvex]) {
                lowcost[p->adjvex] = p->weight;
                adjvex[p->adjvex] = k;
            }
            p = p->next;
        }
    }
}

/**
 * @brief Kruskal Part.
 */
typedef struct {
    int begin;
    int end;
    int weight;
} Edge;

void sortEdges(Edge edges[], int numEdges)
{
    int i, j;
    Edge temp;
    for (i = 0; i < numEdges - 1; i++) {
        for (j = 0; j < numEdges - i - 1; j++) {
            if (edges[j].weight > edges[j + 1].weight) {
                temp = edges[j];
                edges[j] = edges[j + 1];
                edges[j + 1] = temp;
            }
        }
    }
}

int find(int* parent, int f)
{
    while (parent[f] > 0) {
        f = parent[f];
    }
    return f;
}

void Kruskal(Graph* G)
{
    int i, n, m;
    Edge edges[G->numEdges];
    int parent[MAX_VERTICES];

    int index = 0;
    for (i = 0; i < G->numVertices; i++) {
        EdgeNode* p = G->adjList[i].firstEdge;
        while (p) {
            if (i < p->adjvex) {
                edges[index].begin = i;
                edges[index].end = p->adjvex;
                edges[index].weight = p->weight;
                index++;
            }
            p = p->next;
        }
    }

    sortEdges(edges, G->numEdges);

    for (i = 0; i < G->numVertices; i++) {
        parent[i] = 0;
    }

    for (i = 0; i < G->numEdges; i++) {
        n = find(parent, edges[i].begin);
        m = find(parent, edges[i].end);
        if (n != m) {
            parent[n] = m;
            printf("(City %d, City %d) %d\n", G->adjList[edges[i].begin].data, G->adjList[edges[i].end].data, edges[i].weight);
        }
    }
}

void Dijkstra(Graph* G, int v0)
{
    int dist[MAX_VERTICES];
    int path[MAX_VERTICES];
    int final[MAX_VERTICES];
    int i, v, w, min;

    for (v = 0; v < G->numVertices; v++) {
        final[v] = 0;
        dist[v] = INFINITY;
        path[v] = -1;
    }

    EdgeNode* p = G->adjList[v0].firstEdge;
    while (p) {
        dist[p->adjvex] = p->weight;
        path[p->adjvex] = v0;
        p = p->next;
    }

    dist[v0] = 0;
    final[v0] = 1;

    for (i = 1; i < G->numVertices; i++) {
        min = INFINITY;
        for (w = 0; w < G->numVertices; w++) {
            if (!final[w] && dist[w] < min) {
                v = w;
                min = dist[w];
            }
        }
        final[v] = 1;
        p = G->adjList[v].firstEdge;
        while (p) {
            if (!final[p->adjvex] && (min + p->weight < dist[p->adjvex])) {
                dist[p->adjvex] = min + p->weight;
                path[p->adjvex] = v;
            }
            p = p->next;
        }
    }

    printf("The minimum path length for City %d to City 7: %d\n", G->adjList[v0].data, dist[6]);
    printf("Path: City 7 ");
    int k = 6;
    while (path[k] != -1) {
        printf("<- City %d ", G->adjList[path[k]].data);
        k = path[k];
    }
    printf("\n");
}

int main(int argc, char* argv[])
{
    Graph G;
    createGraph(&G);

    printf("DFS:\n");
    DFSTraverse(&G);

    printf("BFS:\n");
    BFSTraverse(&G);

    printf("Minimum Spanning Tree(Prim):\n");
    Prim(&G);

    printf("Minimum Spanning Tree(Kruskal):\n");
    Kruskal(&G);

    printf("Dijkstra:\n");
    Dijkstra(&G, 0);
    return EXIT_SUCCESS;
}
