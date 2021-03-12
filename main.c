#include <stdio.h>
#include <malloc.h>
/*
 * INT_MAX 表示 +INF
 * INT_MIN 表示 -INF
 * */

#define MaxVertexNum 100
#define DefaultVertexValue '\0'
#define DefaultEdgeValue 0
#define  InfoType int
typedef char VertexType;
typedef unsigned EdgeType;

// 邻接矩阵法
typedef struct {
    VertexType vertex[MaxVertexNum];
    EdgeType Edge[MaxVertexNum][MaxVertexNum];
    int vexnum, arcnum;
} MGraph;

// 邻接表法
typedef struct ArcNode { // 边表节点
    InfoType info;
    int adjvex;
    struct ArcNode *next;
} ArcNode;
typedef struct VNode {   // 顶点表节点
    VertexType data;
    struct ArcNode *first;
} VNode, AdjList[MaxVertexNum];  // 顶点表AdjList
typedef struct {
    AdjList vertices;   // 顶点表
    int vexnum, arcnum;  // 顶点数, 边数
} ALGraph;

// 十字链表
typedef struct XArcNode {    // 边节点
    InfoType info; // 弧信息
    int tailvex, headvex;    // headvex,tailvex:弧头/弧尾节点
    struct XArcNode *hlink, *tlink;  // hlink,tlink:指向弧头/弧尾相同的下一条弧
} XArcNode;
typedef struct XNode {   // 顶点
    VertexType data;    // 数据
    XArcNode *firstin, *firstout;    // firstin/firstout:第一个以该点为弧尾/弧头的弧
} XNode, XNodeList[MaxVertexNum];
typedef struct {
    XNodeList xlist;  // 顶点列表
    int vexnum, arcnum;  // 节点数,边数
} OLGraph;

// 邻接多重表
typedef struct MTArcNode {  // 边表
    InfoType info;     // 弧信息
    int mark, ivex, jvex;   // 访问标记,弧头节点,弧尾节点
    struct MTArcNode *ilink, *jlink;    // 下一个同弧头/弧尾的弧
} MTArcNode;
typedef struct MTNode { // 顶点表
    VertexType data;    // 数据
    MTArcNode *firstedge;   // 指向第一条依附该节点的边
} MTNode, MTNodeList[MaxVertexNum];
typedef struct {    // 邻接多重表
    MTNodeList mlist;   // 顶点表
    int vexnum, arcnum; // 顶点数,边数
} MTGraph;

// 取得节点下标,不存在返回-1
int MGraph_GetVertex(MGraph *graph, VertexType x) {
    for (int i = 0; i < MaxVertexNum; ++i) {
        if (graph->vertex[i] == x) return i;
    }
    return -1;
}
int ALGraph_GetVertex(ALGraph *graph, VertexType x) {
    if(x==DefaultVertexValue) return -1;
    for (int i = 0; i < MaxVertexNum; ++i) {
        if (graph->vertices[i].data == x) return i;
    }
    return -1;
}

// 判断图G中是否存在边<x,y>
int MGraph_Adjacent(MGraph *graph, VertexType x, VertexType y) {
    int m = MGraph_GetVertex(graph, x), n = MGraph_GetVertex(graph, y);
    if (m == -1 || n == -1) return 0;
    return graph->Edge[m][n];
}
int ALGraph_Adjacent(ALGraph *graph, VertexType x, VertexType y) {
    int m = ALGraph_GetVertex(graph, x);
    if (m == -1) return 0;
    ArcNode *p = graph->vertices[m].first;
    while (p){
        if(graph->vertices[p->adjvex].data == y) return p->info;
        p = p->next;
    }
    return 0;
}

// 列出图G中与节点x邻接的边
void MGraph_Neighbors(MGraph *graph, VertexType x) {
    int m = MGraph_GetVertex(graph, x);
    if (m == -1) return;
    for (int j = 0; j < MaxVertexNum; ++j) {
        if (m != j && graph->Edge[m][j] > 0) printf("<%c, %c>\n", x, graph->Edge[j]);
    }
    if (graph->Edge[m][m] > 0) printf("<%c, %c>\n", x, graph->Edge[m]);
}
void ALGraph_Neighbors(ALGraph *graph, VertexType x) {
    int m = ALGraph_GetVertex(graph, x);
    if (m == -1) return;
    ArcNode *p = graph->vertices[m].first;
    while (p){
        printf("<%c, %c>\n", x, graph->vertices[p->adjvex].data);
        p = p->next;
    }
}

// 在图G中插入顶点x
int MGraph_InsertVertex(MGraph *graph, VertexType x) {
    if (MGraph_GetVertex(graph, x) != -1) return -1;
    for (int i = 0; i < MaxVertexNum; ++i) {
        if (graph->vertex[i] == DefaultVertexValue) {
            graph->vertex[i] = x;
            graph->vexnum++;
            return i;
        }
    }
    return -1;
}
int ALGraph_InsertVertex(ALGraph *graph, VertexType x) {
    if (ALGraph_GetVertex(graph, x) != -1) return -1;
    for (int i = 0; i < MaxVertexNum; ++i) {
        if (graph->vertices[i].data==DefaultVertexValue) {
            graph->vertices[i].data = x;
            graph->vertices[i].first = NULL;
            graph->vexnum++;
            return i;
        }
    }
    return -1;
}
// 在图G中删除顶点x
int MGraph_DeleteVertex(MGraph *graph, VertexType x) {
    int m = MGraph_GetVertex(graph, x);
    if (m == -1) return 0;
    graph->vertex[m] = DefaultVertexValue;
    graph->vexnum--;
    for (int j = 0; j < MaxVertexNum; ++j) {
        if (graph->Edge[m][j] != DefaultEdgeValue) {
            graph->Edge[m][j] = DefaultEdgeValue;
            graph->arcnum--;
        }
        if (graph->Edge[j][m] != DefaultEdgeValue) {
            graph->Edge[j][m] = DefaultEdgeValue;
            graph->arcnum--;
        }
    }
    return 1;
}
int ALGraph_DeleteVertex(ALGraph *graph, VertexType x) {
    int m = ALGraph_GetVertex(graph, x);
    if (m == -1) return 0;
    for (int i = 0; i < MaxVertexNum; ++i) {
        if(graph->vertices[i].data!=DefaultVertexValue){
            ArcNode temp;
            temp.next = graph->vertices[i].first;
            ArcNode *p = &temp;
            while (p!=NULL&&p->next){
                if(graph->vertices[i].data==x||p->next->adjvex == m){
                    ArcNode *t = p->next;
                    p->next = t->next;
                    free(t);
                }
                p = p->next;
            }
            graph->vertices[i].first = temp.next;
        }
    }
    graph->vertices[m].data = DefaultVertexValue;
    graph->vexnum--;
    return 1;
}

// 若不存在对应边则进行加入<x,y>
void MGraph_AddEdge(MGraph *graph, VertexType x, VertexType y, EdgeType weight) {
    int m = MGraph_GetVertex(graph, x);
    int n = MGraph_GetVertex(graph, y);
    if (m == -1 || n == -1) {
        if (m == -1) m = MGraph_InsertVertex(graph, x);
        if (n == -1) n = MGraph_InsertVertex(graph, y);
    }
    graph->Edge[m][n] = weight;
    graph->arcnum++;
}
void ALGraph_AddEdge(ALGraph *graph, VertexType x, VertexType y, EdgeType weight) {
    int m = ALGraph_GetVertex(graph, x);
    int n = ALGraph_GetVertex(graph, y);
    if (m == -1 || n == -1) {
        if (m == -1) m = ALGraph_InsertVertex(graph, x);
        if (n == -1) n = ALGraph_InsertVertex(graph, y);
    }
    ArcNode temp;
    temp.next = graph->vertices[m].first;
    ArcNode *p = &temp;
    while (p!=NULL){
        if (p->next == NULL){
            p->next = (ArcNode*)malloc(sizeof(ArcNode));
            p->next->next = NULL;
            p->next->adjvex = n;
            p->info = weight;
            break;
        }else if(p->next->adjvex == n){
            p->info = weight;
            break;
        }else{
            p = p->next;
        }
    }
    graph->vertices[m].first = temp.next;
    graph->arcnum++;
}

// 若存在对应边则进行删除<x,y>
void MGraph_RemoveEdge(MGraph *graph, VertexType x, VertexType y) {
    int m = MGraph_GetVertex(graph, x);
    int n = MGraph_GetVertex(graph, y);
    if (m != -1 && n != -1) graph->Edge[m][n] = DefaultEdgeValue;
}
void ALGraph_RemoveEdge(ALGraph *graph, VertexType x, VertexType y) {
    int m = ALGraph_GetVertex(graph, x);
    if (m != -1){
        ArcNode temp;
        temp.next = graph->vertices[m].first;
        ArcNode *p = &temp;
        while (p!=NULL&&p->next!=NULL){
            if(graph->vertices[p->next->adjvex].data == y){
                ArcNode *t = p->next;
                p->next = t->next;
                free(t);
                break;
            }
            p = p->next;
        }
        graph->vertices[m].first = temp.next;
    }
}

// 返回第一个邻接点
int MGraph_FirstNeighbor(MGraph *graph, VertexType x) {
    int m = MGraph_GetVertex(graph, x);
    if (m == -1) return -1;
    for (int i = 0; i < MaxVertexNum; ++i) {
        if (graph->Edge[m][i] != DefaultEdgeValue) return i;
    }
    return -1;
}
int ALGraph_FirstNeighbor(ALGraph *graph, VertexType x) {
    int m = ALGraph_GetVertex(graph, x);
    if (m == -1) return -1;
    if(graph->vertices[m].first==NULL) return -1;
    return graph->vertices[m].first->adjvex;
}

// 假设y是x的邻接点,返回除y以外顶点x的下一个邻接点-不进行判断是否真的存在
int MGraph_NextNeighbor(MGraph *graph, VertexType x, VertexType y) {
    int m = MGraph_GetVertex(graph, x);
    for (int i = MGraph_GetVertex(graph, y) + 1; i < MaxVertexNum; ++i) {
        if (graph->Edge[m][i] != DefaultEdgeValue) return i;
    }
    return -1;
}
int ALGraph_NextNeighbor(ALGraph *graph, VertexType x, VertexType y) {
    int m = ALGraph_GetVertex(graph, x);
    ArcNode *p = graph->vertices[m].first;
    while (p){
        if (graph->vertices[p->adjvex].data==y&&p->next!=NULL){
            return p->next->adjvex;
        }
        p = p->next;
    }
    return -1;
}

// 取得图G中边对应的权值
EdgeType MGraph_Get_edge_value(MGraph *graph, VertexType x, VertexType y) {
    int m = MGraph_GetVertex(graph, x), n = MGraph_GetVertex(graph, y);
    if (m == -1 || n == -1) return 0;
    return graph->Edge[m][n];
}
EdgeType ALGraph_Get_edge_value(ALGraph *graph, VertexType x, VertexType y) {
    int m = ALGraph_GetVertex(graph, x);
    if (m == -1) return 0;
    ArcNode *p = graph->vertices[m].first;
    while (p){
        if(graph->vertices[p->adjvex].data == y){
            return p->info;
        }
        p = p->next;
    }
    return 0;
}

// 设置图G中边对应的权值
int MGraph_Set_edge_value(MGraph *graph, VertexType x, VertexType y, EdgeType v) {
    int m = MGraph_GetVertex(graph, x), n = MGraph_GetVertex(graph, y);
    if (m == -1 || n == -1) return -1;
    graph->Edge[m][n] = v;
    return 1;
}
int ALGraph_Set_edge_value(ALGraph *graph, VertexType x, VertexType y, EdgeType v) {
    int m = ALGraph_GetVertex(graph, x);
    if (m == -1) return -1;
    ArcNode *p = graph->vertices[m].first;
    while (p){
        if(graph->vertices[p->adjvex].data == v){
            p->info = v;
            return 1;
        }
    }
    return -1;
}

// 初始化图
void MGraph_init(MGraph *graph) {
    graph->vexnum = 0;
    graph->arcnum = 0;
    for (int i = 0; i < MaxVertexNum; ++i) {
        graph->vertex[i] = DefaultVertexValue;
        for (int j = 0; j < MaxVertexNum; ++j) {
            graph->Edge[i][j] = DefaultEdgeValue;
        }
    }
}
void ALGraph_init(ALGraph *graph) {
    graph->vexnum = 0;
    graph->arcnum = 0;
    for (int i = 0; i < MaxVertexNum; ++i) {
        graph->vertices[i].first = NULL;
        graph->vertices[i].data = '\0';
    }
}

// 深拷贝图
void MGraph_data_copy(MGraph *resource, MGraph *target) {
    target->vexnum = resource->vexnum;
    target->arcnum = resource->arcnum;
    for (int i = 0; i < MaxVertexNum; ++i) {
        target->vertex[i] = resource->vertex[i];
        for (int j = 0; j < MaxVertexNum; ++j) {
            target->Edge[i][j] = resource->Edge[i][j];
        }
    }
}
void ALGraph_data_copy(ALGraph *resource, ALGraph *target) {
    target->vexnum = resource->vexnum;
    target->arcnum = resource->arcnum;
    for (int i = 0; i < MaxVertexNum; ++i) {
        target->vertices[i].data = resource->vertices[i].data;
        ArcNode temp;
        temp.next = NULL;
        ArcNode *p1 = resource->vertices[i].first,*p2=&temp;
        while (p1){
            p2->next = (ArcNode*)malloc(sizeof(ArcNode));
            p2 = p2->next;
            p2->next = NULL;
            p2->info = p1->info;
            p2->adjvex = p1->adjvex;
            p1 = p1->next;
        }
        target->vertices[i].first = temp.next;
    }
}

// 无向图的最小生成树Prim算法
void MGraph_Prim_MST(MGraph *graph, MGraph *result) {
    MGraph_init(result);    // 清空原数据
    int temp[MaxVertexNum] = {0};   // 标记节点是否被加入
    for (int i = 0; i < MaxVertexNum; ++i) { // 检查每个节点是否加入最小生成树
        if (graph->vertex[i] != '\0' && temp[i] == 0) { // 存在节点并且未被加入
            temp[i] = 1;
            MGraph_InsertVertex(result, graph->vertex[i]);
            int j = i;
            while (j >= 0 && j < MaxVertexNum) {    // 循环寻找最小生成树的边
                int min_index_st = -1; // 下一个节点
                int min_index_en = -1;
                int min_value = INT_MAX; // 与下一个节点相连的边
                for (int l = 0; l < MaxVertexNum; ++l) {    // 寻找下一个节点,复杂度2V
                    if (temp[l] == 0) continue;
                    for (int k = 0; k < MaxVertexNum; ++k) {
                        if (temp[k] == 1 || graph->vertex[k] == '\0') continue;
                        if (graph->Edge[l][k] != DefaultEdgeValue && graph->Edge[l][k] < min_value) {
                            min_index_st = l;
                            min_index_en = k;
                            min_value = graph->Edge[l][k];
                        }
                    }
                }
                if (min_index_en != -1) {  // 存在下一条边时才进行加入
                    MGraph_AddEdge(result, graph->vertex[min_index_st], graph->vertex[min_index_en], min_value);
                    MGraph_AddEdge(result, graph->vertex[min_index_en], graph->vertex[min_index_st], min_value);
                    // printf("%c <-> %c:%d\n", graph->vertex[min_index_st] ,graph->vertex[min_index_en],min_value);
                    temp[min_index_en] = 1;
                }
                j = min_index_en;
            }
        }
    }
}

// 无向图最小生成树的Kruskal算法-并查集
int MGraph_Kruskal_Find(int *temp, int k) {
    if (temp[k] != k) temp[k] = MGraph_Kruskal_Find(temp, temp[k]);
    return temp[k];
}

// 并查集-并
void MGraph_Kruskal_Union(int *temp, int m, int n) {
    temp[MGraph_Kruskal_Find(temp, temp[m])] = MGraph_Kruskal_Find(temp, n);
}

// 最小生成树
void MGraph_Kruskal_MST(MGraph *graph, MGraph *result) {
    int edgeValues[MaxVertexNum * MaxVertexNum][3] = {0};
    int edgeIndex = 0;
    for (int i = 0; i < MaxVertexNum; ++i) {    // 检测矩阵上三角
        for (int j = i; j < MaxVertexNum; ++j) {
            edgeValues[edgeIndex][0] = i;
            edgeValues[edgeIndex][1] = j;
            edgeValues[edgeIndex][2] = (graph->Edge[i][j] == DefaultEdgeValue ? INT_MAX : graph->Edge[i][j]);
            edgeIndex++;
        }
    }
    for (int i = 0; i < edgeIndex; ++i) {   // 根据权值排序-冒泡排序n^2
        for (int j = 1; j < edgeIndex - i; ++j) {
            if (edgeValues[j][2] < edgeValues[j - 1][2]) {
                int t = edgeValues[j][0];   // 交换i
                edgeValues[j][0] = edgeValues[j - 1][0];
                edgeValues[j - 1][0] = t;
                t = edgeValues[j][1];       // 交换j
                edgeValues[j][1] = edgeValues[j - 1][1];
                edgeValues[j - 1][1] = t;
                t = edgeValues[j][2];       // 交换权值
                edgeValues[j][2] = edgeValues[j - 1][2];
                edgeValues[j - 1][2] = t;
            }
        }
    }
    MGraph_init(result);    // 清空原数据
    int temp[MaxVertexNum] = {0};   // 标记节点是否被加入
    for (int i = 0; i < MaxVertexNum; ++i) {    // 节点全部加入
        temp[i] = i;
        result->vertex[i] = graph->vertex[i];
    }
    result->vexnum = graph->vexnum;
    for (int i = 0; i < edgeIndex; ++i) {   // 加入边
        if (edgeValues[i][2] == INT_MAX) break;
        // 不连通则加入边
        if (MGraph_Kruskal_Find(temp, edgeValues[i][0]) != MGraph_Kruskal_Find(temp, edgeValues[i][1])) {
            MGraph_Kruskal_Union(temp, edgeValues[i][0], edgeValues[i][1]);
            result->Edge[edgeValues[i][0]][edgeValues[i][1]] = edgeValues[i][2];
            result->Edge[edgeValues[i][1]][edgeValues[i][0]] = edgeValues[i][2];
            result->arcnum += (edgeValues[i][0] == edgeValues[i][1] ? 1 : 2);
        }
    }
}

// Dijkatra算法:graph-图, st-起始节点, en-结束节点, s-路径, 返回值-路径长度
int MGraph_Dijkatra_Distances(MGraph *graph, VertexType st, VertexType en, VertexType *s, int len) {
    int m = MGraph_GetVertex(graph, st);
    int n = MGraph_GetVertex(graph, en);
    if (m == -1 || n == -1 || len < graph->vexnum * 3) return 0;
    EdgeType dist[MaxVertexNum];
    int visited[MaxVertexNum];
    int count = graph->vexnum;
    int path[MaxVertexNum];
    // 初始化
    for (int i = 0; i < MaxVertexNum; ++i) {
        visited[i] = (graph->vertex[i] == '\0' ? 1 : 0);
        dist[i] = (graph->Edge[m][i] == DefaultEdgeValue ? INT_MAX : graph->Edge[m][i]);
        path[i] = (graph->Edge[m][i] == DefaultEdgeValue ? -1 : m);
    }
    visited[m] = 1;
    count--;
    while (count > 0) {
        int min_index = -1;
        int min_value = INT_MAX;
        for (int i = 0; i < MaxVertexNum; ++i) {
            if (visited[i] == 0 && min_value > dist[i]) {
                min_index = i;
                min_value = dist[i];
            }
        }
        if (min_index == -1) break;
        visited[min_index] = 1;
        count--;
        for (int i = 0; i < MaxVertexNum; ++i) {
            if (visited[i] == 0 && graph->Edge[min_index][i] != DefaultEdgeValue &&
                min_value + graph->Edge[min_index][i] < dist[i]) {
                dist[i] = min_value + graph->Edge[min_index][i];
                path[i] = min_index;
            }
        }
    }
    char s_stack[MaxVertexNum] = "";
    int s_index = 0;
    int r = 0;
    for (int i = n; i != -1; i = path[i]) {
        s_stack[s_index++] = graph->vertex[i];
    }
    while (s_index > 0) {
        s[r++] = s_stack[--s_index];
        if (s_index != 0) {
            s[r++] = '-';
            s[r++] = '>';
        }
    }
    return dist[n] == INT_MAX ? 0 : dist[n];
}

// Floyd算法计算各顶点之间距离
// 1. A^-1[i][j] = Edge[i][j]
// 2. A^k[i][j] = Min{A^(k-1)[i][j], A^(k-1)[i][k]+A^(k-1)[k][j]}
EdgeType **MGraph_Floyd_Distances(MGraph graph) {
    // 计算的临时空间,两空间交替使用
    EdgeType temp1[MaxVertexNum][MaxVertexNum];
    EdgeType temp2[MaxVertexNum][MaxVertexNum];
    // 使用指针方便交换数据空间
    EdgeType (*t1)[MaxVertexNum] = temp1;
    EdgeType (*t2)[MaxVertexNum] = temp2;
    // 初始化区域
    for (int i = 0; i < MaxVertexNum; ++i) {
        for (int j = 0; j < MaxVertexNum; ++j) {
            temp1[i][j] = graph.Edge[i][j];
        }
    }
    // 对于每个节点进行递推公式计算合计 MaxVertexNum^3 次
    for (int k = 0; k < MaxVertexNum; ++k) {
        for (int i = 0; i < MaxVertexNum; ++i) {
            for (int j = 0; j < MaxVertexNum; ++j) {
                t2[i][j] = t1[i][j];
                // 判断是否可以跳转
                if (t1[i][k] != DefaultEdgeValue && t1[k][j] != DefaultEdgeValue) {
                    EdgeType p = t1[i][k] + t1[k][j];
                    if (t2[i][j] > p) t2[i][j] = p;
                }
            }
        }
        // 交换t1和t2
        EdgeType (*t3)[MaxVertexNum] = t1;
        t1 = t2;
        t2 = t3;
    }
    // 返回值为二维数组
    EdgeType **res = malloc(MaxVertexNum * sizeof(EdgeType *)); // 行
    for (int i = 0; i < MaxVertexNum; ++i) {
        res[i] = malloc(MaxVertexNum * sizeof(EdgeType));   // 列
    }
    // 拷贝结果
    for (int i = 0; i < MaxVertexNum; ++i) {
        for (int j = 0; j < MaxVertexNum; ++j) {
            res[i][j] = t1[i][j];
        }
    }
    return res;
}

// 拓扑排序
int MGraph_TopologicalSort(MGraph *graph, VertexType *s, int n) {
    if (graph->vexnum >= n) return 0;
    int stack[MaxVertexNum] = {0};
    int stack_index = 0;
    int InDegree[MaxVertexNum] = {0};
    for (int i = 0; i < MaxVertexNum; ++i) {
        for (int j = 0; j < MaxVertexNum; ++j) {
            if (graph->Edge[i][j] != DefaultEdgeValue) {
                InDegree[j]++;
            }
        }
    }
    for (int i = 0; i < MaxVertexNum; ++i) {
        if (InDegree[i] == 0 && graph->vertex[i] != '\0') stack[stack_index++] = i;
    }
    int count = 0;
    while (stack_index > 0) {
        int top = stack[--stack_index];
        s[count++] = graph->vertex[top];
        for (int i = MGraph_FirstNeighbor(graph, graph->vertex[top]);
             i != -1; i = MGraph_NextNeighbor(graph, graph->vertex[top], graph->vertex[i])) {
            InDegree[i]--;
            if (InDegree[i] == 0) stack[stack_index++] = i;
        }
    }
    if (count < graph->vexnum) {
        s[0] = '\0';
        return 0;
    }
    return 1;
}

// 最早发生时间
int MGraph_AOE_ve(MGraph *graph, EdgeType *ve) {
    for (int i = 0; i < MaxVertexNum; ++i) {
        ve[i] = 0;
    }
    int stack[MaxVertexNum] = {0};
    int stack_index = 0;
    int InDegree[MaxVertexNum] = {0};
    for (int i = 0; i < MaxVertexNum; ++i) {
        for (int j = 0; j < MaxVertexNum; ++j) {
            if (graph->Edge[i][j] != DefaultEdgeValue) {
                InDegree[j]++;
            }
        }
    }
    for (int i = 0; i < MaxVertexNum; ++i) {
        if (InDegree[i] == 0 && graph->vertex[i] != '\0') stack[stack_index++] = i;
    }
    int count = 0;
    while (stack_index > 0) {
        int top = stack[--stack_index];
        for (int i = MGraph_FirstNeighbor(graph, graph->vertex[top]);
             i != -1; i = MGraph_NextNeighbor(graph, graph->vertex[top], graph->vertex[i])) {
            InDegree[i]--;
            if (ve[top] + graph->Edge[top][i] > ve[i]) ve[i] = ve[top] + graph->Edge[top][i];
            if (InDegree[i] == 0) stack[stack_index++] = i;
        }
    }
    if (count < graph->vexnum) {
        return 0;
    }
    return 1;

}

// 最迟发生时间
int MGraph_AOE_vl(MGraph *graph, EdgeType *vl) {
    EdgeType topo[MaxVertexNum] = {0};
    int stack[MaxVertexNum] = {0};
    int stack_index = 0;
    int InDegree[MaxVertexNum] = {0};
    for (int i = 0; i < MaxVertexNum; ++i) {
        for (int j = 0; j < MaxVertexNum; ++j) {
            if (graph->Edge[i][j] != DefaultEdgeValue) {
                InDegree[j]++;
            }
        }
    }
    for (int i = 0; i < MaxVertexNum; ++i) {
        if (InDegree[i] == 0 && graph->vertex[i] != '\0') stack[stack_index++] = i;
    }
    int count = 0;
    while (stack_index > 0) {
        int top = stack[--stack_index];
        topo[count++] = top;
        for (int i = MGraph_FirstNeighbor(graph, graph->vertex[top]);
             i != -1; i = MGraph_NextNeighbor(graph, graph->vertex[top], graph->vertex[i])) {
            InDegree[i]--;
            if (InDegree[i] == 0) stack[stack_index++] = i;
        }
    }
    if (count < graph->vexnum) {
        return 0;
    }
    EdgeType ve[MaxVertexNum] = {0};
    MGraph_AOE_ve(graph, ve);
    for (int i = 0; i < count; ++i) {
        vl[topo[i]] = ve[topo[count - 1]];
    }
    while (count > 0) {
        EdgeType top = topo[--count];
        for (int i = 0; i < MaxVertexNum; ++i) {
            if ((graph->Edge[i][top] != DefaultEdgeValue) && (vl[top] - graph->Edge[i][top] < vl[i])) {
                vl[i] = vl[top] - graph->Edge[i][top];
            }
        }
    }
    return 1;
}

int main() {
    MGraph p;
    MGraph_init(&p);
    // 有向图
    MGraph_AddEdge(&p, '1', '2', 3);
    MGraph_AddEdge(&p, '1', '3', 8);
    MGraph_AddEdge(&p, '2', '4', 7);
    MGraph_AddEdge(&p, '3', '2', 4);
    MGraph_AddEdge(&p, '3', '5', 10);
    MGraph_AddEdge(&p, '2', '5', 6);
    MGraph_AddEdge(&p, '4', '6', 6);
    MGraph_AddEdge(&p, '5', '6', 9);

    VertexType path[300] = "";
    int t = MGraph_Dijkatra_Distances(&p, '1', '6', path, 300);
    EdgeType ve[MaxVertexNum];
    EdgeType vl[MaxVertexNum];
    MGraph_AOE_ve(&p, ve);
    MGraph_AOE_vl(&p, vl);
    for (int i = 0; i < 11; ++i) {
        printf("%d ", ve[i]);
    }
    printf("\n");
    for (int i = 0; i < 11; ++i) {
        printf("%d ", vl[i]);
    }
    printf("\n");
    printf("%s\n", path);
    printf("%d\n", t);
    return 0;
}
