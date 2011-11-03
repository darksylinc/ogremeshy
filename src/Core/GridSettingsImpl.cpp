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

#include "Core/GridSettingsImpl.h"
#include <math.h>

static const double c_presets[] =
{
	1.0,		//Centimeters (default)
	100.0,		//Meters
	2.54,		//Inches
	30.48,		//Foot
	1.0			//Custom
};

GridSettingsImpl::GridSettingsImpl( wxWindow* parent, float cellSize, float cellWidth, float cellDepth ) :
								GridSettings( parent ),
								m_cellSize( cellSize ),
								m_cellWidth( cellWidth ),
								m_cellDepth( cellDepth )
{
	m_presetsChoice->SetSelection( m_presetsChoice->GetCount() - 1 );
	for( unsigned int i=0; i<m_presetsChoice->GetCount(); ++i )
	{
		if( fabs(m_cellSize - c_presets[i]) < 1e-6f )
		{
			m_presetsChoice->SetSelection( i );
			break;
		}
	}

	m_gridCellUnit->SetValue( wxString() << m_cellSize );

	m_gridWidth->SetValue( wxString() << m_cellWidth );
	m_gridDepth->SetValue( wxString() << m_cellDepth );
}

void GridSettingsImpl::OnPresetsChange( wxCommandEvent& event )
{
	unsigned int preset = m_presetsChoice->GetCurrentSelection();
	if( preset < m_presetsChoice->GetCount() - 1 )
		m_gridCellUnit->SetValue( wxString() << c_presets[preset] );
	else
		m_gridCellUnit->SetValue( wxString() << m_cellSize );
}

void GridSettingsImpl::OnCommitChanges( wxCommandEvent& event )
{
	m_gridWidth->GetValue().ToDouble( &m_cellWidth );
	m_gridDepth->GetValue().ToDouble( &m_cellDepth );

	m_gridCellUnit->GetValue().ToDouble( &m_cellSize );

	EndModal( event.GetId() );
}
