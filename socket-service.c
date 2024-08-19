/*
 * @Author: 0xSchnappi 952768182@qq.com
 * @Date: 2024-08-18 22:59:02
 * @LastEditors: 0xSchnappi 952768182@qq.com
 * @LastEditTime: 2024-08-19 14:41:54
 * @FilePath: /gnu-c/socket-service.c
 * @Description: 服务器端
 *
 * Copyright (c) 2024 by github.com/0xSchnappi, All Rights Reserved.
 */

/**
 * TCP 服务端流程
 * 1.创建服务端socket、bind绑定端口、listen监听端口
 * 2.将服务端socket注册到epoll
 * 3.epoll_wait等待连接到来，连接到来时，调用accept获取已连接的socket
 * 4.将已连接的socket注册到epoll
 * 5.epoll_wait等待事件发生
 * 6.对方连接关闭时，我方调用close
 *
 */

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_EVENTS 10
#define BUFFER_SIZE 1024
#define PORT 8080

int main() {
  int listen_fd, conn_fd, epoll_fd;
  struct sockaddr_in server_addr;
  struct epoll_event event, events[MAX_EVENTS];

  /* 1.创建服务端socket */
  listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_fd == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  /* 绑定IP和端口 */
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(PORT);
  /**
   * htonl、htons、ntohl、ntohs 是在主机和网络字节序之间转换
   * htonl、htons 分别是将16位、32位主机字节序转网络字节序
   * ntohl、ntohs 分别是将16位、32位网络字节序转主机字节序
   */

  if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
      -1) {
    perror("bind");
    close(listen_fd);
    exit(EXIT_FAILURE);
  }

  /* 2.监听端口 */
  if (listen(listen_fd, 10) ==-1)
  {
    perror("listen");
    close(listen_fd);
    exit(EXIT_FAILURE);
  }
  

  /* 3.创建epoll实例，并将服务端socket注册到epoll */
  epoll_fd = epoll_create1(0); // 创建epoll实例，参数设置文件描述符的属性
  if (epoll_fd == -1) {
    perror("epoll_create1");
    close(listen_fd);
    exit(EXIT_FAILURE);
  }

  event.events = EPOLLIN; // 关联文件的读取操作
  event.data.fd = listen_fd;
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &event) == -1) {
    perror("epoll_ctl");
    close(listen_fd);
    close(epoll_fd);
    exit(EXIT_FAILURE);
  }

  /* 4.主循环等待事件 */
  while (1) {
    int n_fds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    if (n_fds == -1) {
      perror("epoll_wait");
      close(listen_fd);
      close(epoll_fd);
      exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < n_fds; i++) {
      if (events[i].data.fd == listen_fd) {
        /* 5.处理新连接 */
        struct sockaddr_in caddr = {0};
        int csize = 0;
        conn_fd = accept(listen_fd, (struct sockaddr*)&caddr, &csize);
        if (conn_fd == -1) {
          perror("accept");
          continue;
        }

        /* 将新的连接注册到epoll */
        event.events = EPOLLIN;
        event.data.fd = conn_fd;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn_fd, &event) == -1) {
          perror("epoll_ctl");
          close(conn_fd);
        }

      } else {
        /* 6.处理已连接 socket 上的读写事件 */
        char buffer[BUFFER_SIZE];
        int n = read(events[i].data.fd, buffer, BUFFER_SIZE);
        if (n <= 0) {
          if (n == 0) {
            /* 对方关闭连接 */
            printf("Client disconnected\n");
          } else {
            perror("read");
          }

          close(events[i].data.fd); // 关闭连接
          epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd,
                    NULL); // 从epoll中移除
        } else {
          /* 简单回显 */
          write(events[i].data.fd, buffer, n);
        }
      }
    }
  }

  close(listen_fd);
  close(epoll_fd);
  return 0;
}