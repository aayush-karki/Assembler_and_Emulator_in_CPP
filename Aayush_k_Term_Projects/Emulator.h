/*************************************************************************************/
/// 
/// @file Emulator.h 
/// 
/// @brief  This file is a header file for Emulator class.
/// 
/// It contains all of the declaration of the member funciton of emulator class.
/// It contains all of the inline funcitons defination of  the member funciton of emulator class 
///
/// @author Aayush Karki
/// 
/// @date  December 03, 2021
///  
/*************************************************************************************/

#ifndef _EMULATOR_H     // UNIX way of preventing multiple inclusions.
#define _EMULATOR_H

///
/// @class Emulator "Emulator.h"
/// 
/// @brief This is a class for emulator.
///	
/// Emulator class supports the emulation of VC1620 programs.
///		It runs the trananslated assembly code.
/// 
/// @warning Emulator does not checksif a asseced memory address is valid access or not.
///		This should is done by assembler.
/// @warning If only emulator is used, there lot of instruction errors may occour.
/// 
/// @author Aayush Karki
/// 
/// @date  December 03, 2021
/// 
class Emulator
{

public:

	/// The size of the memory of the VC1620.
	const static int MEMSZ = 100000; 

	/// 
	/// @brief constructor, inializes the memory.
	/// 
	/// It resizes the memory to the size defined by MEMSZ 
	///		and sets each to have the value of 0.
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  December 03, 2021
	/// 
	Emulator() { m_memory.resize( MEMSZ, 0 ); }

	/// 
	/// @brief InsertMemory Records instructions and data into VC1620.
	/// 
	/// InsertMemory checks for valid location
	/// 
	/// @param a_location index of VC1620 memory where the intruction or data should be stored
	/// @param a_contents intruction and data that needs to be stored in the memory 
	/// 
	/// @returns true if instruciton instertion in a memory is successfull;
	///			else false.
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  December 03, 2021
	/// 
	bool InsertMemory( int a_location, long long a_contents );

	/// 
	/// @brief RunProgram Runs the VC1600 program recorded in memory.
	/// 
	/// It there is an error, the execution of the program is stoped.
	/// 
	/// @returns true if recorded program run successfully; else returns false.
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  December 03, 2021
    /// 
    bool RunProgram();

private:
	// ==================================== private function =====================================

	/// 
	/// @private
	/// @brief ReadFromUser get a interger from user
	/// 
	/// ReadFromUser is called for opCode 07 "READ."
	/// It also validates the user input.
	/// 
	/// @note As each word is 12 digit long, a valid userInput is +/- 99,999,999,999 
	///			as one digit is used to store the sign.
	/// 
	/// @returns true if recorded program run successfully; else returns false.
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  December 03, 2021
	///
	bool ReadFromUser( std::string& a_userInput, int a_currAddr );

private:
	// ==================================== private variables =====================================

	std::vector<long long> m_memory;  	// Memory for the VC1620
	const int m_START_ADDRESS = 100;	// location of where the first instruction is 
};

#endif

