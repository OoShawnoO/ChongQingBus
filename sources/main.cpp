#include <cmath>
#include <iostream>
#include <cstring>
#include "Paint.h"
#include "structures.h"
#include "Queue.h"
#include "Nets.h"
#include "dijkstra.h"
#include "bfs.h"

Station stations[Total_stations];
Line lines[Total_lines];





/*
 * function Go -> Ѱ·���������
 * @params:
 * @Nets nets -> ����
 * @char* station_1 -> ��վ1����
 * @char* station_2 -> ��վ2����
 * @Mode mode -> ö������ Ѱ·ģʽ
 *
 * description:
 *  �ȶԳ�վ���� ȷ����վid
 *  ��������ģʽ���벻ͬѰ·ģʽ
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

        if (mode == Most_fast) {
            dijkstra(sta1_id,sta2_id,nets);
        }

        if (mode == Least_change) {
            bfs(sta1_id,sta2_id);
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




int main() {
//    system("chcp 65001");
    Nets nets = Initialize_Nets();
    char sta1[128] = {0};
    char sta2[128] = {0};
    char scanfTip[128] = "��������ʼվ�� ��ֹվ��:";
    printf("%s", scanfTip);
    fflush(stdout);
    scanf("%s %s", sta2, sta1);       /* �������� �����챱վ�Ϲ㳡վ ��ʯ�Ӷ���վ*/ /*���챱վ�Ϲ㳡վ Сʲ��վ*/
    Go(nets, sta1, sta2, Most_fast);
//    GUI();
}

