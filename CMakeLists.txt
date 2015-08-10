#-------------------------------------------------------------------
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

project( OgreMeshy )

cmake_minimum_required(VERSION 2.8)

set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG" )

if( CMAKE_BUILD_TYPE STREQUAL "" )
	set( CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "One of: Debug Release RelWithDebInfo MinSizeRel." FORCE )
endif()

# Setup Ogre
set( OGRE_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/Ogre" )
if( WIN32 )
	set( OGRE_BINARIES "${OGRE_SOURCE}/build/" )
	link_directories( "${OGRE_BINARIES}/lib/$(ConfigurationName)" )
else()
	set( OGRE_BINARIES "${OGRE_SOURCE}/build/${CMAKE_BUILD_TYPE}" )
	link_directories( "${OGRE_BINARIES}/lib" )
endif()

include_directories( "${OGRE_SOURCE}/OgreMain/include" )
include_directories( "${OGRE_BINARIES}/include" )
#include_directories( "${OGRE_SOURCE}/Components/Hlms/Common/include" )
#include_directories( "${OGRE_SOURCE}/Components/Hlms/Unlit/include" )
#include_directories( "${OGRE_SOURCE}/Components/Hlms/Pbs/include" )
include_directories( "${OGRE_SOURCE}/Components/Overlay/include" )
include_directories( "${OGRE_SOURCE}/Components/RTShaderSystem/include" )

set( OGRE_LIBRARIES
	debug OgreMain_d
	debug OgreOverlay_d
	debug OgreRTShaderSystem_d
	#debug OgreHlmsUnlit_d
	#debug OgreHlmsPbs_d

	optimized OgreMain
	optimized OgreOverlay
	#optimized OgreHlmsUnlit
	#optimized OgreHlmsPbs
	)

add_definitions( -DMESHY_USE_RTSS )

# Setup wxWidgets
if( WIN32 )
	set( wxWidgets_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/wxWidgets_3_0" )
	set( wxWidgets_INCLUDE_DIRS "${wxWidgets_SOURCE}/include" )
	include_directories( "${wxWidgets_INCLUDE_DIRS}" )
		# Technically we should include mswu & mswud but we assume they're equal.
		# (CMake can't do per target includes... grr....)
		include_directories( "${wxWidgets_SOURCE}/lib/vc_lib/mswu" )
		link_directories( "${wxWidgets_SOURCE}/lib/vc_lib" )

	set( wxWidgets_LIBRARIES
		debug wxbase30ud
		debug wxmsw30ud_core
		debug wxmsw30ud_aui
		debug wxpngd
		debug wxzlibd

		optimized wxbase30u
		optimized wxmsw30u_core
		optimized wxmsw30u_aui
		optimized wxpng
		optimized wxzlib
		)
else()
	find_package( wxWidgets COMPONENTS core base aui adv REQUIRED )
	include( ${wxWidgets_USE_FILE} )
	include_directories( ${wxWidgets_INCLUDE_DIRS} )
	add_definitions( -DwxUSE_GUI=1 )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${wxWidgets_CXX_FLAGS}" )
endif()

if(UNIX)
	find_package( GTK2 REQUIRED gtk )
	include_directories( ${GTK2_INCLUDE_DIRS} )
	add_definitions( ${GTK2_DEFINITIONS} )

	# Thanks to dermont for this fix. Apparently, this is the cause:
	# https://bugzilla.redhat.com/show_bug.cgi?id=639058
	#pkg_check_modules(PIXBUF gdk-pixbuf-3.0)
	#include_directories( ${PIXBUF_INCLUDE_DIRS} )

	find_package(OpenGL)
endif(UNIX)

# Setup our application
set( EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}" )
if( MSVC )
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /arch:SSE2")
	add_definitions( -DUNICODE -D_UNICODE )
endif()

macro( add_recursive dir retVal )
	file( GLOB_RECURSE ${retVal} ${dir}/*.h ${dir}/*.cpp ${dir}/*.c )
endmacro()

include_directories( "./include" )

add_recursive( ./src SOURCES )
add_recursive( ./include HEADERS )
if( WIN32 )
	# Add Icon
	set( SOURCES ${SOURCES} ./scripts/Resources/Resource.rc )
endif()

add_executable( ${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS} )
target_link_libraries( ${PROJECT_NAME} ${OGRE_LIBRARIES} ${wxWidgets_LIBRARIES} )

if( UNIX )
	target_link_libraries( OgreMeshy ${GTK2_LIBRARIES} ${OPENGL_LIBRARIES} )
endif()

macro( copy_resources BUILD_TYPE )
	file( COPY "./scripts/Resources/Fonts" DESTINATION "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}/Resources" )
	file( COPY "./scripts/Resources/Icons" DESTINATION "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}/Resources" )
	file( COPY "./scripts/Resources/Other" DESTINATION "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}/Resources" )
	file( REMOVE_RECURSE "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}/Resources/Models" )
	file( RENAME "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}/Resources/Other" "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}/Resources/Models" )
	file( COPY "./scripts/Resources/Blender/Axis.material"			 DESTINATION "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}/Resources/Models" )
	file( COPY "./scripts/Resources/Blender/Axis.mesh"				 DESTINATION "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}/Resources/Models" )
	file( COPY "./scripts/Resources/Blender/Bones/BoneMesh.material" DESTINATION "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}/Resources/Models" )
	file( COPY "./scripts/Resources/Blender/Bones/Bones.png"		 DESTINATION "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}/Resources/Models" )
	file( COPY "./scripts/Resources/Blender/Bones/BoneTip.mesh"		 DESTINATION "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}/Resources/Models" )
	file( COPY "./scripts/Resources/Blender/Bones/BoneGlobe.mesh"	 DESTINATION "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}/Resources/Models" )
	file( COPY "${OGRE_SOURCE}/Samples/Media/RTShaderLib"			 DESTINATION "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}/Resources" )

	if( UNIX )
		file( COPY "./scripts/Resources/OgreIcon.ico" DESTINATION "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}/Resources/" )
	endif()
endmacro()

copy_resources( "Debug" )
copy_resources( "Release" )
copy_resources( "RelWithDebInfo" )
copy_resources( "MinSizeRel" )

set( INST_DEST "." )
install(TARGETS OgreMeshy RUNTIME DESTINATION ${INST_DEST})

if( WIN32 )
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
IF( UNIX )
	SET(CPACK_GENERATOR ${CPACK_GENERATOR};STGZ;TGZ)
ENDIF()
IF( LINUX )
	SET(CPACK_GENERATOR ${CPACK_GENERATOR};DEB;RPM)
ENDIF()
IF( MSVC )
	SET(CPACK_GENERATOR ${CPACK_GENERATOR};NSIS)
ENDIF()
IF( APPLE )
	SET(CPACK_GENERATOR ${CPACK_GENERATOR};PackageMaker)
ENDIF()
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}-${CPACK_PACKAGE_VERSION_MINOR}-${CPACK_PACKAGE_VERSION_PATCH}-${CMAKE_SYSTEM_PROCESSOR}")

# some NSIS stuff
IF( WIN32 AND NOT UNIX )
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
