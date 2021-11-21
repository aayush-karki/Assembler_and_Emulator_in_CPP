#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"

Assembler::Assembler( int argc, char *argv[] )
: m_facc( argc, argv )
{
    // Nothing else to do here at this point.
}

Assembler::~Assembler( )
{
}

void Assembler::PassI( ) 
{
    int loc = 0;        // Tracks the location of the instructions to be generated.

    // Successively process each line of source code.
    for( ; ; ) {

        // Read the next line from the source file.
        std::string line; 
        if( ! m_facc.GetNextLine( line ) ) 
        {
            // If there are no more lines, we are missing an end statement.
            // We will let this error be reported by Pass II.
            return;
        }

        // Parse the line and get the instruction type.
        Instruction::InstructionType st =  m_inst.ParseInstruction( line );

        // If this is an end statement, there is nothing left to do in pass I.
        // Pass II will determine if the end is the last statement and report an error if it isn't.
        if (st == Instruction::InstructionType::ST_End) 
        {
            return;
        }

        // Labels can only be on machine language and assembler language
        // instructions.  So, skip comments.
        if( st == Instruction::InstructionType::ST_Comment )
        {
        	continue;
	    }
        // If the instruction has a label, record it and its location in the
        // symbol table.
        if( m_inst.IsLabel( ) ) 
        {
            m_symtab.AddSymbol( m_inst.GetLabel( ), loc );
        }
        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction( loc );
    }
}

void Assembler::PassII()
{
    // rewinding back to the start of the file
    m_facc.Rewind();

    int lineCounter = 0; //  track the current lines in the file

    // Successively process each line of source code.
    for( ; ; ) {

        // updating the line
        ++lineCounter;

        // Read the next line from the source file.
        std::string line;
        
        if( !m_facc.GetNextLine( line ) ) 
        {
            // If there are no more lines, we are missing an end statement.
            // report the error
            std::string errorMessage = "Line--" + lineCounter;
            errorMessage.append("--ERROR--End of File reached. Missing END statement");
            Errors::RecordError( errorMessage );
            return;
        }

        // Parse the line and get the instruction type.
        Instruction::InstructionType st = m_inst.ParseInstruction( line );

        // checking if the end is the last statement and report an error if it isn't.
        if( st == Instruction::InstructionType::ST_End )
        {
            // checking if current statement is the last statement
            if( m_facc.CheckEOF() )
            {
                return;
            }

            // If there are more lines left 
            // End statement is not the last statement.
            // report the error
            std::string errorMessage = "Line--" + lineCounter;
            errorMessage.append( "--ERROR--END statement is not the last statement in the program" );
            Errors::RecordError( errorMessage );

            // printing the error
            std::cout << m_inst.GetOrgiInst() << std::endl;
            std::cout << errorMessage.back() << std::endl;
        }

        // record if the line is a comment and then continue to next line.
        if( st == Instruction::InstructionType::ST_Comment )
        {
            // adding to the machineInstruction Table
            m_machInstTab.AddMachineIntr( m_inst.GetOrgiInst() );
            continue;
        }

        // chekcing for if there is any flagged error when extracting input
        // if(st == Instruction::InstructionType::ST_Error)


        /*
        // If the instruction has a label, record it and its location in the
        // symbol table.
        if( m_inst.IsLabel() ) {

            m_symtab.AddSymbol( m_inst.GetLabel(), loc );
        }
        */
    }
}
