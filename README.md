# keysniffer
A Linux kernel module to grab keys pressed in the keyboard, or a keylogger. Works with the US keyboard (and conforming laptops).

The keypress logs are recorded in debugfs as long as the module is loaded. Only root or sudoers can read the log. The module name has been camouflaged to blend-in with other kernel modules.

You can, however, execute a script at shutdown or reboot (the procedure would be distro-specific) to save the keys to a file.

keysniffer is intended to track your own devices and NOT to trespass on others. The author has never usesd it to compromise someone else's system and is not responsible for any unethical application.

If you find `keysniffer` useful, please consider donating via PayPal. [![Donate Button](https://img.shields.io/badge/paypal-donate-orange.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=RMLTQ76JSXJ4Q)

## Table of contents

- [Compilation](#compilation)
- [Usage](#usage)
- [License](#license)
- [Developer](#developer)
- [Links](#links)

## Compilation
Clone the repository and run:

    $ make
Note that you need to have the linux headers installed for your running kernel version.

## Usage
To insert the module into the kernel, run:

    $ sudo insmod kisni.ko
To view the pressed keys, run:

    $ sudo cat /sys/kernel/debug/kisni/keys
    m
    o
    d
    i
    n
    f
    o

    k
    i
    s
    n
    i
    _ENTER_
    _UP_

    _TAB_
    _TAB_
    k
    _TAB_
    _ENTER_
    _ENTER_
To unload the module (and clear the logs), run:

    $ sudo rmmod kisni

## License
keysniffer is licensed under **GPLv2**.

## Developer
Copyright (C) 2015 [Arun Prakash Jana](mailto:engineerarun@gmail.com)

## Links
http://www.gadgetweb.de/programming/39-how-to-building-your-own-kernel-space-keylogger.html
https://wiki.archlinux.org/index.php/Xmodmap
http://www.osdever.net/bkerndev/Docs/keyboard.htm
http://arjunsreedharan.org/post/99370248137/kernel-201-lets-write-a-kernel-with-keyboard
http://www.linuxquestions.org/questions/slackware-14/how-to-match-x-keyboard-layout-with-the-the-kernel-keyboard-map-4175455061/
http://www.tldp.org/LDP/lkmpg/2.4/html/x1210.html
