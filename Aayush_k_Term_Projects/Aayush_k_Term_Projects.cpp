/*****************************************************************/
///
/// @brief  This file contains the main which executes the assembler program and emulator program
/// 
/// This assembler and emulator for the VC1620 computer. 
/// This is similar to the IBM 1620 that was popular in the 1960s.
/// 
/// @note even though the memory is 12 digit long as each operands are 5 digit long,
///			a range a constant declared by DC operantion code is 99'999
/// 
/// @todo make operand handel -ve value
/// @todo check the for emulator if the value has gone over the memory, 
///		like after adding subtraction and stuff
/// @todo improve dc handeling to handeling 12 digit operation that means operand2 should also 
///		 be able to handel 12 digit addition and stuff
/// 
/// @author Aayush Karki
/// @author Victor Miller
/// 
/// @date  November 10, 2021
///  
/**********************************************************************/

#include "stdafx.h"     // This must be present if you use precompiled headers which you will use. 
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

	if(!assem.GetNoError() )
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