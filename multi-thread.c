#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int mCount = 6;
void *createThread(void *arg);

void *createThread(void *arg) {
  int count = (int)(*((int *)arg));
  pthread_t id = pthread_self();

  for (size_t i = 0; i < count; i++) {
    printf("thread id: %lx\tcreateThread i = %ld.\n", id, i);
    if (i == 3) {
      /* 用于终止调用它的线程，并返回一个void* 指针 */
      pthread_exit("Terminal thread.\n");
    }
  }
}

void example1() {
  pthread_t tid1, tid2;
  pthread_attr_t pthread_attr1, pthread_attr2;
  void *retval1 = NULL;
  void *retval2 = NULL;

  pthread_attr_init(&pthread_attr1);
  pthread_attr_init(&pthread_attr2);

  /**
   * 分离状态表示线程以什么方式结束自己。
   * 线程属于分离状态时，不需要被其他线程等待，调用pthread_join也不会阻塞当前线程，
   * 主线程和子线程相当于两个程序，各自执行，自己运行结束以后，线程就终止了，释放掉自己的资源。
   * 非分离状态的线程，原有主线程会等待创建线程的执行，等待pthread_join函数返回后，才终止线程，释放系统资源。
   *
   * 设置线程的分离状态，PTHREAD_CREATE_JOINABLE表示非分离状态，PTHREAD_CREATE_DETACHED表示分离状态
   */
  pthread_attr_setdetachstate(&pthread_attr1, PTHREAD_CREATE_JOINABLE);
  pthread_attr_setdetachstate(&pthread_attr2, PTHREAD_CREATE_DETACHED);

  pthread_create(&tid1, &pthread_attr1, createThread, &mCount);
  pthread_create(&tid2, &pthread_attr2, createThread, &mCount);

  // 阻塞当前线程，等待线程的结束
  pthread_join(tid1, &retval1);
  pthread_join(tid2, &retval2);

  printf("Thread 1 return : %s\n", (char *)retval1);
  printf("Thread 2 return : %s\n", (char *)retval2);

  /* 删除线程属性 */
  pthread_attr_destroy(&pthread_attr1);
  pthread_attr_destroy(&pthread_attr2);
}

void main() { example1(); }