/*************************************************************************************/
/// 
/// @file MachineInstructionTable.h 
/// 
/// @brief  This file is a header file for MachineInstructionTable class.
/// 
/// It contains all of the declaration of the member funciton of MachineInstructionTable class.
/// It contains all of the inline funcitons defination of  the member funciton of MachineInstructionTable class 
/// It contains struct Stuct_MachineInstruction within the  MachineInstructionTable class.
///
/// @author Aayush Karki
/// 
/// @date  November 28, 2021
///  
/*************************************************************************************/

#pragma once

///
/// @class MachineInstructionTable "MachineInstructionTable.h"
/// 
/// @brief This is a class for MachineInstructionTable.
///	
/// MachineInstructionTable holds translated intruciton in a table
/// 
/// @author Aayush Karki
/// 
/// @date  November 28, 2021
/// 
class MachineInstructionTable
{

public:

	// ========================= Stuct_MachineInstruction and its related funcitons ================== 

	/// 
	/// @struct
	/// 
	/// @brief Stuct_MachineInstruction is a stucture to hold all the translated machine intructure
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 28, 2021
	///  
	struct Stuct_MachineInstruction
	{
		int m_Location;         ///> stroes the location of where the instruciton is in VC1620's memory
								///> is the index for m_memory
		std::string m_Content;  ///> contents that is to be stored in m_memory at index m_location, 
								///>        needs to be converted to long long
		std::string m_AssembInst; ///> copy of orginal Assembly instrction that was used to translate the

		// only for org
		bool m_isOrg;           ///> ==true if current opcode is ORG
		int m_nextLoc;          ///> store the number stored in operand1 if opcode is ORG

		/// 
		/// @brief constructor
		/// 
		/// Initializes the member variable of the structure to 0 if int or "" if string
		///  
		/// @author Aayush Karki
		/// 
		/// @date  November 28, 2021
		/// 
		MachineInstructionTable::Stuct_MachineInstruction()
		{
			m_Location = 0;
			m_Content = "";
			m_AssembInst = "";
		}
	};

	// ====================== end of Stuct_MachineInstruction and its related funcitons ====================

	/// 
	/// @brief AddMachineIntruction adds traslated intruction to the machine instruction table.
	/// 
	/// @param a_AssembInst: orginal assembly intruction used to translate into machine instruction
	/// @param a_Location: location in memory of VC1620 to stored the machine instruction 
	/// @param a_Content: machine instruction for VC1620 
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 28, 2021
	/// 
	void AddMachineIntr( std::string a_AssembInst, int a_Location = -1, std::string a_Content = "-5" );


	/// 
	/// @brief DisplaySymbolTable Display the content of the table to the screen.
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 28, 2021
	/// 
	void DisplayMachineInstTable();

	///
	/// @brief getter function to access the Default location
	/// 
	/// @return GetDefaultLocation returns value of Default location
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 28, 2021
	///
	inline const int GetDefaultLocation() { return DEFAULT_LOCATION; }

	///
	/// @brief getter function to access the Default content
	/// 
	/// @return GetDefaultContent returns value of Default content
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 28, 2021
	///
	inline const std::string GetDefaultContent() { return DEFAULT_CONTENT; }

	///
	/// @brief getter function to access the first element in machine instruction table
	/// 
	/// @return GetDefaultContent returns a pointer to begining of the table.
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 28, 2021
	///
	inline const std::vector<Stuct_MachineInstruction>::iterator GetMachineInstTabBegin() { return m_machineInstTab.begin(); }

	///
	/// @brief getter function to access the end of in machine instruction table
	/// 
	/// @return GetDefaultContent returns a pointer to end of the table
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 28, 2021
	///
	inline const std::vector<Stuct_MachineInstruction>::iterator GetMachineInstTabEnd() { return m_machineInstTab.end(); }

private:

	std::vector<Stuct_MachineInstruction> m_machineInstTab; // stores all the translated intruction in as a pointer
	const int DEFAULT_LOCATION = -888;						// defalault location if not specified
	const std::string DEFAULT_CONTENT = "-777";				// defalault location if not specified
};