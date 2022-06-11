//
// Created by 小胡 on 2022/6/8.
//

#ifndef DSCD_BFS_H
#define DSCD_BFS_H

#include <iostream>
#include "structures.h"

extern Station stations[Total_stations];
extern Line lines[Total_lines];

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

int bfs(int sta1_id,int sta2_id){
    /*最少换乘模式*/
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
    return test(pois);
}


#endif //DSCD_BFS_H
