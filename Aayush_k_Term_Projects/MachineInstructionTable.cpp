/// /*************************************************************************************/
/// 
/// @file MachineInstructionTable.cpp 
/// 
/// @brief  This file is a source file for MachineInstructionTable class.
/// 
/// It contains all of the defination  of the member funciton.
///
/// @author Aayush Karki
/// 
/// @date  November 28, 2021
///
/*************************************************************************************/

#include "stdafx.h"
#include "MachineInstructionTable.h"

void MachineInstructionTable::AddMachineIntr( std::string a_AssembInst, int a_Location, std::string a_Content )
{
	// adding empty stuct to the vector
	m_machineInstTab.push_back( Stuct_MachineInstruction() );

	// adding information to the struct
	m_machineInstTab.back().m_AssembInst = a_AssembInst;
	
	// checking if a_content was suplied or not
	if( a_Content == "-5" )
	{
		m_machineInstTab.back().m_Content = GetDefaultContent();
	}
	else
	{
		m_machineInstTab.back().m_Content = a_Content;
	}
	if( a_Location == -1 )
	{
		m_machineInstTab.back().m_Location = GetDefaultLocation();
	}
	else
	{
		m_machineInstTab.back().m_Location = a_Location;
	}

}

void MachineInstructionTable::DisplayMachineInstTable()
{
	std::vector<Stuct_MachineInstruction>::iterator currInstIte = m_machineInstTab.begin();
	int currLineNum = 0;

	std::cout << "Translation of Program:" << std::endl << std::endl;

	// formating the table and printing the table headign
	std::cout << std::left; 
	std::cout << std::setw( 7 ) << "Line "
		<< std::setw( 9 ) << "Location "
		<< std::setw( 14 ) << "  Content"
		<< "Orignal Statement" << std::endl;

	// printing the values
	while( currInstIte != m_machineInstTab.end() )
	{
		++currLineNum;
		std::cout <<" " << std::setw(6) << currLineNum << "   ";
		std::cout << std::setw( 6 );
		if( currInstIte->m_Location != GetDefaultLocation() )
		{
			std::cout << currInstIte->m_Location;
		} 
		else
		{
			std::cout << " ";
		} 
		std::cout << std::setw( 15 );
		
		if( currInstIte->m_Content != GetDefaultContent() )
		{
			std::cout << currInstIte->m_Content;
		}
		else
		{
			std::cout << " ";
		}
		std::cout << currInstIte->m_AssembInst << std::endl;
		currInstIte++;
	}

	std::cout << std::setfill( '_' ) << std::setw( 90 ) << " " << std::endl << std::endl;
	std::cout << std::setfill( ' ' );

	std::cout << "Press Enter to Continue" << std::endl;

	std::string enter;
	std::getline( std::cin, enter );
}
