# keysniffer
A Linux kernel module to grab keys pressed in the keyboard, or a keylogger.

keysniffer was originally written with the US keyboard (and conforming laptops) in mind. By default it shows human-readable strings for the keys pressed. Optionally, the `keycode shift_mask` pair can be printed in hex or decimal. You can lookup the keycodes in `/usr/include/linux/input-event-codes.h`.

The keypress logs are recorded in debugfs as long as the module is loaded. Only root or sudoers can read the log. The module name has been camouflaged to blend-in with other kernel modules.

You can, however, execute a script at shutdown or reboot (the procedure would be distro-specific) to save the keys to a file.

**DISCLAIMER:** keysniffer is intended to track your own devices and NOT to trespass on others. The author has never used it to compromise any third-party device and is not responsible for any unethical application.

*Love smart and efficient terminal utilities? Explore my repositories. Buy me a cup of coffee if they help you.*

<p align="center">
<a href="https://saythanks.io/to/jarun"><img src="https://img.shields.io/badge/say-thanks!-ff69b4.svg" /></a>
<a href="https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=RMLTQ76JSXJ4Q"><img src="https://img.shields.io/badge/Donate-$5-green.svg" alt="Donate via PayPal!" /></a>
</p>

## Table of contents

- [Compilation](#compilation)
    - [Build](#build)
    - [DKMS support](#dkms-support)
- [Usage](#usage)
- [License](#license)
- [Developer](#developer)
- [Links](#links)

## Compilation
### Build
Clone the repository and run:

    # make
Note that you need to have the linux kernel headers installed for your running kernel version.

To insert the module into the kernel, run:

    # insmod kisni.ko
    OR
    # make load

To unload the module (and clear the logs), run:

    # rmmod kisni

### DKMS support
If you have DKMS installed, you can install keysniffer in such a way that it survives kernel upgrades. It is recommended to remove older versions of keysniffer by running `dkms remove -m kisni -v OLDVERSION --all` as root. To install the new version, run:

    # make -f Makefile.dkms

To uninstall it, run:

    # make -f Makefile.dkms uninstall

## Usage
To view the pressed keys, run:

```
# cat /sys/kernel/debug/kisni/keys
modinfo kisni.ko
cat /sys/kernel/debug/kisni/keys
#
```

To log generic hex keycodes in the format `keycode shift_mask`, run:

```
# insmod kisni.ko codes=1
// Type something
# cat /sys/kernel/debug/kisni/keys
23 0
12 0
26 0
26 0
18 0
39 0
2a 0
2a 1
2a 1
11 1
18 0
13 0
26 0
20 0
2a 0
2a 1
2a 1
2 1
1c 0
1f 0
16 0
20 0
18 0
39 0
2e 0
1e 0
14 0
6a 0
1c 0
```

To log the keycodes in decimal, run:

    # insmod kisni.ko codes=2

To check the module details:

```
# modinfo kisni.ko
filename:       kisni.ko
description:    Sniff and log keys pressed in the system to debugfs
version:        1.6
author:         Arun Prakash Jana <engineerarun@gmail.com>
license:        GPL v2
srcversion:     40AA880EE5017590B293170
depends:
vermagic:       4.4.0-103-generic SMP mod_unload modversions
parm:           codes:log format (0:US keys (default), 1:hex keycodes, 2:dec keycodes) (int)
```

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
