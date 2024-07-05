#!/bin/sh
if [ -f /etc/rc.d/$1 ]; then
	rm -v /etc/rc.d/$1 
	echo "removed $1 from init."
else
	echo "
		** rm-rc.d usage:
		rm-rc.d [rc.d link name]
		
		examples:
		rm-rc.d 40httpd 
		rm-rc.d 40ftpd 
		rm-rc.d 50telnetd 
	"
fi
