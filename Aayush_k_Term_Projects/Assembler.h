/*****************************************************************/
/// 
/// @file Assembler.h 
/// 
/// @brief  This file is a header file for Assembler class.
///
/// @author Aayush Karki
/// 
/// @date  December 06, 2021
///  
/**********************************************************************/


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
/// 
/// @brief This is a container for all the components 
///         that make up the assembler and runs an emulator on it.
/// 
/// @author Aayush Karki
/// 
/// @date  November 06, 2021 
/// 
class Assembler {

public:
    /// 
    /// @brief Constructor  for the Assemble object
    /// 
    /// @note we are passing command line argument from the user in to the constructor
    /// @note argc should be 2 else it will be raised by the file access class raise an error
    /// 
    /// @param argc number of elemets passed to the program + 1
    /// @param argv pointer to the binary of the program and file containting the assembly code
    /// 
    /// @author Aayush Karki
    /// 
    /// @date  November 06, 2021 
    /// 
    Assembler(int argc, char* argv[]):
        m_facc(argc, argv)
    {
        m_noError = true;
    }
    
    /// 
    /// @brief Destructor
    /// 
    /// @author Aayush Karki
    /// 
    /// @date  November 06, 2021 
    /// 
    ~Assembler( );

    /// 
    /// @brief Pass I establishes the location of the labels.
    /// 
    /// Pass I will go through the file from top to bottom 
    /// on line by line basis and finds any label and 
    /// their corresponding memory location adding them to symbol table
    /// 
    /// @note responsible for reporting multi defined label
    /// 
    /// @author Aayush Karki
    /// 
    /// @date  November 06, 2021 
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
    /// @brief DesplayDeclaredConstTab Displays all the declared Constant number along with their lablels
    /// 
    void DisplayDeclaredConstTab();

    /// 
    /// @brief DisplayDeclaredVarMemSizeTab Displays all the declared variable memory along with their lablels
    /// 
    void DisplayDeclaredMemVarTab();

    /// 
    /// @brief Displays the translated machine instruction in the machine instruction table.
    /// 
    void DisplayMachineInstTable() { m_machInstTab.DisplayMachineInstTable(); }

    /// 
    /// @brief Run emulator on the translation.
    /// 
    void RunProgramInEmulator();

    ///
    /// @brief InsertInstToEmulator inserts all the machine instruction stored in
    ///          machine Instruction table into the emulator
    /// 
    /// @return false 
    /// 
    void InsertInstToEmulator();

    // ================ private member funciton ============================
private:
    
   /// 
   /// @brief SmartFillContent filles in any need 0 before apending the num to translated content
   /// 
   /// 
   /// @note for DC the value of operand1 is stored in operand 2, so the a_lengthToFill is 10
   /// @note for every thing the value of operand1 and operand1  is 5 long. So, the a_lengthToFill is 5
   /// @note Opcodes if 2 long. So, a_lengthToFill is 2
   /// 
   /// @param a_TranslatedContent address of the string to append to
   /// @param a_ToAppendNum interger value to append to the string
   /// @param a_LengthToFill final length of the a_toAppendNum
   /// 
   /// @returns ValidateOperand2Lab returns true if it does else false
   /// 
    void SmartFillContent( std::string& a_TranslatedContent, int a_ToAppendNum, int a_LengthToFill = 5 );
    
    /// 
    /// @brief Lookup a symbol in the declared constant table.
    /// 
    /// @param a_symbol address of symbol to lookup
    /// 
    /// @returns LookupSymbol returns true is symbol is found in table else returns false 
    /// 
    bool LookupDeclaredConst( const std::string& a_symbol );
    
    ///  
    /// @brief Lookup a symbol in the declared variable memory table.
    /// 
    /// @param a_symbol address of symbol to lookup
    /// 
    /// @returns LookupSymbol returns true is symbol is found in table else returns false 
    /// 
    bool LookupDeclaredVarMem( const std::string& a_symbol );
    
    ///  
    /// @brief TranslateInstruction translates the given assembly instruction as much
    ///     as possible
    /// 
    /// @param a_TranslatedInstruction: address of string where the translated instruction
    ///         is stored
    /// @param a_loc current loca
    /// 
    void TranslateInstruction( int a_Loc, int a_LineCounter );

    ///
    /// @brief ComputeNextLoc computes next location
    /// 
    /// @param a_loc address of loction variable
    /// @param a_LineCounter address of lineCounter variable
    /// @param a_insufficentMemory address of variable that signifiy if there
    ///         insufficent memory error was flaged or not 
    /// 
    void ComputeNextLoc( int& a_loc, int& a_LineCounter, bool& a_insufficentMemory);

    // ================ private variables ============================
private:

    bool m_noError; // tracks error

    FileAccess m_facc;	    // File Access object
    SymbolTable m_symtab;   // Symbol table object
    Instruction m_inst;	    // Instruction object
    MachineInstructionTable m_machInstTab; // Machine Instruction Table object
    Emulator m_emul;        // Emulator object

    // table to store all the declared Constant number along with their lablels
    // stores all DC instruction label and Operand1 value
    std::unordered_map<std::string, int> m_declaredConstTab;

    // table to store all the declared variable's memory size along with their lablels
    // stores all DS instruction label and Operand1 value
    std::unordered_map<std::string, int> m_declaredMemVarTab;
};

