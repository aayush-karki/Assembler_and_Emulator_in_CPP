/*****************************************************************/
/// 
/// @file Assembler.h 
/// 
/// @brief  This file is a header file for Assembler class.
///
/// @date  November 06, 2021 
/// 
/// @author Aayush Karki
/// 
/**********************************************************************/

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
	/// @author Victor Miller
    /// 
    /// @date  November 10, 2021 
    /// 
	Assembler(int argc, char* argv[]);
    
    /// 
    /// @brief Destructor
    /// 
	/// @author Victor Miller
    /// 
	/// @date  November 10, 2021 
    /// 
    ~Assembler( );

    /// 
    /// @brief Pass I establishes the location of the labels.
    /// 
    /// Pass I will go through the file from top to bottom 
    /// on line by line basis and finds any label and 
    /// their corresponding memory location adding them to symbol table
    /// 
    /// @note responsible for reporting multi defined label error but is displayed
	///		after PassII
    /// 
	/// @author Aayush Karki
	/// @author Victor Miller
    /// 
	/// @date  November 10, 2021 
    /// 
    void PassI( );

    /// 
    /// @brief Pass II generates a translation for the assembey code
    /// 
    /// Pass II is responsible for translating as much of the assemble code to machine 
    ///		code as possible. It also report error if any.
	/// 
	/// @author Aayush Karki
    /// 
    /// @date  November 25, 2021 
    /// 
    void PassII();

    /// 
    /// @brief DisplaySymbolTable displays the symbols in the symbol table.
	/// 
	/// @author Victor Miller
    /// 
	/// @date  November 10, 2021 
    /// 
    void DisplaySymbolTable() { m_symtab.DisplaySymbolTable(); }
    
    /// 
    /// @brief DesplayDeclaredConstTab Displays all the declared Constant number along with their lablels
	/// 
	/// @author Aayush Karki
    /// 
	/// @date  November 25, 2021 
    /// 
    void DisplayDeclaredConstTab();

    /// 
    /// @brief DisplayDeclaredVarMemSizeTab Displays all the declared variable memory along with their lablels
    ///
	/// @author Aayush Karki
    /// 
	/// @date  November 25, 2021 
	///  
    void DisplayDeclaredMemVarTab();

    /// 
    /// @brief Displays the translated machine instruction in the machine instruction table.
    /// 
	/// @author Aayush Karki
    /// 
	/// @date  November 28, 2021
	/// 
    void DisplayMachineInstTable() { m_machInstTab.DisplayMachineInstTable(); }

    /// 
    /// @brief inserts and runs the emulator on the translation.
    /// 
	/// @author Aayush Karki
    /// 
	/// @date  December 03, 2021
	/// 
    void RunProgramInEmulator();

    ///
    /// @brief InsertInstToEmulator inserts all the translated machine instruction 
	///			into the emulator
    /// 
	/// @author Aayush Karki
    /// 
	/// @date  December 03, 2021
    /// 
    void InsertInstToEmulator();

	bool GetNoError() { return m_noError; }

    // ================ private member funciton ============================
private:
    
   /// 
   /// @brief SmartFillContent fills in any need 0 before apending the num to translated content
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
   /// @author Aayush Karki
   /// 
   /// @date  November 25, 2021 
   /// 
    void SmartFillContent( std::string& a_TranslatedContent, int a_ToAppendNum, int a_LengthToFill = 5 );
    
    /// 
    /// @brief Lookup a symbol in the declared constant table.
    /// 
	/// This is used to validate if the lable used as operand1 
	///		for ADD, SUB, MULT, DIV, COPY, READ, and WRITE
	///		should point to memory variable 
	/// 
	/// @see LookupDeclaredVarMem() 
	/// 
	/// @note memory varialble are declared using opcode DC or DS
	/// 
    /// @param a_symbol address of symbol to lookup
    /// 
    /// @returns LookupSymbol returns true is symbol is found in table else returns false
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 25, 2021 
    /// 
    bool LookupDeclaredConst( const std::string& a_symbol );
    
    ///  
    /// @brief Lookup a symbol in the declared variable memory table.
	/// 
	/// This is used to validate if the lable used as operand1 
	///		for ADD, SUB, MULT, DIV, COPY, READ, and WRITE
	///		should point to memory variable
	///  
    /// @see LookupDeclaredConst() 
	/// 
	/// @note memory varialble are declared using opcode DC or DS
	/// 
    /// @param a_symbol address of symbol to lookup
    /// 
    /// @returns LookupSymbol returns true is symbol is found in table else returns false 
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 25, 2021 
    /// 
    bool LookupDeclaredVarMem( const std::string& a_symbol );
    
    ///  
    /// @brief TranslateInstruction translates the given assembly instruction as much
    ///     as possible.
	/// 
	/// All of errors are reported 
    /// 
	/// @param a_loc current location
	/// @param a_LineCounter current line
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 25, 2021 
	/// 
    void TranslateInstruction( int a_Loc, int a_LineCounter );

    ///
    /// @brief ComputeNextLoc computes next location
    /// 
	///	ComputeNextLoc sets the a_loc value to operand1 if opCode is ORG,
	///		adds the value in operand1 to a_loc if opCode is DS,
	///		else it increments a_loc by 1
	/// 
    /// @param a_loc address of loction variable
    /// @param a_LineCounter address of lineCounter variable
    /// @param a_insufficentMemory address of variable that signifiy if there
    ///         insufficent memory error was flaged or not
	/// 
	/// @author Aayush Karki
    /// 
	/// @date  November 25, 2021  
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

