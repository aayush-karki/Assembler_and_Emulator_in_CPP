/*
 * Assembler main program.
 */


// todo: add author and date in every function documentation

#include "stdafx.h"     // This must be present if you use precompiled headers which you will use. 
#include <stdio.h>

#include "Assembler.h"

int main(int argc, char* argv[])
{
    Assembler assem(argc, argv);

    // inializing error
    Errors::InitErrorReporting();

    // Establish the location of the labels:
    assem.PassI();

    // Display the symbol table.
    assem.DisplaySymbolTable();

    // generate the translation.
    if( !assem.PassII() )
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