#-------------------------------------------------------------------
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

macro( findPluginAndSetPath BUILD_TYPE CFG_VARIABLE LIBRARY_NAME )
	set( REAL_LIB_PATH ${LIBRARY_NAME} )
	if( ${BUILD_TYPE} STREQUAL "Debug" )
		set( REAL_LIB_PATH ${REAL_LIB_PATH}_d )
	endif()

	if( WIN32 )
		set( REAL_LIB_PATH "${OGRE_BINARIES}/bin/${BUILD_TYPE}/${REAL_LIB_PATH}.dll" )
	else()
		set( REAL_LIB_PATH "${OGRE_BINARIES}/lib/${REAL_LIB_PATH}.so" )
	endif()

	if( EXISTS ${REAL_LIB_PATH} )
		# DLL Exists, set the variable for Plugins.cfg
		if( ${BUILD_TYPE} STREQUAL "Debug" )
			set( ${CFG_VARIABLE} "Plugin=${LIBRARY_NAME}_d" )
		else()
			set( ${CFG_VARIABLE} "Plugin=${LIBRARY_NAME}" )
		endif()

		# On Windows, copy the DLLs to the folders.
		if( WIN32 )
			file( COPY ${REAL_LIB_PATH} DESTINATION "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}/Plugins" )
		endif()
	endif()
endmacro()

macro( setupPluginFileFromTemplate BUILD_TYPE )
	if( NOT UNIX )
		file( MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}/Plugins" )
	endif()

	findPluginAndSetPath( ${BUILD_TYPE} OGRE_PLUGIN_RS_D3D9		RenderSystem_Direct3D9 )
	#findPluginAndSetPath( ${BUILD_TYPE} OGRE_PLUGIN_RS_D3D11	RenderSystem_Direct3D11 )
	findPluginAndSetPath( ${BUILD_TYPE} OGRE_PLUGIN_RS_GL		RenderSystem_GL )
	#findPluginAndSetPath( ${BUILD_TYPE} OGRE_PLUGIN_RS_GL3PLUS	RenderSystem_GL3Plus )
	findPluginAndSetPath( ${BUILD_TYPE} OGRE_PLUGIN_CG			Plugin_CgProgramManager )
	findPluginAndSetPath( ${BUILD_TYPE} OGRE_PLUGIN_PARTICLEFX	Plugin_ParticleFX )

	configure_file( ${CMAKE_SOURCE_DIR}/CMake/Templates/Plugins.cfg.in ${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}/Plugins.cfg )

	if( WIN32 )
		# Copy standard DLLs
		if( EXISTS "${OGRE_BINARIES}/bin/${BUILD_TYPE}/cg.dll" )
			file( COPY "${OGRE_BINARIES}/bin/${BUILD_TYPE}/cg.dll" DESTINATION "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}" )
		elseif( NOT OGRE_PLUGIN_CG STREQUAL "" )
			message( "Cg Plugin detected, but cg.dll not found in ${OGRE_BINARIES}/bin/${BUILD_TYPE}" )
			message( "The exe may work, but may not in other systems if this file is not installed." )
			message( "" )
		endif()

		if( NOT ${BUILD_TYPE} STREQUAL "Debug" )
			if( EXISTS "${OGRE_BINARIES}/bin/${BUILD_TYPE}/OgreMain.dll" )
				file( COPY "${OGRE_BINARIES}/bin/${BUILD_TYPE}/OgreMain.dll"	DESTINATION "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}" )
				file( COPY "${OGRE_BINARIES}/bin/${BUILD_TYPE}/OgreOverlay.dll"	DESTINATION "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}" )
				if( USE_RTSS )
					file( COPY "${OGRE_BINARIES}/bin/${BUILD_TYPE}/OgreRTShaderSystem.dll"	DESTINATION "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}" )
				endif()
			endif()
		else()
			if( EXISTS "${OGRE_BINARIES}/bin/${BUILD_TYPE}/OgreMain_d.dll" )
				file( COPY "${OGRE_BINARIES}/bin/${BUILD_TYPE}/OgreMain_d.dll"		DESTINATION "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}" )
				file( COPY "${OGRE_BINARIES}/bin/${BUILD_TYPE}/OgreOverlay_d.dll"	DESTINATION "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}" )
				if( USE_RTSS )
					file( COPY "${OGRE_BINARIES}/bin/${BUILD_TYPE}/OgreRTShaderSystem_d.dll"	DESTINATION "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}" )
				endif()
			endif()
		endif()
	endif()
	
	unset( OGRE_PLUGIN_RS_D3D9 )
	unset( OGRE_PLUGIN_RS_D3D11 )
	unset( OGRE_PLUGIN_RS_GL )
	unset( OGRE_PLUGIN_RS_GL3PLUS )
	unset( OGRE_PLUGIN_CG )
	unset( OGRE_PLUGIN_PARTICLEFX )
endmacro()

macro( setupOgre OGRE_SOURCE, OGRE_BINARIES, OGRE_LIBRARIES )

# Guess the paths.
set( OGRE_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/Ogre" CACHE STRING "Path to OGRE source code (see http://www.ogre3d.org/tikiwiki/tiki-index.php?page=CMake+Quick+Start+Guide)" )
if( WIN32 )
	set( OGRE_BINARIES "${OGRE_SOURCE}/build" CACHE STRING "Path to OGRE's build folder generated by CMake" )
	link_directories( "${OGRE_BINARIES}/lib/$(ConfigurationName)" )
else()
	set( OGRE_BINARIES "${OGRE_SOURCE}/build/${CMAKE_BUILD_TYPE}" CACHE STRING "Path to OGRE's build folder generated by CMake" )
	link_directories( "${OGRE_BINARIES}/lib" )
endif()

if( WIN32 )
	option( USE_RTSS "Compile with RTSS material generator (requires Cg plugin). Disable it if you're having issues or crashes." ON )
else()
	option( USE_RTSS "Compile with RTSS material generator (requires Cg plugin). Disable it if you're having issues or crashes." OFF )
endif()

# Ogre config
include_directories( "${OGRE_SOURCE}/OgreMain/include" )

# Ogre includes
include_directories( "${OGRE_BINARIES}/include" )
#include_directories( "${OGRE_SOURCE}/Components/Hlms/Common/include" )
#include_directories( "${OGRE_SOURCE}/Components/Hlms/Unlit/include" )
#include_directories( "${OGRE_SOURCE}/Components/Hlms/Pbs/include" )
include_directories( "${OGRE_SOURCE}/Components/Overlay/include" )

set( OGRE_LIBRARIES
	debug OgreMain_d
	debug OgreOverlay_d
	#debug OgreHlmsUnlit_d
	#debug OgreHlmsPbs_d

	optimized OgreMain
	optimized OgreOverlay
	#optimized OgreHlmsUnlit
	#optimized OgreHlmsPbs
	)

if( USE_RTSS )
	include_directories( "${OGRE_SOURCE}/Components/RTShaderSystem/include" )
	list( APPEND OGRE_LIBRARIES
		debug OgreRTShaderSystem_d
		optimized OgreRTShaderSystem
		)
	add_definitions( -DMESHY_USE_RTSS )
endif()

# Plugins.cfg
if( UNIX )
	set( OGRE_PLUGIN_DIR "${OGRE_BINARIES}/lib" )
else()
	set( OGRE_PLUGIN_DIR "Plugins" )
endif()

message( STATUS "Copying DLLs and generating Plugins.cfg for Debug" )
setupPluginFileFromTemplate( "Debug" )
message( STATUS "Copying DLLs and generating Plugins.cfg for Release" )
setupPluginFileFromTemplate( "Release" )
message( STATUS "Copying DLLs and generating Plugins.cfg for RelWithDebInfo" )
setupPluginFileFromTemplate( "RelWithDebInfo" )
message( STATUS "Copying DLLs and generating Plugins.cfg for MinSizeRel" )
setupPluginFileFromTemplate( "MinSizeRel" )

endmacro()
