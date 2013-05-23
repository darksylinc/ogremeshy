
WARNING := echo "--- WARNING: Custom Ogre libraries some were already installed. We did not overwrite. If there's a missmatch, OgreMeshy may crash. Prefer building from source code ---";

rootinstall:
	mkdir -p /opt/ogremeshy
	mkdir -p /opt/ogremeshy/Plugins
	mkdir -p /opt/ogremeshy/Resources
	
	cp OgreMeshy /opt/ogremeshy
	cp Plugins.cfg /opt/ogremeshy
	cp *.so* /opt/ogremeshy
	cp -r Resources/* /opt/ogremeshy/Resources
	cp -r Plugins/* /opt/ogremeshy/Plugins

	ln -s -f /opt/ogremeshy/OgreMeshy /usr/bin/ogremeshy
	@ln -s /opt/ogremeshy/libOgreMain.so.1.9.0 /usr/local/lib/libOgreMain.so.1.9.0 || $(WARNING)
	@ln -s /opt/ogremeshy/libOgreOverlay.so.1.9.0 /usr/local/lib/libOgreOverlay.so.1.9.0 || $(WARNING)

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