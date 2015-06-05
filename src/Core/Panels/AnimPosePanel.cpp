//----------------------------------------------------------------------
//	Copyright (C) 2011-2013 Matias N. Goldberg ("dark_sylinc")
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

#include "Core/Panels/AnimPosePanel.h"

#include <wx/msgdlg.h>

#include <OgreEntity.h>
#include <OgreAnimation.h>
#include <OgreKeyFrame.h>
#include <OgreMesh.h>

//Avoid accidentally loading a mesh which matches an animation with this name. (Seriously? that name?)
const Ogre::String c_manualPoseAnimationName = "PoseManual#!@@!_  Q";

AnimPosePanel::AnimPosePanel( wxWindow* parent, Ogre::SceneManager *sceneManager ) :
		wxAnimPosePanel( parent ),
		m_loadedEntity( 0 ),
		m_sceneManager( sceneManager ),
		m_currentAnimationState( 0 ),
		m_currentAnimation( 0 ),
		m_playAnimation( false ),
		m_animationRate( 1.0f ),
		m_manualMode( false )
{

}

//-----------------------------------------------------------------------------
//createKeyScrolls()
//Description:
//	Creates all the GUI controls for seeing and manipulating the pose keys
//Notes:
//	1) Assumes mesh is already loaded
//-----------------------------------------------------------------------------
void AnimPosePanel::createKeyScrolls()
{
	const Ogre::MeshPtr &baseMesh = m_loadedEntity->getMesh();

	for( unsigned short i=0; i<baseMesh->getPoseCount(); ++i ) 
	{
		Ogre::Pose *pose = baseMesh->getPose( i );

		PoseCtrls poseCtrl;

		wxBoxSizer* bSizer40;
		bSizer40 = new wxBoxSizer( wxHORIZONTAL );

		wxStaticText *poseName = new wxStaticText( m_scrolledPosePanel, wxID_ANY,
													wxString( pose->getName().c_str(), wxConvUTF8 ),
													wxDefaultPosition, wxDefaultSize, 0 );
		poseName->Wrap( -1 );
		bSizer40->Add( poseName, 1, wxALL, 5 );

		poseCtrl.weightText = new wxTextCtrl( m_scrolledPosePanel, wxID_POSETEXTSTART+i, wxT("0"),
												wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
		bSizer40->Add( poseCtrl.weightText, 0, wxALL, 5 );

		m_scrollWindowSizer->Add( bSizer40, 0, wxEXPAND, 5 );

		poseCtrl.weightSlider = new wxSlider( m_scrolledPosePanel, wxID_POSESLIDERSTART+i, 0, 0, 1000,
												wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
		m_scrollWindowSizer->Add( poseCtrl.weightSlider, 0, wxALL|wxEXPAND, 5 );

		poseCtrl.weightText->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AnimPosePanel::OnPoseWeightBox ), NULL, this );
		poseCtrl.weightSlider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( AnimPosePanel::OnPoseWeightSlider ), NULL, this );
		poseCtrl.weightSlider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( AnimPosePanel::OnPoseWeightSlider ), NULL, this );
		poseCtrl.weightSlider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( AnimPosePanel::OnPoseWeightSlider ), NULL, this );
		poseCtrl.weightSlider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( AnimPosePanel::OnPoseWeightSlider ), NULL, this );
		poseCtrl.weightSlider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( AnimPosePanel::OnPoseWeightSlider ), NULL, this );
		poseCtrl.weightSlider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( AnimPosePanel::OnPoseWeightSlider ), NULL, this );
		poseCtrl.weightSlider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( AnimPosePanel::OnPoseWeightSlider ), NULL, this );
		poseCtrl.weightSlider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( AnimPosePanel::OnPoseWeightSlider ), NULL, this );
		poseCtrl.weightSlider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( AnimPosePanel::OnPoseWeightSlider ), NULL, this );

		poseCtrl.weightText->SetEditable( false );
		poseCtrl.weightSlider->Enable( false );

		//Save our pointer to the controls for this pose
		m_poseCtrls.push_back( poseCtrl );
	}

	//Repaint the scrolled window dialog
	m_scrolledPosePanel->Layout();
	wxSizeEvent emptyEvt;
	m_scrolledPosePanel->HandleOnSize( emptyEvt );
	/*wxSizeEvent event( m_scrolledPosePanel->GetSize(), m_scrolledPosePanel->GetId() );
	event.SetEventObject( m_scrolledPosePanel );
	m_scrolledPosePanel->GetEventHandler()->ProcessEvent( event );*/
}

//-----------------------------------------------------------------------------
//removeKeyScrolls()
//Description:
//	Creates all GUI controls created with createKeyScrolls()
//-----------------------------------------------------------------------------
void AnimPosePanel::removeKeyScrolls()
{
	std::vector<PoseCtrls>::const_iterator itor = m_poseCtrls.begin();
	std::vector<PoseCtrls>::const_iterator end  = m_poseCtrls.end();

	while( itor != end )
	{
		itor->weightText->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AnimPosePanel::OnPoseWeightBox ), NULL, this );
		itor->weightSlider->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( AnimPosePanel::OnPoseWeightSlider ), NULL, this );
		itor->weightSlider->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( AnimPosePanel::OnPoseWeightSlider ), NULL, this );
		itor->weightSlider->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( AnimPosePanel::OnPoseWeightSlider ), NULL, this );
		itor->weightSlider->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( AnimPosePanel::OnPoseWeightSlider ), NULL, this );
		itor->weightSlider->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( AnimPosePanel::OnPoseWeightSlider ), NULL, this );
		itor->weightSlider->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( AnimPosePanel::OnPoseWeightSlider ), NULL, this );
		itor->weightSlider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( AnimPosePanel::OnPoseWeightSlider ), NULL, this );
		itor->weightSlider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( AnimPosePanel::OnPoseWeightSlider ), NULL, this );
		itor->weightSlider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( AnimPosePanel::OnPoseWeightSlider ), NULL, this );

		++itor;
	}

	//Discard our saved pointers, and tell wxWidgets to destroy them
	m_poseCtrls.clear();
	m_scrollWindowSizer->Clear( true );

	//Repaint the scrolled window dialog
	m_scrolledPosePanel->Layout();
	wxSizeEvent emptyEvt;
	m_scrolledPosePanel->HandleOnSize( emptyEvt );
}

//-----------------------------------------------------------------------------
//updateKeyScrolls()
//Description:
//	Updates the shape sliders with the weights of the current time in the
//	current animation being played
//Notes:
//	1) Assumes GUI controls have already been created with createKeyScrolls()
//	2) Assumes a valid animation is currently selected
//-----------------------------------------------------------------------------
void AnimPosePanel::updateKeyScrolls()
{
	assert( m_currentAnimation );

	//Holds which poses weren't updated (we need to reset them to 0). We could
	//reset them all at once and then overwrite those we use, but it causes flickering
	std::vector<bool> writtenPoses;
	writtenPoses.resize( m_poseCtrls.size(), false );

	//Update the shape sliders
	//Get the key frames at this time of animation
	Ogre::KeyFrame *keyFrame1, *keyFrame2;
	const Ogre::TimeIndex timeIdx( m_currentAnimation->_getTimeIndex( m_currentAnimationState->
																		getTimePosition() ) );

	Ogre::Animation::VertexTrackIterator vertexTrackItor = m_currentAnimation->getVertexTrackIterator();
	while( vertexTrackItor.hasMoreElements() )
	{
		const Ogre::VertexAnimationTrack *vertexTrack = vertexTrackItor.getNext();
		if( vertexTrack->getAnimationType() != Ogre::VAT_POSE )
			continue;

		const float w = vertexTrack->getKeyFramesAtTime( timeIdx, &keyFrame1, &keyFrame2 );

		Ogre::VertexPoseKeyFrame *key1 = static_cast<Ogre::VertexPoseKeyFrame*>(keyFrame1);
		Ogre::VertexPoseKeyFrame *key2 = static_cast<Ogre::VertexPoseKeyFrame*>(keyFrame2);

		// For each pose reference in key 1, we need to locate the entry in
		// key 2 and interpolate the influence
		const Ogre::VertexPoseKeyFrame::PoseRefList& poseList1 = key1->getPoseReferences();
		const Ogre::VertexPoseKeyFrame::PoseRefList& poseList2 = key2->getPoseReferences();

		Ogre::VertexPoseKeyFrame::PoseRefList::const_iterator it1 = poseList1.begin();
		Ogre::VertexPoseKeyFrame::PoseRefList::const_iterator en1 = poseList1.end();

		while( it1 != en1 )
		{
			float startInfluence	= it1->influence;
			float endInfluence		= 0;

			//Search for entry in keyframe 2 list (if not there, will be 0)
			Ogre::VertexPoseKeyFrame::PoseRefList::const_iterator it2 = poseList2.begin();
			Ogre::VertexPoseKeyFrame::PoseRefList::const_iterator en2 = poseList2.end();
			while( it2 != en2 )
			{
				if( it1->poseIndex == it2->poseIndex )
				{
					endInfluence = it2->influence;
					break;
				}

				++it2;
			}

			// Interpolate influence
			const float influence = startInfluence + w * ( endInfluence - startInfluence );

			//Update GUI controls
			m_poseCtrls[it1->poseIndex].weightSlider->SetValue( static_cast<int>(influence * 1000.0f) );
			m_poseCtrls[it1->poseIndex].weightText->SetValue( wxString( wxString::Format( wxT("%.4f"),
																		influence ) ) );

			writtenPoses[it1->poseIndex] = true;

			++it1;
		}

		// Now deal with any poses in key 2 which are not in key 1
		Ogre::VertexPoseKeyFrame::PoseRefList::const_iterator it2 = poseList2.begin();
		Ogre::VertexPoseKeyFrame::PoseRefList::const_iterator en2 = poseList2.end();
		while( it2 != en2 )
		{
			bool bFound = false;

			Ogre::VertexPoseKeyFrame::PoseRefList::const_iterator it1 = poseList1.begin();
			Ogre::VertexPoseKeyFrame::PoseRefList::const_iterator en1 = poseList1.end();

			while( it1 != en1 && !bFound )
			{
				if( it1->poseIndex == it2->poseIndex )
					bFound = true;
				++it1;
			}

			if( !bFound )
			{
				const float influence = w * it2->influence;
				//Update GUI controls
				m_poseCtrls[it2->poseIndex].weightSlider->SetValue( static_cast<int>
																	(influence * 1000.0f) );
				m_poseCtrls[it2->poseIndex].weightText->SetValue( wxString(
																	wxString::Format( wxT("%.4f"),
																	influence ) ) );

				writtenPoses[it2->poseIndex] = true;
			}

			++it2;
		}
	}

	for( size_t i=0; i<m_poseCtrls.size(); ++i )
	{
		if( !writtenPoses[i] )
		{
			m_poseCtrls[i].weightText->SetValue( wxT("0") );
			m_poseCtrls[i].weightSlider->SetValue( 0 );
		}
	}
}

//-----------------------------------------------------------------------------
//setManualMode()
//Description:
//	Toogles GUI controls for manually controlling the pose keys between
//	read-only, and read/write.
//	Also disables/enables playback buttons
//Input:
//	1) True to enter manual mode and enable a lot of controls for write access
//-----------------------------------------------------------------------------
void AnimPosePanel::setManualMode( bool bManualMode )
{
	//Reset all unused sliders to 0.0 in case the user is messing with them (they're read-only)
	std::vector<PoseCtrls>::const_iterator itor = m_poseCtrls.begin();
	std::vector<PoseCtrls>::const_iterator end  = m_poseCtrls.end();

	while( itor != end )
	{
		itor->weightText->SetEditable( bManualMode );
		itor->weightSlider->Enable( bManualMode );
		++itor;
	}

	m_playSkelAnimation->Enable( !bManualMode );
	m_StopSkelAnimation->Enable( !bManualMode );
	m_pauseSkelAnimation->Enable( !bManualMode );
	m_animationTimeValue->Enable( !bManualMode );
	m_animationTimeSlider->Enable( !bManualMode );
	m_animLoopCheckBox->Enable( !bManualMode );
	m_rateSkelAnim->Enable( !bManualMode );
	m_resetWeights->Enable( bManualMode );

	m_manualMode = bManualMode;
}

//-----------------------------------------------------------------------------
//updatePose()
//Description:
//	Updates the manual pose with the new influence/weight. Modifies the Mesh'
//	animation for that, which requires the animation state to be updated
//Input:
//	1) Pose index to update
//	2) New weight for the pose
//-----------------------------------------------------------------------------
void AnimPosePanel::updatePose( int poseIdx, float newWeight )
{
	Ogre::Pose *pose = m_loadedEntity->getMesh()->getPose( poseIdx );

	const Ogre::VertexAnimationTrack *vertexTrack = m_currentAnimation->getVertexTrack(
																		pose->getTarget() );

	//No need to check this is a VAT_POSE track, since we're the ones who've created it
	Ogre::VertexPoseKeyFrame *key = static_cast<Ogre::VertexPoseKeyFrame*>(vertexTrack->getKeyFrame(0));

	key->updatePoseReference( poseIdx, newWeight );

	m_currentAnimationState->getParent()->_notifyDirty();
}

//-----------------------------------------------------------------------------
//copyPoseFromTextCtrls()
//Description:
//	Iterates through all text controls and applies their weight values to the
//	respective poses.
//-----------------------------------------------------------------------------
void AnimPosePanel::copyPoseFromTextCtrls()
{
	for( size_t idx=0; idx<m_poseCtrls.size(); ++idx )
	{
		double value;
		if( m_poseCtrls[idx].weightText->GetValue().ToDouble( &value ) )
		{
			const float w = static_cast<float>(value);
			updatePose( idx, w );

			//Update the GUI slider with the new value
			m_poseCtrls[idx].weightSlider->SetValue( static_cast<int>(w * 1000.0f) );
		}
	}
}

//-----------------------------------------------------------------------------
//meshLoaded()
//Description:
//	Called when the parent loaded the mesh
//Input:
//	1) The OGRE entity object
//-----------------------------------------------------------------------------
void AnimPosePanel::meshLoaded( Ogre::Entity *meshEntity )
{
	meshUnload();
	m_loadedEntity = meshEntity;

	const Ogre::MeshPtr &baseMesh = m_loadedEntity->getMesh();

	for( unsigned short i=0; i<baseMesh->getNumAnimations(); ++i )
	{
		Ogre::Animation *animation = baseMesh->getAnimation( i );

		//See if this animation has at least one track with pose animation (note: there may be
		//animations with both pose and morph animations, may God have mercy on those working with
		//such meshes)
		bool hasPoseData = false;
		Ogre::Animation::VertexTrackIterator itor = animation->getVertexTrackIterator();
		while( itor.hasMoreElements() && !hasPoseData )
			hasPoseData = itor.getNext()->getAnimationType() == Ogre::VAT_POSE;

		if( hasPoseData )
			m_animations->Append( wxString( animation->getName().c_str(), wxConvUTF8 ) );
	}

	//See if this mesh has any pose data, if so, create a manual animation state so we can
	//adjust each shape key manually (in an actual game this is very unviable unless you
	//only have one entity per mesh, plus it makes update the animation state set too often)
	if( baseMesh->getPoseCount() )
	{
		//Deal with smarties trying to crash us
		if( baseMesh->hasAnimation( c_manualPoseAnimationName ) )
		{
			wxMessageBox( wxT( "Mesh already has a pose animation named '" ) +
						  wxString( c_manualPoseAnimationName.c_str(), wxConvUTF8 ) +
						  wxT( "' which is a reserved word for Ogre Meshy. Manually editing" )
						  wxT( " poses won't be available for this mesh" ),
						  wxT("Manual Pose keys not available"),
						  wxOK|wxICON_WARNING|wxCENTRE );
			return;
		}

		//Create manual animation
		Ogre::Animation *manualAnim = baseMesh->createAnimation( c_manualPoseAnimationName, 0.0f );

		//NumSubmeshes + 1 because shared geometry counts as idx = '0'
		for( unsigned short i=0; i<baseMesh->getNumSubMeshes()+1; ++i ) 
		{
			//Each submesh needs a track
			Ogre::VertexAnimationTrack *track = manualAnim->createVertexTrack( i, Ogre::VAT_POSE );
			track->createVertexPoseKeyFrame( 0 );
		}

		for( unsigned short i=0; i<baseMesh->getPoseCount(); ++i ) 
		{
			//Fill the track with just a keyframe, containing the actual weights
			Ogre::Pose *pose = baseMesh->getPose( i );
			Ogre::VertexAnimationTrack *track = manualAnim->getVertexTrack( pose->getTarget() );
			Ogre::VertexPoseKeyFrame *keyFrame = track->getVertexPoseKeyFrame( 0 );
			keyFrame->addPoseReference( i, 0.0f );
		}

		for( unsigned short i=0; i<baseMesh->getNumSubMeshes()+1; ++i ) 
		{
			//Ok, I lied, not _all_ submeshes needed a track, since they don't
			//have pose data. Remove those unused
			Ogre::VertexAnimationTrack *track = manualAnim->getVertexTrack( i );
			Ogre::VertexPoseKeyFrame *keyFrame = track->getVertexPoseKeyFrame( 0 );

			if( keyFrame->getPoseReferences().empty() )
				manualAnim->destroyVertexTrack( i );
		}

		//Deal with rare situation where there are pose keys but no predefined animation, thus
		//SnimationStateSet is a null pointer and refreshing will only crash (calling a null ptr)
		if( baseMesh->getNumAnimations() == 1 )
			m_loadedEntity->_initialise( true );
		else
			m_loadedEntity->refreshAvailableAnimationState();

		m_animations->Append( wxT("Manual Animation") );

		createKeyScrolls();
	}
}

//-----------------------------------------------------------------------------
//meshUnload()
//Description:
//	Called when the parent no longer has the mesh loaded
//-----------------------------------------------------------------------------
void AnimPosePanel::meshUnload()
{
	m_animations->Clear();
	m_animations->Append( wxT( "(None)" ) );
	m_animations->SetSelection( 0 );
	removeKeyScrolls();
	setManualMode( false );
	m_loadedEntity			= 0;
	m_currentAnimationState = 0;
	m_currentAnimation		= 0;
}

//-----------------------------------------------------------------------------
//timeUpdated()
//Description:
//	Called every frame by our parent when it updated the animation time,
//	so we can keep our controls updated too
//Notes:
//	1) Assumes there is a current animation selected
//-----------------------------------------------------------------------------
void AnimPosePanel::timeUpdated()
{
	assert( !m_manualMode ); //Div. by zero in manual mode
	assert( m_currentAnimationState );

	const int timePos = static_cast<int>( m_currentAnimationState->getTimePosition() /
											m_currentAnimationState->getLength() * 100.0f );

	const wxString strTimePos( wxString::Format( wxT("%.4f"),
								m_currentAnimationState->getTimePosition() ) );

	wxWindow *focusedObj = wxWindow::FindFocus();
	if( m_animationTimeSlider != focusedObj )
		m_animationTimeSlider->SetValue( static_cast<int>(timePos) );
	if( m_animationTimeValue != focusedObj )
		m_animationTimeValue->SetValue( strTimePos );

	updateKeyScrolls();
}

void AnimPosePanel::OnChangedAnimationName( wxCommandEvent& event )
{
	int sel = m_animations->GetSelection();

	//Disable playing animation
	if( m_currentAnimationState )
		m_currentAnimationState->setEnabled( false );

	//By default stop the animation
	m_playAnimation = false;

	//Avoid selection "(None)"
	if( sel )
	{
		Ogre::String animName;

		if( sel == m_animations->GetCount()-1 )
		{
			//One last update to ensure all controls are valid before copying at the end
			//Avoid bugs when selecting manual animation when it's already selected
			if( !m_manualMode && m_currentAnimationState )
				timeUpdated();

			//Manual animation chosen, special case
			animName = c_manualPoseAnimationName;
			setManualMode( true );
		}
		else
		{
			//Regular animation chosen
			animName = Ogre::String( m_animations->GetString( sel ).mb_str() );

			if( m_manualMode )
				setManualMode( false );
		}

		m_currentAnimationState = m_loadedEntity->getAnimationState( animName );
		m_currentAnimationState->setEnabled( true );

		m_currentAnimation = m_loadedEntity->getMesh()->getAnimation( animName );

		//Display the length
		m_animationLengthText->SetLabel( wxString::Format( wxT("%.4f"),
										 m_currentAnimationState->getLength() ) );
		//Set loop checkbox
		m_animLoopCheckBox->SetValue( m_currentAnimationState->getLoop() );

		//Update the time sliders
		if( !m_manualMode )
			timeUpdated();
		else
			copyPoseFromTextCtrls();
	}
}

void AnimPosePanel::OnAnimationChangeTime( wxCommandEvent& event )
{
	if( !m_currentAnimationState )
		return;

	double value;
	if( m_animationTimeValue->GetValue().ToDouble( &value ) )
	{
		m_currentAnimationState->setTimePosition( static_cast<float>(value) );
		timeUpdated();
	}
}

void AnimPosePanel::OnAnimationChangeTimeSlider( wxScrollEvent& event )
{
	if( !m_currentAnimationState )
		return;

	float fTimePos = m_animationTimeSlider->GetValue() / 100.0f * m_currentAnimationState->getLength();
	m_currentAnimationState->setTimePosition( fTimePos );
	timeUpdated();
}

void AnimPosePanel::OnButtonPlaybackAnim( wxCommandEvent& event )
{
	if( !m_currentAnimationState )
		return;

	switch( event.GetId() )
	{
	case wxID_PLAYANIMATION:
		m_playAnimation = !m_playAnimation;
		break;
	case wxID_PAUSEANIMATION:
		m_playAnimation = false;
		break;
	case wxID_STOPANIMATION:
		m_playAnimation = false;
		m_currentAnimationState->setTimePosition( 0.0f );
		timeUpdated();
		break;
	}
}

void AnimPosePanel::OnAnimationRateChange( wxCommandEvent& event )
{
	double value;
	if( m_rateSkelAnim->GetValue().ToDouble( &value ) )
		m_animationRate = static_cast<float>(value);
}

void AnimPosePanel::OnLoopAnimation( wxCommandEvent& event )
{
	if( m_currentAnimationState )
		m_currentAnimationState->setLoop( m_animLoopCheckBox->GetValue() );
}

void AnimPosePanel::OnPoseWeightBox( wxCommandEvent& event )
{
	int idx = event.GetId() - wxID_POSETEXTSTART;

	assert( idx >= 0 && (unsigned)idx < m_poseCtrls.size() );

	double value;
	if( m_poseCtrls[idx].weightText->GetValue().ToDouble( &value ) )
	{
		const float w = static_cast<float>(value);
		updatePose( idx, w );

		//Update the GUI slider with the new value
		m_poseCtrls[idx].weightSlider->SetValue( static_cast<int>(w * 1000.0f) );
	}
}

void AnimPosePanel::OnPoseWeightSlider( wxScrollEvent& event )
{
	int idx = event.GetId() - wxID_POSESLIDERSTART;

	assert( idx >= 0 && (unsigned)idx < m_poseCtrls.size() );

	const float w = m_poseCtrls[idx].weightSlider->GetValue() / 1000.0f;

	updatePose( idx, w );

	//Update the GUI text box with the new value
	m_poseCtrls[idx].weightText->SetValue( wxString( wxString::Format( wxT("%.4f"), w ) ) );
}

void AnimPosePanel::OnResetWeights( wxCommandEvent& event )
{
	if( m_manualMode )
	{
		std::vector<PoseCtrls>::const_iterator itor = m_poseCtrls.begin();
		std::vector<PoseCtrls>::const_iterator end  = m_poseCtrls.end();
	
		while( itor != end )
		{
			itor->weightText->SetValue( wxT("0") );
			itor->weightSlider->SetValue( 0 );
	
			++itor;
		}
	
		copyPoseFromTextCtrls();
	}
}
