//
// Created by С�� on 2022/6/8.
//

#ifndef DSCD_DIJKSTRA_H
#define DSCD_DIJKSTRA_H

#include <iostream>
#include "structures.h"
#include "Paint.h"

extern Station stations[Total_stations];
extern Line lines[Total_lines];

/*
 * function Dijkstra_ -> Dijkstra �Ӻ���
 * @params:
 * @int *pass -> ;����·����
 * @int *distance -> ��������
 * @int *path -> ;��վ������
 * @int *set -> ռ�ñ�־����
 * @Nets nets -> ����
 * @int sta1_id -> �յ�վ��id
 *
 * description:
 * ��������վ��Ѱ��̾���վ��
 * ��ѯ��̾���վ��Ĺ�ϵ��
 * ɨ���ϵ�� ��������һվ��ľ���+���ڵ㵽�յ�ľ��� -> ����path,֪ͨset�Ѿ�ʹ�ã��´�ɨ�������
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
    /*���·��ģʽ*/
    /*��ʼ������*/
    int distance[Total_stations];
    for (int &i: distance) { i = MAX_DISTANCE; }
    int path[Total_stations] = {-1};
    for (int &i: path) { i = -1; }
    int set[Total_stations] = {_UNUSED};
    int pass[Total_stations];
    for (int &pas: pass) { pas = -1; }
    /*Ϊ����վ�����ö�Ӧ����*/
    distance[sta1_id] = 0;
    set[sta1_id] = _USED;
    path[sta1_id] = sta1_id;
    station_edge *p = nets.stations[sta1_id].first->next;
    /*����Ѱ·�������й�ϵȫ¼��*/
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
    /*ѭ������ֱ���ҵ��յ�*/
    while (distance[sta2_id] == MAX_DISTANCE) {
        Dijkstra_(pass, distance, path, set, nets, sta1_id);
    }
    /*Ϊ����·������ʼ��*/
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
