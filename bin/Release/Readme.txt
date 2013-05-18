OGRE MESHY  v1.5
=================

Copyright (C) 2010-2013 Matías Goldberg ("dark_sylinc)
Under GPLv3 license

File.png, Reload.png & ChangeBGColour.png are icons from H2O KDE package, made by Rogerio de Souza Santos, http://opendesktop.org/content/show.php?content=127149
Bitstream Vera fonts Copyright (c) 2003 by Bitstream, see FONTS_COPYRIGHT.txt for license information that should've been included in Ogre Meshy package or get it from http://www-old.gnome.org/fonts/#Final_Bitstream_Vera_Fonts
Thanks to Duong Pham for Linux tips.

COMMAND LINE OPTIONS
====================
Load specific resources.cfg:   OgreMeshy.exe "/r:C:\MyFolder\resources.cfg"
Show OGRE Config dialog:    OgreMeshy.exe -setup
Load a mesh:                OgreMeshy.exe "C:\MyFolder\MyMesh.mesh"
Display Cmd help:           OgreMeshy.exe -h

OTHER:
======
Zoom In:                                   Hold Right click and move mouse
Rotate camera around center:               Hold Left click and move mouse
Pan camera left/right/up/down:             Hold Shift + Left click and move mouse
Pan camera left/right/forward/backwards:   Hold Shift + Right click and move mouse


TROUBLESHOOTING:
================
If the program doesn't start, you may need the following:

Visual C++ 2008 SP1 redist   http://www.microsoft.com/downloads/details.aspx?familyid=a5c84275-3b97-4ab7-a40d-3802b2af5fc2&displaylang=en
Latest DirectX               http://www.microsoft.com/downloads/details.aspx?familyid=0CEF8180-E94A-4F56-B157-5AB8109CB4F5&displaylang=en

If you click the track node in the tree window when an animation is active, the axis will appear at that specific bone. To hide it, click in the name of the animation at the top (root) of the tree or select "None" as animation.

In linux, you have to "install the program". If you've downloaded the source, check COMPILING instructions, if you've downloaded the compiled package, type "make install" from a terminal

FAQ
===

Q: The whole window is blank, I can only see the toolbar and menu.
A: Possibly the GUI_Layout.cfg file is corrupted. Delete it and restart the application

Q: The displayed bones are too big/small!
A: Go to animation Tab, there is a slider that says "Bone size".

Q: The bone name's text (Alt+D) is too big/small!
A: Go to animation Tab, there is a slider that says "Bone size". It scales both the bones and bone name text when they're displayed. Additionally, you can edit the text file "Default.fontdef" in an external editor, which is located under the folder "Resources/Fonts", and change the default font size.

Linux specific:
---------------
Q: I get to the Render API OpenGL selection screen, hit ok, and then the program exits.
A: Check the folder "~/.ogremeshy" exists, which is where Ogre Meshy saves all config data. (i.e. /home/myusername/.ogremeshy)

Q: Selecting a background colour or a bone text colour results always in black
A: This is a known issue. The bone text colour can be changed by manually editing the text file "~/.ogremeshy/UserSettings.cfg" and modifying the "BoneNameColour" field (the number is in hex). There's no way to change the background colour yet.

Q: The program exits right after saying 'Loading Plugin CgProgramManager' in the console, it doesn't even show up a window
A: Make sure Cg libs are installed. If it still fails or you don't want to install them, edit plugins.cfg with a text editor and remove that plugin.

Q: It doesn't work
A: Linux support is still in beta. Share your experience at http://www.ogre3d.org/forums/viewtopic.php?f=11&t=59600


KNOWN ISSUES:
=============
* On Direct3D, the render window may flicker against the other dockable windows. To fix this, resize the main window or minimize & restore
* On OpenGL, the render window may not be updated after resizing one of the docked windows. To fix this, resize the main window or minimize & restore
* When loading resources.cfg, it attempts to load _all_ resources it founds, some of them may crash the viewer even if they're not materials
* Free mode camera is not yet supported, despite being displayed in the menu
* In Linux, the colour picker dialogs don't work correctly.

THANKS:
======
Alberto Toglia - toglia
guidry
Thomas Fischer - tdev


//----------------------------------------------------------------------
//  Copyright (C) 2010-2013 Matias N. Goldberg ("dark_sylinc")
//  This file is part of Ogre Meshy.
//
//  Ogre Meshy is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Ogre Meshy is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Ogre Meshy.  If not, see <http://www.gnu.org/licenses/>.
//----------------------------------------------------------------------