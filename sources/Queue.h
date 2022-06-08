//
// Created by С�� on 2022/6/8.
//

#ifndef DSCD_QUEUE_H
#define DSCD_QUEUE_H
#include <iostream>
#include "structures.h"

/*
 * function InitQueue -> ��ʼ������
 * @��̬����ռ�
 * return LinkQueue*
 * */

LinkQueue *InitQueue() {
    LinkQueue *linkQueue = (LinkQueue *) malloc(sizeof(LinkQueue));
    QNode *head = (QNode *) malloc(sizeof(QNode));
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

/*
 * function existQueue -> �д�
 * �������� �д�
 * return 0->false 1->true
 * */

int existQueue(LinkQueue *linkQueue, int station_id) {
    auto ptr = linkQueue->head;
    while (ptr->next) {
        if (station_id == ptr->station_id) { return 1; }
        ptr = ptr->next;
    }
    return 0;
}

/*
 * function InQueue -> ���
 * ��̬�����½ڵ� ��β���
 * return void
 * */

void InQueue(LinkQueue *linkQueue, int station_id, int front_id, int line_id, int distance) {
    if (!existQueue(linkQueue, station_id)) {
        auto ptr = linkQueue->head;
        QNode *node = (QNode *) malloc(sizeof(QNode));
        node->station_id = station_id;
        node->front_id = front_id;
        node->line_id = line_id;
        node->distance = distance;
        node->next = nullptr;
        linkQueue->rear++;
        while (ptr->next) {
            ptr = ptr->next;
        }
        ptr->next = node;
    } else {
        return;
    }
}

/*
 * function OutQueue -> ����
 * ��ͷ����
 * return void
 * */

void OutQueue(LinkQueue *linkQueue, int *station_id) {
    if (linkQueue->front < linkQueue->rear) {
        auto ptr = linkQueue->head;
        int i = 0;
        while (i < linkQueue->front) {
            ptr = ptr->next;
            i++;
        }
        *station_id = ptr->station_id;
        linkQueue->front++;
    } else {
        exit(1000);
    }
}

#endif //DSCD_QUEUE_H
