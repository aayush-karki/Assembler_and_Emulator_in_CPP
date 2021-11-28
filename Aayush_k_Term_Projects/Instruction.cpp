#include "stdafx.h"
#include "Instruction.h"

Instruction::InstructionType Instruction::ParseInstruction( std::string a_line )
{
	// clear all of the m_variables
    ClearMemberVariables();

    // storing the orginal instruction
    m_Instruction = a_line;

    // removing everything after ';'
    RemoveComment( a_line );

    // checking if a_line is empty, if yes then it is a comment line
    if (a_line.empty())
    {
        m_Type = Instruction::InstructionType::ST_Comment;
        return m_Type;
    }

    // replacing ',' with ' '
    std::replace( a_line.begin(), a_line.end(), ',', ' ' );
     
    // check if a_line filled with spaces only
    if( std::all_of( a_line.begin(), a_line.end(), isspace ))
    {
        m_Type = Instruction::InstructionType::ST_Comment;
        return m_Type;
    }

    // extract each word from the instruction
    std::istringstream instruction( a_line );

    std::vector<std::string> indivisualInstruction;
    std::string tempString = "";

    while (instruction >> tempString)
    {
        indivisualInstruction.push_back( tempString );
    }

    // check if the instruction is machine level
    // check by seeing if the vector size is 2 and all are int
    // else the inst is assembly
    if( indivisualInstruction.size() == 2 )
    {
        if(IsNum( indivisualInstruction.at( 0 )) && IsNum( indivisualInstruction.at( 1 ) ) )
        {
            m_Type = Instruction::InstructionType::ST_MachineLanguage;
        
            // here a error ropering is necessary for if 

            ///@todo comlete for machine language instruction
            /*
            * handel machine language instruction
            m_Label = indivisualInstruction.at(0); // saving the location

            // validating second entry in the vector is also a number
        
            */
            return m_Type;
        }
    }
    
    // setting the instruction as ST_AssemblerInstr
    m_Type = Instruction::InstructionType::ST_AssemblerInstr;
    
    // setting the member variable
    if( !SetFundamentalMemVar( indivisualInstruction ) )
    {
        // error was flaged
        return m_Type;
    }

    // checking if instruction was END
    if( m_Type == Instruction::InstructionType::ST_End )
    {
        return m_Type;
    }

    // validating m_Label
    if( m_Label !=  "" && !ValidateSymSyntax( m_Label ) )
    {
        // error was flaged
        return m_Type;
    }
    
    // ================== setting derived member variable ===================
    // setting numeric equivalent of operation code
    SetNumOpCode();

    // chekcing if operand 1 is empty 
    if( m_Operand1 != "" )
    {
        // checking if the first operand is a number and seting it if true
        SetNumOperand1();
    }

    // chekcing if operand 2 is empty 
    if( m_Operand2 != "" )
    {
        // checking if the second operand is a number and seting it if true
        SetNumOperand2();
    }

    // validating if the syntax is correct for the given opCode
    ValidateOpCodeSyntax();

	return m_Type;
}

int Instruction::LocationNextInstruction( int a_loc )
{
    /// @todo error if m_IsNumericOperand1 is false
    // if opCode is ORG then next location is the Operand1Value
    if ( m_NumOpCode == 100 )
    {
        return m_Operand1Value;
    }
    // if opCode is DS
    else if ( m_NumOpCode == 300 )
    {
        /// @todo error if m_IsNumericOperand1 is false
        return a_loc + m_Operand1Value;
    }
    
    return ++a_loc;
}

void Instruction::ClearMemberVariables()
{
    // setting all the string member variables to ""
    m_Label = "";
    m_OpCode = ""; 
    m_Operand1 = "";  
    m_Operand2 = "";
    m_Instruction = "";
    m_ErrorMsgType = Errors::ErrorTypes::ERROR_DEFAULT;

    // setting all int variable to 0 
    m_NumOpCode = 0;
    m_Operand1Value = 0;  

    // setting all bool variable to false 
    m_IsNumericOperand1 = false;

    // setting all Instruction::InstructionType variable to ST_Comment 
    m_Type = Instruction::InstructionType::ST_Comment;
}

bool Instruction::SetFundamentalMemVar( std::vector<std::string>& a_indivisualInstruction )
{
    // temp array to smartly set the member variable despite the size of a_indivisualInstruction
    std::string* memberVarArr[4] = { &m_Label, &m_OpCode, &m_Operand1, &m_Operand2 };

    // checking the size,  valid size is < 4 i.e. label, opCode, operand1, operand2
    if( a_indivisualInstruction.size() > 4 )
    {
        // flaging error
        m_ErrorMsgType = Errors::ErrorTypes::ERROR_ExtraStatEle;
        m_Type = Instruction::InstructionType::ST_Error;

        return false;
    }
    // if == 1 then it should be halt or end
    // assign it to m_opCode, later validate the inputs
    else if ( a_indivisualInstruction.size() == 1 && IsOpCode( a_indivisualInstruction.at( 0 ) ) )
    {
        m_OpCode = a_indivisualInstruction.at( 0 );

        if( AllUpperCase( m_OpCode ) == "END" )
        {
            m_Type = Instruction::InstructionType::ST_End;
        }
        else if( AllUpperCase( m_OpCode ) != "HALT" )
        {
            m_ErrorMsgType = Errors::ErrorTypes::ERROR_MissingOperand;
            return false;
        }
    }
    // checking if the second entry in a_indivisualInstruction vector is Op code
    // and the size of vector is 2, or 3, or 4
    // if yes label should be present
    else if( IsOpCode( a_indivisualInstruction.at( 1 ) ) )
    {
        for( unsigned int i = 0; i < a_indivisualInstruction.size(); ++i )
        {
            *( memberVarArr[i] ) = a_indivisualInstruction.at( i );
        }
    }
    // checking if the first entry in a_indivisualInstruction vector is Op code
    //  and the size of vector is 2, or 3
    // if yes label should not be present
    else if( IsOpCode( a_indivisualInstruction.at( 0 ) ) )
    {
        // if the size is 4 then error, extra statement element
        if( a_indivisualInstruction.size() == 4 )
        {
            m_ErrorMsgType = Errors::ErrorTypes::ERROR_ExtraStatEle;
            return false;
        }

        for( unsigned int i = 0; i < a_indivisualInstruction.size(); ++i )
        {
            *( memberVarArr[i + 1] ) = a_indivisualInstruction.at( i );
        }
    }
    // else flag error
    else
    {
        if( IsOpCode( a_indivisualInstruction.at( 2 ) ) )
        {
            m_ErrorMsgType = Errors::ErrorTypes::ERROR_OpCodePos;
        }
        else
        {
            m_ErrorMsgType = Errors::ErrorTypes::ERROR_MissingOpCode;
        }

        m_Type = Instruction::InstructionType::ST_Error;

        return false;
    }

    return true;
}

void Instruction::SetNumOpCode()
{
    // getting the op code position in the table
    std::unordered_map<std::string, int>::const_iterator opCodePos = opCodeTable.find( AllUpperCase( m_OpCode ) );
    m_NumOpCode = opCodePos->second;
}

void Instruction::SetNumOperand1()
{
    // checking if first operand is a number or not
    m_IsNumericOperand1 = IsNum( m_Operand1 );

    // getting m_Operand1Value if operand1 is a number
    if( m_IsNumericOperand1 )
    {
        m_Operand1Value = std::stoi( m_Operand1 );
    }
}

void Instruction::SetNumOperand2()
{
    // checking if second operand is a number or not
    m_IsNumericOperand2 = IsNum( m_Operand2 );

    // getting m_Operand1Value if operand1 is a number
    if( m_IsNumericOperand2 )
    {
        m_Operand2Value = std::stoi( m_Operand2 );
    }
}

bool Instruction::ValidateSymSyntax( std::string a_Symbol )
{
    // checking for length -> which should be max 10
    if( a_Symbol.size() > 10 )
    {
        m_ErrorMsgType = Errors::ErrorTypes::ERROR_InvalidSymSyntaxLen;
        m_Type = Instruction::InstructionType::ST_Error;
        return false;
    }
    

    // checking if first character is a letter or not
    if( !std::isalpha( a_Symbol.at( 0 ) ) )
    {
        m_ErrorMsgType = Errors::ErrorTypes::ERROR_InvalidSymSyntaxfirstChar;
        m_Type = Instruction::InstructionType::ST_Error;
        return false;
    }

    // checking if the symbol is made out of only albphabet and number 
    if( !std::all_of( a_Symbol.begin(), a_Symbol.end(), std::isalnum ) )
    {
        m_ErrorMsgType = Errors::ErrorTypes::ERROR_InvalidSymSyntaxComposition;
        m_Type = Instruction::InstructionType::ST_Error;
        return false;
    }

    return true; // all requirement passed
}

bool Instruction::ValidateOpCodeSyntax()
{
    /*
    *  Opearation Code syntax requiremnet 
    * 
    * labels:
    *   opCodes which must have a label are DC and DS
    *   rest may have a label which we do not check for as it has already been validated
    * 
    * Operand1:
    *   opCodes which cannot have operand1 are END and Halt, other must have a operand1
    *   opCodes which can only have a numberic value for Operand1 are  ORG, DC, and DS
    *   rest must have a label as Operand1
    *  
    * Operand2:
    *   opCodes which must have a label are ADD, SUB, MULT, DIV, COPY, BM, BZ, and BP
    *   rest cannot have a Operand2
    *       
    */

    // ========================= checking for label ========================= 
    // for DC and DS
    if ( (m_NumOpCode == 200 || m_NumOpCode == 300) && m_Label == "" )
    {
        // label is absent
        // set error
        m_ErrorMsgType = Errors::ErrorTypes::ERROR_MissingLabel;
        m_Type = Instruction::InstructionType::ST_Error;
        return false;
    }
    
    // ========================= checking for operand1 ========================= 
    // for END and Halt
    if( m_NumOpCode == 400 || m_NumOpCode == 13 )
    {
        // operand 1 should be empty
        if( m_Operand1 != "" )
        {
            m_ErrorMsgType = Errors::ErrorTypes::ERROR_ExtraOperand1;
            m_Type = Instruction::InstructionType::ST_Error;
            return false;
        }

        // checking if opcode was end
        if( m_NumOpCode == 400 )
        {
            m_Type = Instruction::InstructionType::ST_End;
        }
    }
    //  for ORG, DC, and DS  
    else if( m_NumOpCode >= 100 && m_NumOpCode <= 300 )
    {
        // operand1 should have a numeric value
        // if it is not a numberic value
        if( !m_IsNumericOperand1 )
        {
            m_ErrorMsgType = Errors::ErrorTypes::ERROR_NotNumOperand1;
            m_Type = Instruction::InstructionType::ST_Error;
            return false;
        }
        // its length should not be > 5
        if( m_Operand1.size() > 5 )
        {
            m_ErrorMsgType = Errors::ErrorTypes::ERROR_InvalidOperand1Len;
            m_Type = Instruction::InstructionType::ST_Error;
            return false;
        }
    }
    // rest must have a label as Operand1
    else
    {
        // operand1 can only be a label,
        // checking if it has a numeric value, if == true then error
        if( m_IsNumericOperand1 )
        {
            m_ErrorMsgType = Errors::ErrorTypes::ERROR_NotLabelOperand1;
            m_Type = Instruction::InstructionType::ST_Error;
            return false;
        }
        // check for valid label syntax
        else if( !ValidateSymSyntax( m_Operand1 ) )
        {
            return false;
        }
    }

    // ========================= checking for operand2 =========================
    // for ADD, SUB, MULTI, DIV, COPY, BM, BZ, and BP
    if( (m_NumOpCode>=1 && m_NumOpCode <= 5) || (m_NumOpCode >= 10 && m_NumOpCode <= 12) )
    {
        // operand2 can only be a label
        if( m_Operand2 == "" )
        {
            m_ErrorMsgType = Errors::ErrorTypes::ERROR_MissingOperand;
            m_Type = Instruction::InstructionType::ST_Error;
            return false;
        }
        // checking if it has a numeric value, 
        // if == true error
        else if( m_IsNumericOperand2 )
        {
            m_ErrorMsgType = Errors::ErrorTypes::ERROR_NotLabelOperand2;
            m_Type = Instruction::InstructionType::ST_Error;
            return false;
        }
        // check for valid label syntax
        else if( !ValidateSymSyntax( m_Operand2 ) )
        {
            return false;
        }
    }
    // for everthing else, operand 2 should be empty
    else
    {
        if( m_Operand2 != "" )
        {
            m_ErrorMsgType = Errors::ErrorTypes::ERROR_ExtraOperand2;
            m_Type = Instruction::InstructionType::ST_Error;
            return false;
        }
    }

    return true; // no error found
}