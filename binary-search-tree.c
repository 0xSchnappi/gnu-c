/*
 * @Author: 0xSchnappi 952768182@qq.com
 * @Date: 2024-08-19 16:28:28
 * @LastEditors: 0xSchnappi 952768182@qq.com
 * @LastEditTime: 2024-08-20 11:12:03
 * @FilePath: /gnu-c/binary-search-tree.c
 * @Description: 搜索二叉树BST
 *
 * Copyright (c) 2024 by github.com/0xSchnappi, All Rights Reserved.
 */
#include <stdio.h>
#include <stdlib.h>

// 定义数节点
struct Node {
  int key;
  struct Node *left;
  struct Node *right;
};

// 创建一个新节点
struct Node *create_node(int key) {
  struct Node *node = (struct Node *)malloc(sizeof(struct Node));
  node->key = key;
  node->left = node->right = NULL;
  return node;
}

// 插入一个新节点
struct Node *insert(struct Node *node, int key) {
  // 如果树为空，返回新节点
  if (node == NULL) {
    return create_node(key);
  }

  // 否则递归插入到子数中
  if (key < node->key) {
    node->left = insert(node->left, key);
  } else if (key > node->key) {
    node->right = insert(node->right, key);
  }

  return node;
}

// 查找最小值节点（用于删除节点时）
struct Node *minValueNode(struct Node *node) {
  struct Node *current = node;

  // 找到最左的叶子节点
  while (current && current->left != NULL) {
    current = current->left;
  }

  return current;
}

// 删除一个节点
struct Node *deleteNode(struct Node *root, int key) {
  // 基本情况：如果树为空
  if (root == NULL) {
    return root;
  }

  // 递归寻找要删除的节点
  if (key < root->key) {
    root->left = deleteNode(root->left, key);
  } else if (key > root->key) {
    root->right = deleteNode(root->right, key);
  } else {
    // 找到要删除的节点
    // 只有一个子节点或无子节点的情况
    if (root->left == NULL) {
      struct Node *temp = root->right;
      free(root);
      return temp;
    } else if (root->right == NULL) {
      struct Node *temp = root->left;
      free(root);
      return temp;
    }

    // 有两个子节点的情况
    /**
     * 假如二叉搜索树如下所示：
     *        50
     *       /  \
     *     30    70
     *    / \   /  \
     *   20 40 60  80
     * 删除50(根节点，有两个子节点):
     * - 首先找到后继节点：70的最左子节点，即60
     * - 将60的值复制到50节点，因此50现在变成了60
     * -
     * 然后递归删除60节点（后继节点），由于60节点没有左子节点，它将被直接删除。
     */
    struct Node *temp = minValueNode(root->right);

    // 用后继节点替换当前节点
    root->key = temp->key;

    // 删除后继节点
    root->right = deleteNode(root->right, temp->key);
  }

  return root;
}

// 中序遍历（从小到大遍历）
void inorder(struct Node *root) {
  if (root != NULL) {
    inorder(root->left);
    printf("%d ", root->key);
    inorder(root->right);
  }
}

int main() {
  struct Node *root = NULL;

  // 插入节点
  root = insert(root, 50);
  root = insert(root, 30);
  root = insert(root, 20);
  root = insert(root, 40);
  root = insert(root, 70);
  root = insert(root, 60);
  root = insert(root, 80);

  printf("Inorder traversal of the given tree: \n");
  inorder(root);

  printf("\n\nDelete 20\n");
  root = deleteNode(root, 20);
  printf("Inorder traversal after deleting 20: \n");
  inorder(root);

  printf("\n\nDelete 30\n");
  root = deleteNode(root, 30);
  printf("Inorder traversal after deleting 30: \n");
  inorder(root);

  printf("\n\nDelete 50\n");
  root = deleteNode(root, 50);
  printf("Inorder traversal after deleting 50: \n");
  inorder(root);

  return 0;
}
