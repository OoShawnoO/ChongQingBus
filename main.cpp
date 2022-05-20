#include <cmath>
#include <iostream>
#include <cstring>
#include <graphics.h>


#define _USED 1
#define _UNUSED 0
#define _DROPED -1

const int Total_lines = 528;
const int Total_stations =3431;
const int  MAX_DISTANCE=9999999;




enum Mode{
    Most_save = 1,
    Most_fast,
    Least_change,
    Wanna_Pass,
    Synthesize,
};

enum Algorithm{
    Dijkstra = 1,
    DFS,
    BFS,
};

enum Exit{
    Not_Found = 1,
};

enum Action{
    Next = 1,
    Front,
    Rear,
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
    char name[64];
    float poi_x;
    float poi_y;
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
    char name[64];
    node* head;
}Line;

typedef struct {
    Station stations[Total_stations];
    Line lines[Total_lines];
}Nets;


typedef struct{
    int station_id;
    int line_id;
    int x_axis;
    int y_axis;
}Poi;

typedef struct QNode{
    int station_id;
    int front_id;
    int line_id;
    int distance;
    QNode* next;
}QNode;

typedef struct{
    QNode* head;
    int rear;
    int front;
}LinkQueue;

LinkQueue* InitQueue(){
    LinkQueue* linkQueue =(LinkQueue*)malloc(sizeof(LinkQueue));
    QNode *head = (QNode*)malloc(sizeof(QNode));
    head->next = nullptr;
    head->station_id = -1;
    head->front_id = -1;
    head->line_id = -1;
    head->distance = -1;
    linkQueue->head = head;
    linkQueue->rear = 1;
    linkQueue->front = 1;
    return linkQueue;
}

int existQueue(LinkQueue* linkQueue,int station_id){
    auto ptr = linkQueue->head;
    while(ptr->next){
        if(station_id == ptr->station_id){return 1;}
        ptr = ptr->next;
    }
    return 0;
}

void InQueue(LinkQueue* linkQueue,int station_id,int front_id,int line_id,int distance){
    if(!existQueue(linkQueue,station_id)){
        auto ptr = linkQueue->head;
        QNode* node = (QNode*)malloc(sizeof(QNode));
        node->station_id = station_id;
        node->front_id = front_id;
        node->line_id = line_id;
        node->distance = distance;
        node->next = nullptr;
        linkQueue->rear++;
        while(ptr->next){
            ptr = ptr->next;
        }
        ptr->next = node;
    }
    else{
        return;
    }
}

void OutQueue(LinkQueue* linkQueue,int* station_id){
    if(linkQueue->front<linkQueue->rear){
        auto ptr = linkQueue->head;
        int i = 0;
        while(i<linkQueue->front){
            ptr = ptr->next;
            i++;
        }
        *station_id = ptr->station_id;
        linkQueue->front++;
    }
    else{
        exit(1000);
    }
}

Station* Create_Station(char* name,int station_id,station_edge* first){
    auto *station = (Station*)malloc(sizeof(Station));
    strcpy_s(station->name,name);
    station->station_id = station_id;
    station->first = first;
    return station;
}

static Station stations[Total_stations];
static Line lines[Total_lines];

Line* Create_Line(char* name,int line_id,Node* head){
    auto *line = (Line*)malloc(sizeof(Line));
    strcpy_s(line->name,name);
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

//    FILE *fp_station = fopen("D://C_learn/DSCD/stations.txt","r");
//    FILE *fp_line = fopen("D://C_learn/DSCD/lines.txt","r");
//    FILE *fp_distance = fopen("D://C_learn/DSCD/distance.txt","r");
//    FILE *fp_poi = fopen("D://C_learn/DSCD/station_poi.txt","r");
    FILE *fp_station,*fp_line,*fp_distance,*fp_poi;
    fopen_s(&fp_station,"D://C_learn/DSCD/stations.txt","r");
    fopen_s(&fp_line,"D://C_learn/DSCD/lines.txt","r");
    fopen_s(&fp_distance,"D://C_learn/DSCD/distance.txt","r");
    fopen_s(&fp_poi,"D://C_learn/DSCD/station_poi.txt","r");


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


    for(int i=0;i<Total_stations;i++){
        char name[128] = {'\0'};
        fscanf(fp_poi,"%s",name);
        for(auto & station: stations){
            if(strcmp(name,station.name)==0){
                float x,y;
                char cx[30]={'\0'};char cy[30]={'\0'};
                fscanf(fp_poi,"%s",cx);
                fscanf(fp_poi,"%s",cy);
                x = strtof(cx,NULL);
                y = strtof(cy,NULL);
                station.poi_x = x;
                station.poi_y = y;
                break;
            }
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

//    for(int j=0;j<Total_stations;j++){
//        printf("%d %s ",j,nets.stations[j].name);
////        printf("%s",nets.stations[nets.stations[j].first->next->next_adj_station_id].name);
//        Station_Edge* p = nets.stations[j].first->next;
//        while(p!= nullptr){
//            printf("(%s %s)",nets.stations[p->pre_adj_station_id].name,nets.lines[p->line_id].name);
//            p = p->next;
//        }
//        printf("\n");
//    }

    for(auto & station:stations){
        printf("%f ,%f \n",station.poi_x,station.poi_y);
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

int Compare(int* sta1_line,int* sta2_line){
    for(int i=0;i<Total_lines;i++){
        if(sta1_line[i]!=-1&&sta2_line[i]!=-1){
            return i;
        }
    }
    return -1;
}

void DoFind(int* sta_line,int sta_id,int* set){
    station_edge* head = stations[sta_id].first;
    if(set[sta_id]!=_USED){
        set[sta_id] = _USED;
        while(head->next){
            if(sta_line[head->line_id]==-1){
                sta_line[head->line_id] = head->current_station_id;
            }
            head = head->next;
        }
    }
    else{
        return;
    }
}

void FindWay(LinkQueue* linkQueue,int station_id,int* pass,int* path,int* distance){
    auto ptr = linkQueue->head->next;
    while(ptr->station_id!=station_id){
        ptr = ptr->next;
    }
    if(ptr->front_id!=-1){
        pass[station_id] = ptr->front_id;
        path[station_id] = ptr->line_id;
        distance[station_id] = ptr->distance;
        FindWay(linkQueue,ptr->front_id,pass,path,distance);
    }
    else{
        return;
    }
}

void test(Poi *pois){
    initgraph(600,600);
    setbkcolor(WHITE);
    setfillcolor(BLACK);
    setlinewidth(2);
    ege_enable_aa(true);
    int i=0;
    while(!(pois[i].x_axis==0&&pois[i].y_axis==0&&pois[i].station_id==0&&pois[i].line_id==0)){

//        printf("%d %d %s\n",pois[i].x_axis,pois[i].y_axis,stations[pois[i].station_id].name);
//        putpixel(pois[i].x_axis,pois[i].y_axis,BLACK);
        bar(pois[i].x_axis/2+100,pois[i].y_axis/2,pois[i].x_axis/2+10+100,pois[i].y_axis/2+10);

//        getch();
        outtextxy(pois[i].x_axis/2-20+100,pois[i].y_axis/2+20,stations[pois[i].station_id].name);

        if(i==0){
            moveto(pois[i].x_axis/2+100,pois[i].y_axis/2+5);
        }
        else{
            moveto(pois[i-1].x_axis/2+5+100,pois[i-1].y_axis/2+5);
        }
        int x = getx();
        int y = gety();
        ege::ege_line(float(x), float(y), float(pois[i].x_axis / 2 + 5+100), float(pois[i].y_axis / 2 + 5));
        if(i>0){
            ege::outtextxy(x-((x-(pois[i].x_axis/2+5+100))/2),y-((y-(pois[i].y_axis)/2+5)/2),lines[pois[i].line_id].name);
        }

        i++;
    }

    //    bar(50,100,60,110); /*x坐标50 - 60 , y 坐标 100 - 110*/
    getch();
    closegraph();
}

void StandardPoi(const int *path,const int *pass,int start,int end,Poi *pois){
    int seq[2][200];
    for(int i=0;i<200;i++){
        seq[0][i] = -1;
        seq[1][i] = -1;
    }
    int sequence = 0;
    int x = end;
    while(path[x] != start){
        seq[0][sequence] = x;
        seq[1][sequence++]=pass[x];
        x = path[x];
    }

    /* 使用MinMax标准处理poi数据 */
    int j=0;
    float X[50] = {0};
    float Y[50] = {0};
    int S_X[50] = {0};
    int S_Y[50] = {0};
    float max_x = -99999.0;
    float min_x = 999999.0;
    float max_y = -99999.0;
    float min_y = 999999.0;
    while(seq[0][j]!=-1){
        X[j] = stations[seq[0][j]].poi_x;
        Y[j] = stations[seq[0][j]].poi_y;
        if(X[j]>max_x){max_x = X[j];}
        if(Y[j]>max_y){max_y = Y[j];}
        if(X[j]<min_x){min_x = X[j];}
        if(Y[j]<min_y){min_y = Y[j];}
        j++;
    }
    j = 0;
    while(X[j+1]!=0 && Y[j+1]!=0){
        Poi*  poixy =(Poi *) malloc(sizeof(Poi));
        X[j] = (X[j] - min_x)/(max_x - min_x);
        Y[j] = (Y[j] - min_y)/(max_y - min_y);
        poixy->x_axis =  floor((double)X[j]*1000);
        poixy->y_axis = floor((double)Y[j]*1000);
        poixy->station_id = seq[0][j];
        poixy->line_id = seq[1][j];
        pois[j] = *poixy;
        j++;
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
        if(alg==Dijkstra&&mode==Most_fast){
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
            Poi pois[50];
            for(int i=0;i<50;i++){
                Poi *poix = (Poi*)malloc(sizeof(Poi));
                poix->x_axis=0;
                poix->y_axis=0;
                poix->line_id=0;
                poix->station_id=0;
                pois[i] = *poix;
            }
            StandardPoi(path,pass,sta1_id,sta2_id,pois);
            int x1 = 0;
//            while(pois[x1].x_axis!=0){
//                printf("%d ,%d ,%d ,%d \n",pois[x1].x_axis,pois[x1].y_axis,pois[x1].station_id,pois[x1].line_id);
//                x1++;
//            }

            while(x!=sta1_id){
                printf("%s->乘坐%s->",nets.stations[x].name,nets.lines[pass[x]].name);
                x = path[x];
            }
            printf("%s distance:%d",nets.stations[sta1_id].name,distance[sta2_id]);
            fflush(stdout);
            test(pois);
        }
        if(alg==DFS&&mode==Most_fast){

        }
        if(alg==BFS&&mode==Least_change){
            int sta1_line[Total_lines];
            int sta2_line[Total_lines];
            int set[Total_stations];
            int path[Total_stations];
            int pass[Total_stations];
            int distance[Total_stations];
            for(int & i :  set){i = _UNUSED;}
            for(int & i : sta1_line){i=-1;}
            for(int & i : sta2_line){i=-1;}
            for(int & i : pass){i=-1;}
            for(int & i : path){i=-1;}
            for(int & i : distance){i=-1;}
            int sta1=sta1_id;
            station_edge* head = stations[sta2_id].first;
            while(head->next){
                if(head->line_id!=-1){
                    sta2_line[head->line_id] = head->line_id;
                }
                head = head->next;
            }
            station_edge* head1 = stations[sta1_id].first;
            LinkQueue* linkQueue = InitQueue();
            InQueue(linkQueue,sta1,-1,-1,-1);

            while(Compare(sta1_line,sta2_line)==-1){
                DoFind(sta1_line,sta1,set);
                auto ptr = stations[sta1].first->next;
                while(ptr->next){
                    if(ptr->pre_adj_station_id!=-1){
                        InQueue(linkQueue,ptr->pre_adj_station_id,sta1,ptr->line_id,ptr->pre_distance);
                    }
                    if(ptr->next_adj_station_id!=-1){
                        InQueue(linkQueue,ptr->next_adj_station_id,sta1,ptr->line_id,ptr->next_distance);
                    }
                    ptr = ptr->next;
                }
                OutQueue(linkQueue,&sta1);
            }
            int x = Compare(sta1_line,sta2_line);
            printf("Find! at %s\n",lines[x].name);
            station_edge* ptr = stations[sta2_id].first;
            while(ptr->next){
                if(ptr->line_id==x){
                    break;
                }
                ptr = ptr->next;
            }
            printf("%s,%s\n",stations[ptr->next_adj_station_id].name,stations[ptr->pre_adj_station_id].name);
            int staid = sta1_line[x];
            FindWay(linkQueue,staid,pass,path,distance);
            node* p = lines[x].head;
            node* q = p;
            int z=0,y=0;
            while(p->station_id!=staid){
                p = p->next;
                z++;
            }
            while(q->station_id!=sta2_id){
                q = q->next;
                y++;
            }
            if(z>y){
                while(q!=p){
                    pass[q->station_id] =q->next->station_id ;
                    path[q->station_id] = x;
                    q = q->next;
                }
            }
            else{
                while(q!=p){
                    pass[p->next->station_id] = p->station_id;
                    path[p->station_id] = x;
                    p = p->next;
                }
            }


//            printf("%s(%s)->",stations[staid].name,lines[path[staid]].name);
            staid = sta2_id;
            path[sta2_id] = x;
            while(pass[staid]!=-1){
                printf("%s(%s)->",stations[staid].name,lines[path[staid]].name);
                staid = pass[staid];
            }
            printf("%s",stations[staid].name);
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
//    Show_Lines(nets);
    char sta1[128] = {0};
    char sta2[128] = {0};
    char scanfTip[128] = "请输入起始站点 终止站点:";
    printf("%s",scanfTip);
    fflush(stdout);
    scanf("%s %s",sta2,sta1);       /* 测试用例 ：重庆北站南广场站 大石坝二村站*/ /*重庆北站南广场站 小什字站*/
//    Go(nets,sta1,sta2,Most_fast,Dijkstra);
    Go(nets,sta1,sta2,Least_change,BFS);
//    test();



}

