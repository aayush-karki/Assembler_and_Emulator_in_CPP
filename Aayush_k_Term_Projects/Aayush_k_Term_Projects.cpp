/*****************************************************************/
///
/// @brief  This file contains the main function which executes the assembler 
///		program and emulator program
/// 
/// This assembler and emulator for the VC1620 computer. 
/// This is similar to the IBM 1620 that was popular in the 1960s.
/// 
/// @note VC 1620's word of memory is 12 digit long.
/// 
/// @note a constant declared by DC operantion code can handel range of +/- 99'999'999'999. 
///		That is 11 digit long interger, as one digit is reserved for sign, 
///		'-' for negative and 0 fro positive.
/// 	
/// @author Aayush Karki
/// @author Victor Miller
/// 
/// @date  November 10, 2021 
///  
/**********************************************************************/

#include "stdafx.h"     // This must be present for precompiled headers
#include <stdio.h>

#include "Assembler.h"

int main( int argc, char* argv[] )
{
	Assembler assem( argc, argv );

	// inializing error
	Errors::InitErrorReporting();

	// Establish the location of the labels:
	assem.PassI();

	// Display the symbol table.
	assem.DisplaySymbolTable();

	// generate the translation.
	assem.PassII();

	// checking error was found or not
	if( !assem.GetNoError() )
	{
		// display error messages if any
		Errors::DisplayAllErrors();

		// Display the machine Instruction table for reference.
		assem.DisplayMachineInstTable();
		return -1;
	}

	// Display the machine Instruction table.
	assem.DisplayMachineInstTable();

	// Run the emulator on theVC5000 program that was generated in Pass II.
	assem.RunProgramInEmulator();

	// Terminate indicating all is well.  If there is an unrecoverable error, the 
	// program will terminate at the point that it occurred with an exit(1) call.
	return 0;
}