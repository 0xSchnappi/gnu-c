/*
 * @Author: 0xSchnappi 952768182@qq.com
 * @Date: 2024-08-12 17:26:47
 * @LastEditors: 0xSchnappi 952768182@qq.com
 * @LastEditTime: 2024-08-13 09:27:22
 * @FilePath: /gnu-c/sync-mutil-thread.c
 * @Description: 多线程同步
 *
 * Copyright (c) 2024 by github.com/0xSchnappi, All Rights Reserved.
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int iCount = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void func(void);

void func(void) {
  for (size_t i = 0; i < 3; i++) {
    pthread_mutex_lock(&mutex);
    pthread_t id = pthread_self();
    printf("thread:%lx\tbefore add: iCount = %d.\n", id, iCount);
    iCount++;
    printf("thread:%lx\tafter add: iCount = %d.\n", id, iCount);
    pthread_mutex_unlock(&mutex);
  }
}

void main() {
  pthread_t tid1, tid2;

  pthread_create(&tid1, NULL, (void *)func, NULL);
  pthread_create(&tid2, NULL, (void *)func, NULL);

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);

  pthread_mutex_destroy(&mutex);
}
