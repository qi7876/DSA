#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 8
#define INFINITY 65535

// 边节点结构体
typedef struct EdgeNode {
    int adjvex; // 邻接点域，存储该顶点对应的下标
    int weight; // 权值
    struct EdgeNode* next; // 链域，指向下一个邻接点
} EdgeNode;

// 顶点表节点结构体
typedef struct VertexNode {
    int data; // 顶点域，存储顶点信息 (这里用数字 1-8 表示城市)
    EdgeNode* firstedge; // 边表头指针
} VertexNode, AdjList[MAX_VERTICES];

// 图结构体
typedef struct {
    AdjList adjList;
    int numVertices, numEdges; // 图中当前顶点数和边数
} Graph;

// 创建图
void createGraph(Graph* G)
{
    int i, j, k, w;

    G->numVertices = MAX_VERTICES;
    G->numEdges = 14; // 图 2-1 中共有 13 条边

    // 初始化顶点表
    for (i = 0; i < G->numVertices; i++) {
        G->adjList[i].data = i + 1;
        G->adjList[i].firstedge = NULL;
    }

    // 建立边表
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
        i = edges[k][0] - 1; // 边的起点 (顶点下标需要减 1)
        j = edges[k][1] - 1; // 边的终点
        w = edges[k][2]; // 边的权重

        // 创建边节点 (头插法)
        EdgeNode* e = (EdgeNode*)malloc(sizeof(EdgeNode));
        e->adjvex = j;
        e->weight = w;
        e->next = G->adjList[i].firstedge;
        G->adjList[i].firstedge = e;

        // 无向图，需要添加反向边
        e = (EdgeNode*)malloc(sizeof(EdgeNode));
        e->adjvex = i;
        e->weight = w;
        e->next = G->adjList[j].firstedge;
        G->adjList[j].firstedge = e;
    }
}

// 访问标记数组
int visited[MAX_VERTICES];
int edgeCountDFS = 0; // 统计边数

// 深度优先遍历递归函数
void DFS(Graph* G, int i)
{
    EdgeNode* p;
    visited[i] = 1; // 标记为已访问
    printf("City %d ", G->adjList[i].data);

    p = G->adjList[i].firstedge;
    while (p) {
        if (!visited[p->adjvex]) {
            edgeCountDFS++;
            DFS(G, p->adjvex);
        }
        p = p->next;
    }
}

// 深度优先遍历
void DFSTraverse(Graph* G)
{
    int i;
    edgeCountDFS = 0;
    for (i = 0; i < G->numVertices; i++) {
        visited[i] = 0; // 初始化访问标记数组
    }
    for (i = 0; i < G->numVertices; i++) {
        if (!visited[i]) {
            DFS(G, i);
        }
    }
    printf("\nThe Number of Edges(DFS): %d\n", edgeCountDFS / 2);
}

int edgeCountBFS = 0; // 统计边数

// 广度优先遍历
void BFSTraverse(Graph* G)
{
    int i, j;
    EdgeNode* p;
    int queue[MAX_VERTICES]; // 辅助队列
    int front = 0, rear = 0;
    edgeCountBFS = 0;

    for (i = 0; i < G->numVertices; i++) {
        visited[i] = 0; // 初始化访问标记数组
    }

    for (i = 0; i < G->numVertices; i++) {
        if (!visited[i]) {
            visited[i] = 1;
            printf("City %d ", G->adjList[i].data);
            queue[rear++] = i; // 入队

            while (front != rear) {
                j = queue[front++]; // 出队
                p = G->adjList[j].firstedge;
                while (p) {
                    if (!visited[p->adjvex]) {
                        edgeCountBFS++;
                        visited[p->adjvex] = 1;
                        printf("City %d ", G->adjList[p->adjvex].data);
                        queue[rear++] = p->adjvex; // 入队
                    }
                    p = p->next;
                }
            }
        }
    }
    printf("\nThe Number of Edges(BFS): %d\n", edgeCountBFS / 2);
}

// 普里姆算法生成最小生成树
void Prim(Graph* G)
{
    int min, i, j, k;
    int adjvex[MAX_VERTICES]; // 保存相关顶点下标
    int lowcost[MAX_VERTICES]; // 保存相关顶点间边的权值
    lowcost[0] = 0; // 初始化第一个权值为 0，即 v0 加入生成树
    adjvex[0] = 0; // 初始化第一个顶点下标为 0
    for (i = 1; i < G->numVertices; i++) {
        lowcost[i] = INFINITY; // 初始化都为无穷大
        adjvex[i] = 0; // 初始化都指向v0
    }
    // 遍历v0的邻接点，初始化lowcost数组
    EdgeNode* p = G->adjList[0].firstedge;
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
                min = lowcost[j]; // 找到离生成树最近的顶点
                k = j;
            }
            j++;
        }
        printf("(City %d, City %d) %d\n", G->adjList[adjvex[k]].data, G->adjList[k].data, lowcost[k]); // 打印当前顶点边中权值最小的边
        lowcost[k] = 0; // 将当前顶点的权值设置为 0，表示此顶点已经完成任务

        // 遍历新加入的顶点的所有邻接点
        p = G->adjList[k].firstedge;
        while (p) {
            if (lowcost[p->adjvex] != 0 && p->weight < lowcost[p->adjvex]) {
                lowcost[p->adjvex] = p->weight;
                adjvex[p->adjvex] = k;
            }
            p = p->next;
        }
    }
}

// 边集数组结构体
typedef struct {
    int begin;
    int end;
    int weight;
} Edge;

// 对边集数组的边按权值进行排序（这里使用简单的冒泡排序）
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

// 查找连线顶点的尾部下标
int find(int* parent, int f)
{
    while (parent[f] > 0) {
        f = parent[f];
    }
    return f;
}

// 克鲁斯卡尔算法生成最小生成树
void Kruskal(Graph* G)
{
    int i, n, m;
    Edge edges[G->numEdges]; // 定义边集数组
    int parent[MAX_VERTICES]; // 定义一数组用来判断边与边是否形成环路

    // 将邻接表转换为边集数组
    int index = 0;
    for (i = 0; i < G->numVertices; i++) {
        EdgeNode* p = G->adjList[i].firstedge;
        while (p) {
            if (i < p->adjvex) { // 防止添加重复的边
                edges[index].begin = i;
                edges[index].end = p->adjvex;
                edges[index].weight = p->weight;
                index++;
            }
            p = p->next;
        }
    }

    sortEdges(edges, G->numEdges); // 对边按权值进行排序

    for (i = 0; i < G->numVertices; i++) {
        parent[i] = 0; // 初始化 parent 数组
    }

    for (i = 0; i < G->numEdges; i++) {
        n = find(parent, edges[i].begin);
        m = find(parent, edges[i].end);
        if (n != m) { // 假如 n 与 m 不等，说明此边没有与现有的生成树形成环路
            parent[n] = m; // 将此边的结尾顶点放入下标为起点的 parent 中，表示此顶点已经在生成树集合中
            printf("(City %d, City %d) %d\n", G->adjList[edges[i].begin].data, G->adjList[edges[i].end].data, edges[i].weight);
        }
    }
}

// 迪杰斯特拉算法，求 v0 到各顶点的最短路径
void Dijkstra(Graph* G, int v0)
{
    int dist[MAX_VERTICES]; // 存储 v0 到各顶点的当前最短路径长度
    int path[MAX_VERTICES]; // path[v] 表示 v0 到 v 的最短路径上 v 的前驱顶点
    int final[MAX_VERTICES]; // final[v] = 1 表示求得 v0 至 v 的最短路径
    int i, v, w, min;

    // 初始化数据
    for (v = 0; v < G->numVertices; v++) {
        final[v] = 0;
        dist[v] = INFINITY;
        path[v] = -1;
    }
    // 遍历v0的邻接点，初始化dist数组
    EdgeNode* p = G->adjList[v0].firstedge;
    while (p) {
        dist[p->adjvex] = p->weight;
        path[p->adjvex] = v0;
        p = p->next;
    }

    dist[v0] = 0;
    final[v0] = 1; // v0 至 v0 路径为 0

    // 主循环，每次求得 v0 到某个 v 顶点的最短路径
    for (i = 1; i < G->numVertices; i++) {
        min = INFINITY;
        for (w = 0; w < G->numVertices; w++) {
            if (!final[w] && dist[w] < min) {
                v = w;
                min = dist[w];
            }
        }
        final[v] = 1;
        // 遍历新加入的顶点的所有邻接点
        p = G->adjList[v].firstedge;
        while (p) {
            if (!final[p->adjvex] && (min + p->weight < dist[p->adjvex])) {
                dist[p->adjvex] = min + p->weight;
                path[p->adjvex] = v;
            }
            p = p->next;
        }
    }

    // 打印最短路径
    printf("The minimum path length for City %d to City 7: %d\n", G->adjList[v0].data, dist[6]);
    printf("Path: City 7");
    int k = path[6];
    while (k != -1) {
        printf(" <- City %d", G->adjList[k].data);
        k = path[k];
    }
    printf("\n");
}

int main()
{
    Graph G;
    createGraph(&G);
    printf("Graph created!\n");

    printf("DFS:\n");
    for (int i = 0; i < MAX_VERTICES; i++) {
        visited[i] = 0;
    }
    DFS(&G, 0); // Start DFS from vertex 0 (City 1)

    printf("\nBFS:\n");
    for (int i = 0; i < MAX_VERTICES; i++) {
        visited[i] = 0;
    }
    BFS(&G, 0); // Start BFS from vertex 0 (City 1)

    printf("\nMinimum Spanning Tree (Prim):\n");
    Prim(&G);

    printf("\nMinimum Spanning Tree (Kruskal):\n");
    Kruskal(&G);

    printf("\nDijkstra:\n");
    Dijkstra(&G, 0); // Start Dijkstra from vertex 0 (City 1)

    return 0;
}
