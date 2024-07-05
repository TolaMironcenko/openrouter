#!/bin/sh
if [ -f /etc/init.d/$1 ] && [ "$2" -gt 0 ]; then
	cd /etc/rc.d 
	ln -sv ../init.d/$1 "$2$1"
	echo "added $1 to init."
else
	echo "
		** add-rc.d usage:
		add-rc.d [init.d script name] [order number]
		
		examples:
		add-rc.d httpd 40
		add-rc.d ftpd 40
		add-rc.d telnetd 50
	"
fi
