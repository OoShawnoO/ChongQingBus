#include <cmath>
#include <windows.h>
#include <iostream>
#include <cstring>
#include "structures.h"
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

int Go(const Nets nets, char *station_1, char *station_2, Mode mode) {
    int sta1_id = -1, sta2_id = -1;
    for (int id = 0; id < Total_stations; id++) {
        if (strcmp(nets.stations[id].name, station_1) == 0) { sta1_id = id; }
        if (strcmp(nets.stations[id].name, station_2) == 0) { sta2_id = id; }
        if (sta1_id != -1 && sta2_id != -1) { break; }
    }
    if (sta1_id == -1 || sta2_id == -1) {
        exit(No_Match);
    } else {

        if (mode == Most_fast) {
            return dijkstra(sta1_id,sta2_id,nets);
        }

        if (mode == Least_change) {
            return bfs(sta1_id,sta2_id);
        }
    }
    return ESC;
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


Nets nets = Initialize_Nets();

int main() {
//    system("chcp 65001");
    char sta1[128] = {0};
    char sta2[128] = {0};
    int flag = SPACE;
    Mode mode = Least_change;
    initgraph(800,600);
    setrendermode(ege::RENDER_MANUAL);
    setcaption("重庆公交查询系统");
    PIMAGE image = newimage();
    getimage(image, "../Resource/index.png");
    putimage(0, 0, image);
    delay_ms(0);
    while(true){
        switch(flag){
            case ESC:{
                printf("结束。");
                exit(End);
            }
            case SPACE:{
                printf("---------------------------\n");
                printf("请选择模式：\n");
                printf("1.最快速模式\n2.最少换乘模式\n");
                printf("---------------------------\n");
                int x = 0;
                printf("请选择:");
                fflush(stdout);
                scanf("%d",&x);
                if(x==1){mode = Most_fast;}
                if(x==2){mode = Least_change;}
            }
            case CR:{
//                char scanfTip[128] = "请输入起始站点 终止站点:";
//                printf("%s", scanfTip);
//                fflush(stdout);
//                scanf("%s %s", sta2, sta1);       /* 测试用例 ：重庆北站南广场站 大石坝二村站*/ /*重庆北站南广场站 小什字站*/
                GUI(sta1,sta2);
                flag = Go(nets, sta1, sta2, mode);

            }
        }
    }

}

