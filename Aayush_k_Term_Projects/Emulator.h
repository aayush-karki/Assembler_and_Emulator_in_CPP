/*************************************************************************************/
/// 
/// @file Emulator.h 
/// 
/// @brief  This file is a header file for Emulator class.
/// 
/// It contains all of the declaration of the member funciton of emulator class.
/// It contains all of the defination of  the member funciton of emulator class 
///		who might/are quality as inline funcitons.
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
/// @warning Emulator does not checks if any memory address, which are in bounds, are valid access or not.
///		This is done by assembler.
/// @warning If only emulator is used, there are lot of errors which it does
///		not accounts for.
/// 
/// @author Aayush Karki
/// 
/// @date  December 03, 2021
/// 
class Emulator {

public:

	
    const static int MEMSZ = 100000; ///> The size of the memory of the VC1620.
    
    /// 
    /// @brief Constructor inializes the memory.
    /// 
	/// It resizes the memory to the size defined by MEMSZ 
	///		and sets each to have the value of 0.
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  December 03, 2021
	/// 
    Emulator() {

         m_memory.resize(MEMSZ, 0);
    }

    /// 
    /// @brief InsertMemory Records instructions and data into VC1620.
    /// 
	/// InsertMemory checks for valid location
	/// 
    /// @param a_location index of VC1620 memory, where the intruction or data should be stored
    /// @param a_contents intruction and data that needs to be stored in the memory 
    /// 
    /// @returns true if successfull
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
    /// returns true if recorded program run successfully; else returns false.
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  December 03, 2021
    /// 
    bool RunProgram( );

private:
    // ==================================== private function =====================================

	/// 
	/// @brief ReadFromUser get a interger from user
	/// 
	/// ReadFromUser is called for opCode 07 "READ."
	/// It also validates the user input.
	/// 
	/// @note As each word is 12 digit long, a valid userInput is +/- 999,999,999,999.
	/// 
	/// returns true if recorded program run successfully; else returns false.
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

