/*************************************************************************************/
/// 
/// @file Errors.h 
/// 
/// @brief  This file is a header file for Errors class.
/// 
/// It contains all of the declaration of the member funciton, enum of Errors class.
/// It contains all of the defination of  the member funciton of Errors class 
///		who might/are quality as inline funcitons.
///
/// @author Aayush Karki
///
/// @date  November 10, 2021 
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
/// @Note: all members are static so that we can access them anywhere in program
///			and report errors from anywher in the program.
/// 
/// @author Aayush Karki
/// 
/// @date  November 10, 2021 
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
	/// @note if not implicitly mentioned, all the error used by assembler
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 10, 2021 
	/// 
    enum class ErrorTypes
    {
        ERROR_InvalidInstruction,	///> when operation code in machine level code is not recognized, used by emulator
        ERROR_MissingEnd,	///> when END instruction is missing
        ERROR_MissingHalt,	///> when halt instruction is missing
        ERROR_EndNotLast,	///> when there are more instruciton after END instruction
        ERROR_MachineLangInAssemLang, ///> when machine level code is found in assembly level code
        ERROR_UndefinedLabel, ///> when label used in operand1 or/and operand2 are not present in symbol table
        ERROR_MultipleLabel, ///> when multiple of same label is used
        ERROR_ExtraStatEle, ///> when there are more number of elements in an instruciton than the operation code needs
        ERROR_OpCodePos, ///> when operation code is the third or forth element in the instruction
        ERROR_MissingOpCode, ///> when operation code is absent
        ERROR_MissingLabel, ///> when label is missing for intruction which shcould have instruciton
        ERROR_MissingOperand, ///> when instruction is missing operand. used for both operand1 and operand2
        ERROR_ExtraOperand1,
        ERROR_ExtraOperand2,
        ERROR_NotNumOperand1,
        ERROR_NotLabelOperand1,
        ERROR_NotLabelOperand2,
        ERROR_InvalidOperand1Len,
        ERROR_NotLabelConst,
        ERROR_InvalidSymSyntaxLen,
        ERROR_InvalidSymSyntaxfirstChar,
        ERROR_InvalidSymSyntaxComposition,
        ERROR_InsufficentMemory,
        ERROR_InvalidLoc,
        ERROR_InvalidInputSyn,
        ERROR_InvalidInputRange
    };

    ///  
    /// @brief RecordErrorRecords an error message
    ///
    /// @param a_emsg: error to add to the list
    /// 
    static void RecordError( Errors::ErrorTypes a_errorType, std::string a_instIndicator, int a_LineCounter, std::string a_OrgiInst );
    
    /// 
    /// @brief DisplayAllErrors Displays all the collected error message.
    /// 
    /// @todo: make this more pretty and presentable 
    /// 
    static void DisplayAllErrors();

    static std::string LookUpErrorMsgs( Errors::ErrorTypes  a_errorType);


    ///
    /// @brief getter function to access the first element in error message
    /// 
    /// @return GetErrorMessageBegin returns a pointer to begining of the vector
    ///
    static const std::vector<std::string>::iterator GetErrorMessageBegin(){ return m_ErrorMsgs.begin(); }

    ///
    /// @brief getter function to access the end of in error message
    /// 
    /// @return GetDefaultContent returns a pointer to end of the vector
    ///
    static const std::vector<std::string>::iterator GetErrorMessageEnd(){ return m_ErrorMsgs.end(); }


private:
    static std::vector<std::string> m_ErrorMsgs;  // list of all the error messages record    
};
#endif