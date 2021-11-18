// 
// Assembler class. This is a container for all the components 
// 
#pragma once 

#include "SymTab.h"
#include "Instruction.h"
#include "FileAccess.h"
#include "MachineInstructionTable.h"
#include "Emulator.h"

///
/// @class Assembler "Assemble.h"
/// @brief This is a container for all the components 
///         that make up the assembler.
///
class Assembler {

public:
    /// 
    /// @brief Constructor  
    /// @note we are passing argc and argv to the file access constructor.
    /// @see main program. 
    /// 
    Assembler( int argc, char *argv[] );
    
    /// 
    /// @brief Destructor currently does nothing. 
    /// You might need to add something as you develope this project.
    /// 
    ~Assembler( );

    /// 
    /// @brief Pass I establishes the location of the labels.
    /// 
    /// Pass I will go through the file from top to bottom 
    /// on line by line basis and finds any label and 
    /// their corresponding memory location adding them to symbol table
    /// 
    void PassI( );

    /// 
    /// @brief Pass II - generate a translation
    /// 
    /// Pass II is responsible for translating the assemble code to machine 
    /// code. It also report error if any. 
    /// 
    void PassII();

    /// 
    /// @brief Display the symbols in the symbol table.
    /// 
    void DisplaySymbolTable() { m_symtab.DisplaySymbolTable(); }
    
    /// 
    /// @brief Run emulator on the translation.
    /// 
    void RunProgramInEmulator() {};
    
private:

    FileAccess m_facc;	    // File Access object
    SymbolTable m_symtab;   // Symbol table object
    Instruction m_inst;	    // Instruction object
    MachineInstructionTable m_machInstTab; // Machine Instruction Table object
    Emulator m_emul;        // Emulator object
};

