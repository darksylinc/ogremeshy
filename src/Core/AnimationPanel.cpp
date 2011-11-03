//----------------------------------------------------------------------
//	Copyright (C) 2010 Matias N. Goldberg ("dark_sylinc")
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

#include "Core/AnimationPanel.h"

#include <OgreEntity.h>
#include <OgreSkeletonInstance.h>
#include <OgreAnimation.h>
#include <OgreStringConverter.h>
#include <OgreTagPoint.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

#include "Constants.h"

#include "Core/MovableText.h"

AnimationPanel::AnimationPanel( wxWindow* parent, Ogre::SceneManager *sceneManager ) :
		wxAnimationPanel( parent ),
		m_entitiesId( 0 ),
		m_loadedEntity( 0 ),
		m_sceneManager( sceneManager ),
		m_currentAnimationState( 0 ),
		m_playAnimation( false ),
		m_animationRate( 1.0f ),
		m_visulBoneSize( 0.02f ),
		m_boneNameColour( Ogre::ColourValue::White )
{
	m_axisOnBone.tagNode	= 0;
	m_axisOnBone.entity		= 0;
}

//-----------------------------------------------------------------------------
//meshLoaded()
//Description:
//	Called when the parent loaded the mesh
//Input:
//	1) The OGRE entity object
//-----------------------------------------------------------------------------
void AnimationPanel::meshLoaded( Ogre::Entity *meshEntity )
{
	meshUnload();
	m_loadedEntity = meshEntity;

	Ogre::SkeletonInstance *skeletonInstance = m_loadedEntity->getSkeleton();

	if( skeletonInstance )
	{
		for( unsigned short i=0; i<skeletonInstance->getNumAnimations(); ++i )
		{
			Ogre::Animation *animation = skeletonInstance->getAnimation(i);
			m_animations->Append( wxString( animation->getName().c_str(), wxConvUTF8 ) );
		}
	}
}

//-----------------------------------------------------------------------------
//meshUnload()
//Description:
//	Called when the parent no longer has the mesh loaded
//-----------------------------------------------------------------------------
void AnimationPanel::meshUnload()
{
	hideBones();
	removeAxisFromBone();

	m_animations->Clear();
	m_animations->Append( wxT( "(None)" ) );
	m_animations->SetSelection( 0 );
	m_boneTree->DeleteAllItems();
	m_loadedEntity			= 0;
	m_currentAnimationState = 0;
}

//-----------------------------------------------------------------------------
//timeUpdated()
//Description:
//	Called every frame by our parent when it updated the animation time,
//	so we can keep our controls updated too
//Notes:
//	1) Assumes there is a current animation selected
//-----------------------------------------------------------------------------
void AnimationPanel::timeUpdated()
{
	assert( m_currentAnimationState );

	const int timePos = static_cast<int>( m_currentAnimationState->getTimePosition() /
											m_currentAnimationState->getLength() * 100.0f );

	const wxString strTimePos( wxString::Format( wxT("%.4f"), m_currentAnimationState->getTimePosition() ) );

	wxWindow *focusedObj = wxWindow::FindFocus();
	if( m_animationTimeSlider != focusedObj )
		m_animationTimeSlider->SetValue( static_cast<int>(timePos) );
	if( m_animationTimeValue != focusedObj )
		m_animationTimeValue->SetValue( strTimePos );

	//If bone tips are being displayed, keep them in place
	updateBoneTips();
}

//-----------------------------------------------------------------------------
//showBones()
//Description:
//	Shows a model per each bone that eases visualization, making it quick
//	to follow position, orientation, and scale
//Input:
//	1) True if each bone should have the name printed on the top.
//-----------------------------------------------------------------------------
void AnimationPanel::showBones( bool withNames )
{
	if( !m_loadedEntity )
		return;

	Ogre::SkeletonInstance *skeletonInstance = m_loadedEntity->getSkeleton();

	if( skeletonInstance )
	{
		//Create BoneTips first, because TagPoints are listed as children of the bones too
		Ogre::Skeleton::BoneIterator itor = skeletonInstance->getRootBoneIterator();
		while( itor.hasMoreElements() )
			createBoneTip( itor.getNext() );

		itor = skeletonInstance->getBoneIterator();
		while( itor.hasMoreElements() )
		{
			Ogre::Bone *bone = itor.getNext();
			VisualBone visualBone;
			visualBone.entity  = m_sceneManager->createEntity( "Bone" +
															Ogre::StringConverter::toString(m_entitiesId++),
															"BoneGlobe.mesh", c_InterMeshPermGroup );

			visualBone.text = 0;

			if( withNames )
			{
				visualBone.text = new Ogre::MovableText( "BoneTxt" + Ogre::StringConverter::toString(
														 m_entitiesId ), bone->getName() );
				visualBone.text->setCharacterHeight( 0.75f );
				//Center horizontally and display above the node
				visualBone.text->setTextAlignment( Ogre::MovableText::H_CENTER,
													Ogre::MovableText::V_ABOVE );
				visualBone.text->setColor( m_boneNameColour );
				visualBone.text->showOnTop( true );

				Ogre::TagPoint *tp = m_loadedEntity->attachObjectToBone( bone->getName(),
																		 visualBone.text );
				tp->setScale( Ogre::Vector3(m_loadedEntity->getBoundingRadius() * m_visulBoneSize) );
			}

			visualBone.tagNode = m_loadedEntity->attachObjectToBone( bone->getName(), visualBone.entity );
			visualBone.tagNode->setScale( Ogre::Vector3(m_loadedEntity->getBoundingRadius() *
														m_visulBoneSize) );

			visualBone.entity->setRenderQueueGroup( Ogre::RENDER_QUEUE_SKIES_LATE );
			m_visualBones.push_back( visualBone );
		}
	}
}

//-----------------------------------------------------------------------------
//hideBones()
//Description:
//	Hides the bones created with showBones
//-----------------------------------------------------------------------------
void AnimationPanel::hideBones()
{
	if( !m_loadedEntity )
		return;

	{
		std::vector<VisualBone>::const_iterator itor = m_visualBones.begin();
		std::vector<VisualBone>::const_iterator end  = m_visualBones.end();
		while( itor != end )
		{
			m_loadedEntity->detachObjectFromBone( itor->entity );
			m_sceneManager->destroyEntity( itor->entity );

			if( itor->text )
			{
				m_loadedEntity->detachObjectFromBone( itor->text );
				delete itor->text;
			}

			++itor;
		}

		m_visualBones.clear();
	}

	{
		std::vector<VisualLink>::const_iterator itor = m_visualLinks.begin();
		std::vector<VisualLink>::const_iterator end  = m_visualLinks.end();
		while( itor != end )
		{
			m_loadedEntity->detachObjectFromBone( itor->visualBone.entity );
			m_sceneManager->destroyEntity( itor->visualBone.entity );
			++itor;
		}

		m_visualLinks.clear();
	}
}

//-----------------------------------------------------------------------------
//createBoneTip()
//Description:
//	Recursive function that creates a visual link between a bone and it's
//	child, and doing the same with the children of their children.
//Notes:
//	1) Assumes m_loadedEntity != NULL
//Input:
//	1) Parent Node which will be linked with their children
//-----------------------------------------------------------------------------
void AnimationPanel::createBoneTip( Ogre::Bone *parentBone )
{
	Ogre::Bone::ChildNodeIterator childIt = parentBone->getChildIterator();
	while( childIt.hasMoreElements() )
	{
		VisualLink visualLink;

		//Ugh, unsafe upcast
		Ogre::Bone *childBone	= static_cast<Ogre::Bone*>(childIt.getNext());
		visualLink.linkedChild	= childBone;

		//TODO: Does anyone know a better way to skip (our own?) TagPoints?
		if( dynamic_cast<Ogre::TagPoint*>(childBone) != 0 )
			continue;

		//Link children of children
		if( childBone->numChildren() )
			createBoneTip( childBone );

		//Create an entity between parentNode and childBone
		Ogre::Vector3 vDif( parentBone->_getDerivedOrientation().Inverse() *
							(childBone->_getDerivedPosition() - parentBone->_getDerivedPosition()) );
		const float diffLength = vDif.normalise();
		const Ogre::Quaternion qRot( Ogre::Vector3::UNIT_Y.getRotationTo( vDif ) );

		//Skip Bones in the same position (flat tips only cause confusion)
		if( diffLength > 1e-6f )
		{
			visualLink.visualBone.entity = m_sceneManager->createEntity( "BoneTip" +
															Ogre::StringConverter::toString(m_entitiesId++),
															"BoneTip.mesh", c_InterMeshPermGroup );
			visualLink.visualBone.tagNode = m_loadedEntity->attachObjectToBone(
															parentBone->getName(),
															visualLink.visualBone.entity, qRot );
			visualLink.visualBone.tagNode->setScale( m_loadedEntity->getBoundingRadius() * m_visulBoneSize,
													 diffLength,
													 m_loadedEntity->getBoundingRadius() * m_visulBoneSize );
			visualLink.visualBone.tagNode->setInheritScale( false );

			//Tips are rendered before the globe to make globe's colours sharper
			visualLink.visualBone.entity->setRenderQueueGroup( Ogre::RENDER_QUEUE_SKIES_LATE - 1 );
			m_visualLinks.push_back( visualLink );
		}
	}
}

//-----------------------------------------------------------------------------
//updateBoneTips()
//Description:
//	Bone tips need to be updated every time the animation changes because
//	their relative offsets may have changed (or they don't inherit position/
//	orientation) and the tips would be now out of place
//	See createBoneTip()
//Notes:
//	1) Assumes m_loadedEntity is valid
//-----------------------------------------------------------------------------
void AnimationPanel::updateBoneTips()
{
	std::vector<VisualLink>::const_iterator itor = m_visualLinks.begin();
	std::vector<VisualLink>::const_iterator end  = m_visualLinks.end();
	while( itor != end )
	{
		Ogre::Node *parentBone	= itor->visualBone.tagNode->getParent();
		Ogre::Bone *childBone	= itor->linkedChild;

		Ogre::Vector3 vDif( parentBone->_getDerivedOrientation().Inverse() *
							(childBone->_getDerivedPosition() - parentBone->_getDerivedPosition()) );
		const float diffLength = vDif.normalise();
		const Ogre::Quaternion qRot( Ogre::Vector3::UNIT_Y.getRotationTo( vDif ) );

		itor->visualBone.tagNode->setOrientation( qRot );
		itor->visualBone.tagNode->setScale( m_loadedEntity->getBoundingRadius() * m_visulBoneSize,
											 diffLength,
											 m_loadedEntity->getBoundingRadius() * m_visulBoneSize );
		
		++itor;
	}
}

//-----------------------------------------------------------------------------
//removeAxisFromBone()
//Description:
//	Removes the axis that shows on the bones when a track node is selected
//Notes:
//	1) Assumes m_loadedEntity is valid
//-----------------------------------------------------------------------------
void AnimationPanel::removeAxisFromBone()
{
	if( m_axisOnBone.tagNode )
	{
		m_loadedEntity->detachObjectFromBone( m_axisOnBone.entity );
		m_axisOnBone.tagNode = 0;
	}
}

void AnimationPanel::OnChangedAnimationName( wxCommandEvent& event )
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
		const Ogre::String animName( m_animations->GetString( sel ).mb_str() );
		m_currentAnimationState = m_loadedEntity->getAnimationState( animName );
		m_currentAnimationState->setEnabled( true );

		//Display the length
		m_animationLengthText->SetLabel( wxString::Format( wxT("%.4f"), m_currentAnimationState->getLength() ) );
		//Set loop checkbox
		m_animLoopCheckBox->SetValue( m_currentAnimationState->getLoop() );
		//Set interpolation mode
		unsigned int mode = m_loadedEntity->getSkeleton()->getAnimation( animName )->getInterpolationMode();
		m_radioInterpolationMode->SetSelection( mode );

		//Update skeleton now so anything that depends on their positions works correctly
		m_loadedEntity->_updateAnimation();

		//Update the time sliders
		timeUpdated();

		m_boneTree->DeleteAllItems();
		const unsigned short numTracks = m_loadedEntity->getSkeleton()->getAnimation( animName )->
																						getNumNodeTracks();
		const wxTreeItemId rootId = m_boneTree->AddRoot( wxString( (animName + " (" +
														Ogre::StringConverter::toString( numTracks ) + ")").
														c_str(), wxConvUTF8 ) );
		Ogre::Animation::NodeTrackIterator itor = m_loadedEntity->getSkeleton()->getAnimation( animName )->
													getNodeTrackIterator();
		while( itor.hasMoreElements() )
		{
			Ogre::NodeAnimationTrack *node = itor.getNext();

			if( node->getAssociatedNode() )
			{
				m_boneTree->AppendItem( rootId, wxString( node->getAssociatedNode()->getName().c_str(),
															wxConvUTF8 ) );
			}
		}
		m_boneTree->Expand( rootId );
	}
	else
	{
		removeAxisFromBone();
		m_boneTree->DeleteAllItems();
	}
}

void AnimationPanel::OnAnimationChangeTime( wxCommandEvent& event )
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

void AnimationPanel::OnAnimationChangeTimeScroll( wxScrollEvent& event )
{
	if( !m_currentAnimationState )
		return;

	float fTimePos = m_animationTimeSlider->GetValue() / 100.0f * m_currentAnimationState->getLength();
	m_currentAnimationState->setTimePosition( fTimePos );
	timeUpdated();
}

void AnimationPanel::OnButtonPlaybackSkelAnim( wxCommandEvent& event )
{
	if( !m_currentAnimationState )
		return;

	switch( event.GetId() )
	{
	case wxID_PLAYSKELANIMATION:
		m_playAnimation = !m_playAnimation;
		break;
	case wxID_PAUSESKELANIMATION:
		m_playAnimation = false;
		break;
	case wxID_STOPSKELANIMATION:
		m_playAnimation = false;
		m_currentAnimationState->setTimePosition( 0.0f );
		timeUpdated();
		break;
	}
}

void AnimationPanel::OnAnimationRateChange( wxCommandEvent& event )
{
	double value;
	if( m_rateSkelAnim->GetValue().ToDouble( &value ) )
		m_animationRate = static_cast<float>(value);
}

void AnimationPanel::OnLoopAnimation( wxCommandEvent& event )
{
	if( m_currentAnimationState )
		m_currentAnimationState->setLoop( m_animLoopCheckBox->GetValue() );
}

void AnimationPanel::OnRadioInterpolationMode( wxCommandEvent& event )
{
	if( m_loadedEntity && m_currentAnimationState )
	{
		Ogre::Animation::InterpolationMode mode = static_cast<Ogre::Animation::InterpolationMode>
													(m_radioInterpolationMode->GetSelection());
		m_loadedEntity->getSkeleton()->getAnimation( m_currentAnimationState->getAnimationName() )->
																		setInterpolationMode( mode );
	}
}

void AnimationPanel::OnBoneSizeChange( wxScrollEvent& event )
{
	const float oldSize = m_visulBoneSize;
	m_visulBoneSize = m_sliderBoneSize->GetValue() - 50;
	m_visulBoneSize = m_visulBoneSize * (m_visulBoneSize < 0 ? 0.00035f : 0.0008f) + 0.02f;

	//Update displayed bones
	const float fScale = m_visulBoneSize / oldSize;
	{
		std::vector<VisualBone>::const_iterator itor = m_visualBones.begin();
		std::vector<VisualBone>::const_iterator end  = m_visualBones.end();
		while( itor != end )
		{
			itor->tagNode->scale( Ogre::Vector3( fScale ) );

			if( itor->text )
				itor->text->getParentNode()->scale( Ogre::Vector3( fScale ) );

			++itor;
		}
	}

	{
		std::vector<VisualLink>::const_iterator itor = m_visualLinks.begin();
		std::vector<VisualLink>::const_iterator end  = m_visualLinks.end();
		while( itor != end )
		{
			itor->visualBone.tagNode->scale( fScale, 1.0f, fScale );
			++itor;
		}
	}

	if( m_axisOnBone.tagNode )
		m_axisOnBone.tagNode->scale( Ogre::Vector3( fScale ) );
}

void AnimationPanel::OnNodeTrackSelect( wxTreeEvent& event )
{
	removeAxisFromBone();

	if( m_boneTree->GetRootItem() != event.GetItem() )
	{
		const std::string boneName( m_boneTree->GetItemText( event.GetItem() ).mb_str() );

		if( !m_axisOnBone.entity )
		{
			//First time creating the entity, load it now. Ogre will remove it on exit
			m_axisOnBone.entity = m_sceneManager->createEntity( "AxisOnBone", "Axis.mesh",
																c_InterMeshPermGroup );
		}
		m_axisOnBone.tagNode = m_loadedEntity->attachObjectToBone( boneName, m_axisOnBone.entity );
		m_axisOnBone.tagNode->setScale( Ogre::Vector3( m_loadedEntity->getBoundingRadius() *
														m_visulBoneSize * 2.0f ) );
		m_axisOnBone.entity->setRenderQueueGroup( Ogre::RENDER_QUEUE_SKIES_LATE );
		//event.Skip();
	}
}
