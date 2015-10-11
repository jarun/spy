# keysniffer
A kernel module to grab keys pressed in the keyboard.  
The keys are printed with kernel messages as well as via debugfs.

#License
keysniffer is licensed under GPL v3.

#Compilation
Clone the repository and run:
<pre>$ make</pre>

#Usage
To insert the module into the kernel, run:
<pre>$ sudo insmod kisni.ko</pre>
To view the keys, run:
<pre>$ dmesg</pre>
OR
<pre>$ cat /sys/kernel/debug/kisni/keys</pre>

#Links
http://www.gadgetweb.de/programming/39-how-to-building-your-own-kernel-space-keylogger.html  
https://wiki.archlinux.org/index.php/Xmodmap  
http://www.osdever.net/bkerndev/Docs/keyboard.htm  
http://arjunsreedharan.org/post/99370248137/kernel-201-lets-write-a-kernel-with-keyboard  
http://www.linuxquestions.org/questions/slackware-14/how-to-match-x-keyboard-layout-with-the-the-kernel-keyboard-map-4175455061/  
http://www.tldp.org/LDP/lkmpg/2.4/html/x1210.html  
