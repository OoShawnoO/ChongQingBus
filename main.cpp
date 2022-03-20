#include <iostream>
#include <cstring>

#define Total_lines 30
#define Total_stations 150


/*
 * 将车站作为节点v,公交线路则为关系e.
 * 使用领接表这种数据结构
 * */

typedef struct station_edge{
    int next_adj_station_id;
    int pre_adj_station_id;
    int instance;
    int line_id;
    station_edge* next;
}Station_Edge;

typedef struct station{
    int station_id;
    char name[1024];
    Station_Edge *first;
}Station;

typedef struct node{
    int station_id;
    node* next;
    node* pre;
}Node;

typedef struct line{
    int line_id;
    char name[100];
    node* head;
}Line;

typedef struct {
    Station *stations;
    Line *lines;
}Nets;

Station* Create_Station(char* name,int station_id,station_edge* first){
    auto *station = (Station*)malloc(sizeof(Station));
    strcpy(station->name,name);
    station->station_id = station_id;
    station->first = first;
    return station;
}

Line* Create_Line(char* name,int line_id,Node* head){
    auto *line = (Line*)malloc(sizeof(Line));
    strcpy(line->name,name);
    line->line_id = line_id;
    line->head = head;
    return line;
}

Nets Initialize_Nets(){
    Nets *nets = (Nets *)malloc(sizeof(Nets));
    Station stations[Total_stations];
    Line lines[Total_lines];
    int stations_num=0,lines_num=0;

    FILE *fp_station = fopen("D://C_learn/DSCD/stations.txt","r");
    FILE *fp_line = fopen("D://C_learn/DSCD/lines.txt","r");
    FILE *fp_station_line = fopen("D://C_learn/DSCD/station_lines.txt","r");


    /* 初始化站点 */
    for(;stations_num<Total_stations;stations_num++){
        char str[1024]={'\0'};
        fscanf(fp_station,"%s",str);
        Station *station = Create_Station(str,stations_num,nullptr);
        stations[stations_num] = *station;
    }

    /* 初始化路线 */
    for(;lines_num<Total_lines;lines_num++){
        char sta[100];
        Node *head =(Node *)malloc(sizeof(Node));
        Node *p = head,*pre = nullptr;

        fscanf(fp_line,"%s",sta);
        Line *line = Create_Line(sta,lines_num,p);
        lines[lines_num] = *line;
        while(strcmp(sta,"end") != 0){
            fscanf(fp_line,"%s",sta);
            for(int i=0;i<Total_stations;i++){
                if(strcmp(sta,stations[i].name)==0){
                    head->station_id = i;
                    Node *q = (Node*)malloc(sizeof(Node));
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

    nets->lines = lines;
    nets->stations = stations;
    return *nets;
}

void Show_Lines(Nets nets){
    for(int i=0;i<Total_lines;i++){
       Node* n = nets.lines[i].head;
        printf("%s ",nets.lines[i].name);
        while(n->next!=nullptr){
            printf("%s->",nets.stations[n->station_id].name);
            n = n->next;
        }
        printf("\n");

        while(n->pre!=nullptr){
            printf("%s=>",nets.stations[n->pre->station_id].name);
            n = n->pre;
        }
        printf("\n");
    }

//    for(int j=0;j<Total_stations;j++){
//        printf("%d %s\n",j,nets.stations[j].name);
//    }
}

int main() {
    system("chcp 65001");
    Nets nets = Initialize_Nets();
    Show_Lines(nets);
}
