///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version May  4 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "Core/wxOgreMeshViewerMainFrame.h"

///////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_menuBar = new wxMenuBar( 0 );
	m_menuFile = new wxMenu();
	wxMenuItem* m_menuFileLoad;
	m_menuFileLoad = new wxMenuItem( m_menuFile, wxID_MENUFILELOAD, wxString( wxT("Load...") ) + wxT('\t') + wxT("Ctrl+O"), wxT("Loads a new mesh"), wxITEM_NORMAL );
	m_menuFile->Append( m_menuFileLoad );
	
	wxMenuItem* m_menuReloadMesh;
	m_menuReloadMesh = new wxMenuItem( m_menuFile, wxID_MENURELOADMESH, wxString( wxT("Reload mesh") ) + wxT('\t') + wxT("F5"), wxT("Reloads active mesh from hard drive"), wxITEM_NORMAL );
	m_menuFile->Append( m_menuReloadMesh );
	m_menuReloadMesh->Enable( false );
	
	wxMenuItem* m_separator2;
	m_separator2 = m_menuFile->AppendSeparator();
	
	wxMenuItem* m_menuLoadResCfg;
	m_menuLoadResCfg = new wxMenuItem( m_menuFile, wxID_MENULOADRESCFG, wxString( wxT("Load resources.cfg...") ) + wxT('\t') + wxT("Alt+F"), wxT("Parses a resource.cfg file to load materials and shaders (previously loaded resources will be unloaded)"), wxITEM_NORMAL );
	m_menuFile->Append( m_menuLoadResCfg );
	
	wxMenuItem* m_menuLoadLastResCfg;
	m_menuLoadLastResCfg = new wxMenuItem( m_menuFile, wxID_MENULOADLASTRESCFG, wxString( wxT("Load last resource.cfg") ) + wxT('\t') + wxT("F8"), wxT("(Re)loads the last resource.cfg that was selected"), wxITEM_NORMAL );
	m_menuFile->Append( m_menuLoadLastResCfg );
	
	wxMenuItem* m_separator3;
	m_separator3 = m_menuFile->AppendSeparator();
	
	wxMenuItem* m_saveSnapshot;
	m_saveSnapshot = new wxMenuItem( m_menuFile, wxID_MENUSAVESNAPSHOT, wxString( wxT("Save Snapshot") ) + wxT('\t') + wxT("F12"), wxT("Takes a snapshot of the viewport and saves it under the MyDocuments/OgreMeshy directory (Linux users: Home directory)"), wxITEM_NORMAL );
	m_menuFile->Append( m_saveSnapshot );
	
	wxMenuItem* m_separator5;
	m_separator5 = m_menuFile->AppendSeparator();
	
	wxMenuItem* m_menuFileExit;
	m_menuFileExit = new wxMenuItem( m_menuFile, wxID_MENUFILEEXIT, wxString( wxT("Exit") ) , wxT("Closes the application"), wxITEM_NORMAL );
	m_menuFile->Append( m_menuFileExit );
	
	m_menuBar->Append( m_menuFile, wxT("File") ); 
	
	m_menuView = new wxMenu();
	wxMenuItem* m_menuShowAxes;
	m_menuShowAxes = new wxMenuItem( m_menuView, wxID_MENUVIEWSHOWAXES, wxString( wxT("Show axes") ) + wxT('\t') + wxT("Ctrl+A"), wxT("Shows XYZ axes"), wxITEM_CHECK );
	m_menuView->Append( m_menuShowAxes );
	
	wxMenuItem* m_menuShowBones;
	m_menuShowBones = new wxMenuItem( m_menuView, wxID_MENUSHOWBONES, wxString( wxT("Show Bones") ) + wxT('\t') + wxT("Ctrl+D"), wxT("Displays the skeleton nodes to view their orientation or spot errors"), wxITEM_CHECK );
	m_menuView->Append( m_menuShowBones );
	m_menuShowBones->Enable( false );
	
	wxMenuItem* m_menuShowBoundingBox;
	m_menuShowBoundingBox = new wxMenuItem( m_menuView, wxID_MENUSHOWBOUNDINGBOX, wxString( wxT("Show Bounding box") ) + wxT('\t') + wxT("Ctrl+B"), wxT("Shows or hides a box enclosing the mesh"), wxITEM_CHECK );
	m_menuView->Append( m_menuShowBoundingBox );
	
	wxMenuItem* m_menuShowGrid;
	m_menuShowGrid = new wxMenuItem( m_menuView, wxID_MENUSHOWGRID, wxString( wxT("Show Grid") ) + wxT('\t') + wxT("Ctrl+G"), wxT("Shows or hides the grid"), wxITEM_CHECK );
	m_menuView->Append( m_menuShowGrid );
	
	wxMenuItem* m_menuToggleWireframe;
	m_menuToggleWireframe = new wxMenuItem( m_menuView, wxID_MENUTOGGLEWIREFRAME, wxString( wxT("Toggle Wireframe") ) + wxT('\t') + wxT("Ctrl+W"), wxT("Turns wireframe mode on & off"), wxITEM_CHECK );
	m_menuView->Append( m_menuToggleWireframe );
	
	wxMenuItem* m_separator4;
	m_separator4 = m_menuView->AppendSeparator();
	
	m_boneSubmenu = new wxMenu();
	wxMenuItem* m_showBoneNames;
	m_showBoneNames = new wxMenuItem( m_boneSubmenu, wxID_MENUSHOWBONENAMES, wxString( wxT("Show Bone names") ) + wxT('\t') + wxT("Alt+D"), wxT("Displays the name on top of each bone. 'Show Bones' must be active"), wxITEM_CHECK );
	m_boneSubmenu->Append( m_showBoneNames );
	m_showBoneNames->Enable( false );
	
	wxMenuItem* m_menuItem21;
	m_menuItem21 = new wxMenuItem( m_boneSubmenu, wxID_MENUCHANGENAMECOLOUR, wxString( wxT("Change names colour...") ) , wxT("Changes the colour used to display the names above the bones. The font can't be changed."), wxITEM_NORMAL );
	m_boneSubmenu->Append( m_menuItem21 );
	
	m_menuView->Append( -1, wxT("Bones"), m_boneSubmenu );
	
	m_menu1 = new wxMenu();
	wxMenuItem* m_menuDefaultCamPos;
	m_menuDefaultCamPos = new wxMenuItem( m_menu1, wxID_MENUDEFAULTCAMPOS, wxString( wxT("Default Position") ) + wxT('\t') + wxT("Alt+1"), wxT("Puts the camera back to it's default position (0, centerY, 0)"), wxITEM_NORMAL );
	m_menu1->Append( m_menuDefaultCamPos );
	
	wxMenuItem* m_menuCamOrigin;
	m_menuCamOrigin = new wxMenuItem( m_menu1, wxID_MENUCAMERAORIGIN, wxString( wxT("Center Origin") ) + wxT('\t') + wxT("Alt+2"), wxT("Makes the camera look to the origin (0, 0, 0)"), wxITEM_NORMAL );
	m_menu1->Append( m_menuCamOrigin );
	
	wxMenuItem* m_menuCamCenterMesh;
	m_menuCamCenterMesh = new wxMenuItem( m_menu1, wxID_MENUCAMCENTERMESH, wxString( wxT("Center Mesh") ) + wxT('\t') + wxT("Alt+3"), wxT("Makes the camera point to the center of the mesh"), wxITEM_NORMAL );
	m_menu1->Append( m_menuCamCenterMesh );
	
	wxMenuItem* m_menuCamFreeLook;
	m_menuCamFreeLook = new wxMenuItem( m_menu1, wxID_MENUCAMFREELOOK, wxString( wxT("Free look mode") ) + wxT('\t') + wxT("Alt+4"), wxT("Puts the camera into free look mode"), wxITEM_CHECK );
	m_menu1->Append( m_menuCamFreeLook );
	m_menuCamFreeLook->Enable( false );
	
	m_menuView->Append( -1, wxT("Camera"), m_menu1 );
	
	wxMenuItem* m_menuBgColour;
	m_menuBgColour = new wxMenuItem( m_menuView, wxID_MENUCHANGEBGCOLOUR, wxString( wxT("Change background colour...") ) , wxT("Changes the (\"clear\") colour of the background"), wxITEM_NORMAL );
	m_menuView->Append( m_menuBgColour );
	
	wxMenuItem* m_menuGridSettings;
	m_menuGridSettings = new wxMenuItem( m_menuView, wxID_MENUGRIDSETTINGS, wxString( wxT("Grid settings...") ) , wxT("Configures the size and unit convention of the grid"), wxITEM_NORMAL );
	m_menuView->Append( m_menuGridSettings );
	
	wxMenuItem* m_separator1;
	m_separator1 = m_menuView->AppendSeparator();
	
	wxMenuItem* m_menuViewLog;
	m_menuViewLog = new wxMenuItem( m_menuView, wxID_MENUVIEWLOG, wxString( wxT("Show Log Window") ) , wxT("Shows OGRE log"), wxITEM_NORMAL );
	m_menuView->Append( m_menuViewLog );
	
	wxMenuItem* m_menuViewRenderW;
	m_menuViewRenderW = new wxMenuItem( m_menuView, wxID_MENUVIEWRENDERWINDOW, wxString( wxT("Show Render Window") ) , wxT("Shows OGRE's Render Window"), wxITEM_NORMAL );
	m_menuView->Append( m_menuViewRenderW );
	
	m_menuBar->Append( m_menuView, wxT("View") ); 
	
	m_menuAbout = new wxMenu();
	wxMenuItem* m_menuItem5;
	m_menuItem5 = new wxMenuItem( m_menuAbout, wxID_MENUABOUTABOUT, wxString( wxT("About") ) + wxT('\t') + wxT("F1"), wxEmptyString, wxITEM_NORMAL );
	m_menuAbout->Append( m_menuItem5 );
	
	m_menuBar->Append( m_menuAbout, wxT("About") ); 
	
	this->SetMenuBar( m_menuBar );
	
	m_toolBar = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY );
	m_toolBar->SetToolBitmapSize( wxSize( 32,32 ) );
	m_toolBar->AddTool( wxID_MENUFILELOAD, wxT("tool"), wxBitmap( wxT("Resources/Icons/32x32/File.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Loads a new mesh"), wxT("Loads a new mesh") ); 
	m_toolBar->AddTool( wxID_MENURELOADMESH, wxT("tool"), wxBitmap( wxT("Resources/Icons/32x32/Reload.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Reload mesh"), wxT("Reloads active mesh from hard drive") ); 
	m_toolBar->AddSeparator(); 
	m_toolBar->AddTool( wxID_MENUVIEWSHOWAXES, wxT("tool"), wxBitmap( wxT("Resources/Icons/32x32/ShowAxes.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_CHECK, wxT("Show axes"), wxT("Shows XYZ axes") ); 
	m_toolBar->AddTool( wxID_MENUSHOWBONES, wxT("tool"), wxBitmap( wxT("Resources/Icons/32x32/ShowBones.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_CHECK, wxT("Show Bones"), wxT("Displays the skeleton nodes to view their orientation or spot errors") ); 
	m_toolBar->AddTool( wxID_MENUSHOWBOUNDINGBOX, wxT("tool"), wxBitmap( wxT("Resources/Icons/32x32/BoundingBox.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_CHECK, wxT("Show Bounding Box"), wxT("Shows or hides a box enclosing the mesh") ); 
	m_toolBar->AddTool( wxID_MENUTOGGLEWIREFRAME, wxT("Toggle Wireframe"), wxBitmap( wxT("Resources/Icons/32x32/Wireframe.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_CHECK, wxT("Toggles Wireframe mode"), wxT("Turns wireframe mode on & off") ); 
	m_toolBar->AddTool( wxID_MENUCHANGEBGCOLOUR, wxT("tool"), wxBitmap( wxT("Resources/Icons/32x32/ChangeBGColour.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Change background colour"), wxT("Changes the (\"clear\") colour of the background") ); 
	m_toolBar->Realize();
	
	m_statusBar1 = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	// Connect Events
	this->Connect( wxID_MENUFILELOAD, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Connect( wxID_MENURELOADMESH, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Connect( wxID_MENULOADRESCFG, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Connect( wxID_MENULOADLASTRESCFG, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Connect( wxID_MENUSAVESNAPSHOT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Connect( wxID_MENUFILEEXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Connect( wxID_MENUVIEWSHOWAXES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Connect( wxID_MENUSHOWBONES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Connect( wxID_MENUSHOWBOUNDINGBOX, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Connect( wxID_MENUSHOWGRID, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Connect( wxID_MENUTOGGLEWIREFRAME, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Connect( wxID_MENUSHOWBONENAMES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Connect( wxID_MENUCHANGENAMECOLOUR, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Connect( wxID_MENUDEFAULTCAMPOS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Connect( wxID_MENUCAMERAORIGIN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Connect( wxID_MENUCAMCENTERMESH, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Connect( wxID_MENUCAMFREELOOK, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Connect( wxID_MENUCHANGEBGCOLOUR, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Connect( wxID_MENUGRIDSETTINGS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Connect( wxID_MENUVIEWLOG, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Connect( wxID_MENUVIEWRENDERWINDOW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Connect( wxID_MENUABOUTABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Connect( wxID_MENUFILELOAD, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Connect( wxID_MENURELOADMESH, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Connect( wxID_MENUVIEWSHOWAXES, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Connect( wxID_MENUSHOWBONES, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Connect( wxID_MENUSHOWBOUNDINGBOX, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Connect( wxID_MENUTOGGLEWIREFRAME, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Connect( wxID_MENUCHANGEBGCOLOUR, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	this->Disconnect( wxID_MENUFILELOAD, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Disconnect( wxID_MENURELOADMESH, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Disconnect( wxID_MENULOADRESCFG, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Disconnect( wxID_MENULOADLASTRESCFG, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Disconnect( wxID_MENUSAVESNAPSHOT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Disconnect( wxID_MENUFILEEXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Disconnect( wxID_MENUVIEWSHOWAXES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Disconnect( wxID_MENUSHOWBONES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Disconnect( wxID_MENUSHOWBOUNDINGBOX, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Disconnect( wxID_MENUSHOWGRID, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Disconnect( wxID_MENUTOGGLEWIREFRAME, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Disconnect( wxID_MENUSHOWBONENAMES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Disconnect( wxID_MENUCHANGENAMECOLOUR, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Disconnect( wxID_MENUDEFAULTCAMPOS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Disconnect( wxID_MENUCAMERAORIGIN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Disconnect( wxID_MENUCAMCENTERMESH, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Disconnect( wxID_MENUCAMFREELOOK, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Disconnect( wxID_MENUCHANGEBGCOLOUR, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Disconnect( wxID_MENUGRIDSETTINGS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Disconnect( wxID_MENUVIEWLOG, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Disconnect( wxID_MENUVIEWRENDERWINDOW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Disconnect( wxID_MENUABOUTABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Disconnect( wxID_MENUFILELOAD, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Disconnect( wxID_MENURELOADMESH, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Disconnect( wxID_MENUVIEWSHOWAXES, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Disconnect( wxID_MENUSHOWBONES, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Disconnect( wxID_MENUSHOWBOUNDINGBOX, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Disconnect( wxID_MENUTOGGLEWIREFRAME, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	this->Disconnect( wxID_MENUCHANGEBGCOLOUR, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::OnMenuSelected ) );
	
}

wxAnimationPanel::wxAnimationPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );
	
	wxString m_animationsChoices[] = { wxT("(None)") };
	int m_animationsNChoices = sizeof( m_animationsChoices ) / sizeof( wxString );
	m_animations = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_animationsNChoices, m_animationsChoices, 0 );
	m_animations->SetSelection( 0 );
	bSizer12->Add( m_animations, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Time: "), wxDefaultPosition, wxDefaultSize, 0|wxFULL_REPAINT_ON_RESIZE );
	m_staticText1->Wrap( -1 );
	bSizer15->Add( m_staticText1, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT, 5 );
	
	m_animationTimeValue = new wxTextCtrl( this, wxID_ANIMATIONTIMEVALUE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	bSizer15->Add( m_animationTimeValue, 0, wxALL, 5 );
	
	
	bSizer15->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("Length: "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer15->Add( m_staticText3, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	m_animationLengthText = new wxStaticText( this, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_animationLengthText->Wrap( -1 );
	m_animationLengthText->SetMinSize( wxSize( 50,-1 ) );
	
	bSizer15->Add( m_animationLengthText, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	bSizer13->Add( bSizer15, 0, wxEXPAND, 0 );
	
	m_animationTimeSlider = new wxSlider( this, wxID_ANIMATIONTIMESLIDER, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	bSizer13->Add( m_animationTimeSlider, 0, wxALIGN_CENTER|wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxHORIZONTAL );
	
	m_playSkelAnimation = new wxButton( this, wxID_PLAYSKELANIMATION, wxT("Play"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer18->Add( m_playSkelAnimation, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Rate"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer18->Add( m_staticText2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_rateSkelAnim = new wxTextCtrl( this, wxID_ANY, wxT("1.0"), wxDefaultPosition, wxSize( 40,-1 ), wxTE_PROCESS_ENTER );
	bSizer18->Add( m_rateSkelAnim, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	bSizer18->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_pauseSkelAnimation = new wxButton( this, wxID_PAUSESKELANIMATION, wxT("Pause"), wxPoint( -1,-1 ), wxDefaultSize, 0 );
	bSizer18->Add( m_pauseSkelAnimation, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	m_StopSkelAnimation = new wxButton( this, wxID_STOPSKELANIMATION, wxT("Stop"), wxPoint( -1,-1 ), wxDefaultSize, 0 );
	bSizer18->Add( m_StopSkelAnimation, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	bSizer13->Add( bSizer18, 0, wxEXPAND, 0 );
	
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxVERTICAL );
	
	bSizer20->SetMinSize( wxSize( 130,-1 ) ); 
	m_animLoopCheckBox = new wxCheckBox( this, wxID_ANY, wxT("Loop Animation"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( m_animLoopCheckBox, 0, wxALL, 5 );
	
	wxString m_radioInterpolationModeChoices[] = { wxT("Linear"), wxT("Spline") };
	int m_radioInterpolationModeNChoices = sizeof( m_radioInterpolationModeChoices ) / sizeof( wxString );
	m_radioInterpolationMode = new wxRadioBox( this, wxID_ANY, wxT("Interpolation Mode"), wxDefaultPosition, wxDefaultSize, m_radioInterpolationModeNChoices, m_radioInterpolationModeChoices, 1, wxRA_SPECIFY_COLS );
	m_radioInterpolationMode->SetSelection( 0 );
	bSizer20->Add( m_radioInterpolationMode, 0, wxALL, 5 );
	
	bSizer19->Add( bSizer20, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonGenAnimReport = new wxButton( this, wxID_ANY, wxT("Generate Detailed\nReport"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonGenAnimReport->Enable( false );
	
	bSizer8->Add( m_buttonGenAnimReport, 1, wxALL, 0 );
	
	bSizer9->Add( bSizer8, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Bone size:") ), wxVERTICAL );
	
	m_sliderBoneSize = new wxSlider( this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	sbSizer2->Add( m_sliderBoneSize, 1, wxEXPAND, 5 );
	
	bSizer9->Add( sbSizer2, 0, wxEXPAND, 5 );
	
	bSizer19->Add( bSizer9, 1, wxEXPAND, 5 );
	
	bSizer13->Add( bSizer19, 0, wxEXPAND, 2 );
	
	m_boneTree = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
	bSizer13->Add( m_boneTree, 1, wxALL|wxEXPAND, 5 );
	
	bSizer12->Add( bSizer13, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer12 );
	this->Layout();
	
	// Connect Events
	m_animations->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( wxAnimationPanel::OnChangedAnimationName ), NULL, this );
	m_animationTimeValue->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxAnimationPanel::OnAnimationChangeTime ), NULL, this );
	m_animationTimeSlider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( wxAnimationPanel::OnAnimationChangeTimeScroll ), NULL, this );
	m_animationTimeSlider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( wxAnimationPanel::OnAnimationChangeTimeScroll ), NULL, this );
	m_animationTimeSlider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( wxAnimationPanel::OnAnimationChangeTimeScroll ), NULL, this );
	m_animationTimeSlider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( wxAnimationPanel::OnAnimationChangeTimeScroll ), NULL, this );
	m_animationTimeSlider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( wxAnimationPanel::OnAnimationChangeTimeScroll ), NULL, this );
	m_animationTimeSlider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( wxAnimationPanel::OnAnimationChangeTimeScroll ), NULL, this );
	m_animationTimeSlider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( wxAnimationPanel::OnAnimationChangeTimeScroll ), NULL, this );
	m_animationTimeSlider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( wxAnimationPanel::OnAnimationChangeTimeScroll ), NULL, this );
	m_animationTimeSlider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( wxAnimationPanel::OnAnimationChangeTimeScroll ), NULL, this );
	m_playSkelAnimation->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxAnimationPanel::OnButtonPlaybackSkelAnim ), NULL, this );
	m_rateSkelAnim->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxAnimationPanel::OnAnimationRateChange ), NULL, this );
	m_rateSkelAnim->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxAnimationPanel::OnAnimationRateChange ), NULL, this );
	m_pauseSkelAnimation->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxAnimationPanel::OnButtonPlaybackSkelAnim ), NULL, this );
	m_StopSkelAnimation->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxAnimationPanel::OnButtonPlaybackSkelAnim ), NULL, this );
	m_animLoopCheckBox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( wxAnimationPanel::OnLoopAnimation ), NULL, this );
	m_radioInterpolationMode->Connect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( wxAnimationPanel::OnRadioInterpolationMode ), NULL, this );
	m_sliderBoneSize->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( wxAnimationPanel::OnBoneSizeChange ), NULL, this );
	m_sliderBoneSize->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( wxAnimationPanel::OnBoneSizeChange ), NULL, this );
	m_sliderBoneSize->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( wxAnimationPanel::OnBoneSizeChange ), NULL, this );
	m_sliderBoneSize->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( wxAnimationPanel::OnBoneSizeChange ), NULL, this );
	m_sliderBoneSize->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( wxAnimationPanel::OnBoneSizeChange ), NULL, this );
	m_sliderBoneSize->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( wxAnimationPanel::OnBoneSizeChange ), NULL, this );
	m_sliderBoneSize->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( wxAnimationPanel::OnBoneSizeChange ), NULL, this );
	m_sliderBoneSize->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( wxAnimationPanel::OnBoneSizeChange ), NULL, this );
	m_sliderBoneSize->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( wxAnimationPanel::OnBoneSizeChange ), NULL, this );
	m_boneTree->Connect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( wxAnimationPanel::OnNodeTrackSelect ), NULL, this );
}

wxAnimationPanel::~wxAnimationPanel()
{
	// Disconnect Events
	m_animations->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( wxAnimationPanel::OnChangedAnimationName ), NULL, this );
	m_animationTimeValue->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxAnimationPanel::OnAnimationChangeTime ), NULL, this );
	m_animationTimeSlider->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( wxAnimationPanel::OnAnimationChangeTimeScroll ), NULL, this );
	m_animationTimeSlider->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( wxAnimationPanel::OnAnimationChangeTimeScroll ), NULL, this );
	m_animationTimeSlider->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( wxAnimationPanel::OnAnimationChangeTimeScroll ), NULL, this );
	m_animationTimeSlider->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( wxAnimationPanel::OnAnimationChangeTimeScroll ), NULL, this );
	m_animationTimeSlider->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( wxAnimationPanel::OnAnimationChangeTimeScroll ), NULL, this );
	m_animationTimeSlider->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( wxAnimationPanel::OnAnimationChangeTimeScroll ), NULL, this );
	m_animationTimeSlider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( wxAnimationPanel::OnAnimationChangeTimeScroll ), NULL, this );
	m_animationTimeSlider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( wxAnimationPanel::OnAnimationChangeTimeScroll ), NULL, this );
	m_animationTimeSlider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( wxAnimationPanel::OnAnimationChangeTimeScroll ), NULL, this );
	m_playSkelAnimation->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxAnimationPanel::OnButtonPlaybackSkelAnim ), NULL, this );
	m_rateSkelAnim->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxAnimationPanel::OnAnimationRateChange ), NULL, this );
	m_rateSkelAnim->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxAnimationPanel::OnAnimationRateChange ), NULL, this );
	m_pauseSkelAnimation->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxAnimationPanel::OnButtonPlaybackSkelAnim ), NULL, this );
	m_StopSkelAnimation->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxAnimationPanel::OnButtonPlaybackSkelAnim ), NULL, this );
	m_animLoopCheckBox->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( wxAnimationPanel::OnLoopAnimation ), NULL, this );
	m_radioInterpolationMode->Disconnect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( wxAnimationPanel::OnRadioInterpolationMode ), NULL, this );
	m_sliderBoneSize->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( wxAnimationPanel::OnBoneSizeChange ), NULL, this );
	m_sliderBoneSize->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( wxAnimationPanel::OnBoneSizeChange ), NULL, this );
	m_sliderBoneSize->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( wxAnimationPanel::OnBoneSizeChange ), NULL, this );
	m_sliderBoneSize->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( wxAnimationPanel::OnBoneSizeChange ), NULL, this );
	m_sliderBoneSize->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( wxAnimationPanel::OnBoneSizeChange ), NULL, this );
	m_sliderBoneSize->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( wxAnimationPanel::OnBoneSizeChange ), NULL, this );
	m_sliderBoneSize->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( wxAnimationPanel::OnBoneSizeChange ), NULL, this );
	m_sliderBoneSize->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( wxAnimationPanel::OnBoneSizeChange ), NULL, this );
	m_sliderBoneSize->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( wxAnimationPanel::OnBoneSizeChange ), NULL, this );
	m_boneTree->Disconnect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( wxAnimationPanel::OnNodeTrackSelect ), NULL, this );
	
}

wxLightsPanel::wxLightsPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxVERTICAL );
	
	m_scrolledWindow1 = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxTAB_TRAVERSAL|wxVSCROLL );
	m_scrolledWindow1->SetScrollRate( 5, 5 );
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxHORIZONTAL );
	
	wxArrayString m_lightsChoiceChoices;
	m_lightsChoice = new wxChoice( m_scrolledWindow1, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_lightsChoiceChoices, 0 );
	m_lightsChoice->SetSelection( 0 );
	bSizer16->Add( m_lightsChoice, 1, wxALL|wxEXPAND, 5 );
	
	m_button7 = new wxButton( m_scrolledWindow1, wxID_ADDLIGHT, wxT("Add light"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button7->SetToolTip( wxT("Position & direction is taken from the camera's current position when creating a new light") );
	
	bSizer16->Add( m_button7, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	m_button10 = new wxButton( m_scrolledWindow1, wxID_REMOVELIGHT, wxT("Remove Light"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( m_button10, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	bSizer14->Add( bSizer16, 0, wxEXPAND, 5 );
	
	m_staticline1 = new wxStaticLine( m_scrolledWindow1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer14->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText9 = new wxStaticText( m_scrolledWindow1, wxID_ANY, wxT("Light Type:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	bSizer18->Add( m_staticText9, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString m_choiceLightTypeChoices[] = { wxT("LT_POINT"), wxT("LT_DIRECTIONAL (Default)"), wxT("LT_SPOT") };
	int m_choiceLightTypeNChoices = sizeof( m_choiceLightTypeChoices ) / sizeof( wxString );
	m_choiceLightType = new wxChoice( m_scrolledWindow1, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceLightTypeNChoices, m_choiceLightTypeChoices, 0 );
	m_choiceLightType->SetSelection( 1 );
	bSizer18->Add( m_choiceLightType, 1, wxALL|wxEXPAND, 5 );
	
	bSizer17->Add( bSizer18, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( m_scrolledWindow1, wxID_ANY, wxT("Position") ), wxHORIZONTAL );
	
	m_lightPosX = new wxTextCtrl( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( m_lightPosX, 1, wxALL, 5 );
	
	m_lightPosY = new wxTextCtrl( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( m_lightPosY, 1, wxALL, 5 );
	
	m_lightPosZ = new wxTextCtrl( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( m_lightPosZ, 1, wxALL, 5 );
	
	bSizer17->Add( sbSizer2, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer21;
	sbSizer21 = new wxStaticBoxSizer( new wxStaticBox( m_scrolledWindow1, wxID_ANY, wxT("Direction") ), wxVERTICAL );
	
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxHORIZONTAL );
	
	m_lightDirX = new wxTextCtrl( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( m_lightDirX, 1, wxALL, 5 );
	
	m_lightDirY = new wxTextCtrl( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( m_lightDirY, 1, wxALL, 5 );
	
	m_lightDirZ = new wxTextCtrl( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( m_lightDirZ, 1, wxALL, 5 );
	
	sbSizer21->Add( bSizer21, 1, wxEXPAND, 5 );
	
	m_button11 = new wxButton( m_scrolledWindow1, wxID_DIRNORMALIZE, wxT("Normalize"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer21->Add( m_button11, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	bSizer17->Add( sbSizer21, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticBoxSizer* sbSizer6;
	sbSizer6 = new wxStaticBoxSizer( new wxStaticBox( m_scrolledWindow1, wxID_ANY, wxT("Diffuse") ), wxHORIZONTAL );
	
	m_textDiffuseColour = new wxTextCtrl( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textDiffuseColour->SetMaxLength( 8 ); 
	m_textDiffuseColour->SetToolTip( wxT("Diffuse colour, RGBA syntax") );
	
	sbSizer6->Add( m_textDiffuseColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_diffusePickColour = new wxButton( m_scrolledWindow1, wxID_PICKDIFFUSE, wxT("Pick colour"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer6->Add( m_diffusePickColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	bSizer19->Add( sbSizer6, 1, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer8;
	sbSizer8 = new wxStaticBoxSizer( new wxStaticBox( m_scrolledWindow1, wxID_ANY, wxT("Power") ), wxVERTICAL );
	
	m_spinPower = new wxSpinCtrl( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxSP_ARROW_KEYS, 0, 100, 1 );
	sbSizer8->Add( m_spinPower, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	bSizer19->Add( sbSizer8, 0, wxALL|wxEXPAND, 5 );
	
	bSizer17->Add( bSizer19, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer61;
	sbSizer61 = new wxStaticBoxSizer( new wxStaticBox( m_scrolledWindow1, wxID_ANY, wxT("Specular") ), wxHORIZONTAL );
	
	m_textSpecularColour = new wxTextCtrl( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textSpecularColour->SetMaxLength( 8 ); 
	m_textSpecularColour->SetToolTip( wxT("Specular colour, RGBA syntax") );
	
	sbSizer61->Add( m_textSpecularColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_specularPickColour = new wxButton( m_scrolledWindow1, wxID_PICKSPECULAR, wxT("Pick colour"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer61->Add( m_specularPickColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	bSizer17->Add( sbSizer61, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer62;
	sbSizer62 = new wxStaticBoxSizer( new wxStaticBox( m_scrolledWindow1, wxID_ANY, wxT("Attenuation") ), wxVERTICAL );
	
	wxGridSizer* gSizer2;
	gSizer2 = new wxGridSizer( 4, 2, 0, 0 );
	
	m_staticText10 = new wxStaticText( m_scrolledWindow1, wxID_ANY, wxT("Range"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	gSizer2->Add( m_staticText10, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_textRange = new wxTextCtrl( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer2->Add( m_textRange, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_staticText11 = new wxStaticText( m_scrolledWindow1, wxID_ANY, wxT("Quadric"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	gSizer2->Add( m_staticText11, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_textQuadratic = new wxTextCtrl( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer2->Add( m_textQuadratic, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_staticText12 = new wxStaticText( m_scrolledWindow1, wxID_ANY, wxT("Linear"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	gSizer2->Add( m_staticText12, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_textLinear = new wxTextCtrl( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer2->Add( m_textLinear, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_staticText13 = new wxStaticText( m_scrolledWindow1, wxID_ANY, wxT("Constant"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	gSizer2->Add( m_staticText13, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_textConstant = new wxTextCtrl( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer2->Add( m_textConstant, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	sbSizer62->Add( gSizer2, 1, wxEXPAND, 5 );
	
	bSizer17->Add( sbSizer62, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer191;
	bSizer191 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer9;
	sbSizer9 = new wxStaticBoxSizer( new wxStaticBox( m_scrolledWindow1, wxID_ANY, wxT("Spot params") ), wxVERTICAL );
	
	wxGridSizer* gSizer3;
	gSizer3 = new wxGridSizer( 2, 3, 0, 0 );
	
	m_staticText14 = new wxStaticText( m_scrolledWindow1, wxID_ANY, wxT("Inner radius"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	gSizer3->Add( m_staticText14, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_staticText15 = new wxStaticText( m_scrolledWindow1, wxID_ANY, wxT("Outer radius"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	gSizer3->Add( m_staticText15, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_staticText16 = new wxStaticText( m_scrolledWindow1, wxID_ANY, wxT("Falloff"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	gSizer3->Add( m_staticText16, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_textInnerRadius = new wxTextCtrl( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer3->Add( m_textInnerRadius, 1, wxALIGN_CENTER|wxALL|wxEXPAND, 5 );
	
	m_textOuterRadius = new wxTextCtrl( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer3->Add( m_textOuterRadius, 1, wxALIGN_CENTER|wxALL|wxEXPAND, 5 );
	
	m_textFalloff = new wxTextCtrl( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer3->Add( m_textFalloff, 1, wxALIGN_CENTER|wxALL|wxEXPAND, 5 );
	
	sbSizer9->Add( gSizer3, 1, wxEXPAND, 5 );
	
	bSizer191->Add( sbSizer9, 1, wxEXPAND, 5 );
	
	bSizer17->Add( bSizer191, 0, wxEXPAND, 5 );
	
	bSizer14->Add( bSizer17, 1, wxEXPAND, 5 );
	
	m_scrolledWindow1->SetSizer( bSizer14 );
	m_scrolledWindow1->Layout();
	bSizer14->Fit( m_scrolledWindow1 );
	bSizer20->Add( m_scrolledWindow1, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer20 );
	this->Layout();
	
	// Connect Events
	m_lightsChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( wxLightsPanel::OnLightSelectionChange ), NULL, this );
	m_button7->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxLightsPanel::OnButtonClick ), NULL, this );
	m_button10->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxLightsPanel::OnButtonClick ), NULL, this );
	m_choiceLightType->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( wxLightsPanel::OnLightTypeChange ), NULL, this );
	m_diffusePickColour->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxLightsPanel::OnButtonClick ), NULL, this );
	m_specularPickColour->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxLightsPanel::OnButtonClick ), NULL, this );
}

wxLightsPanel::~wxLightsPanel()
{
	// Disconnect Events
	m_lightsChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( wxLightsPanel::OnLightSelectionChange ), NULL, this );
	m_button7->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxLightsPanel::OnButtonClick ), NULL, this );
	m_button10->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxLightsPanel::OnButtonClick ), NULL, this );
	m_choiceLightType->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( wxLightsPanel::OnLightTypeChange ), NULL, this );
	m_diffusePickColour->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxLightsPanel::OnButtonClick ), NULL, this );
	m_specularPickColour->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxLightsPanel::OnButtonClick ), NULL, this );
	
}

GridSettings::GridSettings( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("Presets:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	bSizer11->Add( m_staticText5, 0, wxALL, 5 );
	
	wxString m_presetsChoiceChoices[] = { wxT("Centimeters (default)"), wxT("Meters"), wxT("Inches"), wxT("Foot"), wxT("Custom") };
	int m_presetsChoiceNChoices = sizeof( m_presetsChoiceChoices ) / sizeof( wxString );
	m_presetsChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_presetsChoiceNChoices, m_presetsChoiceChoices, 0 );
	m_presetsChoice->SetSelection( 0 );
	bSizer11->Add( m_presetsChoice, 1, wxALL, 5 );
	
	bSizer10->Add( bSizer11, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("1 Grid Cell Unit ="), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	bSizer12->Add( m_staticText6, 1, wxALIGN_CENTER|wxALL, 5 );
	
	m_gridCellUnit = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer12->Add( m_gridCellUnit, 1, wxALIGN_CENTER|wxALL, 5 );
	
	bSizer14->Add( bSizer12, 0, wxEXPAND, 5 );
	
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 2, 2, 0, 0 );
	
	m_staticText7 = new wxStaticText( this, wxID_ANY, wxT("Width:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	gSizer1->Add( m_staticText7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_gridWidth = new wxTextCtrl( this, wxID_GRIDWIDTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1->Add( m_gridWidth, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("Depth:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	gSizer1->Add( m_staticText8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_gridDepth = new wxTextCtrl( this, wxID_GRIDDEPTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1->Add( m_gridDepth, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	bSizer14->Add( gSizer1, 0, wxEXPAND, 5 );
	
	bSizer10->Add( bSizer14, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer13->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_OK = new wxButton( this, wxID_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_OK, 0, wxALIGN_BOTTOM|wxALIGN_RIGHT|wxALL, 5 );
	
	m_button6 = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_button6, 0, wxALIGN_BOTTOM|wxALL, 5 );
	
	bSizer10->Add( bSizer13, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer10 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_presetsChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GridSettings::OnPresetsChange ), NULL, this );
	m_OK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GridSettings::OnCommitChanges ), NULL, this );
	m_button6->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GridSettings::OnCommitChanges ), NULL, this );
}

GridSettings::~GridSettings()
{
	// Disconnect Events
	m_presetsChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GridSettings::OnPresetsChange ), NULL, this );
	m_OK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GridSettings::OnCommitChanges ), NULL, this );
	m_button6->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GridSettings::OnCommitChanges ), NULL, this );
	
}

wxAnimPosePanel::wxAnimPosePanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );
	
	wxString m_animationsChoices[] = { wxT("(None)") };
	int m_animationsNChoices = sizeof( m_animationsChoices ) / sizeof( wxString );
	m_animations = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_animationsNChoices, m_animationsChoices, 0 );
	m_animations->SetSelection( 0 );
	bSizer12->Add( m_animations, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Time: "), wxDefaultPosition, wxDefaultSize, 0|wxFULL_REPAINT_ON_RESIZE );
	m_staticText1->Wrap( -1 );
	bSizer15->Add( m_staticText1, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT, 5 );
	
	m_animationTimeValue = new wxTextCtrl( this, wxID_ANIMATIONTIMEVALUE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	bSizer15->Add( m_animationTimeValue, 0, wxALL, 5 );
	
	
	bSizer15->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("Length: "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer15->Add( m_staticText3, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	m_animationLengthText = new wxStaticText( this, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_animationLengthText->Wrap( -1 );
	m_animationLengthText->SetMinSize( wxSize( 50,-1 ) );
	
	bSizer15->Add( m_animationLengthText, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	bSizer13->Add( bSizer15, 0, wxEXPAND, 0 );
	
	m_animationTimeSlider = new wxSlider( this, wxID_ANIMATIONTIMESLIDER, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	bSizer13->Add( m_animationTimeSlider, 0, wxALIGN_CENTER|wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxHORIZONTAL );
	
	m_playSkelAnimation = new wxButton( this, wxID_PLAYANIMATION, wxT("Play"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer18->Add( m_playSkelAnimation, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Rate"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer18->Add( m_staticText2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_rateSkelAnim = new wxTextCtrl( this, wxID_ANY, wxT("1.0"), wxDefaultPosition, wxSize( 40,-1 ), wxTE_PROCESS_ENTER );
	bSizer18->Add( m_rateSkelAnim, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	bSizer18->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_pauseSkelAnimation = new wxButton( this, wxID_PAUSEANIMATION, wxT("Pause"), wxPoint( -1,-1 ), wxDefaultSize, 0 );
	bSizer18->Add( m_pauseSkelAnimation, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	m_StopSkelAnimation = new wxButton( this, wxID_STOPANIMATION, wxT("Stop"), wxPoint( -1,-1 ), wxDefaultSize, 0 );
	bSizer18->Add( m_StopSkelAnimation, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	bSizer13->Add( bSizer18, 0, wxEXPAND, 0 );
	
	m_animLoopCheckBox = new wxCheckBox( this, wxID_ANY, wxT("Loop Animation"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_animLoopCheckBox, 0, wxALL, 5 );
	
	m_scrolledPosePanel = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL );
	m_scrolledPosePanel->SetScrollRate( 5, 5 );
	m_scrollWindowSizer = new wxBoxSizer( wxVERTICAL );
	
	m_scrolledPosePanel->SetSizer( m_scrollWindowSizer );
	m_scrolledPosePanel->Layout();
	m_scrollWindowSizer->Fit( m_scrolledPosePanel );
	bSizer13->Add( m_scrolledPosePanel, 1, wxEXPAND | wxALL, 5 );
	
	m_resetWeights = new wxButton( this, wxID_ANY, wxT("Reset Weights"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_resetWeights, 0, wxALL, 5 );
	
	bSizer12->Add( bSizer13, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer12 );
	this->Layout();
	
	// Connect Events
	m_animations->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( wxAnimPosePanel::OnChangedAnimationName ), NULL, this );
	m_animationTimeValue->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxAnimPosePanel::OnAnimationChangeTime ), NULL, this );
	m_animationTimeSlider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( wxAnimPosePanel::OnAnimationChangeTimeSlider ), NULL, this );
	m_animationTimeSlider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( wxAnimPosePanel::OnAnimationChangeTimeSlider ), NULL, this );
	m_animationTimeSlider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( wxAnimPosePanel::OnAnimationChangeTimeSlider ), NULL, this );
	m_animationTimeSlider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( wxAnimPosePanel::OnAnimationChangeTimeSlider ), NULL, this );
	m_animationTimeSlider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( wxAnimPosePanel::OnAnimationChangeTimeSlider ), NULL, this );
	m_animationTimeSlider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( wxAnimPosePanel::OnAnimationChangeTimeSlider ), NULL, this );
	m_animationTimeSlider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( wxAnimPosePanel::OnAnimationChangeTimeSlider ), NULL, this );
	m_animationTimeSlider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( wxAnimPosePanel::OnAnimationChangeTimeSlider ), NULL, this );
	m_animationTimeSlider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( wxAnimPosePanel::OnAnimationChangeTimeSlider ), NULL, this );
	m_playSkelAnimation->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxAnimPosePanel::OnButtonPlaybackAnim ), NULL, this );
	m_rateSkelAnim->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxAnimPosePanel::OnAnimationRateChange ), NULL, this );
	m_rateSkelAnim->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxAnimPosePanel::OnAnimationRateChange ), NULL, this );
	m_pauseSkelAnimation->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxAnimPosePanel::OnButtonPlaybackAnim ), NULL, this );
	m_StopSkelAnimation->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxAnimPosePanel::OnButtonPlaybackAnim ), NULL, this );
	m_animLoopCheckBox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( wxAnimPosePanel::OnLoopAnimation ), NULL, this );
	m_resetWeights->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxAnimPosePanel::OnResetWeights ), NULL, this );
}

wxAnimPosePanel::~wxAnimPosePanel()
{
	// Disconnect Events
	m_animations->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( wxAnimPosePanel::OnChangedAnimationName ), NULL, this );
	m_animationTimeValue->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxAnimPosePanel::OnAnimationChangeTime ), NULL, this );
	m_animationTimeSlider->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( wxAnimPosePanel::OnAnimationChangeTimeSlider ), NULL, this );
	m_animationTimeSlider->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( wxAnimPosePanel::OnAnimationChangeTimeSlider ), NULL, this );
	m_animationTimeSlider->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( wxAnimPosePanel::OnAnimationChangeTimeSlider ), NULL, this );
	m_animationTimeSlider->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( wxAnimPosePanel::OnAnimationChangeTimeSlider ), NULL, this );
	m_animationTimeSlider->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( wxAnimPosePanel::OnAnimationChangeTimeSlider ), NULL, this );
	m_animationTimeSlider->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( wxAnimPosePanel::OnAnimationChangeTimeSlider ), NULL, this );
	m_animationTimeSlider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( wxAnimPosePanel::OnAnimationChangeTimeSlider ), NULL, this );
	m_animationTimeSlider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( wxAnimPosePanel::OnAnimationChangeTimeSlider ), NULL, this );
	m_animationTimeSlider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( wxAnimPosePanel::OnAnimationChangeTimeSlider ), NULL, this );
	m_playSkelAnimation->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxAnimPosePanel::OnButtonPlaybackAnim ), NULL, this );
	m_rateSkelAnim->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxAnimPosePanel::OnAnimationRateChange ), NULL, this );
	m_rateSkelAnim->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxAnimPosePanel::OnAnimationRateChange ), NULL, this );
	m_pauseSkelAnimation->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxAnimPosePanel::OnButtonPlaybackAnim ), NULL, this );
	m_StopSkelAnimation->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxAnimPosePanel::OnButtonPlaybackAnim ), NULL, this );
	m_animLoopCheckBox->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( wxAnimPosePanel::OnLoopAnimation ), NULL, this );
	m_resetWeights->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxAnimPosePanel::OnResetWeights ), NULL, this );
	
}
