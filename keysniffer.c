#include<init.h>
#include<kernel.h>
#include<module.h>
#include<keyboard.h>

MODULE_LICENSE("GPL v3");
MODULE_AUTHOR("Arun Prakash Jana <engineerarun@gmail.com>");
MODULE_DESCRIPTION("A kernel module to sniff and log the keys pressed in the system");

static struct notifier_block keysniffer_blk = {
	.notifier_call = keysniffer_cb;
};


static int __init keysniffer_init(void)
{
	pr_debug("keysniffer init\n");
	return 0;
}

static void __exit keysniffer_exit(void)
{
	pr_debug("keysniffer exit\n");
	return;
}

module_init(keysniffer_init);
module_exit(keysniffer_exit);
