#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t sem;

#define NUM_RESOURCE 3

void *worker(void *arg) {
  int id = *(int *)arg;

  sem_wait(&sem); // 请求资源
  printf("Thread %d acquired a resource\n", id);

  // 使用资源中
  sleep(1);
  printf("Thread %d released a resource\n", id);
  sem_post(&sem); // 释放资源
  return NULL;
}

int main() {
  pthread_t threads[10];
  int thread_ids[10];

  sem_init(&sem, 0, NUM_RESOURCE); // 初始化信号量

  for (size_t i = 0; i < 10; i++) {
    thread_ids[i] = i;
    pthread_create(&threads[i], NULL, worker, &thread_ids[i]);
  }

  for (size_t i = 0; i < 10; i++) {
    pthread_join(threads[i], NULL);
  }

  sem_destroy(&sem); // 销毁信号量

  return 0;
}