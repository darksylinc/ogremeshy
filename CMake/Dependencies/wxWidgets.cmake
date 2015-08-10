#-------------------------------------------------------------------
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

macro( setupWxWidgets wxWidgets_LIBRARIES GTK2_LIBRARIES OPENGL_LIBRARIES )

if( WIN32 )
	set( wxWidgets_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/wxWidgets_3_0" CACHE STRING "Path to wxWidgets 3.0.0 source code" )
	set( wxWidgets_INCLUDE_DIRS "${wxWidgets_SOURCE}/include" )
	include_directories( "${wxWidgets_INCLUDE_DIRS}" )
		# Technically we should include mswu & mswud but we assume they're equal.
		# (CMake can't do per target includes... grr....)
		include_directories( "${wxWidgets_SOURCE}/lib/vc_lib/mswu" )
		link_directories( "${wxWidgets_SOURCE}/lib/vc_lib" )

	set( wxWidgets_LIBRARIES
		debug wxbase30ud
		debug wxmsw30ud_core
		debug wxmsw30ud_adv
		debug wxmsw30ud_aui
		debug wxpngd
		debug wxzlibd

		optimized wxbase30u
		optimized wxmsw30u_core
		optimized wxmsw30u_adv
		optimized wxmsw30u_aui
		optimized wxpng
		optimized wxzlib

		comctl32 Rpcrt4
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

endmacro()
