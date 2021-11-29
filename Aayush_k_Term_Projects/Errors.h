///
/// 
///
#ifndef _ERRORS_H
#define _ERRORS_H

///
/// @class Errors  "Errors.h"
/// @brief Class to manage error reporting.
/// 
/// This class deals entirely with reporting errors and
/// continue on so that we can read thorough the whole 
/// file and report all the errors in it.
/// 
/// @Note: all members are static so we can access them anywhere.
/// 
class Errors {

public:
    

    /// 
    /// @brief InitErrorReporting Initializes error reports.
    ///
    /// It cleares the vector
    /// 
    static void InitErrorReporting() { m_ErrorMsgs.clear(); }
    
    enum class ErrorTypes
    {
        ERROR_InvalidInstruction,
        ERROR_MissingEnd,
        ERROR_EndNotLast,
        ERROR_MachineLangInAssemLang,
        ERROR_UndefinedLabel,
        ERROR_MultipleLabel,
        ERROR_ExtraStatEle,
        ERROR_OpCodePos,
        ERROR_MissingOpCode,
        ERROR_MissingLabel,
        ERROR_MissingOperand,
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
        ERROR_InsufficentMemory
    };

    ///  
    /// @brief RecordErrorRecords an error message
    ///
    /// @param a_emsg: error to add to the list
    /// 
    static void RecordError( Errors::ErrorTypes a_errorType, int a_LineCounter, std::string a_OrgiInst );
    
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