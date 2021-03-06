#ifndef __LightsPanel__
#define __LightsPanel__

/**
@file
Subclass of wxLightsPanel, which is generated by wxFormBuilder.
*/

#include "../wxOgreMeshViewerMainFrame.h"
#include <OgrePrerequisites.h>
#include <OgreConfigFile.h>

#include <vector>

//// end generated include

/** Implementing wxLightsPanel */
class LightsPanel : public wxLightsPanel
{
	Ogre::SceneManager			*m_sceneManager;
	Ogre::Camera				*m_camera;

	std::vector<Ogre::Light*>	m_lights;

	void addDefaultLight();
	void enableUIforType();
	Ogre::ColourValue pickColour( const Ogre::ColourValue &inColour );

	protected:
		// Handlers for wxLightsPanel events.
		void OnLightSelectionChange( wxCommandEvent& event );
		void OnButtonClick( wxCommandEvent& event );
		void OnLightTypeChange( wxCommandEvent& event );
	public:
		//Be sure all premade lights have been created by the time the constructor is called!
		LightsPanel( wxWindow* parent, Ogre::SceneManager *sceneManager, Ogre::Camera *camera );
	//// end generated class members

		void updateLightParams();
		void saveSettings( std::ofstream &myFile );
		void loadSettings( const Ogre::ConfigFile::SettingsMultiMap &settings );

		void removeDefaultLight();
	
};

#endif // __LightsPanel__
