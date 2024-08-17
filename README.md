# GNU-C

GNU C 语言练习

## 题目

- 多线程

- 多线程同步

- 多进程

- epoll

- 套接字编程

- RPC

- IPC


## 锁

### 互斥锁

互斥锁(`pthread_mutex_t`)是最常用的锁类型，允许只有一个线程在任意给定时间持有锁，从而实现对共享资源的独占访问。

```shell

#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_lock(&mutex);
// 临界区代码
pthread_mutex_unlock(&mutex);


```

### 自旋锁

自旋锁(`pthread_spinlock_t`)在等待时不会阻塞线程，而是不断检查锁是否可用。它适合用于锁的持有时间非常短的场合，以避免上下文切换的开销。

```shell

#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_lock(&mutex);
// 临界区代码
pthread_mutex_unlock(&mutex);


```

### 读写锁

读写锁(`pthread_rwlock_t`)允许多个线程同时读取共享资源，但在有线程写入资源时，其他线程不能读取或写入。读写锁适用于读操作远多于写操作的场景。

```shell

#include <pthread.h>

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

// 读锁
pthread_rwlock_rdlock(&rwlock);
// 读临界区代码
pthread_rwlock_unlock(&rwlock);

// 写锁
pthread_rwlock_wrlock(&rwlock);
// 写临界区代码
pthread_rwlock_unlock(&rwlock);


```

### 条件变量

条件变量(`pthread_cond_t`)通常与互斥锁一起使用，以允许线程等待某个条件发生。条件变量能够使线程在等待某个条件的同时释放持有的互斥锁。
**注意：**当信号量在wait前发送，就会造成信号丢失现象。
**本质：**条件变量就是线程等待cond条件满足时，去执行

```shell

#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_lock(&mutex);
// 等待条件
pthread_cond_wait(&cond, &mutex);
// 当条件满足时继续执行
pthread_mutex_unlock(&mutex);

// 发出信号，唤醒一个线程
pthread_cond_signal(&cond);

/* 发出信号，唤醒所有线程 */
pthread_cond_broadcast (&cond);

/**/

```

### 信号量

**本质：**：为了控制资源而设计，可以理解是一种特殊的锁。比如有三把铁锹，锁到一个保险柜里，你可是设置信号量的数量为3，那么就会有三把钥匙，任何想用铁锹的只需要发出申请，系统保管的钥匙的数量只要大于零，就会给你一把，你就能打开保险柜，获取一把铁锹，当呢归还铁锹后，钥匙又会交到系统保管。

```shell

/* 用value初始化信号量的值(就是最大访问资源的数量)，如果pshared为0，表示只能在当前进程中各个线程进行共享，如果pshared为1，则表示可以在进程中共享。 */
int sem_init (sem_t * sem, int  pshared, unsigned int  value)

/* 信号量销毁 */
int sem_destroy (sem_t * sem) 

/* 信号量P操作 */
// 信号量的值减一，当信号量的值为零时，当前线程阻塞
int sem_wait(sem_t * sem)

// 如果当前信号量为0，不会阻塞当前线程，而会返回EAGAIN。
int sem_trywait (sem_t * sem)

/* 信号量V操作 */
// 信号量的值加一
int sem_post (sem_t * sem)

/* 获取信号量的值 */
int sem_getvalue (sem_t * sem, int * sval)


```

### 屏障

屏障(`pthread_barrier_t`)使一组线程可以在某个同步点上等待，直到所有线程都到达该点，然后所有线程才能继续执行。这对于需要多个线程同步起步或结束的场景非常有用。

```shell

#include <pthread.h>

pthread_barrier_t barrier;

pthread_barrier_init(&barrier, NULL, NUM_THREADS);

// 在屏障点等待
pthread_barrier_wait(&barrier);

pthread_barrier_destroy(&barrier);

```