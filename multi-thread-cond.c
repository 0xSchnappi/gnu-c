/*
 * @Author: 0xSchnappi 952768182@qq.com
 * @Date: 2024-08-13 09:52:38
 * @LastEditors: 0xSchnappi 952768182@qq.com
 * @LastEditTime: 2024-08-13 10:32:32
 * @FilePath: /gnu-c/multi-thread-cond.c
 * @Description:
 *
 * Copyright (c) 2024 by github.com/0xSchnappi, All Rights Reserved.
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct Product {
  int num;
  struct Product *next;
} pProduct;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

pProduct *pHead = NULL;
pProduct *pproduct = NULL;
pProduct *pTail = NULL;

void producer() {
  srand(time(NULL));

  for (size_t i = 0; i < 4; i++) {
    pthread_mutex_lock(&mutex);
    pproduct = (pProduct *)malloc(sizeof(pProduct));
    pproduct->num = rand() % 100;
    pproduct->next = NULL;

    if (pHead == NULL) {
      pHead = pproduct;
      pTail = pHead;
    } else {
      pTail->next = pproduct;
      pTail = pproduct;
    }

    printf("[%d] product is ready.\n", pTail->num);

    pthread_cond_signal(&cond);

    pthread_mutex_unlock(&mutex);

    sleep(3);
  }
}

void customer() {
  for (size_t i = 0; i < 4; i++) {
    pthread_mutex_lock(&mutex);

    while (pHead == NULL) {
      printf("head is NULL.\n");
      pthread_cond_wait(&cond, &mutex);
    }

    printf("[%d] product is saled.\n", pHead->num);

    pHead = pHead->next;

    pthread_mutex_unlock(&mutex);
    sleep(1);
  }
}

int main() {
  pthread_t tid_producer, tid_customer;

  // 设置信号量属性
  // 默认值是PTHREAD_ PROCESS_PRIVATE，即此条件变量被同一进程内的各个线程使用；
  // 如果选择为PTHREAD_PROCESS_SHARED 则为多个进程间各线程公用
  pthread_cond_init(&cond, PTHREAD_PROCESS_PRIVATE);

  pthread_create(&tid_producer, NULL, (void *)&producer, NULL);
  pthread_create(&tid_customer, NULL, (void *)&customer, NULL);

  pthread_join(tid_customer, NULL);
  pthread_join(tid_producer, NULL);

  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);
}