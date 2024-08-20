#include <stdio.h>
#include <stdlib.h>

// 定义树节点
struct Node {
  int key;
  struct Node *left;
  struct Node *right;
  int height;
};

// 创建新节点
struct Node *create_node(int key) {
  struct Node *node = (struct Node *)malloc(sizeof(struct Node));
  node->key = key;
  node->left = node->right = NULL;
  node->height = 1; // 新节点的高度为1
  return node;
}

// 获取节点的高度
int height(struct Node *node) {
  if (node == NULL)
    return 0;
  return node->height;
}

// 获取节点的平衡因子
int getBalance(struct Node *node) {
  if (node == NULL)
    return 0;
  return height(node->left) - height(node->right);
}

// 右旋转操作
struct Node *rightRotate(struct Node *y) {
  struct Node *x = y->left; // 支点
  struct Node *T2 = x->right;

  // 进行旋转
  x->right = y;
  y->left = T2;

  // 更新高度
  y->height = 1 + (height(y->left) > height(y->right) ? height(y->left)
                                                      : height(y->right));
  x->height = 1 + (height(x->left) > height(x->right) ? height(x->left)
                                                      : height(x->right));

  // 返回新的根节点
  return x;
}

// 左旋转操作
struct Node *leftRotate(struct Node *x) {
  struct Node *y = x->right;
  struct Node *T2 = y->left;

  // 进行旋转
  y->left = x;
  x->right = T2;

  // 更新高度
  x->height = 1 + (height(x->left) > height(x->right) ? height(x->left)
                                                      : height(x->right));
  y->height = 1 + (height(y->left) > height(y->right) ? height(y->left)
                                                      : height(y->right));

  // 返回新的根节点
  return y;
}

// 插入节点并保持平衡
struct Node *insert(struct Node *node, int key) {
  // 1. 普通二叉搜索树的插入
  if (node == NULL)
    return create_node(key);

  if (key < node->key)
    node->left = insert(node->left, key);
  else if (key > node->key)
    node->right = insert(node->right, key);
  else // 相同的键不允许插入
    return node;

  // 2. 更新节点的高度
  node->height =
      1 + (height(node->left) > height(node->right) ? height(node->left)
                                                    : height(node->right));

  // 3. 计算平衡因子并检查是否需要平衡
  int balance = getBalance(node);

  // 如果不平衡，进行相应的旋转

  // 左(左子树)左(左节点)情况: 右旋(以root->left为支点，顺时针旋转)
  if (balance > 1 && key < node->left->key)
    return rightRotate(node);

  // 右(右子树)右(右节点)情况： 左旋(以root->right为支点，逆时针旋转)
  if (balance < -1 && key > node->right->key)
    return leftRotate(node);

  // 左(左子树)右(右节点)情况：左旋(以root->left->right为支点，逆时针旋转)+右旋(以root->left为支点，顺时针旋转)(先旋转底部，再旋转顶部)
  if (balance > 1 && key > node->left->key) {
    node->left = leftRotate(node->left);
    return rightRotate(node);
  }

  // 右(右子树)左(左节点)情况：右旋(以root->right->left为支点，顺时针旋转)+左旋(以root->right为支点，逆时针旋转)
  if (balance < -1 && key < node->right->key) {
    node->right = rightRotate(node->right);
    return leftRotate(node);
  }

  // 返回节点指针
  return node;
}

// 查找树中最小值节点（用于删除）
struct Node *minValueNode(struct Node *node) {
  struct Node *current = node;

  // 找到最左边的叶子节点
  while (current->left != NULL)
    current = current->left;

  return current;
}

// 删除节点并保持平衡
struct Node *deleteNode(struct Node *root, int key) {
  // 1. 普通二叉搜索树的删除
  if (root == NULL)
    return root;

  if (key < root->key)
    root->left = deleteNode(root->left, key);
  else if (key > root->key)
    root->right = deleteNode(root->right, key);
  else {
    // 找到要删除的节点
    if (root->left == NULL || root->right == NULL) {
      struct Node *temp = root->left ? root->left : root->right;

      if (temp == NULL) { // 没有子节点的情况
        temp = root;
        root = NULL;
      } else {         // 一个子节点的情况
        *root = *temp; // 复制子节点的内容
      }
      free(temp);
    } else {
      // 有两个子节点的情况
      struct Node *temp = minValueNode(root->right);

      // 用后继节点替换当前节点
      root->key = temp->key;

      // 删除后继节点
      root->right = deleteNode(root->right, temp->key);
    }
  }

  // 如果树只有一个节点，直接返回
  if (root == NULL)
    return root;

  // 2. 更新节点的高度
  root->height =
      1 + (height(root->left) > height(root->right) ? height(root->left)
                                                    : height(root->right));

  // 3. 计算平衡因子并检查是否需要平衡
  int balance = getBalance(root);

  // 如果不平衡，进行相应的旋转

  // 左左情况
  if (balance > 1 && getBalance(root->left) >= 0)
    return rightRotate(root);

  // 左右情况
  if (balance > 1 && getBalance(root->left) < 0) {
    root->left = leftRotate(root->left);
    return rightRotate(root);
  }

  // 右右情况
  if (balance < -1 && getBalance(root->right) <= 0)
    return leftRotate(root);

  // 右左情况
  if (balance < -1 && getBalance(root->right) > 0) {
    root->right = rightRotate(root->right);
    return leftRotate(root);
  }

  // 返回节点指针
  return root;
}

// 中序遍历
void inorder(struct Node *root) {
  if (root != NULL) {
    inorder(root->left);
    printf("%d ", root->key);
    inorder(root->right);
  }
}

// 搜索节点
struct Node *search(struct Node *root, int key) {
  if (root == NULL || root->key == key)
    return root;

  if (key < root->key)
    return search(root->left, key);

  return search(root->right, key);
}

int main() {
  struct Node *root = NULL;

  // 插入节点
  root = insert(root, 10);
  root = insert(root, 20);
  root = insert(root, 30);
  root = insert(root, 40);
  root = insert(root, 50);
  root = insert(root, 25);

  printf("Inorder traversal of the constructed AVL tree is:\n");
  inorder(root);
  printf("\n");

  // 删除节点
  root = deleteNode(root, 40);

  printf("Inorder traversal after deletion of 40:\n");
  inorder(root);
  printf("\n");

  // 搜索节点
  int key = 25;
  struct Node *result = search(root, key);
  if (result != NULL)
    printf("Node with key %d found in the AVL tree.\n", key);
  else
    printf("Node with key %d not found in the AVL tree.\n", key);

  return 0;
}
