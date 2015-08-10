#-------------------------------------------------------------------
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

macro( setupInstallers )

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
SET(CPACK_PACKAGE_VERSION_MINOR "6")
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

endmacro()
