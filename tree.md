# Linux 内核红黑树
## Linux 内核红黑树使用

```c
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/rbtree.h>

struct Number
{
    struct rb_node node;
    int Key;
};

/* 红黑树的根节点 */
struct rb_root mytree = RB_ROOT;

/* 根据key查找节点*/
struct Number *my_search(struct rb_root *root, int new)
{
    struct rb_node *node = root->rb_node;

    while (node)
    {
        struct Number *data = container_of(node, struct Number, node);

        if (data->Key > new)
        {
            node = node->rb_left;
        }
        else if (data < new)
        {
            node = node->rb_right;
        }
        else
        {
            // data == new
            return data;
        }
    }
    return NULL;
}

/* 把一个元素插入红黑树中 */
int my_insert(struct rb_root *root, struct Number *data)
{
    struct rb_node **new = &(root->rb_node), *parent = NULL;

    /* 寻找可以添加新节点的地方 */
    while (*new)
    {
        struct Number *this = container_of(*new, struct Number, node);

        parent = *new;
        if (this->Key > data->Key)
        {
            new = &((*new)->rb_left);
        }
        else if (this->Key < data->Key)
        {
            new = &((*new)->rb_right);
        }
        else
        {
            return -1;
        }
    }

    /* 添加一个新节点 */
    rb_link_node(&data->node, parent, new);
    rb_insert_color(&data->node, root);

    return 0;
}

static int __init my_test_init(void)
{
    int i;
    struct Number *data;
    struct rb_node *node;

    /* 插入元素 */
    for (i = 0; i < 20; i++)
    {
        data = kmalloc(sizeof(struct Number), GFP_KERNEL);
        data->Key = i;
        my_insert(&mytree, data);
    }

    /* 遍历红黑树，输出所有节点的key值 */
    for (node = rb_first(&mytree); node; node = rb_next(node))
    {

        printk("key=%d\n", rb_entry(node, struct Number, node)->Key);
    }

    return 0;
}

static void __exit my_test_exit(void)
{
    struct Number *data;
    struct rb_node *node;
    for (node = rb_first(&mytree); node; node = rb_next(node))
    {
        data = rb_entry(node, struct Number, node);
        if (data)
        {
            rb_erase(&data->node, &mytree);
            kfree(data);
        }
    }
    return;
}

module_init(my_test_init);
module_exit(my_test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("0xScnappi");
MODULE_DESCRIPTION("my test kernel module");
MODULE_ALIAS("mytest");
```

Makefile文件
```Makefile
BASEINCLUDE ?= /lib/modules/$(shell uname -r)/build

mytest-objs := my_test.o
obj-m := mytest.o

all :
	$(MAKE) -C $(BASEINCLUDE) M=$(PWD) modules;

clean:
	$(MAKE) -C $(BASEINCLUDE) M=$(PWD) clean;

	rm -f *.ko
```