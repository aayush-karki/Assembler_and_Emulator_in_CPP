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
    if ( IsNum( indivisualInstruction.at( 0 ) ) )
    {
        m_Type = Instruction::InstructionType::ST_MachineLanguage;
        
        // here a error ropering is necessary for if 

        ///@todo comlete for machine language instruction
        /*
        m_Label = indivisualInstruction.at(0); // saving the location

        // validating second entry in the vector is also a number
        
        */
        return m_Type;
    }
    
    // ============= setting the member variable===============
    
    // temp array to smartly set the member variable despite the size of indivisualInstruction
    std::string* memberVarArr[4] = { &m_Label, &m_OpCode, &m_Operand1, &m_Operand2 };
    
    // checking the size, is == 1 then it should be halt or end
    // assign it to m_opCode, later validate the inputs
    if (indivisualInstruction.size() == 1 && IsOpCode( indivisualInstruction.at( 0 ) ) )
    {
        m_OpCode = indivisualInstruction.at( 0 ); // we later validae
    }
    // checking if the second entry in indivisualInstruction vector is Op code
    // and the size of vector is 2, or 3, or 4
    // if yes label should be present
    else if( indivisualInstruction.size() > 1 && indivisualInstruction.size() <= 4 &&
            IsOpCode( indivisualInstruction.at( 1 ) ) )
    {
        for ( unsigned int i = 0; i < indivisualInstruction.size(); ++i)
        {
            *( memberVarArr[i] ) = indivisualInstruction.at( i );
        }
    }
    // checking if the first entry in indivisualInstruction vector is Op code
    //  and the size of vector is 2, or 3
    // if yes label should not be present
    else if ( indivisualInstruction.size() > 1 && indivisualInstruction.size() <= 3 &&
        IsOpCode( indivisualInstruction.at( 0 ) ) )
    {
        for ( unsigned int i = 0; i < indivisualInstruction.size(); ++i)
        {
            *( memberVarArr[i + 1] ) = indivisualInstruction.at( i );
        }
    }
    // else error
    else
    {
        // flaging error     
        m_ErrorFlag.first = true;
        m_ErrorFlag.second = "Invalid Syntax"; // further diagnoses is needed

        // skip all the processing for this line
        m_Type = Instruction::InstructionType::ST_Comment;
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
        m_ErrorFlag.first = true;
        m_ErrorFlag.second = "Invalid Operand Code"; // further diagnoses is needed
    }
    
    // getting m_IsNumericOperand1
    m_IsNumericOperand1 = IsNum( m_Operand1 );

    // getting m_Operand1Value if operand1 is a number
    if ( m_IsNumericOperand1 )
    {
        m_Operand1Value = std::stoi( m_Operand1 );
    }

    // checking for type of intrustion
    if ( AllUpperCase( m_OpCode ) == "END" )
    {
        m_Type = Instruction::InstructionType::ST_End;
    }
    else
    {
        m_Type = Instruction::InstructionType::ST_AssemblerInstr;
    }

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

    // setting errorFlag to default state
    m_ErrorFlag = std::make_pair( false, "" );


    // setting all int variable to 0 
    m_NumOpCode = 0;
    m_Operand1Value = 0;  

    // setting all bool variable to false 
    m_IsNumericOperand1 = false;

    // setting all Instruction::InstructionType variable to ST_Comment 
    m_Type = Instruction::InstructionType::ST_Comment;
}