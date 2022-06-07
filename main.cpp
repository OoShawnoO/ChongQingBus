#include <cmath>
#include <iostream>
#include <cstring>
#include <graphics.h>
#include <ege/sys_edit.h>

#define _USED 1
#define _UNUSED 0
#define _DROPED -1

const int Total_lines = 528;
const int Total_stations = 3431;
const int MAX_DISTANCE = 9999999;


enum Mode {
    Most_save = 1,
    Most_fast,
    Least_change,
    Wanna_Pass,
    Synthesize,
};

enum Algorithm {
    Dijkstra = 1,
    DFS,
    BFS,
};

enum Exit {
    Not_Found = 1,
};

enum Action {
    Next = 1,
    Front,
    Rear,
};

/*
 * struct station_edge -> 站点关系
 * @params:
 * @int next_adj_station_id -> 下一邻接站id
 * @int pre_adj_station_id -> 上一邻接站id
 * @int next_distance -> 下一邻接站距离
 * @int pre_distance -> 上一邻接站距离
 * @int current_station_id -> 当前站点id
 * @int line_id -> 当前站点关系所在线路
 * @station_edge *next -> 指向下一关系
 * */

typedef struct station_edge {
    int next_adj_station_id;
    int pre_adj_station_id;
    int next_distance;
    int pre_distance;
    int current_station_id;
    int line_id;
    station_edge *next;
} Station_Edge;

/*
 * struct station -> 站点信息
 * @params:
 * @int station_id -> 站点id
 * @char name[64] -> 站点名称
 * @float poi_x -> 站点Poi X值
 * @float poi_y -> 站点Poi Y值
 * @Station_Edge *first -> 站点关系头节点
 * */

typedef struct station {
    int station_id;
    char name[64];
    float poi_x;
    float poi_y;
    Station_Edge *first;
} Station;

/*
 * struct node -> 线路节点
 * params:
 * @int station_id -> 站点id
 * @int next_distance -> 下一节点的距离
 * @int pre_distance -> 上一节点的距离
 * @node *next -> 指向下一节点的指针
 * @node *pre -> 指向上一节点的指针
 * */

typedef struct node {
    int station_id;
    int next_distance;
    int pre_distance;
    node *next;
    node *pre;
} Node;

/*
 * struct line -> 线路信息
 * @params:
 * @int line_id -> 线路id
 * @char name[64] -> 线路名称
 * @node *head -> 站点头结点
 * */

typedef struct line {
    int line_id;
    char name[64];
    node *head;
} Line;


/*
 * struct Nets -> 站点线路网络
 * @params:
 * @Station stations[] -> 站点数组
 * @Line lines[] -> 线路数组
 * */
typedef struct {
    Station stations[Total_stations];
    Line lines[Total_lines];
} Nets;

/*
 * struct Poi -> 地理位置信息
 * @params:
 * int station_id -> 站点id
 * int line_id -> 线路id
 * int x_axis -> x坐标
 * int y_axis -> y坐标
 * */

typedef struct {
    int station_id;
    int line_id;
    int x_axis;
    int y_axis;
} Poi;

/*
 * struct QNode -> 队列节点
 * @params:
 * @int station_id -> 站点id
 * @int front_id -> 队列中前一层次对应的节点
 * @int line_id -> 线路id
 * @int distance -> 距离
 * @QNode *next -> 指向下一节点
 * */
typedef struct QNode {
    int station_id;
    int front_id;
    int line_id;
    int distance;
    QNode *next;
} QNode;

/*
 * struct LinkQueue -> 链式队列
 * @params:
 * @QNode *head -> 头节点
 * @int rear -> 后继
 * @int front -> 前驱
 * */

typedef struct {
    QNode *head;
    int rear;
    int front;
} LinkQueue;

static Station stations[Total_stations];
static Line lines[Total_lines];

/*
 * function InitQueue -> 初始化队列
 * @动态分配空间
 * return LinkQueue*
 * */

LinkQueue *InitQueue() {
    LinkQueue *linkQueue = (LinkQueue *) malloc(sizeof(LinkQueue));
    QNode *head = (QNode *) malloc(sizeof(QNode));
    head->next = nullptr;
    head->station_id = -1;
    head->front_id = -1;
    head->line_id = -1;
    head->distance = -1;
    linkQueue->head = head;
    linkQueue->rear = 1;
    linkQueue->front = 1;
    return linkQueue;
}

/*
 * function existQueue -> 判存
 * 遍历队列 判存
 * return 0->false 1->true
 * */

int existQueue(LinkQueue *linkQueue, int station_id) {
    auto ptr = linkQueue->head;
    while (ptr->next) {
        if (station_id == ptr->station_id) { return 1; }
        ptr = ptr->next;
    }
    return 0;
}

/*
 * function InQueue -> 入队
 * 动态分配新节点 队尾添加
 * return void
 * */

void InQueue(LinkQueue *linkQueue, int station_id, int front_id, int line_id, int distance) {
    if (!existQueue(linkQueue, station_id)) {
        auto ptr = linkQueue->head;
        QNode *node = (QNode *) malloc(sizeof(QNode));
        node->station_id = station_id;
        node->front_id = front_id;
        node->line_id = line_id;
        node->distance = distance;
        node->next = nullptr;
        linkQueue->rear++;
        while (ptr->next) {
            ptr = ptr->next;
        }
        ptr->next = node;
    } else {
        return;
    }
}

/*
 * function OutQueue -> 出队
 * 队头出队
 * return void
 * */

void OutQueue(LinkQueue *linkQueue, int *station_id) {
    if (linkQueue->front < linkQueue->rear) {
        auto ptr = linkQueue->head;
        int i = 0;
        while (i < linkQueue->front) {
            ptr = ptr->next;
            i++;
        }
        *station_id = ptr->station_id;
        linkQueue->front++;
    } else {
        exit(1000);
    }
}

/*
 * function Create_Station -> 初始化站点
 * @params:
 * @char *name -> 名字字符数组
 * @int station_id -> 站点id
 * @station_edge *first -> 头关系指针
 * return Station*
 * */

Station *Create_Station(char *name, int station_id, station_edge *first) {
    auto *station = (Station *) malloc(sizeof(Station));
    strcpy_s(station->name, name);
    station->station_id = station_id;
    station->first = first;
    return station;
}

/*
 * function Create_Line -> 初始化线路
 * @params:
 * @char *name -> 名字字符数组
 * @int line_id -> 线路id
 * @Node* head -> 头节点
 * return Line*
 * */

Line *Create_Line(char *name, int line_id, Node *head) {
    auto *line = (Line *) malloc(sizeof(Line));
    strcpy_s(line->name, name);
    line->line_id = line_id;
    line->head = head;
    return line;
}

/*
 * function Add_Edge -> 添加关系
 * @params:
 * @station_edge *head -> 关系头节点
 * @Node *n -> 节点
 * @int line_id -> 线路id
 * @int station_id -> 站点id
 *
 * description:
 * if 前后节点为空 -> 前后节点id = -1 ,距离 = MAX ,
 * else -> 前后节点id = 传参，关系尾部插入
 *
 * return void
 * */

void Add_Edge(station_edge *head, Node *n, int line_id, int station_id) {
    auto *stationEdge = (station_edge *) malloc(sizeof(station_edge));
    station_edge *p;
    p = head;
    if (n->pre == nullptr) {
        stationEdge->pre_adj_station_id = -1;
        stationEdge->pre_distance = MAX_DISTANCE;
    } else {
        stationEdge->pre_adj_station_id = n->pre->station_id;
        stationEdge->pre_distance = n->pre_distance;
    }
    if (n->next == nullptr) {
        stationEdge->next_adj_station_id = -1;
        stationEdge->next_distance = MAX_DISTANCE;
    } else {
        stationEdge->next_adj_station_id = n->next->station_id;
        stationEdge->next_distance = n->next_distance;
    }
    stationEdge->current_station_id = station_id;
    stationEdge->line_id = line_id;
    stationEdge->next = nullptr;
    while (p->next != nullptr) { p = p->next; }
    p->next = stationEdge;
}

/*
 * function Initialize_Nets -> 初始化网络
 * @params:
 * None
 *
 * description:
 * 动态分配Nets 空间
 * 文件读入 站点信息 线路信息
 * 初始化站点
 * 初始化线路
 * 站点添加关系
 *
 * return Nets
 * */

Nets Initialize_Nets() {
    Nets *nets = (Nets *) malloc(sizeof(Nets));

    int stations_num = 0, lines_num = 0;

//    FILE *fp_station = fopen("D://C_learn/DSCD/stations.txt","r");
//    FILE *fp_line = fopen("D://C_learn/DSCD/lines.txt","r");
//    FILE *fp_distance = fopen("D://C_learn/DSCD/distance.txt","r");
//    FILE *fp_poi = fopen("D://C_learn/DSCD/station_poi.txt","r");
    FILE *fp_station, *fp_line, *fp_distance, *fp_poi;
    fopen_s(&fp_station, "D://C_learn/DSCD/stations.txt", "r");
    fopen_s(&fp_line, "D://C_learn/DSCD/lines.txt", "r");
    fopen_s(&fp_distance, "D://C_learn/DSCD/distance.txt", "r");
    fopen_s(&fp_poi, "D://C_learn/DSCD/station_poi.txt", "r");


    /*初始化站点*/
    for (; stations_num < Total_stations; stations_num++) {
        char str[1024] = {'\0'};
        fscanf(fp_station, "%s", str);
        auto *head = (station_edge *) malloc(sizeof(station_edge));
        head->next = nullptr;
        head->next_adj_station_id = -1;
        head->next_distance = 0;
        head->line_id = -1;
        head->pre_adj_station_id = -1;
        head->pre_distance = 0;
        Station *station = Create_Station(str, stations_num, head);
        stations[stations_num] = *station;
    }


    /*初始化线路*/
    for (; lines_num < Total_lines; lines_num++) {
        char sta[100], str[100];
        int dis;
        int *distance = &dis;
        Node *head = (Node *) malloc(sizeof(Node));
        Node *p = head, *pre = nullptr;

        fscanf(fp_line, "%s", sta);
        fscanf(fp_distance, "%s", str);


        Line *line = Create_Line(sta, lines_num, p);
        lines[lines_num] = *line;
        while (strcmp(sta, "end") != 0) {
            fscanf(fp_line, "%s", sta);
            fscanf(fp_distance, "%s", str);
            *distance = strtol(str, nullptr, 10);

            for (int i = 0; i < Total_stations; i++) {
                if (strcmp(sta, stations[i].name) == 0) {
                    head->station_id = i;
                    head->next_distance = *distance;
                    head->pre_distance = MAX_DISTANCE;
                    if (pre != nullptr) {
                        head->next_distance = *distance;
                        head->pre_distance = head->pre->next_distance;
                    }

                    Node *q = (Node *) malloc(sizeof(Node));
                    q->next = nullptr;
                    q->pre = head;
                    q->station_id = -1;
                    head->next = q;
                    head->pre = pre;
                    pre = head;
                    head = q;

                    break;
                }
            }
        }
    }

    /*为站点添加前后站点以及线路*/
    for (int i = 0; i < Total_lines; i++) {
        Node *n = lines[i].head;
        while (n->next != nullptr) {
            for (auto &station: stations) {
                if (station.station_id == n->station_id) {
                    Add_Edge(station.first, n, i, station.station_id);
                }
            }
            n = n->next;
        }
    }


    for (int i = 0; i < Total_stations; i++) {
        char name[128] = {'\0'};
        fscanf(fp_poi, "%s", name);
        for (auto &station: stations) {
            if (strcmp(name, station.name) == 0) {
                float x, y;
                char cx[30] = {'\0'};
                char cy[30] = {'\0'};
                fscanf(fp_poi, "%s", cx);
                fscanf(fp_poi, "%s", cy);
                x = strtof(cx, NULL);
                y = strtof(cy, NULL);
                station.poi_x = x;
                station.poi_y = y;
                break;
            }
        }
    }


    for (int i = 0; i < Total_lines; i++) {
        nets->lines[i] = lines[i];
    }
    for (int j = 0; j < Total_stations; j++) {
        nets->stations[j] = stations[j];
    }

    fclose(fp_line);
    fclose(fp_distance);
    fclose(fp_station);
    return *nets;
}

void Show_Lines(Nets nets) {
//    for(int i=0;i<Total_lines;i++){
//       Node* n = nets.lines[i].head;
//        printf("%s ",nets.lines[i].name);
//        while(n->next!=nullptr){
//            printf("%s->%d->",nets.stations[n->station_id].name,n->next_distance);
//            n = n->next;
//        }
//        printf("\n");
//        n = n->pre;
//        while(n!=nullptr){
//            printf("%s=>",nets.stations[n->station_id].name);
//            station_edge* q = nets.stations[n->station_id].first;
//            while(q->line_id!=i){q=q->next;}
//            printf("%d=>",q->pre_distance);
//            n = n->pre;
//        }
//        printf("\n");
//    }

//    for(int j=0;j<Total_stations;j++){
//        printf("%d %s ",j,nets.stations[j].name);
////        printf("%s",nets.stations[nets.stations[j].first->next->next_adj_station_id].name);
//        Station_Edge* p = nets.stations[j].first->next;
//        while(p!= nullptr){
//            printf("(%s %s)",nets.stations[p->pre_adj_station_id].name,nets.lines[p->line_id].name);
//            p = p->next;
//        }
//        printf("\n");
//    }

    for (auto &station: stations) {
        printf("%f ,%f \n", station.poi_x, station.poi_y);
    }
}

/*
 * function Dijkstra_ -> Dijkstra 子函数
 * @params:
 * @int *pass -> 途径线路数组
 * @int *distance -> 距离数组
 * @int *path -> 途径站点数组
 * @int *set -> 占用标志数组
 * @Nets nets -> 网络
 * @int sta1_id -> 终点站点id
 *
 * description:
 * 遍历所有站点寻最短距离站点
 * 查询最短距离站点的关系链
 * 扫描关系链 若进入另一站点的距离+本节点到终点的距离 -> 更新path,通知set已经使用，下次扫描可跳过
 *
 * return void
 *
 * */

void Dijkstra_(int *pass, int *distance, int *path, int *set, Nets nets, int sta1_id) {
    int smallest = MAX_DISTANCE, smallest_index = -1;
    for (int x = 0; x < Total_stations; x++) {
        if (distance[x] < smallest && x != sta1_id && set[x] != _DROPED) {
            smallest = distance[x];
            smallest_index = x;
        }
    }
    station_edge *p = nets.stations[smallest_index].first->next;
    while (p != nullptr) {
        int flag = 0;
        if (p->next_adj_station_id != -1) {
            if (distance[path[p->current_station_id]] + p->next_distance < distance[p->next_adj_station_id]) {
                distance[p->next_adj_station_id] = distance[path[p->current_station_id]] + p->next_distance;
                path[p->next_adj_station_id] = p->current_station_id;
                set[p->next_adj_station_id] = _USED;
                pass[p->next_adj_station_id] = p->line_id;
                flag = 1;
            }
        }
        if (p->pre_adj_station_id != -1) {
            if (distance[path[p->current_station_id]] + p->pre_distance < distance[p->pre_adj_station_id]) {
                distance[p->pre_adj_station_id] = distance[path[p->current_station_id]] + p->pre_distance;
                path[p->pre_adj_station_id] = p->current_station_id;
                set[p->pre_adj_station_id] = _USED;
                pass[p->pre_adj_station_id] = p->line_id;
                flag = 1;
            }
        }
        if (flag != 1) {
            set[smallest_index] = _DROPED;
        }

        p = p->next;
    }
}

/*
 * function Compare -> 查询是否有共用线路
 * @params:
 * @int *sta1_line -> 站点1可及线路数组
 * @int *sta2_line -> 站点2可及线路数组
 *
 * return i -> 共用线路的id ， -1 -> 没有共用线路
 * */

int Compare(int *sta1_line, int *sta2_line) {
    for (int i = 0; i < Total_lines; i++) {
        if (sta1_line[i] != -1 && sta2_line[i] != -1) {
            return i;
        }
    }
    return -1;
}

/*
 * function DoFind -> 寻找可及线路
 * @params:
 * @int *sta_line -> 可及线路
 * @int sta_id -> 查询站点
 * @int* set -> 占用标志数组
 *
 * description:
 * 通过查询站点可经过的线路，添加至可及数组中
 * 可及数组仅为起点所有，找到的线路对应在数组中添加所查询的站点id 以便溯源
 *
 * return void
 * */

void DoFind(int *sta_line, int sta_id, int *set) {
    station_edge *head = stations[sta_id].first;
    if (set[sta_id] != _USED) {
        set[sta_id] = _USED;
        while (head->next) {
            if (sta_line[head->line_id] == -1) {
                sta_line[head->line_id] = head->current_station_id;
            }
            head = head->next;
        }
    } else {
        return;
    }
}

/*
 * function FindWay -> 层次遍历递归寻路
 * @params:
 * @LinkQueue *linkQueue -> 链式队列
 * @int station_id -> 查询站点id
 * @int *pass -> 经过站点数组
 * @int *path -> 经过线路
 * @int *distance -> 距离数组
 *
 * description:
 * 不断向父层次对应的站点进行递归寻路，直到到达查询起点，路径在pass,path中组成。
 *
 * */

void FindWay(LinkQueue *linkQueue, int station_id, int *pass, int *path, int *distance) {
    auto ptr = linkQueue->head->next;
    while (ptr->station_id != station_id) {
        ptr = ptr->next;
    }
    if (ptr->front_id != -1) {
        pass[station_id] = ptr->front_id;
        path[station_id] = ptr->line_id;
        distance[station_id] = ptr->distance;
        FindWay(linkQueue, ptr->front_id, pass, path, distance);
    } else {
        return;
    }
}

/*
 * function test -> 绘图
 * @params:
 * @Poi *pois -> 地址信息数组
 *
 * return void
 * */

void test(Poi *pois) {
    initgraph(1000, 1000);
    setbkcolor(WHITE);
    setfillcolor(BLACK);
    setlinewidth(2);
    ege_enable_aa(true);
    int i = 0;
    while (!(pois[i].x_axis == -1 && pois[i].y_axis == -1 && pois[i].station_id == -1 && pois[i].line_id == -1)) {

//        printf("%d %d %s\n",pois[i].x_axis,pois[i].y_axis,stations[pois[i].station_id].name);
//        putpixel(pois[i].x_axis,pois[i].y_axis,BLACK);
        bar(pois[i].x_axis / 2 + 100, pois[i].y_axis / 2, pois[i].x_axis / 2 + 10 + 100, pois[i].y_axis / 2 + 10);

//        getch();
        outtextxy(pois[i].x_axis / 2 - 20 + 100, pois[i].y_axis / 2 + 20, stations[pois[i].station_id].name);

        if (i == 0) {
            moveto(pois[i].x_axis / 2 + 100, pois[i].y_axis / 2 + 5);
        } else {
            moveto(pois[i - 1].x_axis / 2 + 5 + 100, pois[i - 1].y_axis / 2 + 5);
        }
        int x = getx();
        int y = gety();
        ege::ege_line(float(x), float(y), float(pois[i].x_axis / 2 + 5 + 100), float(pois[i].y_axis / 2 + 5));
        if (i > 0) {
            ege::outtextxy(x - ((x - (pois[i].x_axis / 2 + 5 + 100)) / 2), y - ((y - (pois[i].y_axis) / 2 + 5) / 2),
                           lines[pois[i].line_id].name);
        }

        i++;
    }

    //    bar(50,100,60,110); /*x坐标50 - 60 , y 坐标 100 - 110*/
    getch();
    closegraph();
}

/*
 * function StandardPoi -> 标准化处理Pois
 * @params:
 * @const int *path -> 路径数组 不可修改
 * @const int *pass -> 线路数组 不可修改
 * @int start -> 起点站点id
 * @int end -> 终点站点id
 * @Poi *pois -> 地址数组
 *
 * return void
 * */

void StandardPoi(const int *path, const int *pass, int start, int end, Poi *pois) {
    int seq[2][200];
    for (int i = 0; i < 200; i++) {
        seq[0][i] = -1;
        seq[1][i] = -1;
    }
    int sequence = 0;
    int x = end;
    int last = -1;
    while (x != start) {
        seq[0][sequence] = x;
        seq[1][sequence++] = pass[x];
        last = pass[x];
        x = path[x];
    }
    seq[0][sequence] = x;
    seq[1][sequence++] = last;
    /* 使用MinMax标准处理poi数据 */
    int j = 0;
    float X[50] = {0};
    float Y[50] = {0};
    int S_X[50] = {0};
    int S_Y[50] = {0};
    float max_x = -99999.0;
    float min_x = 999999.0;
    float max_y = -99999.0;
    float min_y = 999999.0;
    while (seq[0][j] != -1) {
        X[j] = stations[seq[0][j]].poi_x;
        Y[j] = stations[seq[0][j]].poi_y;
        if (X[j] > max_x) { max_x = X[j]; }
        if (Y[j] > max_y) { max_y = Y[j]; }
        if (X[j] < min_x) { min_x = X[j]; }
        if (Y[j] < min_y) { min_y = Y[j]; }
        j++;
    }
    j = 0;
    int minx = 9999, miny = 9999, maxx = -1, maxy = -1;
    while (X[j] != 0 && Y[j] != 0) {
        Poi *poixy = (Poi *) malloc(sizeof(Poi));
        X[j] = (X[j] - min_x) / (max_x - min_x);
        Y[j] = (Y[j] - min_y) / (max_y - min_y);
        poixy->x_axis = floor((double) X[j] * 1000);
        if (poixy->x_axis < minx) { minx = poixy->x_axis; }
        if (poixy->x_axis > maxx) { maxx = poixy->x_axis; }
        poixy->y_axis = floor((double) Y[j] * 1000);
        if (poixy->y_axis < miny) { miny = poixy->y_axis; }
        if (poixy->y_axis > maxy) { maxy = poixy->y_axis; }
        poixy->station_id = seq[0][j];
        poixy->line_id = seq[1][j];
        pois[j] = *poixy;
        j++;
    }
    int i = 0;
    int avg_x = 1000 / (maxx - minx);
    int avg_y = 1000 / (maxy - miny);
    while (!(pois[i].x_axis == -1 && pois[i].y_axis == -1 && pois[i].station_id == -1 && pois[i].line_id == -1)) {
        pois[i].x_axis = (pois[i].x_axis - minx) * avg_x;
        pois[i].y_axis = (pois[i].y_axis - miny) * avg_y;
        i++;
    }
}

/*
 * function Go -> 寻路主程序入口
 * @params:
 * @Nets nets -> 网络
 * @char* station_1 -> 车站1名称
 * @char* station_2 -> 车站2名称
 * @Mode mode -> 枚举类型 寻路模式
 *
 * description:
 *  比对车站名称 确立车站id
 *  根据输入模式进入不同寻路模式
 * return void;
 * */

void Go(Nets nets, char *station_1, char *station_2, Mode mode) {
    int sta1_id = -1, sta2_id = -1;
    for (int id = 0; id < Total_stations; id++) {
        if (strcmp(nets.stations[id].name, station_1) == 0) { sta1_id = id; }
        if (strcmp(nets.stations[id].name, station_2) == 0) { sta2_id = id; }
        if (sta1_id != -1 && sta2_id != -1) { break; }
    }
    if (sta1_id == -1 || sta2_id == -1) {
        exit(Not_Found);
    } else {
        /*最短路径模式*/
        if (mode == Most_fast) {
            /*初始化数据*/
            int distance[Total_stations];
            for (int &i: distance) { i = MAX_DISTANCE; }
            int path[Total_stations] = {-1};
            for (int &i: path) { i = -1; }
            int set[Total_stations] = {_UNUSED};
            int pass[Total_stations];
            for (int &pas: pass) { pas = -1; }
            /*为出发站点设置对应数据*/
            distance[sta1_id] = 0;
            set[sta1_id] = _USED;
            path[sta1_id] = sta1_id;
            station_edge *p = nets.stations[sta1_id].first->next;
            /*进入寻路，将所有关系全录入*/
            while (p->next != nullptr) {
                if (p->next_distance < distance[p->next_adj_station_id]) {
                    distance[p->next_adj_station_id] = p->next_distance;
                    path[p->next_adj_station_id] = p->current_station_id;
                    set[p->next_adj_station_id] = _USED;
                    pass[p->next_adj_station_id] = p->line_id;
                }
                if (p->pre_distance < distance[p->pre_adj_station_id]) {
                    distance[p->pre_adj_station_id] = p->pre_distance;
                    path[p->pre_adj_station_id] = p->current_station_id;
                    set[p->pre_adj_station_id] = _USED;
                    pass[p->pre_adj_station_id] = p->line_id;
                }
                p = p->next;
            }
            /*循环查找直至找到终点*/
            while (distance[sta2_id] == MAX_DISTANCE) {
                Dijkstra_(pass, distance, path, set, nets, sta1_id);
            }
            /*为绘制路径做初始化*/
            int x = sta2_id;
            Poi pois[50];
            for (int i = 0; i < 50; i++) {
                Poi *poix = (Poi *) malloc(sizeof(Poi));
                poix->x_axis = -1;
                poix->y_axis = -1;
                poix->line_id = -1;
                poix->station_id = -1;
                pois[i] = *poix;
            }
            StandardPoi(path, pass, sta1_id, sta2_id, pois);
            int x1 = 0;
            while (x != sta1_id) {
                printf("%s->乘坐%s->", nets.stations[x].name, nets.lines[pass[x]].name);
                x = path[x];
            }
            printf("%s distance:%d", nets.stations[sta1_id].name, distance[sta2_id]);
            fflush(stdout);
            test(pois);
        }
        /*最少换乘模式*/
        if (mode == Least_change) {
            /*初始化数据*/
            int sta1_line[Total_lines];
            int sta2_line[Total_lines];
            int set[Total_stations];
            int path[Total_stations];
            int pass[Total_stations];
            int distance[Total_stations];
            for (int &i: set) { i = _UNUSED; }
            for (int &i: sta1_line) { i = -1; }
            for (int &i: sta2_line) { i = -1; }
            for (int &i: pass) { i = -1; }
            for (int &i: path) { i = -1; }
            for (int &i: distance) { i = -1; }
            int sta1 = sta1_id;
            /*为终点录入关系以便后续查找同一线路*/
            station_edge *head = stations[sta2_id].first;
            while (head->next) {
                if (head->line_id != -1) {
                    sta2_line[head->line_id] = head->line_id;
                }
                head = head->next;
            }
            station_edge *head1 = stations[sta1_id].first;
            /*初始化链式队列*/
            LinkQueue *linkQueue = InitQueue();
            InQueue(linkQueue, sta1, -1, -1, -1);
            /*不断查找直至起点的衍生节点找到与终点同一线路*/
            while (Compare(sta1_line, sta2_line) == -1) {
                DoFind(sta1_line, sta1, set);
                auto ptr = stations[sta1].first->next;
                while (ptr->next) {
                    if (ptr->pre_adj_station_id != -1) {
                        InQueue(linkQueue, ptr->pre_adj_station_id, sta1, ptr->line_id, ptr->pre_distance);
                    }
                    if (ptr->next_adj_station_id != -1) {
                        InQueue(linkQueue, ptr->next_adj_station_id, sta1, ptr->line_id, ptr->next_distance);
                    }
                    ptr = ptr->next;
                }
                OutQueue(linkQueue, &sta1);
            }
            int x = Compare(sta1_line, sta2_line);
//            printf("Find! at %s\n",lines[x].name);
            station_edge *ptr = stations[sta2_id].first;
            while (ptr->next) {
                if (ptr->line_id == x) {
                    break;
                }
                ptr = ptr->next;
            }
//            printf("%s,%s\n",stations[ptr->next_adj_station_id].name,stations[ptr->pre_adj_station_id].name);
            int staid = sta1_line[x];
            FindWay(linkQueue, staid, pass, path, distance);
            node *p = lines[x].head;
            node *q = p;
            int z = 0, y = 0;
            while (p->station_id != staid) {
                p = p->next;
                z++;
            }
            while (q->station_id != sta2_id) {
                q = q->next;
                y++;
            }
            if (z > y) {
                while (q != p) {
                    pass[q->station_id] = q->next->station_id;
                    path[q->station_id] = x;
                    q = q->next;
                }
            } else {
                while (q != p) {
                    pass[p->next->station_id] = p->station_id;
                    path[p->station_id] = x;
                    p = p->next;
                }
            }
//            printf("%s(%s)->",stations[staid].name,lines[path[staid]].name);
            staid = sta2_id;
            path[sta2_id] = x;
            while (pass[staid] != -1) {
                printf("%s(%s)->", stations[staid].name, lines[path[staid]].name);
                staid = pass[staid];
            }
            printf("%s", stations[staid].name);

            Poi pois[50];
            for (int i = 0; i < 50; i++) {
                Poi *poix = (Poi *) malloc(sizeof(Poi));
                poix->x_axis = -1;
                poix->y_axis = -1;
                poix->line_id = -1;
                poix->station_id = -1;
                pois[i] = *poix;
            }
            StandardPoi(pass, path, sta1_id, sta2_id, pois);
            test(pois);
        }
    }
}


/*
 * Nets -> stations[] / lines[]
 *
 * Station -> station_id
 *          / name
 *          / Station_Edge
 *
 * Station_Edge -> next_adj_station_id
 *              / pre_adj_station_id
 *              / next_instance
 *              / pre_instance
 *              / line_id
 *              / station_edge* next
 *
 * Line -> line_id
 *      / name
 *      / Node* head
 *
 * Node -> station_id
 *  / next_distance
 *  / pre_distance
 *  / Node* next
 *  / Node* pre
 *
 * */

using namespace  std;
void GUI(){
    initgraph(1000,800);
    PIMAGE image = newimage();
    getimage(image,"C://Users/84083/Desktop/order.jpg");
    putimage(0,0,image);
    sys_edit editBox;
    editBox.create(false);
    editBox.move(200,400);
    editBox.size(500,30);
    editBox.setfont(20,0,"");
    char str[200];
    editBox.visible(true);
    while(true){
        if(mousemsg()){
            mouse_msg msg = getmouse();
            if(msg.is_left()){
                cout<<msg.x<<","<<msg.y<<endl;
            }
        }
        editBox.gettext(200,str);
        printf("%s",str);
        xyprintf(0,0,str);
    }
    getch();
    closegraph();
}



int main() {
//    system("chcp 65001");
//    Nets nets = Initialize_Nets();
//    char sta1[128] = {0};
//    char sta2[128] = {0};
//    char scanfTip[128] = "请输入起始站点 终止站点:";
//    printf("%s", scanfTip);
//    fflush(stdout);
//    scanf("%s %s", sta2, sta1);       /* 测试用例 ：重庆北站南广场站 大石坝二村站*/ /*重庆北站南广场站 小什字站*/
//    Go(nets, sta1, sta2, Most_fast);
    GUI();
}

