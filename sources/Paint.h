//
// Created by 小胡 on 2022/6/8.
//

#ifndef DSCD_PAINT_H
#define DSCD_PAINT_H

#include <graphics.h>
#include <ege/sys_edit.h>
#include <iostream>
#include "structures.h"
using namespace std;

extern Station stations[Total_stations];
extern Line lines[Total_lines];
extern Nets nets;
int Go(const Nets nets, char *station_1, char *station_2, Mode mode);

/*
 * function test -> 绘图
 * @params:
 * @Poi *pois -> 地址信息数组
 *
 * return void
 * */

int test(Poi *pois) {
    if(!is_run()){
        initgraph(1000, 1000);
    }
    setbkcolor(WHITE);
    setfillcolor(BLACK);
    setlinewidth(2);
    ege_enable_aa(true);
    int i = 0;
    while (!(pois[i].x_axis == -1 && pois[i].y_axis == -1 && pois[i].station_id == -1 && pois[i].line_id == -1)) {

//        printf("%d %d %s\n",pois[i].x_axis,pois[i].y_axis,stations[pois[i].station_id].name);
//        putpixel(pois[i].x_axis,pois[i].y_axis,BLACK);
        bar(pois[i].x_axis / 2 + 100, pois[i].y_axis / 2, pois[i].x_axis / 2 + 10 + 100, pois[i].y_axis / 2 + 10);

//        getch();
        outtextxy(pois[i].x_axis / 2 - 20 + 100, pois[i].y_axis / 2 + 20, stations[pois[i].station_id].name);

        if (i == 0) {
            moveto(pois[i].x_axis / 2 + 100, pois[i].y_axis / 2 + 5);
        } else {
            moveto(pois[i - 1].x_axis / 2 + 5 + 100, pois[i - 1].y_axis / 2 + 5);
        }
        int x = getx();
        int y = gety();
        ege::ege_line(float(x), float(y), float(pois[i].x_axis / 2 + 5 + 100), float(pois[i].y_axis / 2 + 5));
        if (i > 0) {
            ege::outtextxy(x - ((x - (pois[i].x_axis / 2 + 5 + 100)) / 2), y - ((y - (pois[i].y_axis) / 2 + 5) / 2),
                           lines[pois[i].line_id].name);
        }

        i++;
    }

    //    bar(50,100,60,110); /*x坐标50 - 60 , y 坐标 100 - 110*/
    xyprintf(500,600,"按任意键关闭。");
    int x = getch();
    cleardevice();
    closegraph();
    return x;
}

/*
 * function StandardPoi -> 标准化处理Pois
 * @params:
 * @const int *path -> 路径数组 不可修改
 * @const int *pass -> 线路数组 不可修改
 * @int start -> 起点站点id
 * @int end -> 终点站点id
 * @Poi *pois -> 地址数组
 *
 * return void
 * */

void StandardPoi(const int *path, const int *pass, int start, int end, Poi *pois) {
    int seq[2][200];
    for (int i = 0; i < 200; i++) {
        seq[0][i] = -1;
        seq[1][i] = -1;
    }
    int sequence = 0;
    int x = end;
    int last = -1;
    while (x != start) {
        seq[0][sequence] = x;
        seq[1][sequence++] = pass[x];
        last = pass[x];
        x = path[x];
    }
    seq[0][sequence] = x;
    seq[1][sequence++] = last;
    /* 使用MinMax标准处理poi数据 */
    int j = 0;
    float X[50] = {0};
    float Y[50] = {0};
    int S_X[50] = {0};
    int S_Y[50] = {0};
    float max_x = -99999.0;
    float min_x = 999999.0;
    float max_y = -99999.0;
    float min_y = 999999.0;
    while (seq[0][j] != -1) {
        X[j] = stations[seq[0][j]].poi_x;
        Y[j] = stations[seq[0][j]].poi_y;
        if (X[j] > max_x) { max_x = X[j]; }
        if (Y[j] > max_y) { max_y = Y[j]; }
        if (X[j] < min_x) { min_x = X[j]; }
        if (Y[j] < min_y) { min_y = Y[j]; }
        j++;
    }
    j = 0;
    int minx = 9999, miny = 9999, maxx = -1, maxy = -1;
    while (X[j] != 0 && Y[j] != 0) {
        Poi *poixy = (Poi *) malloc(sizeof(Poi));
        X[j] = (X[j] - min_x) / (max_x - min_x);
        Y[j] = (Y[j] - min_y) / (max_y - min_y);
        poixy->x_axis = floor((double) X[j] * 1000);
        if (poixy->x_axis < minx) { minx = poixy->x_axis; }
        if (poixy->x_axis > maxx) { maxx = poixy->x_axis; }
        poixy->y_axis = floor((double) Y[j] * 1000);
        if (poixy->y_axis < miny) { miny = poixy->y_axis; }
        if (poixy->y_axis > maxy) { maxy = poixy->y_axis; }
        poixy->station_id = seq[0][j];
        poixy->line_id = seq[1][j];
        pois[j] = *poixy;
        j++;
    }
    int i = 0;
    int avg_x = 1000 / (maxx - minx);
    int avg_y = 1000 / (maxy - miny);
    while (!(pois[i].x_axis == -1 && pois[i].y_axis == -1 && pois[i].station_id == -1 && pois[i].line_id == -1)) {
        pois[i].x_axis = (pois[i].x_axis - minx) * avg_x;
        pois[i].y_axis = (pois[i].y_axis - miny) * avg_y;
        i++;
    }
}

void GUI() {
    initgraph(1000, 800);
    setbkcolor(WHITE);
    PIMAGE image = newimage();
    getimage(image, "../Resource/tj.png");
    putimage(0, 0, image);
    sys_edit editBox1,editBox2;

    editBox1.create(false);
    editBox1.move(203, 61);
    editBox1.size(418, 97);
    editBox1.setfont(40, 0, "");
    editBox1.visible(true);

    editBox2.create(false);
    editBox2.move(203, 232);
    editBox2.size(418, 97);
    editBox2.setfont(40, 0, "");
    editBox2.visible(true);

    char str1[200];
    char str2[200];
    while (true) {
        if (mousemsg()) {
            mouse_msg msg = getmouse();
            if (msg.is_left()&&msg.x>=241&&msg.x<=560&&msg.y>=461&&msg.y<=567) {
                editBox1.gettext(200, str1);
                editBox2.gettext(200,str2);
                if(is_run()){
                    editBox1.visible(false);
                    editBox2.visible(false);
                    cleardevice();
                }
                break;
            }
        }
        if(kbhit()){
            int x = getch();
            if(x==ESC){
                break;
            }
            if(x==CR){

            }
        }
    }
    Go(nets,str1,str2,Least_change);
}

#endif //DSCD_PAINT_H
