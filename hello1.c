#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>

#include "hello1.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sevil Medgitova");
MODULE_DESCRIPTION("Hello1 module");

struct hello_entry {
    struct list_head list;
    ktime_t time_before;
    ktime_t time_after;
};

static LIST_HEAD(hello_list);

int print_hello(void)
{
    struct hello_entry *entry;

    entry = kmalloc(sizeof(*entry), GFP_KERNEL);

    if (!entry) {
        pr_err("hello1: memory allocation failed\n");
        return -ENOMEM;
    }

    entry->time_before = ktime_get();

    pr_info("Hello, world!\n");

    entry->time_after = ktime_get();

    list_add_tail(&entry->list, &hello_list);

    return 0;
}

EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void)
{
    pr_info("hello1: module loaded\n");
    return 0;
}

static void __exit hello1_exit(void)
{
    struct hello_entry *entry, *tmp;

    list_for_each_entry_safe(entry, tmp, &hello_list, list) {
        pr_info("hello1: print time = %lld ns\n",
                (long long)ktime_to_ns(
                    ktime_sub(entry->time_after,
                              entry->time_before)));

        list_del(&entry->list);
        kfree(entry);
    }

    pr_info("hello1: module unloaded\n");
}

module_init(hello1_init);
module_exit(hello1_exit);
