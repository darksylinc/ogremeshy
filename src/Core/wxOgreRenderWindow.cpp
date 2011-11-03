#include "Core/wxOgreRenderWindow.h"
#include "Core/wxOgreRenderWindowListener.h"

#ifdef __WXGTK__
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <wx/gtk/win_gtk.h>
#include <GL/glx.h>
#endif

const wxWindowID ID_RENDERTIMER = wxNewId ();

IMPLEMENT_CLASS (wxOgreRenderWindow, wxControl)

BEGIN_EVENT_TABLE (wxOgreRenderWindow, wxControl)
#ifndef __WXMSW__
EVT_PAINT (wxOgreRenderWindow::OnPaint)
#endif
EVT_TIMER (ID_RENDERTIMER, wxOgreRenderWindow::OnRenderTimer)
EVT_SIZE (wxOgreRenderWindow::OnSize)
EVT_MOUSE_EVENTS (wxOgreRenderWindow::OnMouseEvents)
END_EVENT_TABLE ()

Ogre::Root *wxOgreRenderWindow::msOgreRoot = 0;
//------------------------------------------------------------------------------
unsigned int wxOgreRenderWindow::msNextRenderWindowId = 1;
//------------------------------------------------------------------------------
wxOgreRenderWindow::wxOgreRenderWindow (wxWindow *parent, wxWindowID id,
										const wxPoint &pos, const wxSize &size, long style, const wxValidator &validator)
{
											Init ();
											Create (parent, id, pos, size, style, validator);
}
//------------------------------------------------------------------------------
wxOgreRenderWindow::wxOgreRenderWindow ()
{
	Init ();
}
//------------------------------------------------------------------------------
bool wxOgreRenderWindow::Create( wxWindow *parent, wxWindowID id,
								  const wxPoint &pos, const wxSize &size, long style,
									const wxValidator &validator)
{
	 // Error creating the base class
	 if (!wxControl::Create (parent, id, pos, size, style, validator))
		 return false;

	 // Use the told size or let the sizers pick one.
	 SetInitialSize (size);

	 SetBackgroundStyle(wxBG_STYLE_CUSTOM);

	 if (msOgreRoot)
		 CreateRenderWindow ();

	 return true;
}
//------------------------------------------------------------------------------
wxOgreRenderWindow::~wxOgreRenderWindow ()
{
	/*if (mRenderWindow && msOgreRoot)
		msOgreRoot->detachRenderTarget (mRenderWindow);*/

	mRenderWindow = 0;

	if (mRenderTimer)
		delete mRenderTimer;
}
//------------------------------------------------------------------------------
void wxOgreRenderWindow::Init ()
{
	mRenderWindow = 0;

	// Callbacks
	mRenderWindowListener = 0;

	mRenderTimer = new wxTimer (this, ID_RENDERTIMER);
	mRenderTimer->Start( 10 );
}
//------------------------------------------------------------------------------
inline wxSize wxOgreRenderWindow::DoGetBestSize () const
{
	//return wxSize( -1, -1 );
	return wxSize (512, 512);
}
//------------------------------------------------------------------------------
Ogre::Root *wxOgreRenderWindow::GetOgreRoot ()
{
	return msOgreRoot;
}
//------------------------------------------------------------------------------
void wxOgreRenderWindow::SetOgreRoot (Ogre::Root *root)
{
	msOgreRoot = root;
}
//------------------------------------------------------------------------------
Ogre::RenderWindow *wxOgreRenderWindow::GetRenderWindow () const
{
	return mRenderWindow;
}
//------------------------------------------------------------------------------
void wxOgreRenderWindow::SetRenderTimerPeriod (int period)
{
	if (!mRenderTimer)
		return;

	if (period <= 0)
		mRenderTimer->Stop ();
	else
		mRenderTimer->Start (period);
}
//------------------------------------------------------------------------------
void wxOgreRenderWindow::Update ()
{
	if (msOgreRoot)
	{
		msOgreRoot->renderOneFrame ();

		//Without this in Linux you'll get a black window, don't know why
		if( mRenderWindow )
			mRenderWindow->update();
	}
}
//------------------------------------------------------------------------------
void wxOgreRenderWindow::setRenderWindowListener(wxOgreRenderWindowListener *listener)
{
	mRenderWindowListener = listener;
}
//------------------------------------------------------------------------------
void wxOgreRenderWindow::OnPaint (wxPaintEvent &evt)
{
	Update ();
}
//------------------------------------------------------------------------------
void wxOgreRenderWindow::OnRenderTimer (wxTimerEvent &evt)
{
	Update ();
}
//------------------------------------------------------------------------------
void wxOgreRenderWindow::OnSize (wxSizeEvent &evt)
{
	if (mRenderWindow)
{
		// Setting new size;
		int width;
		int height;
		wxSize size = evt.GetSize ();
		width = size.GetWidth ();
		height = size.GetHeight ();

		mRenderWindow->resize (width, height);
		// Letting Ogre know the window has been resized;
		mRenderWindow->windowMovedOrResized ();

		Ogre::Viewport *vp = mRenderWindow->getViewport(0);
		if( vp )
		{
			vp->getCamera()->setAspectRatio( Ogre::Real( vp->getActualWidth() ) /
												Ogre::Real( vp->getActualHeight() ) );
		}
	}

	Update ();
}
//------------------------------------------------------------------------------
void wxOgreRenderWindow::OnMouseEvents (wxMouseEvent &evt)
{
	if (mRenderWindowListener)
		mRenderWindowListener->OnMouseEvents( evt );
}
//------------------------------------------------------------------------------
void wxOgreRenderWindow::CreateRenderWindow ()
{
	Ogre::NameValuePairList params;
#ifdef __WXMSW__
	params["externalWindowHandle"] = GetOgreHandle ();
#elif defined(__WXGTK__)
	params["parentWindowHandle"] = GetOgreHandle ();
#endif
	//params["useNVPerfHUD"] = "Yes";

#ifdef __WXMSW__
	//Enforce vsync only in Windows. In Linux the GUI becomes waaaaaaay laggy
	params["vsync"] = "true"; 
#endif

	// Get wx control window size
	int width;
	int height;
	GetSize (&width, &height);
	// Create the render window
	mRenderWindow = Ogre::Root::getSingleton ().createRenderWindow (
		Ogre::String ("OgreRenderWindow") + Ogre::StringConverter::toString (msNextRenderWindowId++),
		width, height, false, &params);

	//We update manually because in Linux it's broken
	mRenderWindow->setAutoUpdated( false );

	mRenderWindow->setActive (true);
}
//------------------------------------------------------------------------------
Ogre::String wxOgreRenderWindow::GetOgreHandle () const
{
	Ogre::String handle;

#ifdef __WXMSW__
	// Handle for Windows systems
	handle = Ogre::StringConverter::toString((size_t)((HWND)GetHandle()));
#elif defined(__WXGTK__)
	// Handle for GTK-based systems

	GtkWidget *widget = m_wxwindow;
	gtk_widget_set_double_buffered (widget, FALSE);
	gtk_widget_realize( widget );

	// Grab the window object
	GdkWindow *gdkWin = GTK_PIZZA (widget)->bin_window;
	Display* display = GDK_WINDOW_XDISPLAY(gdkWin);
	Window wid = GDK_WINDOW_XWINDOW(gdkWin);

	XSync( display, false );		//Added by Dark Sylinc
	XMapWindow( display, wid );		//Added by Dark Sylinc 

	std::stringstream str;

	// Display
	/*Deprecated externalWindowHandle
	str << (unsigned long)display << ':';

	// Screen (returns "display.screen")
	std::string screenStr = DisplayString(display);
	std::string::size_type dotPos = screenStr.find(".");
	screenStr = screenStr.substr(dotPos+1, screenStr.size());
	str << screenStr << ':';

	// XID
	str << wid << ':';

	// Retrieve XVisualInfo
	int attrlist[] = { GLX_RGBA, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, 16, GLX_STENCIL_SIZE, 8, None };
	XVisualInfo* vi = glXChooseVisual(display, DefaultScreen(display), attrlist);
	str << (unsigned long)vi;*/

	//parentWindowHandle just uses the XID
	str << wid;

	handle = str.str();
#else
	// Any other unsupported system
#error Not supported on this platform.
#endif

	return handle;
}
