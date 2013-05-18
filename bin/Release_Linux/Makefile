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
	mkdir -p $(HOME)/.ogremeshy
	su root -c make rootinstall
	
ubuntuinstall:
	mkdir -p $(HOME)/.ogremeshy
	sudo make rootinstall

rootuninstall:
	rm -r /opt/ogremeshy
	rm /usr/bin/ogremeshy

uninstall:
	rm -r $(HOME)/.ogremeshy
	su root -c make rootuninstall