#include <iostream>
#include <cstring>

#define _USED 1
#define _UNUSED 0
#define _DROPED -1

const int Total_lines = 528;
const int Total_stations =3431;
const int  MAX_DISTANCE=9999999;


/*
 * ����վ��Ϊ�ڵ�v,������·��Ϊ��ϵe.
 * ʹ����ӱ��������ݽṹ
 * */

enum Mode{
    Most_save = 1, /*���ģʽ  �����Ǻܶ೵��ȫ�̶���2Ԫ ���߳�������10վ ÿ100m+0.1Ԫ�� */
    Most_fast,     /*�����ģʽ*/
    Least_change,   /*���ٻ���ģʽ*/
    Wanna_Pass,     /*��������ģʽ*/
    Synthesize,     /*�ۺ�ģʽ*/
};

enum Algorithm{     /*���õ��㷨*/
    Dijkstra = 1,   /*Dijkstra�㷨*/
    DFS,            /*�������*/
    BFS,            /*�������*/
};

enum Exit{
    Not_Found = 1,
};

typedef struct station_edge{
    int next_adj_station_id;
    int pre_adj_station_id;
    int next_distance;
    int pre_distance;
    int current_station_id;
    int line_id;
    station_edge* next;
}Station_Edge;

typedef struct station{
    int station_id;
    char name[512];
    Station_Edge *first;
}Station;

typedef struct node{
    int station_id;
    int next_distance;
    int pre_distance;
    node* next;
    node* pre;
}Node;

typedef struct line{
    int line_id;
    char name[512];
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

static Station stations[Total_stations];
static Line lines[Total_lines];

Line* Create_Line(char* name,int line_id,Node* head){
    auto *line = (Line*)malloc(sizeof(Line));
    strcpy(line->name,name);
    line->line_id = line_id;
    line->head = head;
    return line;
}

void Add_Edge(station_edge* head,Node *n,int line_id,int station_id){
    auto* stationEdge = (station_edge*)malloc(sizeof(station_edge));
    station_edge *p;
    p = head;
    if(n->pre== nullptr){
        stationEdge->pre_adj_station_id = -1;
        stationEdge->pre_distance = MAX_DISTANCE;
    }
    else{
        stationEdge->pre_adj_station_id = n->pre->station_id;
        stationEdge->pre_distance = n->pre_distance;
    }
    if(n->next==nullptr){
        stationEdge->next_adj_station_id = -1;
        stationEdge->next_distance = MAX_DISTANCE;
    }
    else{
        stationEdge->next_adj_station_id = n->next->station_id;
        stationEdge->next_distance = n->next_distance;
    }
    stationEdge->current_station_id = station_id;
    stationEdge->line_id = line_id;
    stationEdge->next = nullptr;
    while(p->next!=nullptr){p=p->next;}
    p->next = stationEdge;
}

Nets Initialize_Nets(){
    Nets *nets = (Nets *)malloc(sizeof(Nets));

    int stations_num=0,lines_num=0;

    FILE *fp_station = fopen("D://C_learn/DSCD/stations.txt","r");
    FILE *fp_line = fopen("D://C_learn/DSCD/lines.txt","r");
    FILE *fp_distance = fopen("D://C_learn/DSCD/distance.txt","r");

    /*初始化站点*/
    for(;stations_num<Total_stations;stations_num++){
        char str[1024]={'\0'};
        fscanf(fp_station,"%s",str);
        auto* head = (station_edge*)malloc(sizeof(station_edge));
        head->next = nullptr;
        head->next_adj_station_id = -1;
        head->next_distance = 0;
        head->line_id = -1;
        head->pre_adj_station_id = -1;
        head->pre_distance = 0;
        Station *station = Create_Station(str,stations_num,head);
        stations[stations_num] = *station;
    }


    /*初始化线路*/
    for(;lines_num<Total_lines;lines_num++){
        char sta[100],str[100];
        int dis;
        int* distance = &dis;
        Node *head =(Node *)malloc(sizeof(Node));
        Node *p = head,*pre = nullptr;

        fscanf(fp_line,"%s",sta);
        fscanf(fp_distance,"%s",str);


        Line *line = Create_Line(sta,lines_num,p);
        lines[lines_num] = *line;
        while(strcmp(sta,"end") != 0){
            fscanf(fp_line,"%s",sta);
            fscanf(fp_distance,"%s",str);
            *distance = strtol(str,nullptr,10);

            for(int i=0;i<Total_stations;i++){
                if(strcmp(sta,stations[i].name)==0){
                    head->station_id = i;
                    head->next_distance = *distance;
                    head->pre_distance = MAX_DISTANCE;
                    if(pre!=nullptr){
                        head->next_distance = *distance;
                        head->pre_distance = head->pre->next_distance;
                    }

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

    /*为站点添加前后站点以及线路*/
    for(int i=0;i<Total_lines;i++) {
        Node *n = lines[i].head;
        while(n->next!=nullptr){
            for(auto & station : stations){
                if(station.station_id == n->station_id){
                    Add_Edge(station.first,n,i,station.station_id);
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
    fclose(fp_line);
    fclose(fp_distance);
    fclose(fp_station);
    return *nets;
}

void Show_Lines(Nets nets){
//    for(int i=0;i<Total_lines;i++){
//       Node* n = nets.lines[i].head;
//        printf("%s ",nets.lines[i].name);
//        while(n->next!=nullptr){
//            printf("%s->%d->",nets.stations[n->station_id].name,n->next_distance);
//            n = n->next;
//        }
//        printf("\n");
//        n = n->pre;
//        while(n!=nullptr){
//            printf("%s=>",nets.stations[n->station_id].name);
//            station_edge* q = nets.stations[n->station_id].first;
//            while(q->line_id!=i){q=q->next;}
//            printf("%d=>",q->pre_distance);
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

void Dijkstra_(int* pass,int* distance,int* path,int* set,Nets nets,int sta1_id){
    int smallest = MAX_DISTANCE,smallest_index = -1;
    for(int x=0;x<Total_stations;x++){
        if(distance[x]<smallest && x!=sta1_id && set[x]!=_DROPED){smallest=distance[x];smallest_index=x;}
    }
    station_edge *p = nets.stations[smallest_index].first->next;
    while(p!= nullptr) {
        int flag = 0;
        if(p->next_adj_station_id!=-1){
            if(distance[path[p->current_station_id]]+p->next_distance<distance[p->next_adj_station_id]){
                distance[p->next_adj_station_id] = distance[path[p->current_station_id]]+p->next_distance;
                path[p->next_adj_station_id] = p->current_station_id;
                set[p->next_adj_station_id] = _USED;
                pass[p->next_adj_station_id] = p->line_id;
                flag =1;
            }
        }
        if(p->pre_adj_station_id!=-1){
            if(distance[path[p->current_station_id]]+p->pre_distance<distance[p->pre_adj_station_id]){
                distance[p->pre_adj_station_id] = distance[path[p->current_station_id]]+p->pre_distance;
                path[p->pre_adj_station_id] = p->current_station_id;
                set[p->pre_adj_station_id] = _USED;
                pass[p->pre_adj_station_id] = p->line_id;
                flag =1;
            }
        }
        if(flag != 1){
            set[smallest_index] = _DROPED;
        }

        p = p->next;
    }
}

void Go(Nets nets,char* station_1,char* station_2,Mode mode,Algorithm alg,...){
    int sta1_id=-1,sta2_id=-1;
    for(int id=0;id<Total_stations;id++){
        if(strcmp(nets.stations[id].name,station_1)==0){sta1_id = id;}
        if(strcmp(nets.stations[id].name,station_2)==0){sta2_id = id;}
        if(sta1_id!=-1&&sta2_id!=-1){break;}
    }
    if(sta1_id==-1||sta2_id==-1){
        exit(Not_Found);
    }
    else{
        if(alg==Dijkstra){
            int distance[Total_stations];
            for(int & i : distance){i=MAX_DISTANCE;}
            int path[Total_stations] = {-1};
            for(int & i : path){i=-1;}
            int set[Total_stations] = {_UNUSED};
            int pass[Total_stations];
            for(int & pas : pass){pas=-1;}

            distance[sta1_id] = 0;
            set[sta1_id] = _USED;
            path[sta1_id] = sta1_id;
            station_edge *p = nets.stations[sta1_id].first->next;
            while(p->next!=nullptr) {
                if(p->next_distance<distance[p->next_adj_station_id]){
                    distance[p->next_adj_station_id] = p->next_distance;
                    path[p->next_adj_station_id] = p->current_station_id;
                    set[p->next_adj_station_id] = _USED;
                    pass[p->next_adj_station_id] = p->line_id;

                }
                if(p->pre_distance<distance[p->pre_adj_station_id]){
                    distance[p->pre_adj_station_id] = p->pre_distance;
                    path[p->pre_adj_station_id] = p->current_station_id;
                    set[p->pre_adj_station_id] = _USED;
                    pass[p->pre_adj_station_id] = p->line_id;

                }
                p = p->next;
            }
            while(distance[sta2_id]==MAX_DISTANCE){
                Dijkstra_(pass,distance,path,set,nets,sta1_id);
            }
            int x = sta2_id;
            while(x!=sta1_id){
                printf("%s->乘坐%s->",nets.stations[x].name,nets.lines[pass[x]].name);
                x = path[x];
            }
            printf("%s distance:%d",nets.stations[sta1_id].name,distance[sta2_id]);
        }
        if(alg==DFS){

        }
        if(alg==BFS){

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

#include <graphics.h>

void test(){
    initgraph(800,600);
    getch();
    closegraph();
}

int main() {
//    system("chcp 65001");
//    Nets nets = Initialize_Nets();
//    Show_Lines(nets);
//    char sta1[512] = {0};
//    char sta2[512] = {0};
//    char scanfTip[512] = "请输入起始站点 终止站点:";
//    printf("%s",scanfTip);
//    fflush(stdout);
//    scanf("%s %s",sta2,sta1);
//    Go(nets,sta1,sta2,Most_fast,Dijkstra);
    test();
}
