/*************************************************************************************/
/// 
/// @file Instruction.h 
/// 
/// @brief  This file is a header file for Instruction class.
/// 
/// It contains all of the declaration of the member funciton of Instruction class.
/// It contains all of the inline funcitons defination of  the member funciton of Instruction class 
/// It contains enum class InstructionType within the Instruction class 
/// 
/// @author Aayush Karki
/// 
/// @date  November 12, 2021 
///  
/*************************************************************************************/

#pragma once

#include "Errors.h"

///
/// @class Instruction  "Instruction.h"
/// @brief Class to parse and provide information about instructions. 
/// 
/// This class deals entirely with the input string realated things.
///		Like scanning the given line of string and dividing it into componenets,
///		validating the instructions.
/// 
/// @note this class is not used by Emulator class
/// 
/// @author Aayush Karki
/// 
/// @date  November 12, 2021 
/// 
class Instruction
{


public:
	// ============================= public member functions ============================

	/// 
	/// @brief Constructor for the Instruction class. 
	/// 
	/// It sets int variable to 0, bool variable to  false, string variable to "", 
	///     and Instruction::Instruction variavle to ST_Comment.
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 12, 2021
	/// 
	Instruction() { ClearMemberVariables(); }

	/// 
	/// @brief Deconstructor for the Instruction class. 
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 12, 202
	/// 
	~Instruction() {};

	///
	/// @enum InstructionType
	/// 
	/// @brief Instruction type indicates the type of instruction we are processing.
	/// 
	/// @author Victor Miller
	/// @author Aayush Karki
	/// 
	/// @date  November 12, 202
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
	/// ParseInstruction function goes through the passed instruction and 
	///     seperates them into their indivisual componenet which are stored and 
	///     be used by parsing
	/// 
	/// @note It calls on private memmber funcitons to validate the input 
	///		and break it apart
	/// 
	/// @param a_line: a string read from the file
	/// 
	/// @return ParseInstruction return a corresponding Instruction::InstructionType.
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 20, 202
	///          
	Instruction::InstructionType ParseInstruction( std::string a_line );

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
	/// @author Aayush Karki
	/// 
	/// @date  November 20, 202
	/// 
	int LocationNextInstruction( int a_loc );

	///
	/// @brief getter function to access the label
	/// 
	/// @return location of the label if present; 
	///         if label is not present return NULL
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 20, 202
	/// 
	inline const std::string& GetLabel() { return m_Label; }

	///
	/// @brief IsLabel checks if a label is blank.
	/// 
	/// @return IsLabel returns true if label variable is not empty; 
	///     false if label is absent
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 20, 202
	/// 
	inline bool IsLabel() const { return !m_Label.empty(); }

	///
	/// @brief getter function to access the numerical 
	///         value of op code
	/// 
	/// @return GetOpCodeNum returns of the Numeric OpCode 
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 20, 202
	///
	inline const int GetOpCodeNum() { return m_NumOpCode; }

	///
	/// @brief getter function to access the content 
	///         stored in of operand1
	/// 
	/// @return GetOperand1 returns content stored in of operand1
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 20, 202
	///
	inline const std::string GetOperand1() { return m_Operand1; }

	///
	/// @brief getter function to access the content 
	///         stored in of operand2
	/// 
	/// @return GetOperand2 returns content stored in of operand2
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 20, 202
	///
	inline const std::string GetOperand2() { return m_Operand2; }

	///
	/// @brief getter function to access the numerical 
	///         value of operand 1
	/// 
	/// @return GetNumOperand1 returns numeric value of operand1
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 20, 202 
	///
	inline const long long GetNumOperand1() { return m_Operand1Value; }

	///
	/// @brief getter function to access the numerical 
	///         value of operand 2
	/// 
	/// @return GetNumOperand2 returns numeric value of operand2 
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 20, 202
	///
	inline const int GetNumOperand2() { return m_Operand2Value; }

	///
	/// @brief IsNumOperand1 check to see if operand1 is numeric
	/// 
	/// @return IsNumOperand1 returns true if Operand 1 is numeric
	///         else returns false
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 20, 202
	///
	inline const bool IsNumOperand1() { return m_IsNumericOperand1; }

	///
	/// @brief IsNumOperand2 check to see if operand2 is numeric
	/// 
	/// @return IsNumOperand2 returns true if Operand 2 is numeric
	///         else returns false
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 20, 202
	///
	inline const bool IsNumOperand2() { return m_IsNumericOperand2; }

	///
	/// @brief getter function to access the orginal instruction
	/// 
	/// @return GetOrgiInst returns orginal instruction
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 20, 202
	///
	inline const std::string GetOrgiInst() { return m_Instruction; }

	/// 
	/// @brief Getter function to get start of error message type.
	/// 
	/// @return GetErrorMsgTypeBegin returns pointer to start of ErrorMessageType
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 20, 202
	/// 
	std::vector<Errors::ErrorTypes>::iterator GetErrorMsgTypeBegin() { return m_ErrorMsgType.begin(); }

	/// 
	/// @brief Getter function to get end of error message type.
	/// 
	/// 
	/// @return GetErrorMsgTypeEnd returns pointer to end of ErrorMessageType
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 20, 202
	/// 
	std::vector<Errors::ErrorTypes>::iterator GetErrorMsgTypeEnd() { return m_ErrorMsgType.end(); }

	///
	/// @brief getter function
	/// 
	/// @return IsErrorLabelSyn returns true if error was found for op code
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 20, 202
	///
	inline const bool IsErrorOpCode() { return m_ErrorOpCode; }

	///
	/// @brief getter function
	/// 
	/// @return IsErrorOperand1 returns true if error was found for operand 1
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 20, 202
	///
	inline const bool IsErrorOperand1() { return m_ErrorOperand1; }

	///
	/// @brief getter function
	/// 
	/// @return IsErrorOperand2 returns true if error was found for operand 1
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 20, 202
	///
	inline const bool IsErrorOperand2() { return m_ErrorOperand2; }

	///
	/// @brief getter function
	/// 
	/// If this is true, then instruction is already translated
	/// 
	/// @see  SetFundamentalVar(), error case 2 and case 3 
	/// 
	/// @return IsErrorFundVar returns true if error was found for Fundamental variable
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 20, 202
	///
	inline const bool IsErrorFundVar() { return m_ErrorFundVar; }

private:
	// ===================== private functions ===================

	/// 
	/// @private
	/// @brief ClearMemberVariables cleans all non-constant member variable.
	/// 
	/// ClearMemberVariable sets the member varialbe to 0 for int type, 
	///		false for bool type,
	///		"" for string type, and
	///		 ST_Comment for Instruction::InstructionType type 
	/// 
	/// @note Instruction::InstructionType is a enum
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 20, 202
	/// 
	void ClearMemberVariables();

	/// 
	/// @private
	/// @brief RemoveComment removes everything after first instances of a ';'
	///		from the parameter a_line
	/// 
	/// @param a_line: address of the instuction 
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 20, 202 
	/// 
	inline void RemoveComment( std::string& a_line )
	{
		a_line = a_line.substr( 0, a_line.find( ';' ) );
	}

	/// 
	/// @private
	/// @brief IsNum returns true if the parameter string is a digit
	/// 
	/// IsNum check if all the character in the string
	///     is a digit.
	/// 
	/// @param a_string: string to which the test is done on
	/// 
	/// @returns IsNum returns true if the argument 
	///         string is a digit, else false 
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 20, 202
	///    
	inline const bool IsNum( std::string a_string )
	{
		// std::all_of checks for all the element in a container
		return std::all_of( a_string.begin(), a_string.end(), std::isdigit );
	}

	/// 
	/// @private
	/// @brief IsOpCode returns true if the parameter a_string is in 
	///     enum Instruction::OpCode
	/// 
	/// @param a_string: string to which the test is done on
	/// 
	/// @returns IsOpCode returns true if the argument 
	///         string is found in the opCodeTable, else false
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 20, 20 
	/// 
	const bool IsOpCode( std::string a_string )
	{
		return opCodeTable.find( AllUpperCase( a_string ) ) != opCodeTable.end();
	}

	///
	/// @private
	/// @brief  AllUpperCase capitalises teh content of parameter a_string
	///
	/// @note This funciton is called tor converting operation code to upper case
	/// 
	/// @param a_string: string to which the test is done on
	/// 
	/// @returns AllUpperCase returns a uppercased string
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 20, 20
	///     
	std::string AllUpperCase( std::string a_string )
	{
		std::transform( a_string.begin(), a_string.end(), a_string.begin(), std::toupper );
		return a_string;
	}

	///
	/// @private
	/// @brief  SetFundamentalVar sets the member varible unless there is a Invalid syntax
	///
	/// This funciton detects error in input and deals them according to the error cases.
	/// 
	/// Error Cases:
	/// 
	///     case 1: if extra statement element error was detected it deletes the extra statement element
	///			while flaging the error
	/// 
	///     case 2: if only opcode was suppled, with the exception of END and HALT it set the corresponding
	///			numerical equivalent of the opcode and  error signifier of set operand1 and operand2 to true
	///			while flaging the error and set the numerical equivalent of the opCode
	/// 
	///     case 3: if any other error was found it sets error signifier of label, opCode, operand1 and
	///         operand2 to true depending to what caused the error while flaging the error 
	///         and tries to continue translation
	/// 
	/// @note SetFundamentalVar only sets he fundamental variable, it does not validate the input
	///     validation is done by ValidateOpCodeSyntax()
	/// 
	/// @param a_indivisualInstruction: is the extraxted words form the line
	///
	/// @return ValidateSymSyntax returns false in error case 2 and 3; else true
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 20, 20
	/// 
	bool SetFundamentalVar( std::vector<std::string>& a_indivisualInstruction );

	///
	/// @private
	/// @brief setter function to set the Numeric equivalent of Operation code
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 20, 20
	/// 
	void SetNumOpCode();

	///
	/// @private
	/// @brief setter function to set numiric first operand
	/// 
	/// checks if the operand1 is a number if it is converst it into 
	///     long long and stores it into m_Operand1Value member variable
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 20, 20
	/// 
	void SetNumOperand1();

	///
	/// @private
	/// @brief setter function to set numiric second operand
	/// 
	/// checks if the operand2 is a number if it is converst it into 
	///     integer and stores it into m_Operand2Value member variable
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 20, 20
	/// 
	void SetNumOperand2();

	/// 
	/// @private
	/// @brief ValidateSymSyntax checks if a symbol meets the requirement
	/// 
	/// If the symbol does not meet the requirement it set the error
	/// 
	/// @param a_Symbol symbol to check the requirements on
	/// 
	/// @return ValidateSymSyntax returns false if error was found
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 20, 20
	///
	bool ValidateSymSyntax( std::string a_Symbol );

	/// 
	/// @private
	/// @brief ValidateOpCodeSyntax checks if the instruction syntax was vaild or not
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 20, 20
	/// 
	void ValidateOpCodeSyntax();


private:
	// ================ private variables ============================

	// =========== Fundamental Values ====================

	std::string m_Instruction;  // The original instruction.

								// The elemements of a instruction
	std::string m_Label;        // The label.
	std::string m_OpCode;       // The symbolic op code.
	std::string m_Operand1;     // The first operand. 
	std::string m_Operand2;     // The second operand.

	// error indicators
	std::vector<Errors::ErrorTypes> m_ErrorMsgType;    // Contains the error message

	bool m_ErrorOpCode;     // == true if error in opCode
	bool m_ErrorOperand1;   // == true if error in Operand1
	bool m_ErrorOperand2;   // == true if error in Operand2
	bool m_ErrorFundVar;    // == true if error was found in fundamental variables

	// ==============  Derived values ==============
	Instruction::InstructionType m_Type; // The type of instruction.


	// The numerical value of the op code for machine language equivalents.
	int m_NumOpCode;

	bool m_IsNumericOperand1;   // == true if the operand1 is numeric.
	long long m_Operand1Value;   // The value of the operand1 if it is numeric.

	bool m_IsNumericOperand2;   // == true if the operand2 is numeric.
	int m_Operand2Value;   // The value of the operand2 if it is numeric.

	/// 
	/// @private
	/// @brief operation code lookup table 
	/// 
	/// This is a list of all the operation code recognized by the assembler.
	///		This list maps the Operation code to their numerical value 
	///		recognized by the emulator 
	/// 
	/// @note the lookup table contains operation code used by both the 
	///		emulator and assembler 
	/// @note assemble only intruction are multiple of 100
	///     so that when dividing the code number by 100 the remender
	///     will be the actual code number
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 20, 20
	/// 
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


