#-------------------------------------------------------------------
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

cmake_minimum_required(VERSION 2.8.5)
cmake_policy(SET CMP0003 NEW)

# Use relative paths
# This is mostly to reduce path size for command-line limits on windows
if(WIN32)
	# This seems to break Xcode projects so definitely don't enable on Apple builds
	set(CMAKE_USE_RELATIVE_PATHS true)
	set(CMAKE_SUPPRESS_REGENERATION true)

	set(CMAKE_MODULE_PATH "$ENV{OGRE_HOME}/CMake/;${CMAKE_MODULE_PATH}")
	#set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};E:/Projects/SDK/OgreLatest/CMake/Utils/;E:/Projects/SDK/OgreLatest/CMake/Packages/")
endif()

if(UNIX)
	if(EXISTS "/usr/local/lib/OGRE/cmake")
		set(CMAKE_MODULE_PATH "/usr/local/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
	elseif(EXISTS "/usr/lib/OGRE/cmake")
		set(CMAKE_MODULE_PATH "/usr/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
	elseif(EXISTS "/usr/share/OGRE/cmake/modules")
		set(CMAKE_MODULE_PATH "/usr/share/OGRE/cmake/modules;${CMAKE_MODULE_PATH}")
	else ()
		message(SEND_ERROR "Failed to find module path.")
	endif(EXISTS "/usr/local/lib/OGRE/cmake")
endif(UNIX)

if (CMAKE_BUILD_TYPE STREQUAL "")
	# CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
	# differentiation between debug and release builds.
	set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

#set(CMAKE_DEBUG_POSTFIX "_d")

PROJECT( OgreMeshy )

FIND_PACKAGE(wxWidgets COMPONENTS core base aui adv REQUIRED)
include( "${wxWidgets_USE_FILE}" )

FIND_PACKAGE(OGRE COMPONENTS Overlay REQUIRED)
include_directories( ${OGRE_INCLUDE_DIRS} )
foreach(dir ${OGRE_INCLUDE_DIRS})
   set(OGRE_Overlay_INCLUDE_DIR ${dir}/Overlay ${OGRE_Overlay_INCLUDE_DIR})
   set(OGRE_RTSS_INCLUDE_DIR ${dir}/RTShaderSystem ${OGRE_RTSS_INCLUDE_DIR})
endforeach(dir)
include_directories( ${OGRE_Overlay_INCLUDE_DIR} )
include_directories( ${OGRE_RTSS_INCLUDE_DIR} )

if(UNIX)
	FIND_PACKAGE(GTK2 COMPONENTS gtk)
	INCLUDE_DIRECTORIES(${GTK2_INCLUDE_DIRS})

	# Thanks to dermont for this fix. Apparently, this is the cause:
	# https://bugzilla.redhat.com/show_bug.cgi?id=639058
	pkg_check_modules(PIXBUF gdk-pixbuf-2.0)
	INCLUDE_DIRECTORIES( ${PIXBUF_INCLUDE_DIRS} )

	FIND_PACKAGE(OpenGL)
endif(UNIX)

#ADD_SUBDIRECTORY( "${OgreMeshy_SOURCE_DIR}/src" )
#ADD_SUBDIRECTORY( "${OgreMeshy_SOURCE_DIR}/src/Core" )
include_directories( "${OgreMeshy_SOURCE_DIR}/include" )

set (HEADER_FILES
	include/CmdSettings.h
	include/Constants.h
	include/Core/AnimationPanel.h
	include/Core/GridSettingsImpl.h
	include/Core/MovableText.h
	include/Core/Panels/AnimPosePanel.h
	include/Core/Panels/LightsPanel.h
	include/Core/ShaderGeneratorTechniqueResolverListener.h
	include/Core/wxOgreMeshViewerMainFrame.h
	include/Core/wxOgreMeshViewerMainFrameImpl.h
	include/Core/wxOgreRenderWindow.h
	include/Core/wxOgreRenderWindowListener.h
	)

set (SOURCE_FILES
	src/main.cpp
	src/Core/AnimationPanel.cpp
	src/Core/GridSettingsImpl.cpp
	src/Core/MovableText.cpp
	src/Core/Panels/AnimPosePanel.cpp
	src/Core/Panels/LightsPanel.cpp
	src/Core/ShaderGeneratorTechniqueResolverListener.cpp
	src/Core/wxOgreMeshViewerMainFrame.cpp
	src/Core/wxOgreMeshViewerMainFrameImpl.cpp
	src/Core/wxOgreRenderWindow.cpp
	)


if(WIN32)
	# add icon
	set (SOURCE_FILES ${SOURCE_FILES} ${OgreMeshy_SOURCE_DIR}/scripts/Resources/Resource.rc)
endif()

link_directories(${OGRE_LIBRARY_DIRS})
ADD_EXECUTABLE(OgreMeshy WIN32 ${HEADER_FILES} ${SOURCE_FILES} )
TARGET_LINK_LIBRARIES( OgreMeshy ${wxWidgets_LIBRARIES} )
TARGET_LINK_LIBRARIES( OgreMeshy ${OGRE_LIBRARIES} )
set(OGRE_Overlay_LIBRARIES OgreOverlay)
set(OGRE_RTSS_LIBRARIES OgreRTShaderSystem)
TARGET_LINK_LIBRARIES( OgreMeshy ${OGRE_Overlay_LIBRARIES} ${OGRE_RTSS_LIBRARIES} )

if(UNIX)
	TARGET_LINK_LIBRARIES( OgreMeshy ${GTK2_LIBRARIES} )
	TARGET_LINK_LIBRARIES( OgreMeshy ${OPENGL_LIBRARIES} )
endif(UNIX)

set(INST_DEST ".")
install(TARGETS OgreMeshy RUNTIME DESTINATION ${INST_DEST})

if(WIN32)
	install(FILES ${OgreMeshy_SOURCE_DIR}/bin/Release/ChangeLog.txt DESTINATION ${INST_DEST})
	install(FILES ${OgreMeshy_SOURCE_DIR}/bin/Release/FONTS_COPYRIGHT.txt DESTINATION ${INST_DEST})
	install(FILES ${OgreMeshy_SOURCE_DIR}/bin/Release/LICENSE.txt DESTINATION ${INST_DEST})
	install(FILES ${OgreMeshy_SOURCE_DIR}/bin/Release/Readme.txt DESTINATION ${INST_DEST})

	# add 3rd party files: Ogre, etc
	install(FILES ${OgreMeshy_SOURCE_DIR}/bin/Release/OgreMain.dll DESTINATION ${INST_DEST})
	install(FILES ${OgreMeshy_SOURCE_DIR}/bin/Release/Plugins.cfg DESTINATION ${INST_DEST} )
	install(FILES ${OgreMeshy_SOURCE_DIR}/bin/Release/Plugins/Plugin_CgProgramManager.dll DESTINATION ${INST_DEST}/Plugins )
	install(FILES ${OgreMeshy_SOURCE_DIR}/bin/Release/Plugins/Plugin_OctreeSceneManager.dll DESTINATION ${INST_DEST}/Plugins )
	install(FILES ${OgreMeshy_SOURCE_DIR}/bin/Release/Plugins/Plugin_ParticleFX.dll DESTINATION ${INST_DEST}/Plugins )
	install(FILES ${OgreMeshy_SOURCE_DIR}/bin/Release/Plugins/RenderSystem_Direct3D9.dll DESTINATION ${INST_DEST}/Plugins )
	install(FILES ${OgreMeshy_SOURCE_DIR}/bin/Release/Plugins/RenderSystem_GL.dll DESTINATION ${INST_DEST}/Plugins )

	install(FILES ${OgreMeshy_SOURCE_DIR}/bin/Release/Plugins/cg.dll DESTINATION ${INST_DEST}/Plugins )
	
	# and the resources
	FILE(GLOB files "${OgreMeshy_SOURCE_DIR}/bin/Release/Resources/Fonts/*.*")
	INSTALL(FILES ${files} DESTINATION ${INST_DEST}/Resources/Fonts)

	FILE(GLOB files "${OgreMeshy_SOURCE_DIR}/bin/Release/Resources/Icons/32x32/*.*")
	INSTALL(FILES ${files} DESTINATION ${INST_DEST}/Resources/Icons/32x32)

	FILE(GLOB files "${OgreMeshy_SOURCE_DIR}/bin/Release/Resources/Models/*.*")
	INSTALL(FILES ${files} DESTINATION ${INST_DEST}/Resources/Models)
	
endif()

# and CPack
# cpack
set(CPACK_PACKAGE_DESCRIPTION "OgreMeshy")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Ogre Meshy is a tool for viewing OGRE mesh files.")
set(CPACK_PACKAGE_NAME "OgreMeshy")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "ois ogre")
set(CPACK_PACKAGE_CONTACT "http://www.ogre3d.org/tikiwiki/Ogre+Meshy")
set(CPACK_PACKAGE_VENDOR "http://www.ogre3d.org/tikiwiki/Ogre+Meshy")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
#SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ReadMe.txt")
# TODO: add the version variable at the top in here
SET(CPACK_PACKAGE_VERSION_MAJOR "1")
SET(CPACK_PACKAGE_VERSION_MINOR "5")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "OgreMeshy")

set(CPACK_FILE_ASSOCIATION_EXTENSION ".mesh")

# SET(CPACK_GENERATOR ZIP)
IF(UNIX)
	SET(CPACK_GENERATOR ${CPACK_GENERATOR};STGZ;TGZ)
ENDIF(UNIX)
IF(LINUX)
	SET(CPACK_GENERATOR ${CPACK_GENERATOR};DEB;RPM)
ENDIF(LINUX)
IF(MSVC)
	SET(CPACK_GENERATOR ${CPACK_GENERATOR};NSIS)
ENDIF(MSVC)
IF(APPLE)
	SET(CPACK_GENERATOR ${CPACK_GENERATOR};PackageMaker)
ENDIF(APPLE)
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}-${CPACK_PACKAGE_VERSION_MINOR}-${CPACK_PACKAGE_VERSION_PATCH}-${CMAKE_SYSTEM_PROCESSOR}")

# some NSIS stuff
IF(WIN32 AND NOT UNIX)
	# There is a bug in NSI that does not handle full unix paths properly. Make
	# sure there is at least one set of four (4) backlasshes.
	SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY}")
	SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\http://www.ogre3d.org/tikiwiki/Ogre+Meshy")
	SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\http://www.ogre3d.org/tikiwiki/Ogre+Meshy")
	SET(CPACK_NSIS_CONTACT "dark_sylinc@yahoo.com.ar")
	SET(CPACK_NSIS_MODIFY_PATH OFF)
	#SET(CPACK_NSIS_MUI_FINISHPAGE_RUN "OgreMeshy.exe")
	#SET(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
	set(CPACK_NSIS_MENU_LINKS "OgreMeshy.exe" "Ogre Meshy" "Readme.txt" "Readme")
	#set(CPACK_PACKAGE_EXECUTABLES "OgreMeshy" "OgreMeshy")
	SET(CPACK_NSIS_INSTALLED_ICON_NAME "OgreMeshy.exe")
ELSE(WIN32 AND NOT UNIX)
	#SET(CPACK_STRIP_FILES "bin/MyExecutable")
	SET(CPACK_SOURCE_STRIP_FILES "")
ENDIF(WIN32 AND NOT UNIX)
	#SET(CPACK_PACKAGE_EXECUTABLES "MyExecutable" "My Executable")
INCLUDE(CPack)