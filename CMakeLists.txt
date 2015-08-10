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

include( CMake/Dependencies/OGRE.cmake )
include( CMake/Dependencies/wxWidgets.cmake )
include( CMake/Install.cmake )

setupOgre( OGRE_SOURCE, OGRE_BINARIES, OGRE_LIBRARIES )
setupWxWidgets( wxWidgets_LIBRARIES GTK2_LIBRARIES OPENGL_LIBRARIES )

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
	# Add embedded icon
	set( SOURCES ${SOURCES} ./scripts/Resources/Resource.rc )
endif()

add_executable( ${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS} )
target_link_libraries( ${PROJECT_NAME} ${OGRE_LIBRARIES} ${wxWidgets_LIBRARIES} )

if( UNIX )
	target_link_libraries( OgreMeshy ${GTK2_LIBRARIES} ${OPENGL_LIBRARIES} )
endif()

#Copy resources to bin folders
macro( copy_resources BUILD_TYPE )
	file( REMOVE_RECURSE "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}/Resources" )
	file( COPY "./scripts/Resources/Fonts" DESTINATION "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}/Resources" )
	file( COPY "./scripts/Resources/Icons" DESTINATION "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}/Resources" )
	file( COPY "./scripts/Resources/Other" DESTINATION "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}/Resources" )
	file( RENAME "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}/Resources/Other" "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}/Resources/Models" )
	file( COPY "./scripts/Resources/Blender/Axis.material"			 DESTINATION "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}/Resources/Models" )
	file( COPY "./scripts/Resources/Blender/Axis.mesh"				 DESTINATION "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}/Resources/Models" )
	file( COPY "./scripts/Resources/Blender/Bones/BoneMesh.material" DESTINATION "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}/Resources/Models" )
	file( COPY "./scripts/Resources/Blender/Bones/Bones.png"		 DESTINATION "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}/Resources/Models" )
	file( COPY "./scripts/Resources/Blender/Bones/BoneTip.mesh"		 DESTINATION "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}/Resources/Models" )
	file( COPY "./scripts/Resources/Blender/Bones/BoneGlobe.mesh"	 DESTINATION "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}/Resources/Models" )
	if( USE_RTSS )
		file( COPY "${OGRE_SOURCE}/Samples/Media/RTShaderLib"		 DESTINATION "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}/Resources" )
	endif()

	if( UNIX )
		file( COPY "./scripts/Resources/OgreIcon.ico" DESTINATION "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE}/Resources/" )
	endif()
endmacro()

copy_resources( "Debug" )
copy_resources( "Release" )
copy_resources( "RelWithDebInfo" )
copy_resources( "MinSizeRel" )

setupInstallers()
