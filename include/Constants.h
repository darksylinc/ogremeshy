//----------------------------------------------------------------------
//	Copyright (C) 2010 Matias N. Goldberg ("dark_sylinc")
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

#ifndef __Constants_defined__
#define __Constants_defined__

#include <OgreString.h>

//Resources in group "InternalMeshGroup" get erased and reinitialized all the time (to load the mesh)
//Resources in group "InternalPermanentMeshGroup" are loaded once and used by the application
static const Ogre::String c_InternMeshGroup		= "InternalMeshGroup";
static const Ogre::String c_InterMeshPermGroup	= "InternalPermanentMeshGroup";

static const Ogre::String c_userSettingsFile	= "UserSettings.cfg";
static const Ogre::String c_layoutSettingsFile	= "GUI_Layout.cfg";

#endif
