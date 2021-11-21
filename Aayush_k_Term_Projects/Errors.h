///
/// 
///
#ifndef _ERRORS_H
#define _ERRORS_H

#include <string>
#include <vector>


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
    

    /// 
    /// @brief RecordErrorRecords an error message
    ///
    /// @param a_emsg: error to add to the list
    /// 
    static void RecordError( std::string a_emsg ) { m_ErrorMsgs.push_back( a_emsg ); }
    
    /// 
    /// @brief DisplayAllErrors Displays all the collected error message.
    /// 
    /// @todo: make this more pretty and presentable 
    /// 
    static void DisplayAllErrors();

private:

    static std::vector<std::string> m_ErrorMsgs;  // list of all the error messages record    
};
#endif