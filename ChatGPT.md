# Linux C 编程学习大纲

## 1. Linux 系统编程基础

- **C 编程基础复习**
  - 基本语法、数据类型、指针、结构体、函数指针（常用）
- **GNU 编译器和 Makefile**
  - 使用 `gcc` 编译 C 程序
  - 编写 `Makefile` 自动化编译流程（常用）
- **标准库函数**
  - `stdlib.h`, `stdio.h`, `string.h`, `time.h`（常用）
- **内存管理**
  - 动态内存分配：`malloc`, `calloc`, `realloc`, `free`（常用）
  - 内存泄漏检测工具：`valgrind`（常用）

## 2. 套接字编程（Socket Programming）

- **网络基础**
  - TCP/IP 协议栈（常用）
  - IP 地址、端口号、MAC 地址的概念（常用）
- **套接字 API**
  - 套接字创建：`socket()`（常用）
  - 地址绑定：`bind()`（常用）
  - 监听连接：`listen()`（常用）
  - 接受连接：`accept()`（常用）
  - 数据发送与接收：`send()`, `recv()`, `sendto()`, `recvfrom()`（常用）
  - 连接到服务器：`connect()`（常用）
  - 关闭连接：`close()`（常用）
- **TCP 和 UDP 套接字编程**
  - TCP 客户端/服务器模型（常用）
  - UDP 客户端/服务器模型（常用）
- **高级套接字编程**
  - 非阻塞 I/O：`fcntl()`, `ioctl()`（常用）
  - 套接字选项：`setsockopt()`, `getsockopt()`（常用）
  - 广播和组播（不常用）
  - 多播（Multicasting）（不常用）
  - 原始套接字（Raw Socket）（不常用）

## 3. 进程间通信（IPC）

- **基本概念**
  - 进程、线程的定义与区别（常用）
- **传统 IPC 机制**
  - 管道（Pipe）：`pipe()`, `popen()`, `pclose()`（常用）
  - 命名管道（FIFO）：`mkfifo()`（常用）
  - 信号（Signals）：`signal()`, `sigaction()`（常用）
  - 消息队列（Message Queue）：`msgget()`, `msgsnd()`, `msgrcv()`（常用）
  - 共享内存（Shared Memory）：`shmget()`, `shmat()`, `shmdt()`, `shmctl()`（常用）
  - 信号量（Semaphore）：`semget()`, `semop()`, `semctl()`（常用）
- **高级 IPC 机制**
  - POSIX 消息队列：`mq_open()`, `mq_send()`, `mq_receive()`（不常用）
  - 远程过程调用（RPC）（不常用）
  - 基于文件的锁：`flock()`，`fcntl()` 文件锁（常用）

## 4. 事件驱动编程与 epoll

- **I/O 多路复用基础**
  - `select()`, `poll()`（常用）
- **epoll**
  - 基本概念：边沿触发（ET）和水平触发（LT）（常用）
  - 创建 epoll 实例：`epoll_create()`, `epoll_create1()`（常用）
  - 添加/修改/删除事件：`epoll_ctl()`（常用）
  - 等待事件：`epoll_wait()`（常用）
- **高级 epoll 编程**
  - epoll 的高效性和局限性（常用）
  - epoll 与多线程结合的使用（常用）

## 5. 远程过程调用（RPC）

- **基本概念**
  - RPC 的定义与用途（不常用）
  - RPC 通信原理（不常用）
- **RPC 实现**
  - Sun RPC（不常用）
  - gRPC（Google RPC）基础与应用（常用）
- **RPC 编程**
  - 编写简单的 RPC 服务和客户端（不常用）
  - 使用 IDL（接口定义语言）定义服务（不常用）
  - RPC 安全性（不常用）

## 6. 多线程编程

- **线程基础**
  - 线程创建：`pthread_create()`（常用）
  - 线程同步与互斥：`pthread_mutex_t`, `pthread_cond_t`（常用）
  - 线程终止：`pthread_exit()`，`pthread_join()`（常用）
- **线程间通信**
  - 线程间数据共享与锁（常用）
  - 条件变量：`pthread_cond_wait()`, `pthread_cond_signal()`（常用）
- **高级线程编程**
  - 线程池的实现（常用）
  - 线程局部存储（Thread Local Storage）：`pthread_key_t`（不常用）
  - 多线程与信号处理（常用）

## 7. 系统调试与优化

- **调试工具**
  - 使用 `gdb` 调试程序（常用）
  - 使用 `strace` 追踪系统调用（常用）
  - 使用 `ltrace` 追踪库函数调用（不常用）
- **性能优化**
  - 使用 `perf` 工具进行性能分析（常用）
  - 优化多线程程序的性能与资源使用（常用）
- **内存调试**
  - 内存泄漏检测：`valgrind`（常用）
  - 使用 `gprof` 进行程序分析（不常用）


