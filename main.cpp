#include <iostream>
#include <cstring>

//#define Total_lines 527
//#define Total_stations 3431
const int Total_lines = 528;
const int Total_stations =3431;



/*
 * 将车站作为节点v,公交线路则为关系e.
 * 使用领接表这种数据结构
 * */

typedef struct station_edge{
    int next_adj_station_id;
    int pre_adj_station_id;
    int next_instance;
    int pre_instance;
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
    char name[1024];
    node* head;
}Line;

typedef struct {
    Station stations[Total_stations];
    Line lines[Total_lines];
}Nets;

Station* Create_Station(char* name,int station_id,station_edge* first){
    auto *station = (Station*)malloc(sizeof(Station));
    strcpy(station->name,name);
    station->station_id = station_id;
    station->first = first;
    return station;
}


Station stations[Total_stations];
Line lines[Total_lines];

Line* Create_Line(char* name,int line_id,Node* head){
    auto *line = (Line*)malloc(sizeof(Line));
    strcpy(line->name,name);
    line->line_id = line_id;
    line->head = head;
    return line;
}

void Add_Edge(station_edge* head,Node *n,int line_id){
    auto* stationEdge = (station_edge*)malloc(sizeof(station_edge));
    station_edge *p;
    p = head;
    if(n->pre== nullptr){
        stationEdge->pre_adj_station_id = -1;
    }
    else{
        stationEdge->pre_adj_station_id = n->pre->station_id;
    }
    if(n->next==nullptr){
        stationEdge->next_adj_station_id = -1;
    }
    else{
        stationEdge->next_adj_station_id = n->next->station_id;
    }
    stationEdge->line_id = line_id;
    stationEdge->next_instance = 0;
    stationEdge->pre_instance = 0;
    stationEdge->next = nullptr;
    while(p->next!=nullptr){p=p->next;}
    p->next = stationEdge;
}

Nets Initialize_Nets(){
    Nets *nets = (Nets *)malloc(sizeof(Nets));

    int stations_num=0,lines_num=0;

    FILE *fp_station = fopen("D://C_learn/DSCD/stations.txt","r");
    FILE *fp_line = fopen("D://C_learn/DSCD/lines.txt","r");
//    FILE *fp_station_line = fopen("D://C_learn/DSCD/station_lines.txt","r");


    /* 初始化站点 */
    for(;stations_num<Total_stations;stations_num++){
        char str[1024]={'\0'};
        fscanf(fp_station,"%s",str);
        auto* head = (station_edge*)malloc(sizeof(station_edge));
        head->next = nullptr;
        head->next_adj_station_id = -1;
        head->next_instance = 0;
        head->line_id = -1;
        head->pre_adj_station_id = -1;
        head->pre_instance = 0;
        Station *station = Create_Station(str,stations_num,head);
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


    for(int i=0;i<Total_lines;i++) {
        Node *n = lines[i].head;
        while(n->next!=nullptr){
            for(int j=0;j<Total_stations;j++){
                if(stations[j].station_id == n->station_id){
                    Add_Edge(stations[j].first,n,i);
                }
            }
            n = n->next;
        }
    }

    for(int i=0;i<Total_lines;i++){
        nets->lines[i] = lines[i];
    }
    for(int j=0;j<Total_stations;j++){
        nets->stations[j] = stations[j];
    }
    return *nets;
}

void Show_Lines(Nets nets){
//    for(int i=0;i<Total_lines;i++){
//       Node* n = nets.lines[i].head;
//        printf("%s ",nets.lines[i].name);
//        while(n->next!=nullptr){
//            printf("%s->",nets.stations[n->station_id].name);
//            n = n->next;
//        }
//        printf("\n");
//
//        while(n->pre!=nullptr){
//            printf("%s=>",nets.stations[n->pre->station_id].name);
//            n = n->pre;
//        }
//        printf("\n");
//    }

    for(int j=0;j<Total_stations;j++){
        printf("%d %s ",j,nets.stations[j].name);
//        printf("%s",nets.stations[nets.stations[j].first->next->next_adj_station_id].name);
        Station_Edge* p = nets.stations[j].first->next;
        while(p!= nullptr){
            printf("(%s %s)",nets.stations[p->pre_adj_station_id].name,nets.lines[p->line_id].name);
            p = p->next;
        }
        printf("\n");
    }

}

int main() {
    system("chcp 65001");
    Nets nets = Initialize_Nets();
    Show_Lines(nets);
}
