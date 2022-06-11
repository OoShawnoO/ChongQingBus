//
// Created by С�� on 2022/6/8.
//

#ifndef DSCD_BFS_H
#define DSCD_BFS_H

#include <iostream>
#include "structures.h"

extern Station stations[Total_stations];
extern Line lines[Total_lines];

/*
 * function Compare -> ��ѯ�Ƿ��й�����·
 * @params:
 * @int *sta1_line -> վ��1�ɼ���·����
 * @int *sta2_line -> վ��2�ɼ���·����
 *
 * return i -> ������·��id �� -1 -> û�й�����·
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
 * function DoFind -> Ѱ�ҿɼ���·
 * @params:
 * @int *sta_line -> �ɼ���·
 * @int sta_id -> ��ѯվ��
 * @int* set -> ռ�ñ�־����
 *
 * description:
 * ͨ����ѯվ��ɾ�������·��������ɼ�������
 * �ɼ������Ϊ������У��ҵ�����·��Ӧ���������������ѯ��վ��id �Ա���Դ
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
 * function FindWay -> ��α����ݹ�Ѱ·
 * @params:
 * @LinkQueue *linkQueue -> ��ʽ����
 * @int station_id -> ��ѯվ��id
 * @int *pass -> ����վ������
 * @int *path -> ������·
 * @int *distance -> ��������
 *
 * description:
 * �����򸸲�ζ�Ӧ��վ����еݹ�Ѱ·��ֱ�������ѯ��㣬·����pass,path����ɡ�
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
    /*���ٻ���ģʽ*/
    /*��ʼ������*/
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
    /*Ϊ�յ�¼���ϵ�Ա��������ͬһ��·*/
    station_edge *head = stations[sta2_id].first;
    while (head->next) {
        if (head->line_id != -1) {
            sta2_line[head->line_id] = head->line_id;
        }
        head = head->next;
    }
    station_edge *head1 = stations[sta1_id].first;
    /*��ʼ����ʽ����*/
    LinkQueue *linkQueue = InitQueue();
    InQueue(linkQueue, sta1, -1, -1, -1);
    /*���ϲ���ֱ�����������ڵ��ҵ����յ�ͬһ��·*/
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
