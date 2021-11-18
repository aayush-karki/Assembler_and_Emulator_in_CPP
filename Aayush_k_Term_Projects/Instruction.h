//
// Instruction Class
// 

#pragma once

#include <unordered_map>

#include "Errors.h"

///
/// @class Instruction  "Instruction.h"
/// @brief Class to parse and provide information about instructions. 
/// 
/// This class deals entirely with the input string realated things.
/// Like scanning the given line of string  
/// and dividing it into componenets 
/// 
/// @note you will be adding more functionality.
/// The elements of an instruction.
class Instruction {

public:

    // todo: add a function to clean the variable after each loop/or before parsing 

    /// 
    /// @brief Constructor for the Instruction class. 
    /// 
    /// It sets int variable to 0, bool variable to  false, string variable to "", 
    ///     and Instruction::Instruction variavle to ST_Comment.
    /// 
    Instruction() { ClearMemberVariables(); }
    
    /// 
    /// @brief Deconstructor for the Instruction class. 
    /// 
    ~Instruction( ) { };

    ///
    /// @enum InstructionType
    /// 
    /// @brief Instruction type indicates the type of instruction we are processing.  
    /// 
    enum class InstructionType 
    {
        ST_MachineLanguage, 	///< A machine language instruction.
        ST_AssemblerInstr,  	///< Assembler Language instruction.
        ST_Comment,          	///< Comment or blank line
        ST_End                  ///< end instruction.
    };

    ///
    /// @brief ParseInstruction Parses the line of string into its component.
    /// 
    /// ParseInstruction function goes through the passed string and 
    ///     seperates them into their indivisual componenet which are stored and 
    ///     be used by parsing
    /// 
    /// @param a_line: a string read from the file
    /// 
    /// @return ParseInstruction return a corresponding Instruction::InstructionType.
    ///          
    Instruction::InstructionType ParseInstruction( std::string a_line);

    ///
    /// @brief LocationNextInstruction Compute the location of the next instruction.
    /// 
    /// If opCode is ORG, next intruction location is the value in operand 1,
    /// if opCode is DS, it increases current location by the value in operand 1;
    /// else it increases current location by  1.
    /// 
    /// @param a_loc: Location of current instruction
    /// 
    /// @return LocationNextInstruction return a location of 
    ///     next intruction
    /// 
    int LocationNextInstruction( int a_loc );

    ///
    /// @brief getter function to access the label
    /// 
    /// @return location of the label if present; 
    ///         if label is not present return NULL
    /// 
    inline const std::string& GetLabel(){ return m_Label; }

    ///
    /// @brief IsLabel checks if a label is blank.
    /// 
    /// @return IsLabel returns true if label is present; 
    ///     false if label is absent
    /// 
    inline bool IsLabel() const{ return !m_Label.empty(); }
    
    ///
    /// @brief getter function to access the numerical 
    ///         value of op code
    /// 
    /// @return GetOpCodeNum returns location of the 
    ///         Numeric OpCode 
    ///
    inline const int GetOpCodeNum(){ return m_NumOpCode % 100; }

    ///
    /// @brief getter function to access the orginal instruction
    /// 
    /// @return GetOrgiInst returns orginal instruction
    ///
    inline const std::string GetOrgiInst() { return m_Instruction; }

    // ===================== private functions ===================
private:

    /// 
    /// @brief ClearMemberVariables cleans all non-constant member variable.
    /// 
    /// ClearMemberVariable sets them to 0 if int, false if bool,
    ///  "" if string, and ST_Comment if Instruction::InstructionType
    /// 
    void ClearMemberVariables();

    /// 
    /// @brief RemoveComment removes everything after first instances of a ';'
    /// 
    /// @param a_line: address of the instuction  
    /// 
    inline void RemoveComment( std::string& a_line )
    { 
        a_line = a_line.substr( 0, a_line.find( ';' ) ); 
    }

    /// 
    /// @brief IsNum returns true if the argument string is a digit
    /// 
    /// IsNum check if all the character in the string
    ///     is a digit.
    /// 
    /// @param a_string: string to which the test is done on
    /// 
    /// @return IsNum returns true if the argument 
    ///         string is a digit, else false 
    ///    
    inline const bool IsNum(std::string a_string)
    {
        // std::all_of checks for all the element in a container
        return !a_string.empty() &&
            std::all_of( a_string.begin(), a_string.end(), std::isdigit );
    }

    /// 
    /// @brief IsOpCode returns true if the argument string is in 
    ///     enum Instruction::OpCode
    /// 
    /// @param a_string: string to which the test is done on
    /// 
    /// @return IsOpCode returns true if the argument 
    ///         string is found in the opCodeTable, else false 
    /// 
    const bool IsOpCode(std::string a_string) 
    {
        return opCodeTable.find( AllUpperCase( a_string ) ) != opCodeTable.end();
    }

    
    /// @brief  AllUpperCase capitalises the passes string argument
    ///
    /// @param a_string: string to which the test is done on
    /// 
    /// @return AllUpperCase returns a uppercased string
    ///     
    std::string AllUpperCase(std::string a_string)
    {
        std::transform( a_string.begin(), a_string.end(), a_string.begin(), std::toupper );
        return a_string;
    }

    // ================ private variables ============================
private:
    
    std::string m_Instruction;  // The original instruction.
    
                                // The elemements of a instruction
    std::string m_Label;        // The label.
    std::string m_OpCode;       // The symbolic op code.
    std::string m_Operand1;     // The first operand. 
    std::string m_Operand2;     // The second operand.

    std::pair<bool, std::string> m_ErrorFlag;             // m_ErrorFlag.first == true if there is error in the input
                                                        // m_ErrorFlag.second containst the type of error

    // ===========  Derived values ===========
    Instruction::InstructionType m_Type; // The type of instruction.
    
    // The numerical value of the op code for machine language equivalents.
    int m_NumOpCode;     

    bool m_IsNumericOperand1;   // == true if the operand is numeric.
    int m_Operand1Value;   // The value of the operand1 if it is numeric.

    // The value of the operand2 if it is numeric, 
    // only used if machine language instruction
    int m_Operand2Value;

    // note assemble only intrusction are multiple of 100
    // so that when dividing the code number by 100 the remender
    // will be the actual code number
    // opCode lookup table 
    const std::unordered_map<std::string, int> opCodeTable
    {
        {"ADD", 1},
        {"SUB", 2},
        {"MULT", 3},
        {"DIV", 4},
        {"COPY", 5},
        {"READ", 7},
        {"WRITE", 8},
        {"B", 9},
        {"BM", 10},
        {"BZ", 11},
        {"BP", 12},
        {"HALT", 13},
        {"ORG", 100},
        {"DC", 200},
        {"DS", 300},
        {"END", 400},
    };
};


