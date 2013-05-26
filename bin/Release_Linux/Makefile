
WARNING := echo "--- WARNING: Custom Ogre libraries were already installed. We did not overwrite. It appears Ogre's shared libs are different from this build, OgreMeshy may crash. We strongly encourage to build from source code in this case ---";

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
	@ln -s /opt/ogremeshy/libOgreMain.so.1.9.0 /usr/local/lib/libOgreMain.so.1.9.0 || cmp -s /opt/ogremeshy/libOgreMain.so.1.9.0 /usr/local/lib/libOgreMain.so.1.9.0 || $(WARNING)
	@ln -s /opt/ogremeshy/libOgreOverlay.so.1.9.0 /usr/local/lib/libOgreOverlay.so.1.9.0 || cmp -s /opt/ogremeshy/libOgreOverlay.so.1.9.0 /usr/local/lib/libOgreOverlay.so.1.9.0 || $(WARNING)

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