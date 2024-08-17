# Linux 内核双向链表

## Linux 内核双向链表使用

```c
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>

struct Number
{
    int i;
    struct list_head list;
};

void list_test(void)
{
    // 把100个数字添加到这个链表中，循环该链表以输出所有成员的值
    unsigned long i;
    struct Number *num;
    struct Number *num1;

    LIST_HEAD(test_list);

    /* Add */
    for (i = 0; i < 100; i++)
    {
        num = kmalloc(sizeof(*num), GFP_KERNEL);
        num->i = i;
        list_add_tail(&num->list, &test_list);
    }

    i = 0;
    /* print list */
    printk("printk the list\n");
    list_for_each_entry(num, &test_list, list)
    {
        printk("%2d ", num->i);
        if ((i + 1) % 10 == 0)
        {
            printk("\n");
        }
        i++;
    }
    printk("\n");

    /* Delete */
    list_for_each_entry_safe(num, num1, &test_list, list)
    {
        list_del(&num->list);
        kfree(num);
    }

    if (list_empty(&test_list))
    {
        printk("Free test_list successfully\n");
    }

    return;
}

static int __init my_test_init(void)
{
    list_test();

    printk("my first kernel module init\n");
    return 0;
}

static void __exit my_test_exit(void)
{
    printk("goodbye\n");
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