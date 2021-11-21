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
    
    // setting the member variable
    SetFundamentalMemVar( indivisualInstruction );
    
    // checking if invalid syntax was detected
    if( m_Type == Instruction::InstructionType::ST_Error )
    {
        return m_Type;    
    }

    // checking if OpCode was END
    if( m_Type == Instruction::InstructionType::ST_End )
    {
        return m_Type;
    }
    
    // ================== setting derived member variable ===================
    // getting m_NumOpcode
    // checking if there is a valid OpCode
    if ( IsOpCode( m_OpCode ) )
    {
        // getting the op code position in the table
        std::unordered_map<std::string, int>::const_iterator opCodePos = opCodeTable.find( AllUpperCase( m_OpCode ) );
        m_NumOpCode = opCodePos->second;
    }
    else
    {
        // flag error
        m_ErrorMessage = "Illegal Operation Code";                                                
    }
    
    // getting m_IsNumericOperand1
    m_IsNumericOperand1 = IsNum( m_Operand1 );

    // getting m_Operand1Value if operand1 is a number
    if ( m_IsNumericOperand1 )
    {
        m_Operand1Value = std::stoi( m_Operand1 );
    }
    
    m_Type = Instruction::InstructionType::ST_AssemblerInstr;

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
    m_ErrorMessage = "";

    // setting all int variable to 0 
    m_NumOpCode = 0;
    m_Operand1Value = 0;  

    // setting all bool variable to false 
    m_IsNumericOperand1 = false;

    // setting all Instruction::InstructionType variable to ST_Comment 
    m_Type = Instruction::InstructionType::ST_Comment;
}

void Instruction::SetFundamentalMemVar( std::vector<std::string>& a_indivisualInstruction )
{
    // temp array to smartly set the member variable despite the size of a_indivisualInstruction
    std::string* memberVarArr[4] = { &m_Label, &m_OpCode, &m_Operand1, &m_Operand2 };

    // checking the size,  valid size is < 4 for label, opCode, operand1, operand2
    if( a_indivisualInstruction.size() > 4 )
    {
        m_ErrorMessage = "Extra statement elements";

        m_Type = Instruction::InstructionType::ST_Error;
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
            m_ErrorMessage = "Invalid Statement--Operation Code should be at the start of the statement or followed by a label";
        }
        else
        {
            m_ErrorMessage = "Missing Operation Code";
        }

        m_Type = Instruction::InstructionType::ST_Error;
    }

}
