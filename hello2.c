#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include "hello1.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sevil Medgitova");
MODULE_DESCRIPTION("Hello2 module");

static unsigned int count = 1;

module_param(count, uint, 0444);
MODULE_PARM_DESC(count, "Number of greetings");

static int __init hello2_init(void)
{
    unsigned int i;
    int ret;

    if (count == 0 || count > 10) {
        pr_err("hello2: invalid count = %u\n", count);
        return -EINVAL;
    }

    if (count >= 5) {
        pr_warn("hello2: count is %u\n", count);
    }

    pr_info("hello2: module loaded\n");

    for (i = 0; i < count; i++) {
        ret = print_hello();

        if (ret)
            return ret;
    }

    return 0;
}

static void __exit hello2_exit(void)
{
    pr_info("hello2: module unloaded\n");
}

module_init(hello2_init);
module_exit(hello2_exit);
