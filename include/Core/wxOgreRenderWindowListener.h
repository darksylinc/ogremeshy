
#ifndef __wxOgreRenderWindowListener_defined__
#define __wxOgreRenderWindowListener_defined__

class wxMouseEvent;

class wxOgreRenderWindowListener
{
public:
	virtual void OnMouseEvents( wxMouseEvent &evt ) = 0;
};

#endif
