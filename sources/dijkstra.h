//
// Created by 小胡 on 2022/6/8.
//

#ifndef DSCD_DIJKSTRA_H
#define DSCD_DIJKSTRA_H

#include <iostream>
#include "structures.h"
#include "Paint.h"

extern Station stations[Total_stations];
extern Line lines[Total_lines];

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

int dijkstra(int sta1_id,int sta2_id,Nets nets){
    /*最短路径模式*/
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
        printf("%s(%s)->", nets.stations[x].name, nets.lines[pass[x]].name);
        x = path[x];
    }
    printf("\n%s distance:%d", nets.stations[sta1_id].name, distance[sta2_id]);
    fflush(stdout);
    return test(pois);
}


#endif //DSCD_DIJKSTRA_H
