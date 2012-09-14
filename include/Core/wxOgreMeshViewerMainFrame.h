///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version May  4 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __wxOgreMeshViewerMainFrame__
#define __wxOgreMeshViewerMainFrame__

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/toolbar.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/choice.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/radiobox.h>
#include <wx/statbox.h>
#include <wx/treectrl.h>
#include <wx/panel.h>
#include <wx/statline.h>
#include <wx/spinctrl.h>
#include <wx/scrolwin.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame 
{
	private:
	
	protected:
		enum
		{
			wxID_MENUFILELOAD = 1000,
			wxID_MENURELOADMESH,
			wxID_MENULOADRESCFG,
			wxID_MENULOADLASTRESCFG,
			wxID_MENUSAVESNAPSHOT,
			wxID_MENUFILEEXIT,
			wxID_MENUVIEWSHOWAXES,
			wxID_MENUSHOWBONES,
			wxID_MENUSHOWBOUNDINGBOX,
			wxID_MENUSHOWGRID,
			wxID_MENUTOGGLEWIREFRAME,
			wxID_MENUSHOWBONENAMES,
			wxID_MENUCHANGENAMECOLOUR,
			wxID_MENUDEFAULTCAMPOS,
			wxID_MENUCAMERAORIGIN,
			wxID_MENUCAMCENTERMESH,
			wxID_MENUCAMFREELOOK,
			wxID_MENUCHANGEBGCOLOUR,
			wxID_MENUGRIDSETTINGS,
			wxID_MENUVIEWLOG,
			wxID_MENUVIEWRENDERWINDOW,
			wxID_MENUABOUTABOUT,
		};
		
		wxMenuBar* m_menuBar;
		wxMenu* m_menuFile;
		wxMenu* m_menuView;
		wxMenu* m_boneSubmenu;
		wxMenu* m_menu1;
		wxMenu* m_menuAbout;
		wxToolBar* m_toolBar;
		wxStatusBar* m_statusBar1;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnMenuSelected( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Ogre Meshy"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1000,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~MainFrame();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class wxAnimationPanel
///////////////////////////////////////////////////////////////////////////////
class wxAnimationPanel : public wxPanel 
{
	private:
	
	protected:
		enum
		{
			wxID_ANIMATIONTIMEVALUE = 1000,
			wxID_ANIMATIONTIMESLIDER,
			wxID_PLAYSKELANIMATION,
			wxID_PAUSESKELANIMATION,
			wxID_STOPSKELANIMATION,
		};
		
		wxChoice* m_animations;
		wxStaticText* m_staticText1;
		wxTextCtrl* m_animationTimeValue;
		
		wxStaticText* m_staticText3;
		wxStaticText* m_animationLengthText;
		wxSlider* m_animationTimeSlider;
		wxButton* m_playSkelAnimation;
		wxStaticText* m_staticText2;
		wxTextCtrl* m_rateSkelAnim;
		
		wxButton* m_pauseSkelAnimation;
		wxButton* m_StopSkelAnimation;
		wxCheckBox* m_animLoopCheckBox;
		wxRadioBox* m_radioInterpolationMode;
		wxButton* m_buttonGenAnimReport;
		wxSlider* m_sliderBoneSize;
		wxTreeCtrl* m_boneTree;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnChangedAnimationName( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAnimationChangeTime( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAnimationChangeTimeScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void OnButtonPlaybackSkelAnim( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAnimationRateChange( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLoopAnimation( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRadioInterpolationMode( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBoneSizeChange( wxScrollEvent& event ) { event.Skip(); }
		virtual void OnNodeTrackSelect( wxTreeEvent& event ) { event.Skip(); }
		
	
	public:
		
		wxAnimationPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 478,484 ), long style = wxTAB_TRAVERSAL );
		~wxAnimationPanel();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class wxLightsPanel
///////////////////////////////////////////////////////////////////////////////
class wxLightsPanel : public wxPanel 
{
	private:
	
	protected:
		enum
		{
			wxID_ADDLIGHT = 1000,
			wxID_REMOVELIGHT,
			wxID_DIRNORMALIZE,
			wxID_PICKDIFFUSE,
			wxID_PICKSPECULAR,
		};
		
		wxScrolledWindow* m_scrolledWindow1;
		wxChoice* m_lightsChoice;
		wxButton* m_button7;
		wxButton* m_button10;
		wxStaticLine* m_staticline1;
		wxStaticText* m_staticText9;
		wxChoice* m_choiceLightType;
		wxTextCtrl* m_lightPosX;
		wxTextCtrl* m_lightPosY;
		wxTextCtrl* m_lightPosZ;
		wxTextCtrl* m_lightDirX;
		wxTextCtrl* m_lightDirY;
		wxTextCtrl* m_lightDirZ;
		wxButton* m_button11;
		wxTextCtrl* m_textDiffuseColour;
		wxButton* m_diffusePickColour;
		wxSpinCtrl* m_spinPower;
		wxTextCtrl* m_textSpecularColour;
		wxButton* m_specularPickColour;
		wxStaticText* m_staticText10;
		wxTextCtrl* m_textRange;
		wxStaticText* m_staticText11;
		wxTextCtrl* m_textQuadratic;
		wxStaticText* m_staticText12;
		wxTextCtrl* m_textLinear;
		wxStaticText* m_staticText13;
		wxTextCtrl* m_textConstant;
		wxStaticText* m_staticText14;
		wxStaticText* m_staticText15;
		wxStaticText* m_staticText16;
		wxTextCtrl* m_textInnerRadius;
		wxTextCtrl* m_textOuterRadius;
		wxTextCtrl* m_textFalloff;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnLightSelectionChange( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLightTypeChange( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		wxLightsPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 353,616 ), long style = wxTAB_TRAVERSAL );
		~wxLightsPanel();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class GridSettings
///////////////////////////////////////////////////////////////////////////////
class GridSettings : public wxDialog 
{
	private:
	
	protected:
		enum
		{
			wxID_GRIDWIDTH = 1000,
			wxID_GRIDDEPTH,
		};
		
		wxStaticText* m_staticText5;
		wxChoice* m_presetsChoice;
		wxStaticText* m_staticText6;
		wxTextCtrl* m_gridCellUnit;
		wxStaticText* m_staticText7;
		wxTextCtrl* m_gridWidth;
		wxStaticText* m_staticText8;
		wxTextCtrl* m_gridDepth;
		
		wxButton* m_OK;
		wxButton* m_button6;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnPresetsChange( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCommitChanges( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		GridSettings( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Grid settings..."), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 243,185 ), long style = wxDEFAULT_DIALOG_STYLE );
		~GridSettings();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class wxAnimPosePanel
///////////////////////////////////////////////////////////////////////////////
class wxAnimPosePanel : public wxPanel 
{
	private:
	
	protected:
		enum
		{
			wxID_ANIMATIONTIMEVALUE = 1000,
			wxID_ANIMATIONTIMESLIDER,
			wxID_PLAYANIMATION,
			wxID_PAUSEANIMATION,
			wxID_STOPANIMATION,
		};
		
		wxChoice* m_animations;
		wxStaticText* m_staticText1;
		wxTextCtrl* m_animationTimeValue;
		
		wxStaticText* m_staticText3;
		wxStaticText* m_animationLengthText;
		wxSlider* m_animationTimeSlider;
		wxButton* m_playSkelAnimation;
		wxStaticText* m_staticText2;
		wxTextCtrl* m_rateSkelAnim;
		
		wxButton* m_pauseSkelAnimation;
		wxButton* m_StopSkelAnimation;
		wxCheckBox* m_animLoopCheckBox;
		wxScrolledWindow* m_scrolledPosePanel;
		wxBoxSizer* m_scrollWindowSizer;
		wxButton* m_resetWeights;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnChangedAnimationName( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAnimationChangeTime( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAnimationChangeTimeSlider( wxScrollEvent& event ) { event.Skip(); }
		virtual void OnButtonPlaybackAnim( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAnimationRateChange( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLoopAnimation( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnResetWeights( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		wxAnimPosePanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 478,484 ), long style = wxTAB_TRAVERSAL );
		~wxAnimPosePanel();
	
};

#endif //__wxOgreMeshViewerMainFrame__
