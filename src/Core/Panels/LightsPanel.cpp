#include "Core/Panels/LightsPanel.h"

#include <wx/colordlg.h>

#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include <OgreLight.h>

//Convert decimal integer to string containing hexadecimal
//representation. Supports returning up to 0xFF. Note the "0x"
//prefix isn't added.
//Assumes buf has at least 3 bytes
static wxChar hexArray[] = wxT("0123456789ABCDEF");
void decToHex( unsigned char dec, wxChar *buf )
{
    //Divide by 16 and get the remainder
    unsigned char firstDigit  = dec >> 4;
    unsigned char secondDigit = dec & 0x0F; //Or dec % 16
    buf[0] = hexArray[firstDigit];
    buf[1] = hexArray[secondDigit];
    buf[2] = 0;
}

//TODO: Doesn't consider endianess
wxString decToHex( unsigned long dec )
{
	wxString retVal;
	//Strict aliasing isn't broken because of type char
	unsigned char *decPtr = reinterpret_cast<unsigned char*>(&dec);
	for( int i=0; i<sizeof(unsigned long); ++i )
	{
		//Calls the 2-byte, unsigned char variant
		wxChar outBuff[3];
		decToHex( *decPtr++, outBuff );
		retVal += outBuff;
	}

	return retVal;
}

LightsPanel::LightsPanel( wxWindow* parent, Ogre::SceneManager *sceneManager, Ogre::Camera *camera ) :
		wxLightsPanel( parent ),
		m_sceneManager( sceneManager ),
		m_camera( camera )
{
	//Create the default light
	Ogre::Light *light = m_sceneManager->createLight( "DefaultLight" );
	light->setType( Ogre::Light::LT_DIRECTIONAL );
	light->setDirection( Ogre::Vector3( 1.0f, -1.0f, -1.0f ).normalisedCopy() );
	light->setDiffuseColour( 1.0f, 1.0f, 1.0f );
	light->setSpecularColour( 0.25f, 0.25f, 0.25f );

	//Add all lights from scene that were created in the past
	Ogre::SceneManager::MovableObjectIterator itor = m_sceneManager->getMovableObjectIterator(
														Ogre::LightFactory::FACTORY_TYPE_NAME );

	while( itor.hasMoreElements() )
	{
		Ogre::Light *light = static_cast<Ogre::Light*>(itor.getNext());
		m_lights.push_back( light );
		m_lightsChoice->Append( wxString( light->getName().c_str(), wxConvUTF8 ) );
	}

	m_lightsChoice->SetSelection( 0 );
	wxCommandEvent emptyEvt;
	OnLightSelectionChange( emptyEvt );
}

//-----------------------------------------------------------------------------
//addDefaultLight()
//Description:
//	Creates a new light, with default parameters, some are based on camera's
//	position and orientation
//-----------------------------------------------------------------------------
void LightsPanel::addDefaultLight()
{
	Ogre::Light *newLight = m_sceneManager->createLight();
	newLight->setPosition( m_camera->getDerivedPosition() );
	newLight->setDirection( m_camera->getDerivedDirection() );
	newLight->setType( Ogre::Light::LT_DIRECTIONAL );

	m_lights.push_back( newLight );
	m_lightsChoice->Append( wxString( newLight->getName().c_str(), wxConvUTF8 ) );
	m_lightsChoice->SetSelection( m_lightsChoice->GetCount() - 1 );

	wxCommandEvent emptyEvt;
	OnLightSelectionChange( emptyEvt );
}

//-----------------------------------------------------------------------------
//enableUIforType()
//Description:
//	Enables or disables GUI elements depending on the current light type
//	from the selected light
//Notes:
//	1) The type is retrieved from the actual Ogre data, not what the GUI says
//-----------------------------------------------------------------------------
void LightsPanel::enableUIforType()
{
	int idx = m_lightsChoice->GetSelection();
	if( (unsigned int)idx < m_lights.size() )
	{
		const Ogre::Light *light = m_lights[idx];

		//Stuff disabled for directional lights (like position and attenuation)
		bool bEnablePos = light->getType() != Ogre::Light::LT_DIRECTIONAL;
		m_lightPosX->Enable( bEnablePos );
		m_lightPosY->Enable( bEnablePos );
		m_lightPosZ->Enable( bEnablePos );

		m_textRange->Enable(		bEnablePos );
		m_textQuadratic->Enable(	bEnablePos );
		m_textLinear->Enable(		bEnablePos );
		m_textConstant->Enable(		bEnablePos );

		//Stuff disabled for point lights (direction)
		bool bEnableDir = light->getType() != Ogre::Light::LT_POINT;
		m_lightDirX->Enable( bEnableDir );
		m_lightDirY->Enable( bEnableDir );
		m_lightDirZ->Enable( bEnableDir );

		//Stuff only enabled for spot lights (radius, falloff)
		bool bEnableSpot = light->getType() == Ogre::Light::LT_SPOTLIGHT;
		m_textInnerRadius->Enable(	bEnableSpot );
		m_textOuterRadius->Enable(	bEnableSpot );
		m_textFalloff->Enable(		bEnableSpot );
	}
}

//-----------------------------------------------------------------------------
//pickColour()
//Description:
//	Displays a colour picker dialog with the selected input and returns
//	the new value.
//Notes:
//	1) The Alpha component is left untouched
//Input:
//	1) Current colour value
//Returns:
//	1) New colour value
//-----------------------------------------------------------------------------
Ogre::ColourValue LightsPanel::pickColour( const Ogre::ColourValue &inColour )
{
	Ogre::ColourValue retVal = inColour;

	wxColourData colourData;
	colourData.SetChooseFull( true );
	colourData.SetColour( wxColour( inColour.getAsABGR() ) );
	colourData.SetCustomColour( 0, wxColour( inColour.getAsABGR() ) );

	wxColourDialog colourDlg( this, &colourData );

	if( colourDlg.ShowModal() == wxID_OK )
	{
		float alpha = retVal.a;
		retVal.setAsABGR( colourDlg.GetColourData().GetColour().GetPixel() );
		retVal.a = alpha;
	}

	return retVal;
}

void LightsPanel::OnLightSelectionChange( wxCommandEvent& event )
{
	enableUIforType();

	//Now fill the elements (even unused ones)
	int idx = m_lightsChoice->GetSelection();
	if( (unsigned int)idx < m_lights.size() )
	{
		const Ogre::Light *light = m_lights[idx];
		m_choiceLightType->SetSelection( static_cast<int>(light->getType()) );

		m_lightPosX->SetValue( wxString() << light->getPosition().x );
		m_lightPosY->SetValue( wxString() << light->getPosition().y );
		m_lightPosZ->SetValue( wxString() << light->getPosition().z );

		m_lightDirX->SetValue( wxString() << light->getDirection().x );
		m_lightDirY->SetValue( wxString() << light->getDirection().y );
		m_lightDirZ->SetValue( wxString() << light->getDirection().z );

		m_spinPower->SetValue( wxString() << light->getPowerScale() );

		m_textDiffuseColour->SetValue( decToHex( light->getDiffuseColour().getAsABGR() ) );
		m_diffusePickColour->SetBackgroundColour( wxColour( light->getDiffuseColour().getAsABGR() ) );
		m_textSpecularColour->SetValue( decToHex( light->getSpecularColour().getAsABGR() ) );
		m_specularPickColour->SetBackgroundColour( wxColour( light->getSpecularColour().getAsABGR() ) );

		m_textRange->SetValue( wxString() << light->getAttenuationRange() );
		m_textQuadratic->SetValue( wxString() << light->getAttenuationQuadric() );
		m_textLinear->SetValue( wxString() << light->getAttenuationLinear() );
		m_textConstant->SetValue( wxString() << light->getAttenuationConstant() );

		m_textInnerRadius->SetValue( wxString() << light->getSpotlightInnerAngle().valueDegrees() );
		m_textOuterRadius->SetValue( wxString() << light->getSpotlightOuterAngle().valueDegrees() );
		m_textFalloff->SetValue( wxString() << light->getSpotlightFalloff() );
	}
}

void LightsPanel::OnButtonClick( wxCommandEvent& event )
{
	wxCommandEvent emptyEvt;

// TODO: Implement OnButtonClick
	if( event.GetId() == wxID_ADDLIGHT )
	{
		addDefaultLight();
	}
	else if( event.GetId() == wxID_REMOVELIGHT )
	{
		int light = m_lightsChoice->GetSelection();
		if( (unsigned int)light < m_lights.size() )
		{
			m_sceneManager->destroyLight( m_lights[light] );
			m_lights.erase( m_lights.begin() + light );
			m_lightsChoice->Delete( light );
			m_lightsChoice->SetSelection( std::min<unsigned int>( m_lightsChoice->GetCount() - 1, light ) );

			OnLightSelectionChange( emptyEvt );
		}
	}
	else if( event.GetId() == wxID_PICKDIFFUSE )
	{
		int idx = m_lightsChoice->GetSelection();
		if( (unsigned int)idx < m_lights.size() )
		{
			Ogre::Light *light = m_lights[idx];
			light->setDiffuseColour( pickColour( light->getDiffuseColour() ) );
			OnLightSelectionChange( emptyEvt );
		}
	}
	else if( event.GetId() == wxID_PICKSPECULAR )
	{
		int idx = m_lightsChoice->GetSelection();
		if( (unsigned int)idx < m_lights.size() )
		{
			Ogre::Light *light = m_lights[idx];
			light->setSpecularColour( pickColour( light->getSpecularColour() ) );
			OnLightSelectionChange( emptyEvt );
		}
	}
}

void LightsPanel::OnLightTypeChange( wxCommandEvent& event )
{
	int idx		= m_lightsChoice->GetSelection();
	int type	= m_choiceLightType->GetSelection();
	if( (unsigned int)idx < m_lights.size() )
	{
		//First save light parameters, if we haven't already.
		updateLightParams();

		Ogre::Light *light = m_lights[idx];
		light->setType( static_cast<Ogre::Light::LightTypes>(type) );
		enableUIforType();
	}
}

//-----------------------------------------------------------------------------
//updateLightParams()
//Description:
//	Copies the GUI params back to the actual light
//	When the light page is active, ought to be called once per frame
//	OnLightSelectionChange does the opposite (copy parameters from the light
//	to the GUI)
//Notes:
//	1) LightType is not updated here (it is done through events)
//-----------------------------------------------------------------------------
void LightsPanel::updateLightParams()
{
	int idx = m_lightsChoice->GetSelection();

	if( (unsigned int)idx < m_lights.size() )
	{
		Ogre::Light *light = m_lights[idx];

		double dValue;

		//Position, only valid parameters are considered
		Ogre::Vector3 vTmp = light->getPosition();
		if( m_lightPosX->GetValue().ToDouble( &dValue ) )
			vTmp.x = static_cast<float>(dValue);
		if( m_lightPosY->GetValue().ToDouble( &dValue ) )
			vTmp.y = static_cast<float>(dValue);
		if( m_lightPosZ->GetValue().ToDouble( &dValue ) )
			vTmp.z = static_cast<float>(dValue);

		light->setPosition( vTmp );

		//Direction, only valid parameters are considered
		vTmp = light->getDirection();
		if( m_lightDirX->GetValue().ToDouble( &dValue ) )
			vTmp.x = static_cast<float>(dValue);
		if( m_lightDirY->GetValue().ToDouble( &dValue ) )
			vTmp.y = static_cast<float>(dValue);
		if( m_lightDirZ->GetValue().ToDouble( &dValue ) )
			vTmp.z = static_cast<float>(dValue);

		//vTmp.normalise();
		light->setDirection( vTmp );

		//Set power
		/*if( m_spinPower->GetValue().ToDouble( &dValue ) )
			light->setPowerScale( static_cast<float>(dValue) );*/

		unsigned long lValue;
		Ogre::ColourValue cTmp;
		wxString colourString;

		//Get diffuse colour
		colourString = wxT("0x") + m_textDiffuseColour->GetValue();
		if( colourString.size() < 10 )
		{
			while( 10 - colourString.size() )
				colourString += wxT("0");
		}
		if( colourString.ToULong( &lValue, 0 ) )
		{
			cTmp.setAsRGBA( lValue );
			light->setDiffuseColour( cTmp );
			m_diffusePickColour->SetBackgroundColour( wxColour( light->getDiffuseColour().getAsABGR() ) );
		}
		//Get specular colour
		colourString = wxT("0x") + m_textSpecularColour->GetValue();
		if( colourString.size() < 10 )
		{
			while( 10 - colourString.size() )
				colourString += wxT("0");
		}
		if( colourString.ToULong( &lValue, 0 ) )
		{
			cTmp.setAsRGBA( lValue );
			light->setSpecularColour( cTmp );
			m_specularPickColour->SetBackgroundColour( wxColour( light->getSpecularColour().getAsABGR() ) );
		}

		//Attenuation
		double dValues[4];
		if( !m_textRange->GetValue().ToDouble( &dValues[0] ) )
			dValues[0] = light->getAttenuationRange();
		if( !m_textQuadratic->GetValue().ToDouble( &dValues[1] ) )
			dValues[1] = light->getAttenuationQuadric();
		if( !m_textLinear->GetValue().ToDouble( &dValues[2] ) )
			dValues[2] = light->getAttenuationLinear();
		if( !m_textConstant->GetValue().ToDouble( &dValues[3] ) )
			dValues[3] = light->getAttenuationConstant();

		light->setAttenuation( dValues[0], dValues[3], dValues[2], dValues[1] );

		//Spotlight params
		if( m_textInnerRadius->GetValue().ToDouble( &dValue ) )
			light->setSpotlightOuterAngle( Ogre::Degree( dValue ) );
		if( m_textOuterRadius->GetValue().ToDouble( &dValue ) )
			light->setSpotlightOuterAngle( Ogre::Degree( dValue ) );
		if( m_textFalloff->GetValue().ToDouble( &dValue ) )
			light->setSpotlightFalloff( dValue );
	}
}

//-----------------------------------------------------------------------------
//saveSettings()
//Description:
//	Writes all light settings to a file.
//Notes:
//	1) Assumes the file is already opened and has write access
//Input:
//	1) The stream to the file
//-----------------------------------------------------------------------------
void LightsPanel::saveSettings( std::ofstream &myFile )
{
	for( size_t i=0; i<m_lights.size(); ++i )
	{
		myFile << "\n";
		myFile << "[Light" << i << "]"	<< "\n";
		myFile << "Type = "		 << m_lights[i]->getType()			<< "\n";
		myFile << "PositionX = " << m_lights[i]->getPosition().x	<< "\n";
		myFile << "PositionY = " << m_lights[i]->getPosition().y	<< "\n";
		myFile << "PositionZ = " << m_lights[i]->getPosition().z	<< "\n";
		myFile << "DirectionX = " << m_lights[i]->getDirection().x	<< "\n";
		myFile << "DirectionY = " << m_lights[i]->getDirection().y	<< "\n";
		myFile << "DirectionZ = " << m_lights[i]->getDirection().z	<< "\n";
		myFile << "Diffuse = "	<< std::hex << m_lights[i]->getDiffuseColour().getAsABGR() << "\n";
		myFile << "Specular = "	<< std::hex << m_lights[i]->getSpecularColour().getAsABGR()<< "\n";
		myFile << "Range = "	<< m_lights[i]->getAttenuationRange()	<< "\n";
		myFile << "Quadric = "	<< m_lights[i]->getAttenuationQuadric()	<< "\n";
		myFile << "Linear = "	<< m_lights[i]->getAttenuationLinear()	<< "\n";
		myFile << "Constant = "	<< m_lights[i]->getAttenuationConstant()<< "\n";
		myFile << "InnerRad = "	<< m_lights[i]->getSpotlightInnerAngle().valueDegrees() << "\n";
		myFile << "OuterRad = "	<< m_lights[i]->getSpotlightOuterAngle().valueDegrees() << "\n";
		myFile << "Falloff = "	<< m_lights[i]->getSpotlightFalloff()	<< "\n";
	}
}

//-----------------------------------------------------------------------------
//loadSettings()
//Description:
//	Loads lights saved from a configuration file
//Notes:
//	1) If this gets called at least once, this means the default light is
//	   no longer necessary. Call removeDefaultLight() afterwards
//Input:
//	1) SettingsMultiMap to parse
//-----------------------------------------------------------------------------
void LightsPanel::loadSettings( const Ogre::ConfigFile::SettingsMultiMap &settings )
{
	Ogre::Vector3 vPos, vDir;
	Ogre::ColourValue diffuse, specular;
	float atten[4];

	Ogre::Light *light = m_sceneManager->createLight();

	Ogre::ConfigFile::SettingsMultiMap::const_iterator i;
	for( i = settings.begin(); i != settings.end(); ++i )
	{
		if( i->first == "Type" )
			light->setType( static_cast<Ogre::Light::LightTypes>(std::min<unsigned long>(
											Ogre::Light::LT_SPOTLIGHT+1,
											Ogre::StringConverter::parseUnsignedLong( i->second ) ) ) );
		else if( i->first == "PositionX" )
			vPos.x = Ogre::StringConverter::parseReal( i->second );
		else if( i->first == "PositionY" )
			vPos.y = Ogre::StringConverter::parseReal( i->second );
		else if( i->first == "PositionZ" )
			vPos.z = Ogre::StringConverter::parseReal( i->second );

		else if( i->first == "DirectionX" )
			vDir.x = Ogre::StringConverter::parseReal( i->second );
		else if( i->first == "DirectionY" )
			vDir.y = Ogre::StringConverter::parseReal( i->second );
		else if( i->first == "DirectionZ" )
			vDir.z = Ogre::StringConverter::parseReal( i->second );

		else if( i->first == "Diffuse" )
		{
			unsigned long lValue;
			if( wxString( wxT("0x") + wxString( i->second.c_str(), wxConvUTF8 ) ).ToULong( &lValue, 0 ) )
				diffuse.setAsABGR( lValue );
		}

		else if( i->first == "Specular" )
		{
			unsigned long lValue;
			if( wxString( wxT("0x") + wxString( i->second.c_str(), wxConvUTF8 ) ).ToULong( &lValue, 0 ) )
				specular.setAsABGR( lValue );
		}

		else if( i->first == "Range" )
			atten[0] = Ogre::StringConverter::parseReal( i->second );
		else if( i->first == "Quadric" )
			atten[1] = Ogre::StringConverter::parseReal( i->second );
		else if( i->first == "Linear" )
			atten[2] = Ogre::StringConverter::parseReal( i->second );
		else if( i->first == "Constant" )
			atten[3] = Ogre::StringConverter::parseReal( i->second );

		else if( i->first == "InnerRad" )
			light->setSpotlightInnerAngle( Ogre::Degree( Ogre::StringConverter::parseReal( i->second ) ) );
		else if( i->first == "OuterRad" )
			light->setSpotlightOuterAngle( Ogre::Degree( Ogre::StringConverter::parseReal( i->second ) ) );
		else if( i->first == "Falloff" )
			light->setSpotlightFalloff( Ogre::StringConverter::parseReal( i->second ) );
	}

	light->setAttenuation( atten[0], atten[3], atten[2], atten[1] );
	light->setPosition( vPos );
	light->setDirection( vDir );
	light->setDiffuseColour( diffuse );
	light->setSpecularColour( specular );

	m_lights.push_back( light );
	m_lightsChoice->Append( wxString( light->getName().c_str(), wxConvUTF8 ) );
	m_lightsChoice->SetSelection( m_lightsChoice->GetCount() - 1 );

	wxCommandEvent emptyEvt;
	OnLightSelectionChange( emptyEvt );
}

//-----------------------------------------------------------------------------
//removeDefaultLight()
//Description:
//	Removes, if exists, the default light
//-----------------------------------------------------------------------------
void LightsPanel::removeDefaultLight()
{
	if( m_sceneManager->hasLight( "DefaultLight" ) )
	{
		//There should be at least the default light and a custom loaded one
		assert( m_lights.size() > 1 );

		m_sceneManager->destroyLight( "DefaultLight" );

		m_lights.erase( m_lights.begin() );
		m_lightsChoice->Delete( 0 );
		m_lightsChoice->SetSelection( m_lightsChoice->GetCount() - 1 );

		wxCommandEvent emptyEvt;
		OnLightSelectionChange( emptyEvt );
	}
}