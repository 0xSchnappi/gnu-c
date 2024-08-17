/*
 * @Author: 0xSchnappi 952768182@qq.com
 * @Date: 2024-08-16 09:38:27
 * @LastEditors: 0xSchnappi 952768182@qq.com
 * @LastEditTime: 2024-08-17 21:59:59
 * @FilePath: /gnu-c/list.h
 * @Description: 定义双向链表
 * 
 * Copyright (c) 2024 by github.com/0xSchnappi, All Rights Reserved. 
 */

#ifndef __LINUX_LIST_H__
#define __LINUX_LIST_H__

struct list_head
{
    struct list_head* prev, *next;
};

#define offset_of(type, member) (size_t)&(((type*)0)->member)

void list_add(struct list_head* add, struct list_head*head, bool write_once)
{
  struct list_head* temp = head->next;
  if (write_once)
    {
        /* 这部分代码只写一次 */
        head->prev = add;
    }
    head->next = add;
    add->next = temp;
    add->prev = head;
}

void del_list(struct list_head* del)
{
    struct list_head* prev,*next;
    prev = del->prev;
    next = del->next;
    next->prev = prev;
    prev->next = next;
    

    del->next = NULL;   // 内核使用的值是为了防止double free
    del->prev = NULL;
}

int list_empty(const struct list_head* head)
{
    return head->next == head;
}


#endif