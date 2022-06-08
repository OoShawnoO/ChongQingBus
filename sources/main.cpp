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
    char scanfTip[128] = "请输入起始站点 终止站点:";
    printf("%s", scanfTip);
    fflush(stdout);
    scanf("%s %s", sta2, sta1);       /* 测试用例 ：重庆北站南广场站 大石坝二村站*/ /*重庆北站南广场站 小什字站*/
    Go(nets, sta1, sta2, Most_fast);
//    GUI();
}

