/*************************************************************************************/
/// 
/// @file Errors.h 
/// 
/// @brief  This file is a header file for Errors class.
/// 
/// It contains all of the declaration of the member funciton, enum of Errors class.
/// It contains all of the inline funcitons defination of  the member funciton of Errors class 
///
/// @author Aayush Karki
///
/// @date  November 11, 2021 
///  
/*************************************************************************************/

#ifndef _ERRORS_H
#define _ERRORS_H

///
/// @class Errors  "Errors.h"
/// 
/// @brief Class to manage all error reporting.
/// 
/// Errors class deals entirely with reporting errors.
/// 
/// @note: all members are static so that we can access them anywhere in program
///			and report errors from anywher in the program.
/// 
/// @author Aayush Karki
/// 
/// @date  November 11, 2021 
/// 
class Errors {

public:
    

    /// 
    /// @brief InitErrorReporting Initializes error reports.
    ///
    /// It cleares the lists of error messages
    /// 
    static void InitErrorReporting() { m_ErrorMsgs.clear(); }
    
	///
	/// @enum ErrorTypes  "Errors::ErrorTypes"
	/// 
	/// @brief ErrorTypes contains all type of errors that could occur during the 
	///		execution of the program
	/// 
	/// @note if not implicitly mentioned, all the error types are  used by assembler
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 11, 2021 
	/// 
    enum class ErrorTypes
    {
        ERROR_InvalidInstruction,			///> when operation code in machine level code is not recognized, used by emulator
        ERROR_MissingEnd,					///> when END instruction is missing
        ERROR_MissingHalt,					///> when halt instruction is missing
        ERROR_EndNotLast,					///> when there are more instruciton after END instruction
        ERROR_MachineLangInAssemLang,		///> when machine level code is found in assembly level code
        ERROR_UndefinedLabel,				///> when label used in operand1 or/and operand2 are not present in symbol table
        ERROR_MultipleLabel,				///> when multiple of same label is used
        ERROR_ExtraStatEle,					///> when there are more number of elements in an instruciton than the operation code needs
        ERROR_OpCodePos,					///> when operation code is the third or forth element in the instruction
        ERROR_MissingOpCode,				///> when operation code is absent
        ERROR_MissingLabel,					///> when label is missing for intruction which shcould have instruciton
        ERROR_MissingOperand,				///> when instruction is missing operand. used for both operand1 and operand2
        ERROR_ExtraOperand1,				///> when operation code does not allow operand1
        ERROR_ExtraOperand2,				///> when operation code does not allow operand2
        ERROR_NotNumOperand1,				///> when operation code only accepts number as operand1
        ERROR_NotLabelOperand1,				///> when operation code only accepts label as operand1
        ERROR_NotLabelOperand2,				///> when operation code only accepts label as operand2 
        ERROR_InvalidOperand1Len,			///> when length of number user as operand1 is more than 5
        ERROR_NotLabelConst,				///> when label used for operand does points to memory variable
        ERROR_InvalidSymSyntaxLen,			///> when label length is more than 10 characters
        ERROR_InvalidSymSyntaxfirstChar,	///> when first character of a label is not a alphabet
        ERROR_InvalidSymSyntaxComposition,  ///> when even one of hte character used for label name is not alphabet or number
        ERROR_InsufficentMemory,			///> when there is insufficent memory for instruction translation
        ERROR_InvalidLoc,					///>  when memory address is less than 0 or more than MEMSZ, used by emulator
        ERROR_InvalidInputSyn,				///> when a non-number input is entered by the user
        ERROR_InvalidInputRange				///> when number input entered by user is out of  (+/-)999,999,999,999 range
    };

    /// 
	/// @brief RecordError constructs an error message accoring to the error type
	///		and adds the constructed message to the list 
	/// 
	/// @note RecordError is a static member funciton.
	/// 
    /// @param a_errorType type of error found
    /// @param a_instIndicator is Line if instruction is assembly code; else should be Loc for machine code 
    /// @param a_LineCounter current line number or memory location
    /// @param a_OrgiInst orginal instruction where error was found/occured
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 11, 2021 
	/// 
    static void RecordError( Errors::ErrorTypes a_errorType, std::string a_instIndicator, int a_LineCounter, std::string a_OrgiInst );
    
    /// 
    /// @brief DisplayAllErrors Displays all the recorded error message.
    /// 
    /// @author Aayush Karki
	/// 
	/// @date  November 12, 2021
    /// 
    static void DisplayAllErrors();

    /// 
	/// @brief LookUpErrorMsgs Looks up error message associated with each error type.
    /// 
	/// @param a_errorType type of error to look up
    /// 
	/// @return error message of that error
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 12, 2021 
	/// 
    static std::string LookUpErrorMsgs( Errors::ErrorTypes  a_errorType);


    ///
    /// @brief Getter function to get the beginning of error message
    /// 
    /// @return GetErrorMessageBegin returns a pointer to begining of the error message
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 12, 2021 
    ///
    static const std::vector<std::string>::iterator GetErrorMessageBegin(){ return m_ErrorMsgs.begin(); }

    ///
    /// @brief getter function to get the end of error message
    /// 
    /// @return GetDefaultContent returns a pointer to end of the error message
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 12, 2021 
    ///
    static const std::vector<std::string>::iterator GetErrorMessageEnd(){ return m_ErrorMsgs.end(); }


private:
    static std::vector<std::string> m_ErrorMsgs;  // list of all the error messages record    
};
#endif