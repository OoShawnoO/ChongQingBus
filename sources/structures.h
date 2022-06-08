//
// Created by 小胡 on 2022/6/8.
//

#ifndef DSCD_STRUCTURES_H
#define DSCD_STRUCTURES_H

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

#endif //DSCD_STRUCTURES_H
