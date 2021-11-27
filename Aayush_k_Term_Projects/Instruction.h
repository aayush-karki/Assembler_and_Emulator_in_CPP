//
// Instruction Class
// 

#pragma once

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
        ST_End,                 ///< end instruction.
        ST_Error                ///< error dectected in instruction.
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
    inline const int GetOpCodeNum(){ return m_NumOpCode; }

    ///
    /// @brief getter function to access the content 
    ///         stored in of operand1
    /// 
    /// @return GetOperand1 returns content stored in of operand1
    ///
    inline const std::string GetOperand1(){ return m_Operand1; }

    ///
    /// @brief getter function to access the content 
    ///         stored in of operand2
    /// 
    /// @return GetOperand2 returns content stored in of operand2
    ///
    inline const std::string GetOperand2(){ return m_Operand2; }

    ///
    /// @brief getter function to access the numerical 
    ///         value of operand 1
    /// 
    /// @return GetNumOperand1 returns numeric value of operand1 
    ///
    inline const int GetNumOperand1(){ return m_Operand1Value; }

    ///
    /// @brief getter function to access the numerical 
    ///         value of operand 2
    /// 
    /// @return GetNumOperand2 returns numeric value of operand2 
    ///
    inline const int GetNumOperand2(){ return m_Operand2Value; }

    ///
    /// @brief IsNumOperand1 check to see if operand1 is numeric
    /// 
    /// @return IsNumOperand1 returns true if Operand 1 is numeric
    ///         else returns false
    ///
    inline const bool IsNumOperand1(){ return m_IsNumericOperand1; }

    ///
    /// @brief IsNumOperand2 check to see if operand2 is numeric
    /// 
    /// @return IsNumOperand2 returns true if Operand 2 is numeric
    ///         else returns false
    ///
    inline const bool IsNumOperand2(){ return m_IsNumericOperand2; }

    ///
    /// @brief getter function to access the orginal instruction
    /// 
    /// @return GetOrgiInst returns orginal instruction
    ///
    inline const std::string GetOrgiInst() { return m_Instruction; }

    /// 
    /// @brief Getter function.
    /// 
    /// @returns GetErrorFlagReport returns the 
    /// 
    /// @return GetErrorFlagReport returns value stored in error Flag
    /// 
    const std::string GetErrorMessage() { return m_ErrorMessage; }

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
    /// @returns IsNum returns true if the argument 
    ///         string is a digit, else false 
    ///    
    inline const bool IsNum(std::string a_string)
    {
        // std::all_of checks for all the element in a container
        return std::all_of( a_string.begin(), a_string.end(), std::isdigit );
    }

    /// 
    /// @brief IsOpCode returns true if the argument string is in 
    ///     enum Instruction::OpCode
    /// 
    /// @param a_string: string to which the test is done on
    /// 
    /// @returns IsOpCode returns true if the argument 
    ///         string is found in the opCodeTable, else false 
    /// 
    const bool IsOpCode(std::string a_string) 
    {
        return opCodeTable.find( AllUpperCase( a_string ) ) != opCodeTable.end();
    }
    
    ///
    /// @brief  AllUpperCase capitalises the passes string argument
    ///
    /// @param a_string: string to which the test is done on
    /// 
    /// @returns AllUpperCase returns a uppercased string
    ///     
    std::string AllUpperCase(std::string a_string)
    {
        std::transform( a_string.begin(), a_string.end(), a_string.begin(), std::toupper );
        return a_string;
    }

    ///
    /// @brief  SetFundamentalMemVar sets the member varible unless there is a Invalid syntax
    ///
    /// @param a_indivisualInstruction: is the extraxted words form the line
    ///
    ///  @return ValidateSymSyntax returns false if error was found
    /// 
    bool SetFundamentalMemVar( std::vector<std::string>& a_indivisualInstruction );

    ///
    /// @brief setter function to set the Numeric equivalent of Operation code
    /// 
    void SetNumOpCode();

    ///
    /// @brief setter function to set numiric first operand
    /// 
    /// checks if the operand1 is a number if it is converst it into 
    ///     integer and stores it into m_Operand1Value member variable
    /// 
    void SetNumOperand1();

    ///
    /// @brief setter function to set numiric second operand
    /// 
    /// checks if the operand2 is a number if it is converst it into 
    ///     integer and stores it into m_Operand2Value member variable
    /// 
    void SetNumOperand2();

    /// 
    /// @brief ValidateSymSyntax checks if a symbol meets the requirement
    /// 
    /// If the symbol does not meet the requirement it set the error
    /// 
    /// @param a_Symbol symbol to check the requirements on
    /// 
    /// @return ValidateSymSyntax returns false if error was found
    ///
    bool ValidateSymSyntax(std::string a_Symbol);

    /// 
    /// @brief ValidateOpCodeSyntax calls a correspoing validating opCode syntax function
    /// @todo fix this see if fucntion makes sense
    /// 
    /// @return ValidateSymSyntax returns false if error was found
    /// 
    /// 
    bool ValidateOpCodeSyntax();

    // ================ private variables ============================
private:
    // =========== Fundamental Values ====================

    std::string m_Instruction;  // The original instruction.
    
                                // The elemements of a instruction
    std::string m_Label;        // The label.
    std::string m_OpCode;       // The symbolic op code.
    std::string m_Operand1;     // The first operand. 
    std::string m_Operand2;     // The second operand.


    // ===========  Derived values ===========
    Instruction::InstructionType m_Type; // The type of instruction.
    
    std::string m_ErrorMessage;    // Containst the error message
    
    // The numerical value of the op code for machine language equivalents.
    int m_NumOpCode;     

    bool m_IsNumericOperand1;   // == true if the operand1 is numeric.
    int m_Operand1Value;   // The value of the operand1 if it is numeric.

    bool m_IsNumericOperand2;   // == true if the operand2 is numeric.
    int m_Operand2Value;   // The value of the operand2 if it is numeric.

    /// @note assemble only intrusction are multiple of 100
    ///     so that when dividing the code number by 100 the remender
    ///     will be the actual code number
    
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


