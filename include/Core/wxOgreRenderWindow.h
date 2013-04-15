
#ifndef WXOGRERENDERWINDOW_H
#define WXOGRERENDERWINDOW_H
 
#include "Ogre.h"
//#include "OgreNoMemoryMacros.h"
#include "wx/wx.h"

class wxOgreRenderWindowListener;

//#include "OgreMemoryMacros.h"
 
/** wxWidgets Ogre render window widget.
	Strongly based on the existing wxOgre widget implementation, this one
	isolates the wx component from Ogre, acting as a simple bind to render
	inside a wxWidgets window.
 
	@author Jesús Alonso Abad 'Kencho', Other contributors (original wxOgre).
 */
class wxOgreRenderWindow : public wxControl {
	DECLARE_CLASS (wxOgreRenderWindow)
	DECLARE_EVENT_TABLE ()
protected:
	/// A shared reference to the Ogre root.
	static Ogre::Root *msOgreRoot;

	/// This control's own render window reference.
	Ogre::RenderWindow *mRenderWindow;

	/// Timer to sync the rendering to a "constant" frame rate.
	wxTimer *mRenderTimer;

	/// The Id of the next render window
	static unsigned int msNextRenderWindowId;

	// Registered callbacks
	/// Callback for mouse events.
	wxOgreRenderWindowListener *mRenderWindowListener;

	// Methods ---------------------------------------------------------------------
public:
	/** wx-like Constructor.
	@param parent The parent wxWindow component.
	@param id The control id.
	@param pos The default position.
	@param size The default size.
	@param style The default style for this component.
	@param validator A default validator for the component.
	*/
	wxOgreRenderWindow (wxWindow *parent, wxWindowID id,
		const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize,
		long style = wxSUNKEN_BORDER, const wxValidator &validator = wxDefaultValidator);

	/** Default constructor.
	Allows the "standard" wxWidgets' two-step construction.
	*/
	wxOgreRenderWindow ();

	/** Creation method (for the two-step construction).
	@param parent The parent wxWindow component.
	@param id The control id.
	@param pos The default position.
	@param size The default size.
	@param style The default style for this component.
	@param validator A default validator for the component.
	*/
	bool Create (wxWindow *parent, wxWindowID id,
		const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize,
		long style = wxSUNKEN_BORDER, const wxValidator &validator = wxDefaultValidator);

	/** Virtual destructor.
	*/
	virtual ~wxOgreRenderWindow ();

	/** Initialisation method.
	*/
	virtual void Init ();

	/** Overrides the default implementation.
	This override is here for convenience. Returns a symbolic 320x240px size.
	@return A size of 320x240 (just a symbolic 4:3 size).
	*/
	virtual wxSize DoGetBestSize () const;

	/** Gets the current Ogre root reference associated.
	@return The current reference to Ogre's Root.
	*/
	static Ogre::Root *GetOgreRoot ();

	/** Sets the Ogre root for all the wxOgreRenderWindow instances.
	@param root The new OgreRoot.
	*/
	static void SetOgreRoot (Ogre::Root *root);

	/** Gets the associated Ogre render window.
	@return The render window used to paint this control.
	*/
	Ogre::RenderWindow *GetRenderWindow () const;

	/** Sets the render timer period.
	@param period The number of milliseconds before the next notification.
	A negative or zero value will stop the timer.
	*/
	void SetRenderTimerPeriod (int period);

	/** Sets the mouse events callback.
	@param callback The callback function.
	*/
	void setRenderWindowListener( wxOgreRenderWindowListener *listener);

	/** Updating function.
	*/
	virtual void Update ();

	/** Painting event callback.
	@param evt Data regarding the painting event.
	*/
	virtual void OnPaint (wxPaintEvent &evt);

	/** Render timer event callback.
	@param evt Data regarding the timer event.
	*/
	virtual void OnRenderTimer (wxTimerEvent &evt);

	/** Resizing events callback.
	@param evt Data regarding the resize event.
	*/
	virtual void OnSize (wxSizeEvent &evt);

	/** Mouse events callback.
	@remarks Note this will call the specified callback function to process
	the event.
	@param evt Data regarding the mouse event.
	*/
	virtual void OnMouseEvents (wxMouseEvent &evt);

	//Overload because wxAUI sets the size to 1x1 multiple times. Doesn't always work though.
	virtual wxSize GetMinSize() const { return wxSize(256, 256); }

protected:
	/** Creates an Ogre render window for this widget.
	*/
	virtual void CreateRenderWindow ();

	/** Gets the handle for the render window.
	@return The render window handle.
	*/
	virtual Ogre::String GetOgreHandle () const;

};

#endif // WXOGRERENDERWINDOW_H
