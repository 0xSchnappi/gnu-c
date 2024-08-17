/*
 * @Author: 0xSchnappi 952768182@qq.com
 * @Date: 2024-08-16 09:38:43
 * @LastEditors: 0xSchnappi 952768182@qq.com
 * @LastEditTime: 2024-08-17 22:57:02
 * @FilePath: /gnu-c/list.c
 * @Description: 该代码是将linux内核以宏展开的方式编写，sample1是以宏展开的方式编写，方便理解，
 * sample2是定义结构体时list_head设置在结构体第一个成员，就可以直接使用类型转换list_head直接转换为NumberPlus。
 * 时间有限未实现list_add_tail函数，这个底层和list_add函数一样，只是一些小变动。
 * 
 *
 * Copyright (c) 2024 by github.com/0xSchnappi, All Rights Reserved.
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"

struct Number {
  unsigned long data;
  struct list_head list;
};

struct NumberPlus {
  struct list_head list;
  unsigned long data;
};

void sample1() {
  struct list_head root = {&root, &root};
  struct Number *num, *num1;

  for (size_t i = 0; i < 100; i++) {
    /* code */
    num = (struct Number *)malloc(sizeof(struct Number));
    num->data = i;
    if (i == 0) {
      list_add(&num->list, &root, true);
    } else {
      list_add(&num->list, &root, false);
    }
  }

  struct list_head *list = &root;
  size_t i = 0;
  for (; list->next != &root;
       num = (typeof(num))((size_t)list->next - (size_t) &
                           (((typeof(num))0)->list))) {
    printf("%2ld  ", num->data);
    if (i % 10 == 9)
      printf("\n");
    list = list->next;
    i++;
  }

  /* 删除链表 */
  list = &root;
  for (num = (typeof(num))((size_t)list->next - (size_t) &
                           (((typeof(num))0)->list));
       list->next != &root; num = (typeof(num))((size_t)list->next - (size_t) &
                                                (((typeof(num))0)->list))) {

    del_list(&num->list);
    free(num);

    // list = list->next;
  }

  if (list_empty(&root)) {
    printf("Free sample1 successfully\n");
  }
}

void sample2() {

  struct NumberPlus *sample = NULL;

  /* 链表初始化 */
  struct list_head root = {&root, &root};

  /* 链表添加 */
  for (size_t i = 0; i < 100; i++) {
    sample = (struct NumberPlus *)malloc(sizeof(struct NumberPlus));
    sample->data = i;
    if (i == 0) {
      list_add(&sample->list, &root, true);
    } else {
      list_add(&sample->list, &root, false);
    }
  }

  /* 链表遍历打印 */
  struct list_head *list = &root;
  size_t i = 0;
  sample = NULL;
  for (sample = (struct NumberPlus *)list->next; list->next != &root;
       list = list->next, sample = (struct NumberPlus *)list->next) {
    // sample = (struct NumberPlus *)list->next;
    printf("%2ld  ", sample->data);
    if (i % 10 == 9)
      printf("\n");
    i++;
  }

  /* 删除链表 */
  list = &root;
  for (sample = (struct NumberPlus *)list->next; list->next != &root;
       sample = (struct NumberPlus *)list->next) {
    del_list(&sample->list);
    free(sample);
  }

  if (list_empty(&root)) {
    printf("Free sample2 successfully\n");
  }
}
void main() {
  sample1();
  sample2();
}