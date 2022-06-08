//
// Created by С�� on 2022/6/8.
//

#ifndef DSCD_NETS_H
#define DSCD_NETS_H
#include <iostream>
#include "structures.h"
#include "Queue.h"
extern Station stations[Total_stations];
extern Line lines[Total_lines];
/*
 * function Create_Station -> ��ʼ��վ��
 * @params:
 * @char *name -> �����ַ�����
 * @int station_id -> վ��id
 * @station_edge *first -> ͷ��ϵָ��
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
 * function Create_Line -> ��ʼ����·
 * @params:
 * @char *name -> �����ַ�����
 * @int line_id -> ��·id
 * @Node* head -> ͷ�ڵ�
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
 * function Add_Edge -> ��ӹ�ϵ
 * @params:
 * @station_edge *head -> ��ϵͷ�ڵ�
 * @Node *n -> �ڵ�
 * @int line_id -> ��·id
 * @int station_id -> վ��id
 *
 * description:
 * if ǰ��ڵ�Ϊ�� -> ǰ��ڵ�id = -1 ,���� = MAX ,
 * else -> ǰ��ڵ�id = ���Σ���ϵβ������
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
 * function Initialize_Nets -> ��ʼ������
 * @params:
 * None
 *
 * description:
 * ��̬����Nets �ռ�
 * �ļ����� վ����Ϣ ��·��Ϣ
 * ��ʼ��վ��
 * ��ʼ����·
 * վ����ӹ�ϵ
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
    fopen_s(&fp_station, "D://C_learn/DSCD/Resource/stations.txt", "r");
    fopen_s(&fp_line, "D://C_learn/DSCD/Resource/lines.txt", "r");
    fopen_s(&fp_distance, "D://C_learn/DSCD/Resource/distance.txt", "r");
    fopen_s(&fp_poi, "D://C_learn/DSCD/Resource/station_poi.txt", "r");


    /*��ʼ��վ��*/
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


    /*��ʼ����·*/
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

    /*Ϊվ�����ǰ��վ���Լ���·*/
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


#endif //DSCD_NETS_H
