//
// Created by С�� on 2022/6/8.
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
 * struct station_edge -> վ���ϵ
 * @params:
 * @int next_adj_station_id -> ��һ�ڽ�վid
 * @int pre_adj_station_id -> ��һ�ڽ�վid
 * @int next_distance -> ��һ�ڽ�վ����
 * @int pre_distance -> ��һ�ڽ�վ����
 * @int current_station_id -> ��ǰվ��id
 * @int line_id -> ��ǰվ���ϵ������·
 * @station_edge *next -> ָ����һ��ϵ
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
 * struct station -> վ����Ϣ
 * @params:
 * @int station_id -> վ��id
 * @char name[64] -> վ������
 * @float poi_x -> վ��Poi Xֵ
 * @float poi_y -> վ��Poi Yֵ
 * @Station_Edge *first -> վ���ϵͷ�ڵ�
 * */

typedef struct station {
    int station_id;
    char name[64];
    float poi_x;
    float poi_y;
    Station_Edge *first;
} Station;

/*
 * struct node -> ��·�ڵ�
 * params:
 * @int station_id -> վ��id
 * @int next_distance -> ��һ�ڵ�ľ���
 * @int pre_distance -> ��һ�ڵ�ľ���
 * @node *next -> ָ����һ�ڵ��ָ��
 * @node *pre -> ָ����һ�ڵ��ָ��
 * */

typedef struct node {
    int station_id;
    int next_distance;
    int pre_distance;
    node *next;
    node *pre;
} Node;

/*
 * struct line -> ��·��Ϣ
 * @params:
 * @int line_id -> ��·id
 * @char name[64] -> ��·����
 * @node *head -> վ��ͷ���
 * */

typedef struct line {
    int line_id;
    char name[64];
    node *head;
} Line;


/*
 * struct Nets -> վ����·����
 * @params:
 * @Station stations[] -> վ������
 * @Line lines[] -> ��·����
 * */
typedef struct {
    Station stations[Total_stations];
    Line lines[Total_lines];
} Nets;

/*
 * struct Poi -> ����λ����Ϣ
 * @params:
 * int station_id -> վ��id
 * int line_id -> ��·id
 * int x_axis -> x����
 * int y_axis -> y����
 * */

typedef struct {
    int station_id;
    int line_id;
    int x_axis;
    int y_axis;
} Poi;

/*
 * struct QNode -> ���нڵ�
 * @params:
 * @int station_id -> վ��id
 * @int front_id -> ������ǰһ��ζ�Ӧ�Ľڵ�
 * @int line_id -> ��·id
 * @int distance -> ����
 * @QNode *next -> ָ����һ�ڵ�
 * */
typedef struct QNode {
    int station_id;
    int front_id;
    int line_id;
    int distance;
    QNode *next;
} QNode;

/*
 * struct LinkQueue -> ��ʽ����
 * @params:
 * @QNode *head -> ͷ�ڵ�
 * @int rear -> ���
 * @int front -> ǰ��
 * */

typedef struct {
    QNode *head;
    int rear;
    int front;
} LinkQueue;

#endif //DSCD_STRUCTURES_H
