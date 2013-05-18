//----------------------------------------------------------------------
//	Copyright (C) 2010-2013 Matias N. Goldberg ("dark_sylinc")
//  This file is part of Ogre Meshy.
//
//  Ogre Meshy is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Ogre Meshy is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Ogre Meshy.  If not, see <http://www.gnu.org/licenses/>.
//----------------------------------------------------------------------

#include <wx/wx.h>
#include <wx/aui/aui.h>
#include <wx/colordlg.h>
#include <wx/filedlg.h>
#include <wx/aboutdlg.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/utils.h>
#include "Core/wxOgreMeshViewerMainFrameImpl.h"

#include "Core/wxOgreRenderWindow.h"

#include "Core/AnimationPanel.h"
#include "Core/Panels/AnimPosePanel.h"
#include "Core/Panels/LightsPanel.h"
#include "Core/GridSettingsImpl.h"

#include "OgreResourceGroupManager.h"
#include "OgreException.h"

#include "iostream"

#include "Constants.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	#include <windows.h>
	#include <shlobj.h>
#endif

MeshyMainFrameImpl::MeshyMainFrameImpl( wxWindow* parent, const CmdSettings &cmdSettings ) :
			MainFrame( parent ),
			m_wxAuiManager( 0 ),
			m_ogreLog( 0 ),
			m_root( 0 ),
			m_sceneManager( 0 ),
			m_camera( 0 ),
			m_cameraNode( 0 ),
			m_wxOgreRenderWindow( 0 ),
			m_mainNotebook( 0 ),
			m_meshInfoPage( 0 ),
			m_animationPanel( 0 ),
			m_animPosePanel( 0 ),
			m_lightsPanel( 0 ),
			m_lastOpenResCfgDir( wxT("") ),
			m_meshName( "" ),
			m_meshEntity( 0 ),
			m_meshSceneNode( 0 ),
			m_axisNode( 0 ),
			m_gridNode( 0 ),
			m_cellSize( 1 ),
			m_cellWidth( 50 ),
			m_cellDepth( 50 ),
			m_wasLeftPressed( false ),
			m_wasRightPressed( false ),
			m_mouseX( 0 ),
			m_mouseY( 0 )
{
#ifndef __WXMSW__
	//Set config directory to user home directory
	m_configDirectory = std::string(wxGetHomeDir().mb_str()) + "/.ogremeshy/";
#else
	//TODO: Use wxStandardPaths::GetUserConfigDir()
	//Windows: use User/AppData
	TCHAR path[MAX_PATH];
	if( SUCCEEDED( SHGetFolderPath( NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, path ) != S_OK ) )
	{
		//Need to convert to OEM codepage so that fstream can
		//use it properly on international systems.
		TCHAR oemPath[MAX_PATH];
		CharToOem( path, oemPath );
		m_configDirectory = oemPath;
		m_configDirectory += "\\OgreMeshy\\";

		//Attempt to create directory where config files go
		if( !CreateDirectory( m_configDirectory.c_str(), NULL ) &&
			GetLastError() != ERROR_ALREADY_EXISTS )
		{
			//Couldn't create directory (no write access?),
			//fall back to current working dir
			m_configDirectory = "";
		}
	}
#endif

	SetIcon( wxIcon(wxT("OgreIcon")) ); //This probably only works in Windows

	//wxFormBuilder doesn't allow default disabled for toolbars
	m_toolBar->EnableTool( wxID_MENUSHOWBONES, false );

	//Prepare status bar to show FPS, batch count, etc
	int widths[] = {-1, 80, 80, 80, 80};
	m_statusBar1->SetFieldsCount( sizeof(widths) / sizeof(int), widths );

	//Create the Advanced UI system to handle dockable windows
	m_wxAuiManager = new wxAuiManager();
	m_wxAuiManager->SetManagedWindow( this );

	//Don't create the log window by default
	createLogWindow( false );

	//Initialize Ogre and the control that renders it
	initOgre( cmdSettings.setupRenderSystems );

	m_mainNotebook		= new wxAuiNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
											wxAUI_NB_BOTTOM | wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_MOVE |
											wxAUI_NB_SCROLL_BUTTONS );
	m_meshInfoPage		= new wxTreeCtrl( this );
	m_animationPanel	= new AnimationPanel( this, m_sceneManager );
	m_animPosePanel		= new AnimPosePanel( this, m_sceneManager );
	m_lightsPanel		= new LightsPanel( this, m_sceneManager, m_camera );
	m_mainNotebook->AddPage( m_meshInfoPage, wxT("Info"), true );
	m_mainNotebook->AddPage( m_animationPanel, wxT("Animations") );
	m_mainNotebook->AddPage( m_animPosePanel, wxT("Poses") );
	m_mainNotebook->AddPage( m_lightsPanel, wxT("Lights") );
	m_wxAuiManager->AddPane( m_mainNotebook, wxAuiPaneInfo().Name(wxT("TabsPane")).Caption(wxT("Tabs")).
		BestSize(450, -1).Left().Layer(1).CloseButton(false).PaneBorder(false) );

	m_lightPanelPageIdx = m_mainNotebook->GetPageIndex( m_lightsPanel );

	//Commit changes made to the AUI manager
	m_wxAuiManager->Update();

	//Initialize internal, permanent resources
	Ogre::ResourceGroupManager::getSingleton().createResourceGroup( c_InterMeshPermGroup );
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation( "Resources/Models", "FileSystem",
																	c_InterMeshPermGroup );
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation( "Resources/Fonts", "FileSystem",
																	c_InterMeshPermGroup );
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup( c_InterMeshPermGroup );

	//Create this one here now to avoid an exception when we try to delete it
	Ogre::ResourceGroupManager::getSingleton().createResourceGroup( c_InternMeshGroup );

	loadSettings();

	if( cmdSettings.resoucesCfgPath != wxT("") )
		loadResourcesCfg( cmdSettings.resoucesCfgPath );

	try
	{
		if( cmdSettings.meshFullPath != "" )
			openMesh( cmdSettings.meshFullPath );
	}
	catch( Ogre::Exception &e )
	{
		wxMessageBox( wxT( "Error while loading " ) + wxString( cmdSettings.meshFullPath.c_str(),
																wxConvUTF8 ) +
					  wxT( "\nSee the Log. Try loading resources first\n\n" +
					  wxString( e.getFullDescription().c_str(), wxConvUTF8 ) ),
					  wxT("Error while loading mesh"),
					  wxOK|wxICON_ERROR|wxCENTRE );

		//Save the folder for fast retrieval (i.e. quick reload)
		//Works in Unix & Windows
		const size_t pos	= cmdSettings.meshFullPath.find_last_of("/\\");
		m_lastOpenMeshDir	= cmdSettings.meshFullPath.substr( 0, pos );
	}

	createGrid();
	showGrid();
}

MeshyMainFrameImpl::~MeshyMainFrameImpl()
{
	saveSettings();

	if( m_wxOgreRenderWindow )
	{
		m_wxOgreRenderWindow->Destroy();
		m_wxOgreRenderWindow = 0;
	}

	if( m_root )
	{
		delete m_root;
		m_root = 0;
	}

	if( m_wxAuiManager )
	{
		m_wxAuiManager->UnInit();
		delete m_wxAuiManager;
		m_wxAuiManager = 0;
	}
}

//-----------------------------------------------------------------------------
//saveSettings()
//Description:
//	Writes current user settings to a file
//	TODO: Assumes it has write access?
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::saveSettings()
{
	{
		std::ofstream myFile( (m_configDirectory + c_userSettingsFile).c_str() );

		if( myFile.is_open() )
		{
			myFile << "[UserSettings]"		<< "\n";
			myFile << "LastResourcePath = " << m_lastOpenResCfgDir	<< "\n";
			myFile << "LastMeshPath = "		<< m_lastOpenMeshDir	<< "\n";
			myFile << "LastResourceFile = " << m_resourcesCfgFile	<< "\n";
			myFile << "CellSize = "			<< m_cellSize			<< "\n";
			myFile << "CellWidth = "		<< m_cellWidth			<< "\n";
			myFile << "CellDepth = "		<< m_cellDepth			<< "\n";
			myFile << "BoneNameColour = "	<< std::hex <<
								m_animationPanel->getBoneNameColour().getAsABGR() << "\n";

			if( m_lightsPanel )
				m_lightsPanel->saveSettings( myFile );

			myFile.close();
		}
	}

	{
		//Save wxAUI layout
		std::ofstream myFile( (m_configDirectory + c_layoutSettingsFile).c_str(),
								std::ios_base::out|std::ios_base::binary );
		if( myFile.is_open() )
		{
			const wxString layoutString = m_wxAuiManager->SavePerspective();
			myFile.write( layoutString.mb_str(), layoutString.size() );
			myFile.close();
		}
	}
}

//-----------------------------------------------------------------------------
//loadSettings()
//Description:
//	Loads user settings from a file (if exists)
//Notes:
//	1) Assumes the values are already defaulted (in case settings file wasn't
//	   found)
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::loadSettings()
{
	if( wxFileExists( wxString( (m_configDirectory + c_userSettingsFile).c_str(), wxConvUTF8 ) ) )
	{
		bool bRemoveDefaultLight = false;

		Ogre::ConfigFile cf;
		cf.load( m_configDirectory + c_userSettingsFile );

		//Go through all sections & settings in the file
		Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

		Ogre::String secName, typeName, archName;
		while( seci.hasMoreElements() )
		{
			secName = seci.peekNextKey();
			Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();

			if( secName.find( "Light" ) != Ogre::String::npos )
			{
				//[LightXX]
				bRemoveDefaultLight = true;
				m_lightsPanel->loadSettings( *settings );
			}
			else
			{
				Ogre::ConfigFile::SettingsMultiMap::iterator i;
				for (i = settings->begin(); i != settings->end(); ++i)
				{
					//[UserSettings]
					if( i->first == "LastResourcePath" )
						m_lastOpenResCfgDir = wxString( i->second.c_str(), wxConvUTF8 );
					else if( i->first == "LastMeshPath" )
						m_lastOpenMeshDir	= i->second;
					else if( i->first == "LastResourceFile" )
						m_resourcesCfgFile	= i->second;

					else if( i->first == "CellSize" )
						m_cellSize = Ogre::StringConverter::parseReal( i->second );
					else if( i->first == "CellWidth" )
						m_cellWidth = Ogre::StringConverter::parseReal( i->second );
					else if( i->first == "CellDepth" )
						m_cellDepth = Ogre::StringConverter::parseReal( i->second );

					else if( i->first == "BoneNameColour" )
					{
						unsigned long lValue;
						if( wxString( wxT("0x") + wxString( i->second.c_str(), wxConvUTF8 ) ).
							ToULong( &lValue, 0 ) )
						{
							Ogre::ColourValue newOgreColour;
							newOgreColour.setAsABGR( lValue );
							m_animationPanel->setBoneNameColour( newOgreColour );
						}
					}
				}
			}
		}

		if( bRemoveDefaultLight )
			m_lightsPanel->removeDefaultLight();
	}

	{
		//Load wxAUI layout
		std::ifstream myFile( (m_configDirectory + c_layoutSettingsFile).c_str(),
								std::ios_base::in|std::ios_base::ate|std::ios_base::binary );
		if( myFile.is_open() )
		{
			std::string layoutString;
			layoutString.resize( myFile.tellg() );
			myFile.seekg( 0 );
			myFile.read( &layoutString[0], layoutString.size() );
			m_wxAuiManager->LoadPerspective( wxString( layoutString.c_str(), wxConvUTF8 ), true );
			myFile.close();
		}
	}
}

//-----------------------------------------------------------------------------
//initOgre()
//Description:
//	Initializes Ogre and the control that renders it
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::initOgre( bool bForceSetup )
{
#ifndef __WXMSW__
	//Set config directory
	const std::string c_pluginsCfg = "/opt/ogremeshy/";
#else
	const std::string c_pluginsCfg = "";
#endif

	m_root = new Ogre::Root( c_pluginsCfg + "Plugins.cfg", m_configDirectory + "ogre.cfg",
							m_configDirectory + "Ogre.log"  );
	Ogre::LogManager::getSingleton().getDefaultLog()->addListener( this );
	if( bForceSetup || !m_root->restoreConfig() )
		m_root->showConfigDialog();

	m_root->initialise( false );

	wxOgreRenderWindow::SetOgreRoot( m_root );
	//wxWindow *dummy = new wxWindow( this, wxID_ANY ); //put it in a dummy window to behave nice w/ AUI
	m_wxOgreRenderWindow = new wxOgreRenderWindow( this, wxID_ANY );
	m_wxOgreRenderWindow->Show();
	m_wxOgreRenderWindow->setRenderWindowListener( this );

	m_wxOgreRenderWindow->SetFocus();

	createSystems();

	//m_wxAuiManager->AddPane( m_wxOgreRenderWindow, wxLEFT|wxTOP, wxT("OGRE Render Window"));
	m_wxAuiManager->AddPane( m_wxOgreRenderWindow, wxAuiPaneInfo().Name(wxT("RenderWindow")).
													Caption(wxT("OGRE Render Window")).CenterPane().
													PaneBorder(false).MinSize( 256, 256 ).
													CloseButton(false) );
}

//-----------------------------------------------------------------------------
//createSystems()
//Description:
//	Creates Ogre secondary stuff like cameras and scene managers
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::createSystems()
{
	m_sceneManager	= m_root->createSceneManager( Ogre::ST_GENERIC, "ExampleSMInstance" );
	m_camera		= m_sceneManager->createCamera( "Main Camera" );

	m_cameraNode = m_sceneManager->getRootSceneNode()->createChildSceneNode( "Camera Node" );
	m_cameraNode->attachObject( m_camera );
	m_cameraNode->pitch( Ogre::Degree( -45.0f ) );

	m_camera->setPosition( Ogre::Vector3::UNIT_Z * 10.0f );
	//m_camera->setPosition( Ogre::Vector3( 0, 10.0f, 10.0f) );
	m_camera->lookAt( Ogre::Vector3( 0.0f, 0.0f, 0.0f ) );
	m_camera->setNearClipDistance( 0.1f );
	m_camera->setFarClipDistance( 100.0f );

	Ogre::Viewport* vp = m_wxOgreRenderWindow->GetRenderWindow()->addViewport( m_camera );
	//vp->setClearEveryFrame( true, Ogre::FBT_COLOUR | Ogre::FBT_DEPTH );
	vp->setBackgroundColour( Ogre::ColourValue( 0.25f, 0.25f, 0.25f ) );
	m_camera->setAspectRatio( Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()) );

	m_root->addFrameListener( this );

	m_root->getRenderSystem()->_initRenderTargets();
	m_root->clearEventTimes();

	m_sceneManager->setAmbientLight( Ogre::ColourValue( 0.2f, 0.2f, 0.2f ) );
}

//-----------------------------------------------------------------------------
//createLogWindow()
//Description:
//	Creates a text window which displays Ogre log messages.
//	If the window was hidden, it is displayed again
//Notes:
//	1) This only creates the wx control. The log listener is registered
//	   in initOgre()
//Input:
//	1) True to show it, false to hide the window
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::createLogWindow( bool bShow )
{
	//Create the log window
	if( !m_ogreLog )
	{
		m_ogreLog = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 500, 150 ),
									wxNO_BORDER | wxTE_MULTILINE | wxTE_DONTWRAP | wxTE_RICH | wxTE_RICH2 );
		m_ogreLog->SetEditable( false );

		m_wxAuiManager->AddPane( m_ogreLog, wxAuiPaneInfo().Name(wxT("LogWindow")).
									Caption(wxT("Ogre Log")).Bottom().Layer(0).Position(2).
									LeftDockable(false).RightDockable(false).CloseButton().
									DestroyOnClose(false) );

		wxAuiPaneInfo &logPane = m_wxAuiManager->GetPane( wxT("LogWindow") );
		logPane.Hide();

		m_wxAuiManager->Update();
	}

	if( m_ogreLog && bShow )
	{
		//Already created, let's see if it needs to be shown again
		if( !m_ogreLog->IsShown() )
		{
			//m_ogreLog->Show( true );
			wxAuiPaneInfo &logPane = m_wxAuiManager->GetPane( wxT("LogWindow") );
			logPane.Show();
			m_wxAuiManager->Update();
		}
	}
}

//-----------------------------------------------------------------------------
//changeBackgroundColour()
//Description:
//	Displays a colour picker dialog and changes the bg colour
//	(i.e. the clear colour) from the scene
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::changeBackgroundColour()
{
	const Ogre::ColourValue &ogreColour = m_camera->getViewport()->getBackgroundColour();

	wxColourData colourData;
	colourData.SetChooseFull( true );
	colourData.SetColour( wxColour( ogreColour.getAsABGR() ) );
	colourData.SetCustomColour( 0, wxColour( ogreColour.getAsABGR() ) );

	wxColourDialog colourDlg( this, &colourData );

	if( colourDlg.ShowModal() == wxID_OK )
	{
		Ogre::ColourValue newOgreColour;
		newOgreColour = Ogre::ColourValue(	colourDlg.GetColourData().GetColour().Red() / 255.0f,
											colourDlg.GetColourData().GetColour().Green() / 255.0f,
											colourDlg.GetColourData().GetColour().Blue() / 255.0f,
											1.0f );
		m_camera->getViewport()->setBackgroundColour( newOgreColour );
	}
}

//-----------------------------------------------------------------------------
//changeBoneNameColour()
//Description:
//	Displays a colour picker dialog and changes the colour of the text in
//	the bones.
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::changeBoneNameColour()
{
	const Ogre::ColourValue &ogreColour = m_animationPanel->getBoneNameColour();

	wxColourData colourData;
	colourData.SetChooseFull( true );
	colourData.SetColour( wxColour( ogreColour.getAsABGR() ) );
	colourData.SetCustomColour( 0, wxColour( ogreColour.getAsABGR() ) );

	wxColourDialog colourDlg( this, &colourData );

	if( colourDlg.ShowModal() == wxID_OK )
	{
		Ogre::ColourValue newOgreColour;
		newOgreColour = Ogre::ColourValue(	colourDlg.GetColourData().GetColour().Red() / 255.0f,
											colourDlg.GetColourData().GetColour().Green() / 255.0f,
											colourDlg.GetColourData().GetColour().Blue() / 255.0f,
											1.0f );
		m_animationPanel->setBoneNameColour( newOgreColour );

		if( m_menuView->IsChecked( wxID_MENUSHOWBONENAMES ) )
			toggleShowBoneNames();
	}
}


//-----------------------------------------------------------------------------
//showAboutBox()
//Description:
//	Displays an "About" dialog box with copyright notice
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::showAboutBox()
{
	wxAboutDialogInfo info;

	Ogre::String ogreVersion = Ogre::StringConverter::toString( OGRE_VERSION_MAJOR ) + "." +
								Ogre::StringConverter::toString( OGRE_VERSION_MINOR ) + "." +
								Ogre::StringConverter::toString( OGRE_VERSION_PATCH ) +
								" (" OGRE_VERSION_SUFFIX ", " OGRE_VERSION_NAME ")";
	wxString descLinked = wxString( ("\nLinked against:\n"
							"\t* Ogre " + ogreVersion + "\n"
							"\t* wxWidgets " ).c_str(), wxConvUTF8 ) + wxVERSION_NUM_DOT_STRING_T;

	info.SetName(_T("Ogre Meshy"));
#ifdef __WXMSW__
	info.SetVersion(wxT("1.5"));
#else
	info.SetVersion(wxT("1.5 for Linux beta"));
#endif
    info.SetDescription(_T("Simple application to view mesh properties\n") + descLinked);
    info.SetCopyright(_T("(C) 2010-2013 Matias N. Goldberg \"dark_sylinc\""));
	info.AddDeveloper(_T("Matias N. Goldberg - dark_sylinc"));
	info.AddDeveloper(_T("Thomas Fischer - tdev"));
	info.AddDeveloper(_T("Alberto Toglia - toglia"));
	info.AddArtist( wxT("\nMatias N. Goldberg") );
	info.AddArtist( wxT("\nRogerio de Souza Santos (File.png, Reload.png & ChangeBGColour.png)") );

    wxAboutBox( info );
}

//-----------------------------------------------------------------------------
//openMeshDialog()
//Description:
//	Shows a dialog so the user can choose which mesh to load
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::openMeshDialog()
{
	wxFileDialog openFileDialog( this, _("Open mesh file"), wxString( m_lastOpenMeshDir.c_str(),
																		wxConvUTF8 ),
								wxT(""), wxT("*.mesh"), wxFD_OPEN|wxFD_FILE_MUST_EXIST, wxDefaultPosition);
 
	if( openFileDialog.ShowModal() == wxID_OK )
		openMesh( std::string( openFileDialog.GetPath().mb_str() ) );
}

//-----------------------------------------------------------------------------
//openMesh()
//Description:
//	Receives a fullPath and splits into path and filename.mesh to call the
//	proper openMesh() function
//Input:
//	1) Full path to the file
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::openMesh( const std::string &fullPath )
{
	//Works in Unix & Windows
	const size_t pos			= fullPath.find_last_of("/\\");
	const std::string path		= fullPath.substr( 0, pos );
	const std::string meshName	= fullPath.substr( pos + 1 );
 
	if( path != "" && meshName != "" )
	{
		m_lastOpenMeshDir = path;
		openMesh( path, meshName );
	}
}

//-----------------------------------------------------------------------------
//openMesh()
//Description:
//	Loads the mesh from the given path
//Input:
//	1) Directory path to the mesh (i.e. C:\MyFolder\)
//	2) Name of the mesh, without the path.  (i.e. my3D.mesh)
//	3) Set to true to reset camera
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::openMesh( const std::string &directory, const std::string &meshName,
									bool bResetCamera )
{
	m_meshDir  = directory;
	m_meshName = meshName;

	if( m_meshEntity )
	{
		m_animationPanel->meshUnload();
		m_animPosePanel->meshUnload();

		m_meshSceneNode->getParentSceneNode()->removeAndDestroyChild( m_meshSceneNode->getName() );
		m_sceneManager->destroyEntity( m_meshEntity );

		//Disable reload option
		m_menuFile->Enable( wxID_MENURELOADMESH, false );
		m_toolBar->EnableTool( wxID_MENURELOADMESH, false );
	}

	//Ogre::ResourceGroupManager::getSingleton().unloadResourceGroup( DEFAULT_RESOURCE_GROUP_NAME, false );
	Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup( c_InternMeshGroup );
	Ogre::ResourceGroupManager::getSingleton().createResourceGroup( c_InternMeshGroup );
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation( directory, "FileSystem",
																	c_InternMeshGroup );
	try
	{
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	}
	catch(...)
	{
		//Some material or may be even an unrelated scripts crashed the engine.
		//Load the model plain simple, no materials. Only skeleton and mesh.
	}

	m_meshEntity	= m_sceneManager->createEntity( "MeshEntity", meshName, c_InternMeshGroup );
	m_meshSceneNode	= m_sceneManager->getRootSceneNode()->createChildSceneNode();
	m_meshSceneNode->attachObject( m_meshEntity );

	//Keep show bounding box setting
	m_meshSceneNode->showBoundingBox( m_menuBar->IsChecked( wxID_MENUSHOWBOUNDINGBOX ) );

	//Reorient the camera so that it looks at the whole model
	if( !bResetCamera )
	{
		defaultCamera();
		m_camera->setFarClipDistance( m_meshEntity->getBoundingRadius() * 10.0f );
	}

	if( m_axisNode )
		m_axisNode->setScale( Ogre::Vector3::UNIT_SCALE * m_meshEntity->getBoundingRadius() * 0.2f );

	//Load our GUI panels with the info we get from this mesh
	m_animationPanel->meshLoaded( m_meshEntity );
	m_animPosePanel->meshLoaded( m_meshEntity );
	m_menuFile->Enable( wxID_MENURELOADMESH, true );
	m_toolBar->EnableTool( wxID_MENURELOADMESH, true );
	showMeshInfo();

	//Keep show bones setting after telling the animation panel we loaded a new mesh
	if( m_menuView->IsChecked( wxID_MENUSHOWBONES ) )
		toggleShowBones();

	//Disable show bones option if there's no skeleton
	m_menuView->Enable( wxID_MENUSHOWBONES, m_meshEntity->getSkeleton() != 0 );
	m_toolBar->EnableTool( wxID_MENUSHOWBONES, m_meshEntity->getSkeleton() != 0 );
	m_menuView->Enable( wxID_MENUSHOWBONENAMES, m_meshEntity->getSkeleton() != 0 );
}

//-----------------------------------------------------------------------------
//loadResourcesCfgDialog()
//Description:
//	Shows a dialog so the user can choose what resources.cfg to load
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::loadResourcesCfgDialog()
{
	wxFileDialog openFileDialog( this, _("Open resources.cfg file to parse"), m_lastOpenResCfgDir, wxT(""),
								 wxT("*.cfg"), wxFD_OPEN|wxFD_FILE_MUST_EXIST, wxDefaultPosition );
 
	if( openFileDialog.ShowModal() == wxID_OK )
	{
		const wxString fullPath = openFileDialog.GetPath();
		const size_t pos	= fullPath.find_last_of(wxT("/\\"));
		m_lastOpenResCfgDir	= fullPath.substr( 0, pos );
		loadResourcesCfg( fullPath );
	}
}

//-----------------------------------------------------------------------------
//loadResourcesCfg()
//Description:
//	Loads a custom resources.cfg file so that all of the materials, shaders,
//	etc. are available.
//Notes:
//	1) The CFG should not contain resource group c_InternMeshGroup or
//	   bad things may happen
//	2) Folders on the CFG are relative to the path the CFG is located
//	3) Do NOT use "m_resourcesCfgFile" as input, as the reference is
//	   overwritten!
//	4) Assumes the cfg file exists!
//Input:
//	1) Full path to the resources.cfg file.
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::loadResourcesCfg( const wxString &file )
{
	//Unbind mesh' material since it may have been part of the resources.cfg. If the
	//material is loaded again but was modified (and errors ocurred) bad things will
	//happen (a simple reload won't work)
	std::vector<Ogre::String> materialNames;
	if( m_meshEntity )
	{
		materialNames.reserve( m_meshEntity->getNumSubEntities() );
		for( unsigned int i=0; i<m_meshEntity->getNumSubEntities(); ++i )
		{
			Ogre::SubEntity *subEnt = m_meshEntity->getSubEntity(i);
			materialNames.push_back( subEnt->getMaterialName() );
			subEnt->setMaterialName( "BaseWhite" );
		}
	}

	unloadResourcesCfg();

	m_resourcesCfgFile = file.mb_str();

	const wxFileName fileName( file );
	const std::string relativePath( (fileName.GetPath() + wxT("/")).mb_str() );

	Ogre::ConfigFile cf;
	cf.load( m_resourcesCfgFile );

	//Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	Ogre::String secName, typeName, archName;
	while( seci.hasMoreElements() )
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;

			//Make relative paths work, only if they're relative
#ifdef __WXMSW__
			if( archName.find_first_of( ":" ) == Ogre::String::npos )
				archName = relativePath + archName;
#else
			if( !archName.empty() )
			{
				if( archName[0] != '/' )
					archName = relativePath + archName;
			}
#endif

			Ogre::ResourceGroupManager::getSingleton().addResourceLocation( archName, typeName, secName );
		}
	}

	try
	{
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	}
	catch(...)
	{
		//We need to catch so we can restore materials (or at least, their names)
	}

	if( m_meshEntity )
	{
		//Set materials back again
		for( unsigned int i=0; i<m_meshEntity->getNumSubEntities(); ++i )
		{
			//Reloading the material is necessary because the material may not have been defined
			//among the resources we're reloading, but this material may be using shaders that no
			//long exist.
			Ogre::MaterialPtr pMat = Ogre::MaterialManager::getSingleton().getByName( materialNames[i] );
			pMat->reload();
			m_meshEntity->getSubEntity(i)->setMaterial( pMat );
		}
	}
}

//-----------------------------------------------------------------------------
//unloadResourcesCfg()
//Description:
//	See loadResourcesCfg(), this function unloads it
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::unloadResourcesCfg()
{
	m_resourcesCfgFile = "";
	Ogre::StringVector resourceNames = Ogre::ResourceGroupManager::getSingleton().getResourceGroups();

	Ogre::StringVector::const_iterator itor = resourceNames.begin();
	Ogre::StringVector::const_iterator end  = resourceNames.end();

	while( itor != end )
	{
		if( *itor != c_InternMeshGroup && *itor != c_InterMeshPermGroup &&
			*itor != "Autodetect" && *itor != "Internal" )
		{
			Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup( *itor );
			Ogre::ParticleSystemManager::getSingleton().removeTemplatesByResourceGroup( *itor );
		}
		++itor;
	}
}

//-----------------------------------------------------------------------------
//showMeshInfo()
//Description:
//	
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::showMeshInfo()
{
#define toStr(x) Ogre::StringConverter::toString(x)
	static const wxString c_VES_SEMANTICS[] =
	{
		wxT("Reserved"),
		wxT("VES_POSITION"),
		wxT("VES_BLEND_WEIGHTS"),
		wxT("VES_BLEND_INDICES"),
		wxT("VES_NORMAL"),
		wxT("VES_DIFFUSE"),
		wxT("VES_SPECULAR"),
		wxT("VES_TEXTURE_COORDINATES"),
		wxT("VES_BINORMAL"),
		wxT("VES_TANGENT")
	};

	m_meshInfoPage->DeleteAllItems();
	const wxTreeItemId rootId = m_meshInfoPage->AddRoot( wxString( m_meshName.c_str(), wxConvUTF8 ) );

	std::set<Ogre::String> materials;
	const wxTreeItemId materialId	= m_meshInfoPage->AppendItem( rootId, wxT("Materials") );
	const wxTreeItemId geomId		= m_meshInfoPage->AppendItem( rootId, wxT("Geometry") );

	m_meshInfoPage->AppendItem( geomId, wxT("EdgeList for Stencil shadows: ") +
										(m_meshEntity->getMesh()->isEdgeListBuilt() ?
											wxString(wxT("Yes")) : wxString(wxT("No")) ) );

	for( unsigned int i=0; i<m_meshEntity->getNumSubEntities(); ++i )
	{
		Ogre::SubMesh const *subMesh = m_meshEntity->getSubEntity( i )->getSubMesh();
		materials.insert( subMesh->getMaterialName() );

		const wxTreeItemId subMeshId = m_meshInfoPage->AppendItem( geomId, wxString(wxT("Submesh ")) << i );
		m_meshInfoPage->AppendItem( subMeshId, wxT("Dedicated vertices: ") +
												(subMesh->useSharedVertices ?
												wxString(wxT("No")) : wxString(wxT("Yes"))) );
		m_meshInfoPage->AppendItem( subMeshId, wxString( ("Material: " + subMesh->getMaterialName()).c_str(), wxConvUTF8 ) );

		if( subMesh->indexData && !subMesh->indexData->indexBuffer.isNull() )
		{
			m_meshInfoPage->AppendItem( subMeshId, wxString(wxT("Indices: ")) << subMesh->indexData->indexCount
												<< ((subMesh->indexData->indexBuffer->getType() ==
																	Ogre::HardwareIndexBuffer::IT_16BIT) ?
													wxT(" (16-bit)") : wxT(" (32-bit)" )) );
		}
		else
		{
			m_meshInfoPage->AppendItem( subMeshId, wxString(wxT("Indices: Not present")) );
		}

		if( !subMesh->useSharedVertices )
		{
			m_meshInfoPage->AppendItem( subMeshId, wxString(wxT("Bones used: "))  << subMesh->blendIndexToBoneIndexMap.size() );
			m_meshInfoPage->AppendItem( subMeshId, wxString(wxT("Vertices: ")) << subMesh->vertexData->vertexCount );
			const Ogre::VertexDeclaration::VertexElementList& veList =
										subMesh->vertexData->vertexDeclaration->getElements();

			size_t vertexSize = 0;
			for( unsigned short i=0; i<=subMesh->vertexData->vertexDeclaration->getMaxSource(); ++i )
				vertexSize += subMesh->vertexData->vertexDeclaration->getVertexSize(i);

			std::vector<wxTreeItemId> sourceTreeIds;
			const wxTreeItemId vertexDeclId = m_meshInfoPage->AppendItem( subMeshId, wxT("Vertex Declaration") );
			m_meshInfoPage->AppendItem( vertexDeclId, wxString(wxT("Size: ")) << vertexSize << wxT(" bytes") );
			Ogre::VertexDeclaration::VertexElementList::const_iterator itor = veList.begin();
			Ogre::VertexDeclaration::VertexElementList::const_iterator end  = veList.end();

			while( itor != end )
			{
				while( itor->getSource() >= sourceTreeIds.size() )
				{
					const size_t vertSize = subMesh->vertexData->vertexDeclaration->getVertexSize(
																				sourceTreeIds.size() );
					wxTreeItemId id = m_meshInfoPage->AppendItem( vertexDeclId, wxString(wxT("Source: ")) <<
																  sourceTreeIds.size() <<
																  wxT("  (") << vertSize << wxT(" bytes )") );
					sourceTreeIds.push_back( id );
				}

				const size_t semantic = itor->getSemantic();
				wxString semanticText;
				if( semantic > 0 && semantic <= Ogre::VES_TANGENT )
					 semanticText = c_VES_SEMANTICS[semantic];
				else
					semanticText = wxString(wxT("VES_UNKNOWN (number = ")) << semantic << wxT(")");

				const wxTreeItemId id = m_meshInfoPage->AppendItem( sourceTreeIds[itor->getSource()],
																	semanticText );
				m_meshInfoPage->AppendItem( id, wxString(wxT("Offset: ")) << itor->getOffset() );
				m_meshInfoPage->AppendItem( id, wxString(wxT("Size: ")) << itor->getSize() );
				m_meshInfoPage->AppendItem( id, wxString(wxT("Index: ")) << itor->getIndex() );

				++itor;
			}
		}
	}

	//Now that we've gather the materials used, and we made sure they're unique, add them
	std::set<Ogre::String>::const_iterator itor = materials.begin();
	std::set<Ogre::String>::const_iterator end  = materials.end();
	while( itor != end )
	{
		m_meshInfoPage->AppendItem( materialId, wxString( itor->c_str(), wxConvUTF8 ) );
		++itor;
	}

	const wxTreeItemId boundId = m_meshInfoPage->AppendItem( rootId, wxT("Bounds") );

	std::stringstream stream;
	const Ogre::Vector3 &vMin( m_meshEntity->getBoundingBox().getMinimum() );
	const Ogre::Vector3 &vMax( m_meshEntity->getBoundingBox().getMaximum() );
	stream << "Minimum: " << vMin;
	m_meshInfoPage->AppendItem( boundId, wxString( stream.str().c_str(), wxConvUTF8 ) ); stream.str("");
	stream << "Maximum: " << vMax;
	m_meshInfoPage->AppendItem( boundId, wxString( stream.str().c_str(), wxConvUTF8 ) ); stream.str("");
	stream << "Center: " << ( (vMin + vMax) * 0.5f );
	m_meshInfoPage->AppendItem( boundId, wxString( stream.str().c_str(), wxConvUTF8 ) ); stream.str("");
	m_meshInfoPage->AppendItem( boundId, wxString(wxT("Width:  ")) << (vMax-vMin).x);
	m_meshInfoPage->AppendItem( boundId, wxString(wxT("Height: ")) << (vMax-vMin).y);
	m_meshInfoPage->AppendItem( boundId, wxString(wxT("Depth:  ")) << (vMax-vMin).z);
	m_meshInfoPage->AppendItem( boundId, wxString(wxT("Area:   ")) << (vMax-vMin).x * (vMax-vMin).y);
	m_meshInfoPage->AppendItem( boundId, wxString(wxT("Volume: ")) << (vMax-vMin).x * (vMax-vMin).y * (vMax-vMin).z );
	m_meshInfoPage->AppendItem( boundId, wxString(wxT("Bounding Radius: ")) << m_meshEntity->getBoundingRadius() );
#undef toStr
}

//-----------------------------------------------------------------------------
//showAxis()
//Description:
//	Shows the XYZ axis at the origin, using a prefabricated mesh model
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::showAxis()
{
	if( m_axisNode )
		return;

	Ogre::Entity *entity = m_sceneManager->createEntity( "AxisEntity", "Axis.mesh", c_InterMeshPermGroup );
	m_axisNode			 = m_sceneManager->getRootSceneNode()->createChildSceneNode();
	m_axisNode->attachObject( entity );

	if( m_meshEntity )
		m_axisNode->setScale( Ogre::Vector3::UNIT_SCALE * m_meshEntity->getBoundingRadius() * 0.2f );

	//Alpha blended, ensure rendered last
	entity->setRenderQueueGroup( Ogre::RENDER_QUEUE_SKIES_LATE );

	m_menuView->Check( wxID_MENUVIEWSHOWAXES, true );
	m_toolBar->ToggleTool( wxID_MENUVIEWSHOWAXES, true );
}

//-----------------------------------------------------------------------------
//hideAxis()
//Description:
//	Hides the XYZ axis at the origin, created in showAxis()
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::hideAxis()
{
	if( m_axisNode )
	{
		Ogre::Entity *entity = static_cast<Ogre::Entity*>(m_axisNode->getAttachedObject(0));
		m_axisNode->getParentSceneNode()->removeAndDestroyChild( m_axisNode->getName() );
		m_sceneManager->destroyEntity( entity );

		m_axisNode = 0;

		m_menuView->Check( wxID_MENUVIEWSHOWAXES, false );
		m_toolBar->ToggleTool( wxID_MENUVIEWSHOWAXES, false );
	}
}

//-----------------------------------------------------------------------------
//toggleShowBones()
//Description:
//	Shows a visual representation of the bones, and turns them off if they
//	were on. Also sets the GUI to right values
//Notes:
//	1) If the mesh doesn't have a skeleton, the GUI remains unchecked
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::toggleShowBones()
{
	if( m_animationPanel->isShowingBones() )
		m_animationPanel->hideBones();
	else
		m_animationPanel->showBones( m_menuView->IsChecked( wxID_MENUSHOWBONENAMES ) );

	const bool bMenuCheck = m_animationPanel->isShowingBones();

	m_menuView->Check( wxID_MENUSHOWBONES, bMenuCheck );
	m_toolBar->ToggleTool( wxID_MENUSHOWBONES, bMenuCheck );
}

//-----------------------------------------------------------------------------
//toggleShowBoneNames()
//Description:
//	If a visual representation of the bones is active (see toggleShowBones),
//	destroys it and then recreates it with bone names too (or without them).
//	Easier to recreate everything (which takes almost no time) than create the
//	names alone
//Notes:
//	1) Unlike toggleShowBones, this function will read what wxWidgets says.
//	   Therefore calling toggleShowBoneNames twice won't enable and then
//	   disable (or viceversa) unless you've toggled wxWidgets control too
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::toggleShowBoneNames()
{
	if( m_animationPanel->isShowingBones() )
	{
		m_animationPanel->hideBones();
		m_animationPanel->showBones( m_menuView->IsChecked( wxID_MENUSHOWBONENAMES ) );
	}
}

//-----------------------------------------------------------------------------
//createGrid()
//Description:
//	
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::createGrid()
{
	Ogre::MeshManager::getSingleton().createPlane( "GridMesh", c_InterMeshPermGroup,
													Ogre::Plane( Ogre::Vector3::UNIT_Y, 0 ),
													m_cellWidth * m_cellSize, m_cellDepth * m_cellSize,
													1, 1, false, 1,
													m_cellWidth, m_cellDepth,
													Ogre::Vector3::UNIT_Z );
}

//-----------------------------------------------------------------------------
//showGrid()
//Description:
//	Displays the grid
//Notes:
//	1) Assumes createGrid() was already called
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::showGrid()
{
	if( m_gridNode )
		return;

	Ogre::Entity *entity = m_sceneManager->createEntity( "Grid", "GridMesh" );
	entity->setMaterialName( "wxOgreMeshViewer/Grid" );
	entity->setCastShadows( false );

	m_gridNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
	m_gridNode->attachObject( entity );

	m_menuView->Check( wxID_MENUSHOWGRID, true );
}

//-----------------------------------------------------------------------------
//hideGrid()
//Description:
//	Hides the grid created by showGrid()
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::hideGrid()
{
	if( m_gridNode )
	{
		Ogre::Entity *entity = static_cast<Ogre::Entity*>(m_gridNode->getAttachedObject(0));
		m_gridNode->getParentSceneNode()->removeAndDestroyChild( m_gridNode->getName() );
		m_sceneManager->destroyEntity( entity );

		m_gridNode = 0;
		//Ogre::MeshManager::getSingleton().remove( "GridMesh" );

		m_menuView->Check( wxID_MENUSHOWGRID, false );
	}
}

//-----------------------------------------------------------------------------
//viewGridSettings()
//Description:
//	Displays a GUI window where the grid settings can be configured
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::viewGridSettings()
{
	GridSettingsImpl gridSettingsImpl( this, m_cellSize, m_cellWidth, m_cellDepth );
	if( gridSettingsImpl.ShowModal() == wxID_OK )
	{
		m_cellSize	= gridSettingsImpl.getCellSize();
		m_cellWidth	= gridSettingsImpl.getCellWidth();
		m_cellDepth	= gridSettingsImpl.getCellDepth();

		Ogre::ResourcePtr resPtr = Ogre::MeshManager::getSingleton().getByName(
																		"GridMesh", c_InterMeshPermGroup );
		Ogre::MeshManager::getSingleton().remove( resPtr );

		//Recreate the grid to reflect changes
		if( m_gridNode )
		{
			hideGrid();
			createGrid();
			showGrid();
		}
		else
			createGrid();
	}
}

//-----------------------------------------------------------------------------
//defaultCamera()
//Description:
//	Defaults camera position to 0, centerY, 0, with the default angle,
//	making it look to the model as a whole
//Notes:
//	1) If no model is loaded, it redirects to originCamera()
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::defaultCamera()
{
	if( m_meshEntity )
	{
		Ogre::Vector3 vCenter = Ogre::Vector3( 0.0f, (m_meshEntity->getBoundingBox().getMaximum().y +
													m_meshEntity->getBoundingBox().getMinimum().y) * 0.5f,
												0.0f );
		/*Ogre::Vector3 vCenter = (m_meshEntity->getBoundingBox().getMaximum() +
								 m_meshEntity->getBoundingBox().getMinimum()) * 0.5f;
		m_camera->setPosition( vCenter + Ogre::Vector3( 0, 1.5f, 1.5f) * m_meshEntity->getBoundingRadius() );
		m_camera->lookAt( vCenter );*/
		m_camera->setPosition( Ogre::Vector3::UNIT_Z * m_meshEntity->getBoundingRadius() * 1.8f );
		m_cameraNode->setOrientation( Ogre::Quaternion::IDENTITY );
		m_cameraNode->pitch( Ogre::Degree( -45.0f ) );
		m_cameraNode->setPosition( vCenter );
	}
	else
		originCamera();
}

//-----------------------------------------------------------------------------
//originCamera()
//Description:
//	Defaults camera position to 0, 0, 0, with the default angle
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::originCamera()
{
	float factor = 10.0f;
	if( m_meshEntity )
		factor = m_meshEntity->getBoundingRadius() * 1.8f;

	m_camera->setPosition( Ogre::Vector3::UNIT_Z * factor );
	m_cameraNode->setOrientation( Ogre::Quaternion::IDENTITY );
	m_cameraNode->pitch( Ogre::Degree( -45.0f ) );
	m_cameraNode->setPosition( Ogre::Vector3::ZERO );
}

//-----------------------------------------------------------------------------
//centerMeshCamera()
//Description:
//	Like defaultCamera(), but pointing at the exact center of the mesh
//Notes:
//	1) Does nothing if the mesh isn't loaded
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::centerMeshCamera()
{
	if( m_meshEntity )
	{
		const Ogre::Vector3 vCenter = (m_meshEntity->getBoundingBox().getMaximum() +
										m_meshEntity->getBoundingBox().getMinimum()) * 0.5f;
		m_camera->setPosition( Ogre::Vector3::UNIT_Z * m_meshEntity->getBoundingRadius() * 1.8f );
		m_cameraNode->setOrientation( Ogre::Quaternion::IDENTITY );
		m_cameraNode->pitch( Ogre::Degree( -45.0f ) );
		m_cameraNode->setPosition( vCenter );
	}
}


//-----------------------------------------------------------------------------
//rotateCamera()
//Description:
//	Manipulates the yaw and pitch of the camera, around and relative to the
//	center of the mesh, when the mouse is moved
//Input:
//	1) X delta, in pixels
//	2) Y delta, in pixels
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::rotateCamera( int x, int y )
{
	m_cameraNode->yaw( Ogre::Degree( -x * 0.4f ), Ogre::Node::TS_PARENT );
	m_cameraNode->pitch( Ogre::Degree( -y * 0.4f ) );
}

//-----------------------------------------------------------------------------
//zoomInCamera()
//Description:
//
//Input:
//	1) Z delta
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::zoomInCamera( Ogre::Real wheelDelta )
{
	float factor = 1.0f;
	if( m_meshEntity )
		factor = m_meshEntity->getBoundingRadius() * 0.5f;

	m_camera->move( Ogre::Vector3::UNIT_Z * wheelDelta * factor );

	//Clamp max zoom in, to keep going use slideCamera
	if( m_camera->getPosition().z < 0.0f )
		m_camera->setPosition( Ogre::Vector3::ZERO );
}

//-----------------------------------------------------------------------------
//moveCamera()
//Description:
//	Moves the whole camera among the relative X and Y axis
//Input:
//	1) X delta, in pixels
//	2) Y delta, in pixels
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::moveCamera( int x, int y )
{
	if( m_meshEntity )
	{
		Ogre::Vector3 vTrans( -x, y, 0 );
		vTrans *= m_meshEntity->getBoundingRadius() * 0.005f;
		m_cameraNode->translate( vTrans, Ogre::Node::TS_LOCAL );
	}
}

//-----------------------------------------------------------------------------
//slideCamera()
//Description:
//	Moves the whole camera among the relative X and Z axis
//Notes:
//	1) slideCamera( x, 0 ) == moveCamera( x, 0 )
//Input:
//	1) X delta, in pixels
//	2) Z delta, in pixels
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::slideCamera( int x, int z )
{
	if( m_meshEntity )
	{
		Ogre::Vector3 vTrans( -x, 0, -z );
		vTrans *= m_meshEntity->getBoundingRadius() * 0.005f;
		m_cameraNode->translate( vTrans, Ogre::Node::TS_LOCAL );
	}
}

//-----------------------------------------------------------------------------
//formatBytes()
//Description:
//	Given an amount of bytes, it converts them to text using an appropiate
//	unit scale (bytes, KBs, MBs, etc).
//Input:
//	1) Amount of bytes
//Returns:
//	1) Formatted text
//-----------------------------------------------------------------------------
wxString MeshyMainFrameImpl::formatBytes( unsigned long bytes ) const
{
	static const wxString si_prefix[] = { wxT("B"), wxT("KB"), wxT("MB"), wxT("GB"), wxT("TB"),
											wxT("PB"), wxT("EB"), wxT("ZB"), wxT("YB") };
	const double base = 1024.0;
	int e = std::min<int>( (int)( log( (double)bytes )/log( base ) ), sizeof(si_prefix) - 1 );
	return wxString::Format( wxT("%1.2f "), bytes / pow( base, e ) ) + si_prefix[e];
}

//-----------------------------------------------------------------------------
//takeSnapshot()
//Description:
//	
//Notes:
//	1) If it fails to save the image due to write access permissions, this
//	   function will call itself again (i.e. recursive) when askLocation ==
//	   false, but this time set to true.
//Input:
//	1) True if ask the user to select a different location
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::takeSnapshot( bool askLocation )
{
	int saveDlgResult = wxID_OK;

	wxString now = wxNow();

	now.Replace( wxT(":"), wxT("."), true );
	wxString newFileName = wxT("Screenshot ") + wxString( m_meshName.c_str(), wxConvUTF8 );

	//Embed animation (skeletal and pose) information into the filename
	for( int i=0; i<2; ++i )
	{
		Ogre::AnimationState *animState = i ? m_animationPanel->getCurrentAnimationState() :
												m_animPosePanel->getCurrentAnimationState();
		if( animState )
		{
			newFileName += wxT(" ") + wxString( animState->getAnimationName().c_str(), wxConvUTF8 ) + wxT("_");
			newFileName	+= wxString::Format( wxT("%0.2f"), animState->getTimePosition() );
		}
	}

	newFileName += wxT(" ") + now + wxT(".png");					//Add the timestamp and extension
	wxString docsDir = wxStandardPaths::Get().GetDocumentsDir();	//Get the default docs folder

	//No default document folder?
	if( docsDir.empty() )
		askLocation = true;

	if( askLocation )
	{
		wxFileDialog saveFileDialog( this, wxT("Save screenshot"), docsDir,
										newFileName, wxT("*.png"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT,
										wxDefaultPosition );
 
		saveDlgResult = saveFileDialog.ShowModal();
		if( saveDlgResult == wxID_OK )
		{
			docsDir		= saveFileDialog.GetDirectory();
			newFileName	= saveFileDialog.GetFilename();
		}
	}

	wxString fullPath = docsDir + wxT("/") + newFileName;

	if( saveDlgResult == wxID_OK )
	{
		try
		{
			m_wxOgreRenderWindow->GetRenderWindow()->writeContentsToFile( Ogre::String(
																			fullPath.mb_str() ) );
			m_statusBar1->SetStatusText( wxT( "Screenshot: " + fullPath ), 0 );
		}
		catch( Ogre::Exception &e )
		{
			if( e.getNumber() == Ogre::Exception::ERR_CANNOT_WRITE_TO_FILE && !askLocation )
			{
				//This could've happened because the documents dir doesn't exist and/or we don't
				//have write access to file. Try once more, this time asking the user where
				//to save the file.
				takeSnapshot( askLocation );
			}
			else
			{
				wxMessageBox( wxT( "Error while saving " ) + fullPath +
							  wxString( e.getFullDescription().c_str(), wxConvUTF8 ),
							  wxT( "Error while saving screenshot" ),
							  wxOK|wxICON_ERROR|wxCENTRE );
			}
		}
	}
}

//-----------------------------------------------------------------------------
//frameStarted()
//Description:
//
//Input:
//	1)
//-----------------------------------------------------------------------------
bool MeshyMainFrameImpl::frameStarted( const Ogre::FrameEvent& evt )
{
	if( m_animationPanel->getPlayAnimation() && m_animationPanel->getCurrentAnimationState() )
	{
		m_animationPanel->getCurrentAnimationState()->addTime( evt.timeSinceLastFrame *
																m_animationPanel->getAnimationRate() );
		m_animationPanel->timeUpdated();
	}

	if( m_animPosePanel->isManualMode() )
		m_animPosePanel->manualModeUpdate();
	else if( m_animPosePanel->getPlayAnimation() && m_animPosePanel->getCurrentAnimationState() )
	{
		m_animPosePanel->getCurrentAnimationState()->addTime( evt.timeSinceLastFrame *
																m_animPosePanel->getAnimationRate() );
		m_animPosePanel->timeUpdated();
	}

	//Only update lights once per frame when the light page is selected
	if( m_mainNotebook->GetSelection() == m_lightPanelPageIdx )
	{
		m_lightsPanel->updateLightParams();
	}

	// update statusbar info
	const Ogre::RenderTarget::FrameStats& stats = m_wxOgreRenderWindow->GetRenderWindow()->
													getStatistics();

	m_statusBar1->SetStatusText( wxString::Format( wxT("%0.2f FPS"), stats.avgFPS ),		1 );
	m_statusBar1->SetStatusText( wxString::Format( wxT("polys: %d"), stats.triangleCount ),	2 );
	m_statusBar1->SetStatusText( wxString::Format( wxT("batches: %d"), stats.batchCount ),	3 );

	if(m_meshEntity)
	{
		const size_t bytes = Ogre::MeshManager::getSingleton().getMemoryUsage();
		m_statusBar1->SetStatusText( formatBytes( bytes ), 4 );
	}


	return true;
}

//-----------------------------------------------------------------------------
//OnMenuSelected()
//Description:
//
//Input:
//	1)
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::OnMenuSelected( wxCommandEvent& event )
{
	switch( event.GetId() )
	{
	case wxID_MENUFILELOAD:
		openMeshDialog();
		break;
	case wxID_MENURELOADMESH:
		if( m_meshEntity )
			openMesh( m_meshDir, m_meshName, true);
		break;
	case wxID_MENULOADRESCFG:
		loadResourcesCfgDialog();
		break;
	case wxID_MENULOADLASTRESCFG:
		//Hard copy (see loadResourcesCfg notes)
		if( wxFileExists( wxString( m_resourcesCfgFile.c_str(), wxConvUTF8 ) ) )
			loadResourcesCfg( wxString( m_resourcesCfgFile.c_str(), wxConvUTF8 ) );
		break;
	case wxID_MENUSAVESNAPSHOT:
		takeSnapshot( false );
		break;
	case wxID_MENUFILEEXIT:
		Close();
		break;
	case wxID_MENUVIEWLOG:
		createLogWindow( true );
		break;
	case wxID_MENUVIEWRENDERWINDOW:
		/*if( !m_wxOgreRenderWindow->IsShown() )
		{
			m_wxOgreRenderWindow->Show( true );
			m_wxAuiManager->AddPane( m_wxOgreRenderWindow, wxBOTTOM, wxT("OGRE Render Window") );
			m_wxAuiManager->Update();
		}*/
		break;
	case wxID_MENUABOUTABOUT:
		showAboutBox();
		break;
	case wxID_MENUVIEWSHOWAXES:
		if( m_axisNode )
			hideAxis();
		else
			showAxis();
		break;
	case wxID_MENUSHOWBONES:
		toggleShowBones();
		break;
	case wxID_MENUSHOWBOUNDINGBOX:
		if( m_meshSceneNode )
		{
			m_meshSceneNode->showBoundingBox( !m_meshSceneNode->getShowBoundingBox() );
			m_menuView->Check( wxID_MENUSHOWBOUNDINGBOX, m_meshSceneNode->getShowBoundingBox() );
			m_toolBar->ToggleTool( wxID_MENUSHOWBOUNDINGBOX, m_meshSceneNode->getShowBoundingBox() );
		}
		break;
	case wxID_MENUSHOWGRID:
		if( m_gridNode )
			hideGrid();
		else
			showGrid();
		break;
	case wxID_MENUTOGGLEWIREFRAME:
		{
			const bool bWireframe = m_camera->getPolygonMode() == Ogre::PM_SOLID;
			m_camera->setPolygonMode( bWireframe ? Ogre::PM_WIREFRAME : Ogre::PM_SOLID );
			m_menuView->Check( wxID_MENUTOGGLEWIREFRAME, bWireframe );
			m_toolBar->ToggleTool( wxID_MENUTOGGLEWIREFRAME, bWireframe );
		}
		break;
	//Submenu Bones
		case wxID_MENUSHOWBONENAMES:
			toggleShowBoneNames();
			break;
		case wxID_MENUCHANGENAMECOLOUR:
			changeBoneNameColour();
			break;
	//Submenu Camera
		case wxID_MENUDEFAULTCAMPOS:
			defaultCamera();
			break;
		case wxID_MENUCAMERAORIGIN:
			originCamera();
			break;
		case wxID_MENUCAMCENTERMESH:
			centerMeshCamera();
			break;
	//End submenu
	case wxID_MENUCHANGEBGCOLOUR:
		changeBackgroundColour();
		break;
	case wxID_MENUGRIDSETTINGS:
		viewGridSettings();
		break;
	}
}

//-----------------------------------------------------------------------------
//OnMouseEvents()
//Description:
//	Called when our render window receives mouse events
//Input:
//	1)
//-----------------------------------------------------------------------------
void MeshyMainFrameImpl::OnMouseEvents( wxMouseEvent &evt )
{
	const int oldX = m_mouseX;
	const int oldY = m_mouseY;

	if( evt.LeftIsDown() )
	{
		evt.GetPosition( &m_mouseX, &m_mouseY );

		if( m_wasLeftPressed )
		{
			if( !evt.ShiftDown() )
				rotateCamera( m_mouseX - oldX, m_mouseY - oldY );
			else
				moveCamera( m_mouseX - oldX, m_mouseY - oldY );
		}
	}
	else if( evt.RightIsDown() )
	{
		evt.GetPosition( &m_mouseX, &m_mouseY );

		if( m_wasRightPressed )
		{
			if( !evt.ShiftDown() )
				zoomInCamera( (m_mouseY - oldY) * 0.05f );
			else
				slideCamera( m_mouseX - oldX, m_mouseY - oldY );
		}
	}

	//TODO: Wheel event seems being missed...
	if( evt.GetWheelRotation() )
		zoomInCamera( -evt.GetWheelRotation()/(float)evt.GetWheelDelta() );

	m_wasLeftPressed	= evt.LeftIsDown();
	m_wasRightPressed	= evt.RightIsDown();

	m_wxOgreRenderWindow->Update();

	evt.Skip();
}

void MeshyMainFrameImpl::messageLogged( const Ogre::String& message, Ogre::LogMessageLevel lml,
										bool maskDebug, const Ogre::String &logName )
{
	bool skipThisMessage = false;
	messageLogged( message, lml, maskDebug, logName, skipThisMessage );
}

void MeshyMainFrameImpl::messageLogged( const Ogre::String& message, Ogre::LogMessageLevel lml,
													bool maskDebug, const Ogre::String &logName, bool& skipThisMessage )
{
	static const wxColour c_darkYellow( 130, 127, 0 );
	static const wxColour c_darkGreen ( 0, 127, 0 );

	// now if we got an error, show the log!
	wxString msg = wxString( (message + "\n").c_str(), wxConvUTF8 );
	
	// black is the default
	m_ogreLog->SetDefaultStyle( wxTextAttr( *wxBLACK ) );
	
	// check for the levels
	if( lml == Ogre::LML_CRITICAL)
		m_ogreLog->SetDefaultStyle( wxTextAttr( *wxRED ) );
	else if( lml == Ogre::LML_TRIVIAL)
		m_ogreLog->SetDefaultStyle( wxTextAttr( c_darkGreen ) );

	// then look for keywords
	if( message.find("you should upgrade it as soon as possible using the OgreMeshUpgrade tool")
		!= Ogre::String::npos )
	{
		m_ogreLog->SetDefaultStyle(wxTextAttr( c_darkYellow ));
	}
	else if( message.find("WARNING:") != Ogre::String::npos )
	{
		m_ogreLog->SetDefaultStyle(wxTextAttr( c_darkYellow ));
	}
	else if( message.find("Can't assign material ") != Ogre::String::npos )
	{
		createLogWindow( true );
		m_ogreLog->SetDefaultStyle(wxTextAttr( *wxRED ));
	}
	else if( message.find("Compiler error: ") != Ogre::String::npos )
	{
		createLogWindow( true );
		m_ogreLog->SetDefaultStyle(wxTextAttr( *wxRED ));
	}
	else if( message.find("Error loading texture ") != Ogre::String::npos )
	{
		createLogWindow( true );
		m_ogreLog->SetDefaultStyle(wxTextAttr( *wxRED ));
	}
	else if( message.find("Mesh: Loading ") != Ogre::String::npos )
	{
		m_ogreLog->SetDefaultStyle(wxTextAttr( c_darkGreen ));
	}
	else if( message.find("Loading 2D Texture") != Ogre::String::npos )
	{
		m_ogreLog->SetDefaultStyle(wxTextAttr( c_darkGreen ));
	}
	else if( message.find("Texture: ") != Ogre::String::npos )
	{
		m_ogreLog->SetDefaultStyle(wxTextAttr( c_darkGreen ));
	}

	if( message.find("D3D9 Device ") != Ogre::String::npos )
	{
		// discard this ...
		return;
	}
	else if( message.find("D3D9 device: ") != Ogre::String::npos )
	{
		// discard this ...
		return;
	}
	else if( message.find("!!! Direct3D Device successfully restored.") != Ogre::String::npos )
	{
		// discard this ...
		return;
	}
	
	m_ogreLog->AppendText( msg );

	if( m_ogreLog->IsShown() )
	{
		//HACK: Seems only way to smoothly move update it (may be wxWidgets bug?)
		m_ogreLog->SetFocus();
	}
}
