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
    /// @brief DesplayDeclaredConstTab Displays all the declared Constant number along with their lablels
    /// 
    void DisplayDeclaredConstTab();
    
    /// 
    /// @brief Displays the translated machine instruction in the machine instruction table.
    /// 
    void DisplayMachineInstTable() { m_machInstTab.DisplayMachineInstTable(); }

    /// 
    /// @brief Run emulator on the translation.
    /// 
    void RunProgramInEmulator() {};

    // ================ private member funciton ============================
private:
    /// 
    /// @brief ValidateOperand1Lab checks to see if Label in Operand1 points to a constant
    /// 
    /// ValidateOperand1Lab only checks if operand 1 of ORG, DC or DS is a label 
    ///     and if it is it checkes if it points to a constant
    /// 
    /// If the the Label does not point to a declared constant number
    ///     it reports a error
    /// 
    /// @param a_LineCounter current line number, necessary to falg error reports
    /// 
    /// @returns ValidateOperand1Lab returns true if it does else false
    /// 
    bool ValidateOperand1Lab( int a_LineCounter );

    /// 
    /// @brief ValidateOperand2Lab checks to see if Label in Operand2 points to a constant
    /// 
    /// ValidateOperand2Lab only checks if operand 1 of  ADD, SUB, MULT, or DIV is a label 
    ///     and if it is it checkes if it points to a constant
    /// 
    /// If the the Label does not point to a declared constant number
    ///     it reports a error
    /// 
    /// @param a_LineCounter current line number, necessary to falg error reports
    /// 
    /// @returns ValidateOperand2Lab returns true if it does else false
    /// 
    bool ValidateOperand2Lab( int a_LineCounter );

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

    // ================ private variables ============================
private:

    FileAccess m_facc;	    // File Access object
    SymbolTable m_symtab;   // Symbol table object
    Instruction m_inst;	    // Instruction object
    MachineInstructionTable m_machInstTab; // Machine Instruction Table object
    Emulator m_emul;        // Emulator object

    // table to store all the declared Constant number along with their lablels
    std::unordered_map<std::string, int> m_declaredConstTab; 
};

