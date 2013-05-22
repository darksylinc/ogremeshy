rootinstall:
	mkdir -p /opt/ogremeshy
	mkdir -p /opt/ogremeshy/Plugins
	mkdir -p /opt/ogremeshy/Resources
	
	cp OgreMeshy /opt/ogremeshy
	cp Plugins.cfg /opt/ogremeshy
	cp -r Resources/* /opt/ogremeshy/Resources
	cp -r Plugins/* /opt/ogremeshy/Plugins
	
	ln -s -f /opt/ogremeshy/OgreMeshy /usr/bin/ogremeshy

install:
	su root -c make rootinstall
	
ubuntu_install:
	sudo make rootinstall

rootuninstall:
	rm -r /opt/ogremeshy
	rm /usr/bin/ogremeshy

uninstall:
	rm -r $(HOME)/.ogremeshy
	su root -c make rootuninstall
	
ubuntu_uninstall:
	rm -r $(HOME)/.ogremeshy
	sudo make rootuninstall