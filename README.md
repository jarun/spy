# keysniffer
A Linux kernel module to grab keys pressed in the keyboard, or a keylogger. Works with the US keyboard (and conforming laptops).  

The keypress logs are printed via debugfs. Only root or sudoers can read the log. The module name has been crafted to blend-in with other kernel modules.  

keysniffer is intended to track your own devices and NOT to trespass on others. The author is not reponsible for any unethical usage.  

If you find `keysniffer` useful, please consider donating via PayPal.  
<a href="https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&amp;hosted_button_id=RMLTQ76JSXJ4Q"><img src="https://www.paypal.com/en_US/i/btn/btn_donateCC_LG.gif" alt="Donate Button with Credit Cards" /></a>

#License
keysniffer is licensed under GPL v3.

#Compilation
Clone the repository and run:
<pre>$ make</pre>

#Usage
To insert the module into the kernel, run:
<pre>$ sudo insmod kisni.ko</pre>
To view the pressed keys, run:
<pre>$ sudo cat /sys/kernel/debug/kisni/keys</pre>
To unload the module (and clear the logs), run:
<pre>$ sudo rmmod kisni</pre>

#Links
http://www.gadgetweb.de/programming/39-how-to-building-your-own-kernel-space-keylogger.html  
https://wiki.archlinux.org/index.php/Xmodmap  
http://www.osdever.net/bkerndev/Docs/keyboard.htm  
http://arjunsreedharan.org/post/99370248137/kernel-201-lets-write-a-kernel-with-keyboard  
http://www.linuxquestions.org/questions/slackware-14/how-to-match-x-keyboard-layout-with-the-the-kernel-keyboard-map-4175455061/  
http://www.tldp.org/LDP/lkmpg/2.4/html/x1210.html  
