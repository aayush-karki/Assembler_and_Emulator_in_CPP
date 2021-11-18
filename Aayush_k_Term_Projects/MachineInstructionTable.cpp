#include "stdafx.h"
#include "MachineInstructionTable.h"


void MachineInstructionTable::AddMachineIntr( std::string a_AssembInst, int a_Location, long long a_Content )
{
	// adding empty stuct to the vector
	m_machineInstTab.push_back( Stuct_MachineInstruction() );

	// adding information to the struct
	m_machineInstTab.back().m_AssembInst = a_AssembInst;
	
	// checking if a_content was suplied or not
	if( a_Content != -1 )
	{
		m_machineInstTab.back().m_Content = a_Content;
	}
	else
	{
		m_machineInstTab.back().m_Content = DEFAULT_CONTENT;
	}
	if( a_Content != -1 )
	{
		m_machineInstTab.back().m_Location = a_Location;
	}
	else
	{
		m_machineInstTab.back().m_Location = DEFAULT_LOCATION;
	}

}
