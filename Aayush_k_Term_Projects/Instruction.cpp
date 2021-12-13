/*************************************************************************************/
/// 
/// @file Instruction.cpp 
/// 
/// @brief  This file is a source file for Instruction class.
/// 
/// It contains all of the defination  of the member funciton.
///
/// @author Aayush Karki
/// 
/// @date  November 12, 2021 
///

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

    // if vector size is 2 and all element are int, then it is machine level
    if( indivisualInstruction.size() == 2 && 
        IsNum( indivisualInstruction.at( 0 ) ) && 
        IsNum( indivisualInstruction.at( 1 ) ) )
    {
            m_Type = Instruction::InstructionType::ST_MachineLanguage;
            return m_Type;
    }

    // else the inst is assembly
    // setting the instruction as ST_AssemblerInstr
    m_Type = Instruction::InstructionType::ST_AssemblerInstr;
    
    // setting the member variable
    if( !SetFundamentalVar( indivisualInstruction ) )
    {
        m_ErrorFundVar = true;
        return m_Type;
    }
    

    // checking if instruction was END
    if( m_Type == Instruction::InstructionType::ST_End )
    {
        return m_Type;
    }

    // validating m_Label
    if( m_Label !=  "" )
    {
        ValidateSymSyntax( m_Label );
    }
    
    // ================== setting derived member variable ===================
    
    // setting numeric equivalent of operation code
    SetNumOpCode();

    // checking operand1 if it is not empty 
    if( m_Operand1 != ""  )
    {
        SetNumOperand1();
    }

    // checking operand2 if it is not empty 
    if( m_Operand2 != "")
    {
        SetNumOperand2();
    }

    // validating if the syntax is correct for the given opCode
    ValidateOpCodeSyntax();

	return m_Type;
}

int Instruction::LocationNextInstruction( int a_loc )
{
    // if opCode is ORG then next location is the Operand1Value
    if ( m_NumOpCode == 100 )
    {
		return (int)m_Operand1Value;
    }
    // if opCode is DS then calculating the next loc
    else if ( m_NumOpCode == 300 )
    {
        return a_loc + (int)m_Operand1Value;
    }
    // else increment by 1
    return ++a_loc;
}

void Instruction::ClearMemberVariables()
{
    // setting all the string member variables to ""ERROR_MissingOpCode
    m_Label = "";
    m_OpCode = ""; 
    m_Operand1 = "";  
    m_Operand2 = "";
    m_Instruction = "";

    // resizing the vector to 0
    m_ErrorMsgType.resize(0);

    // setting all int / long long variable to 0 
    m_NumOpCode = 0;
    m_Operand1Value = 0;  

    // setting all bool variable to false 
	m_Operand1Const = false;
	m_Operand2Const = false;
    m_IsNumericOperand1 = false; 
    m_ErrorOpCode = false;
    m_ErrorOperand1 = false;
    m_ErrorOperand2 = false;
    m_ErrorFundVar = false;

    // setting all Instruction::InstructionType variable to ST_Comment 
    m_Type = Instruction::InstructionType::ST_Comment;
}

bool Instruction::SetFundamentalVar( std::vector<std::string>& a_indivisualInstruction )
{
    // temp array to set the member variable despite the size of a_indivisualInstruction
    std::string* memberVarArr[4] = { &m_Label, &m_OpCode, &m_Operand1, &m_Operand2 };

    // checking the size,  valid size is < 4 i.e. label, opCode, operand1, operand2
    if( a_indivisualInstruction.size() > 4 )
    {
        // flaging error
        m_ErrorMsgType.push_back( Errors::ErrorTypes::ERROR_ExtraStatEle);
        m_Type = Instruction::InstructionType::ST_Error;
        
        // delete extra elements, shrinking the size to 4
        // and continue translation
        int instSize = a_indivisualInstruction.size();

        while( instSize != 4 )
        {
            a_indivisualInstruction.pop_back();
            --instSize;
        }
    }

    // if vector size == 1 then it should be halt or end
    if ( a_indivisualInstruction.size() == 1 )
    {
        // checking if opcode is present
        if( IsOpCode( a_indivisualInstruction.at( 0 ) ) )
        {
            m_OpCode = a_indivisualInstruction.at( 0 );

            // for end
            if( AllUpperCase( m_OpCode ) == "END" )
            {
                m_Type = Instruction::InstructionType::ST_End;
            }
            // for everything except halt, error case 2
            else if( AllUpperCase( m_OpCode ) != "HALT" )
            {
                // setting up its value
                SetNumOpCode();

                // reporting error
                m_ErrorOperand1 = true;
                m_ErrorOperand2 = true;
            
                m_ErrorMsgType.push_back( Errors::ErrorTypes::ERROR_MissingOperand );
                m_Type = Instruction::InstructionType::ST_Error;
                return false;
            }
        }
        else
        {
            // opcode is absent, reporting error
            m_ErrorMsgType.push_back( Errors::ErrorTypes::ERROR_MissingOpCode );
            m_Type = Instruction::InstructionType::ST_Error;
                        
            m_ErrorOpCode = true;
            m_ErrorOperand1 = true;
            m_ErrorOperand2 = true;

            m_Type = Instruction::InstructionType::ST_Error;

            return false;
        }
    }
    // checking if the second entry is Op code
    else if( IsOpCode( a_indivisualInstruction.at( 1 ) ) )
    {
        // label should be present
        
        // extractng the element into label, opCode, operand1, and operand2 in order 
        for( unsigned int i = 0; i < a_indivisualInstruction.size(); ++i )
        {
            *( memberVarArr[i] ) = a_indivisualInstruction.at( i );
        }
    }
    // checking if the first entry is Op code
    else if( IsOpCode( a_indivisualInstruction.at( 0 ) ) )
    {
        // label should not be present
        
        // if the size is 4 then error, extra statement element
        if( a_indivisualInstruction.size() == 4 )
        {
            m_ErrorMsgType.push_back( Errors::ErrorTypes::ERROR_ExtraStatEle );
            m_Type = Instruction::InstructionType::ST_Error;

            // removing the last element so that we can traslate as much as possible
            a_indivisualInstruction.pop_back(); 
        }

        // extractng the element into opCode, operand1, and operand2 in order 
        for( unsigned int i = 0; i < a_indivisualInstruction.size(); ++i )
        {
            *( memberVarArr[i + 1] ) = a_indivisualInstruction.at( i );
        }
    }
    // else flag error
    else
    {
        if( a_indivisualInstruction.size() > 2 && IsOpCode( a_indivisualInstruction.at( 2 ) ) )
        {
            m_ErrorMsgType.push_back( Errors::ErrorTypes::ERROR_OpCodePos );
        }
        else
        {
            m_ErrorMsgType.push_back( Errors::ErrorTypes::ERROR_MissingOpCode );
        }

        // setting error flags
        m_ErrorOpCode = true;
        m_ErrorOperand1 = true;
        m_ErrorOperand2 = true;

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
	// makign a copy of operand 2 so that we do not mess up the orignal content of operand2
	std::string orgiOperand1Cpy = m_Operand1;

	// checking for negative input 
	char sign = '0';

	// checking if the first char is - or +
	if( orgiOperand1Cpy.front() == '-' || orgiOperand1Cpy.front() == '+' )
	{
		sign = orgiOperand1Cpy.front();
		orgiOperand1Cpy.erase( 0, 1 ); // removing the first char

		// updating first char to 1 if it is '-' else to 0 if it is '+'
		if( sign == '-' )
		{
			sign = 1;
		}
		else
		{
			sign = 0;
		}
	}
	
    // checking if first operand is a number or not
    m_IsNumericOperand1 = IsNum( orgiOperand1Cpy );

    // getting m_Operand1Value if operand1 is a number
    if( m_IsNumericOperand1 )
    {
		// checking for length of the string, max is 12 cause word of VC1600 
		// memory can only handel 12 digits;
		// and first digit is reserved to indicate sign 
		// so it sould only have 11 digits which is be 99,999,999,999
		if( orgiOperand1Cpy.size() > 11 )
		{
			m_ErrorMsgType.push_back( Errors::ErrorTypes::ERROR_InvalidNumOperandLen );
			m_ErrorOperand1 = true;
			return;
		}

		// every thing is digit, so, adding the first char back
		orgiOperand1Cpy = sign + orgiOperand1Cpy;

        m_Operand1Value = std::stoll( orgiOperand1Cpy );
		m_Operand1Const = true;
    }
}

void Instruction::SetNumOperand2()
{
	// makign a copy of operand 2 so that we do not mess up the orignal content of operand2
	std::string orgiOperand2Cpy = m_Operand2;

	// checking for negative input 
	char sign = '0';

	// checking if the first char is - or +
	if( orgiOperand2Cpy.front() == '-' || orgiOperand2Cpy.front() == '+' )
	{
		sign = orgiOperand2Cpy.front();
		orgiOperand2Cpy.erase( 0, 1 ); // removing the first char

		// updating first char to 1 if it is '-' else to 0 if it is '+'
		if( sign == '-' )
		{
			sign = 1;
		}
		else
		{
			sign = 0;
		}
	}

	// checking if first operand is a number or not
	m_IsNumericOperand2 = IsNum( orgiOperand2Cpy );

	// getting m_Operand1Value if operand1 is a number
	if( m_IsNumericOperand2 )
	{
		// checking for length of the string, max is 12 cause word of VC1600 
		// memory can only handel 12 digits;
		// and first digit is reserved to indicate sign 
		// so it sould only have 11 digits which is be 99,999,999,999
		if( orgiOperand2Cpy.size() > 11 )
		{
			m_ErrorMsgType.push_back( Errors::ErrorTypes::ERROR_InvalidNumOperandLen );
			m_ErrorOperand2 = true;
			return;
		}
		// every thing is digit, so, adding the first char back
		orgiOperand2Cpy = sign + orgiOperand2Cpy;

		m_Operand2Value = std::stoll( orgiOperand2Cpy );
		m_Operand1Const = true;
	}
}

bool Instruction::ValidateSymSyntax( std::string a_Symbol )
{
    // checking for length -> which should be max 10
    if( a_Symbol.size() > 10 )
    {
        m_ErrorMsgType.push_back( Errors::ErrorTypes::ERROR_InvalidSymSyntaxLen );
        m_Type = Instruction::InstructionType::ST_Error;
        return false;
    }

    // checking if first character is a letter or not
    if( !std::isalpha( a_Symbol.at( 0 ) ) )
    {
        m_ErrorMsgType.push_back( Errors::ErrorTypes::ERROR_InvalidSymSyntaxfirstChar );
        m_Type = Instruction::InstructionType::ST_Error;
        return false;
    }

    // checking if the symbol is made out of only albphabet and number 
    if( !std::all_of( a_Symbol.begin(), a_Symbol.end(), std::isalnum ) )
    {
        m_ErrorMsgType.push_back( Errors::ErrorTypes::ERROR_InvalidSymSyntaxComposition );
        m_Type = Instruction::InstructionType::ST_Error;
        return false;
    }

    return true; // all requirement passed
}

void Instruction::ValidateOpCodeSyntax()
{
    //  Opearation Code syntax requiremnet 
    /*
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
    // for DC and DS should always have a label
    if ( (m_NumOpCode == 200 || m_NumOpCode == 300) && m_Label == "" )
    {
        // label is missing
        m_ErrorMsgType.push_back( Errors::ErrorTypes::ERROR_MissingLabel );
        m_Type = Instruction::InstructionType::ST_Error;
    }
    
    // ========================= checking for operand1 ========================= 
    
    // for END and Halt, operand 1 should be empty
    if( m_NumOpCode == 400 || m_NumOpCode == 13 )
    {
        if( m_Operand1 != "" )
        {
            m_ErrorMsgType.push_back( Errors::ErrorTypes::ERROR_ExtraOperand1 );
            m_ErrorOperand1 = true;
        }
        
        // checking if opcode was end
        // this needs to be checked again because incase label was suppled with END op code
        if( m_NumOpCode == 400 )
        {
            m_Type = Instruction::InstructionType::ST_End;
        }
    }
    //  for ORG, DC, and DS, operand1 should have a numeric value
    else if( m_NumOpCode >= 100 && m_NumOpCode <= 300 )
    {
        // if it is not a numberic value
        if( !m_IsNumericOperand1 )
        {
            m_ErrorMsgType.push_back( Errors::ErrorTypes::ERROR_NotNumOperand1 );
            m_ErrorOperand1 = true;
        }
    }
    // rest must have a label as Operand1
    else
    {
        // checking if it has a numeric value, then error
        if( m_IsNumericOperand1 )
        {
            m_ErrorMsgType.push_back( Errors::ErrorTypes::ERROR_NotLabelOperand1 );
            m_ErrorOperand1 = true;
        }
        // check for valid label syntax
        else if( !ValidateSymSyntax( m_Operand1 ) )
        {
            m_ErrorOperand1 = true;
        }
    }

    // ========================= checking for operand2 =========================
    
    // for ADD, SUB, MULTI, DIV, COPY, BM, BZ, and BP
    if( (m_NumOpCode>=1 && m_NumOpCode <= 5) || (m_NumOpCode >= 10 && m_NumOpCode <= 12) )
    {
        // operand2 can only be a label
        if( m_Operand2 == "" )
        {
            m_ErrorMsgType.push_back( Errors::ErrorTypes::ERROR_MissingOperand );
            m_ErrorOperand2 = true;
        }
        // checking if it has a numeric value
        else if( m_IsNumericOperand2 )
        {
            m_ErrorMsgType.push_back( Errors::ErrorTypes::ERROR_NotLabelOperand2 );
            m_ErrorOperand2 = true;
        }
        // check for valid label syntax
        else if( !ValidateSymSyntax( m_Operand2 ) )
        {
            m_ErrorOperand2 = true;
        }
    }
    // for everthing else, operand 2 should be empty
    else
    {
        if( m_Operand2 != "" )
        {
            m_ErrorMsgType.push_back( Errors::ErrorTypes::ERROR_ExtraOperand2 );
            m_ErrorOperand2 = true;
        }
    }

    // checking if error was found
    if( m_ErrorOperand1 || m_ErrorOperand2 )
    {
        m_Type = Instruction::InstructionType::ST_Error;
    }

    return;
}