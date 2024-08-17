#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 5

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int task_queue = 0; // 任务队列中的任务数量

void *worker(void *arg) {
  int id = *(int *)arg;

  while (1) {
    pthread_mutex_lock(&mutex);

    while (task_queue == 0) {
      // 没有任务，等待条件变量
      pthread_cond_wait(&cond, &mutex);
    }

    // 处理任务
    task_queue--; // 从队列中取出任务
    printf("Thread %d is processing a task, Remaining tasks: %d\n", id, task_queue);

    pthread_mutex_unlock(&mutex);

    // 模拟任务处理器
    sleep(1);
  }
  return NULL;
}

void add_task(){
    pthread_mutex_lock(&mutex);
    task_queue++;   // 添加一个任务
    pthread_cond_broadcast(&cond);      // 广播通知所有等待线程
    pthread_mutex_unlock(&mutex);
}

int main(){
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, worker, &thread_ids[i]);
    }

    // 主线程添加任务
    for (size_t i = 0; i < 10; i++)
    {
        add_task();
        sleep(1);   // 模拟任务间隔
    }

    // 等待所有线程完成（在实际应用中，可能需要一种停止机制）
    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
    
    
    
}