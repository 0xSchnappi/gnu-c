/*
 * @Author: 0xSchnappi 952768182@qq.com
 * @Date: 2024-08-18 22:59:16
 * @LastEditors: 0xSchnappi 952768182@qq.com
 * @LastEditTime: 2024-08-19 14:32:54
 * @FilePath: /gnu-c/socket-client.c
 * @Description: 客户端
 *
 * Copyright (c) 2024 by github.com/0xSchnappi, All Rights Reserved.
 */
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

int main() {
  int sock_fd;
  struct sockaddr_in server_addr;
  char buffer[BUFFER_SIZE];
  int n;

  // 创建 socket
  sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_fd == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  // 初始化服务器地址
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(SERVER_PORT);

  // 将字符串形式的 IP 地址转换为二进制形式，并填充到 sockaddr_in 结构中
  if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
    perror("inet_pton");
    close(sock_fd);
    exit(EXIT_FAILURE);
  }

  // 连接到服务器
  if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
      -1) {
    perror("connect");
    close(sock_fd);
    exit(EXIT_FAILURE);
  }

  printf("Connected to server\n");

  // 主循环，读取用户输入并发送到服务器
  while (1) {
    printf("Enter message: ");
    fgets(buffer, BUFFER_SIZE, stdin); // 从标准输入获取消息

    // 发送消息到服务器
    if (write(sock_fd, buffer, strlen(buffer)) == -1) {
      perror("write");
      close(sock_fd);
      exit(EXIT_FAILURE);
    }

    // 从服务器接收回显
    n = read(sock_fd, buffer, BUFFER_SIZE);
    if (n == -1) {
      perror("read");
      close(sock_fd);
      exit(EXIT_FAILURE);
    } else if (n == 0) {
      printf("Server closed connection\n");
      break;
    }

    // 打印服务器的回显消息
    buffer[n] = '\0'; // 确保字符串以 NULL 结尾
    printf("Echo from server: %s\n", buffer);
  }

  // 关闭 socket
  close(sock_fd);
  return 0;
}
