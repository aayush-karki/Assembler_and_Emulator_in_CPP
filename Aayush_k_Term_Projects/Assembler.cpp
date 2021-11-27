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

    int lineCounter = 0; //  track the current lines in the file

    // Successively process each line of source code.
    for( ; ; ) 
    {
        // updating the line
        ++lineCounter;
        
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
            m_symtab.AddSymbol( m_inst.GetLabel(), loc, lineCounter );

            // checking if label for opCode DC point to a number
            // only DC can declare a number 
            // if error detected, reported in PassII
            if( m_inst.GetOpCodeNum() == 200 )
            {
                // adding the labe to declared constant table
                m_declaredConstTab.insert( { m_inst.GetLabel(),m_inst.GetNumOperand1() } );
            }
        }

        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction( loc );
    }
}

void Assembler::PassII()
{
    int loc = 0;        // Tracks the location of the instructions to be generated.

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
            std::string errorMessage = "Line--" + std::to_string( lineCounter );
            errorMessage.append("--ERROR--End of File reached. Missing END statement");
            Errors::RecordError( errorMessage );
            return;
        }

        // Parse the line and get the instruction type.
        Instruction::InstructionType st = m_inst.ParseInstruction( line );

        // check if there were any error while parseing the instruction
        if( st == Instruction::InstructionType::ST_Error )
        {
            std::string errorMessage = "Line--" + std::to_string( lineCounter ) + "--";
            errorMessage.append( m_inst.GetErrorMessage() );
            Errors::RecordError( errorMessage );
        }

        // checking if the end is the last statement and report an error if it isn't.
        else if( st == Instruction::InstructionType::ST_End )
        {
            // checking if current statement is the last statement
            if( m_facc.CheckEOF() )
            {
                // adding END inst to the machineInstruction Table
                m_machInstTab.AddMachineIntr( m_inst.GetOrgiInst() );
                return;
            }

            // If END statement is not the last statement.
            // report the error
            std::string errorMessage = "Line--" + std::to_string( lineCounter );
            errorMessage.append( "--ERROR--END statement is not the last statement in the program" );
            Errors::RecordError( errorMessage );
        }

        // record if the line is a comment and then continue to next line.
        else if( st == Instruction::InstructionType::ST_Comment )
        {
            // adding the comment to the machineInstruction Table
            m_machInstTab.AddMachineIntr( m_inst.GetOrgiInst() );
            continue;
        }
        // record error if the line is a mahcine instruction
        else if( st == Instruction::InstructionType::ST_MachineLanguage )
        {
            std::string errorMessage = "Line--" + std::to_string( lineCounter );
            errorMessage.append( "--ERROR--Machine Level code detected" );
            Errors::RecordError( errorMessage );
        }
        // it is assembly code
        else
        {
            // validating if Opcode is ORG, DC or DS 
            // and if their Operand 1 is a label it points to a constant number
            ValidateOperand1Lab( lineCounter );

            // if Opcode is ADD, SUB, MULT, or DIV then if their Operand 2 is a label
            // check if that label points to a constant
            ValidateOperand2Lab( lineCounter );
            
            std::string translatedContent; // translating the intruction into machine readable form
            
            int opCodeNum = m_inst.GetOpCodeNum(); // getting the opCodeNum of current instruction

            // preparing to add to the machine instruction table
            // for ORG, DS do nothing to translatedContent
            if ( opCodeNum == 100 || opCodeNum  == 300 )
            {
                // adding it to the  machine inst table
                m_machInstTab.AddMachineIntr( m_inst.GetOrgiInst(), loc );
            }
            // for operation code DC
            else if( opCodeNum == 200 )
            {
                // so first 2 digit of content should be 00 
                SmartFillContent( translatedContent, 0, 2 );

                // for DC the value of operand1 is stored in operand 2
                SmartFillContent( translatedContent, m_inst.GetNumOperand1(), 10);

                // adding it to the  machine inst table
                m_machInstTab.AddMachineIntr( m_inst.GetOrgiInst(), loc,  translatedContent  );
            }
            else if( opCodeNum == 13 )
            {
                // it is halt and its content only has starting 13
                // adding it to the  machine inst table
                m_machInstTab.AddMachineIntr( m_inst.GetOrgiInst(), loc, "130000000000" );
            }
            else
            {
                // everything else 
                // content has opCode|operand1|operand2
                
                // apending the opcode to the content
                SmartFillContent( translatedContent, opCodeNum, 2 );
                
                // appending operand 1 to the content
                // operand 1 can only be label

                // getting the label
                std::string label = m_inst.GetOperand1();
                
                // getting the location that label points to
                int labelLoc;

                if( !m_symtab.LookupSymbol( label, labelLoc ) )
                {
                    // error
                    std::string errorMessage = "Line--" + std::to_string( lineCounter );
                    errorMessage.append( "--ERROR--Undefined Label" );
                    Errors::RecordError( errorMessage );
                    // @todo retun statement here 
                }
                else
                {
                    SmartFillContent( translatedContent, labelLoc );                        
                }

                // appending operand 2 to the content
                // operand 2 can be both label or numeric or empty
                if( m_inst.GetOperand2() == "" )
                {
                    // empty, append 00000
                    SmartFillContent( translatedContent, 0 );
                }
                else if( m_inst.IsNumOperand2() )
                {
                    // append the content of Operand2
                    SmartFillContent( translatedContent, m_inst.GetNumOperand2() );
                }
                else
                {
                    // it is a label
                    // getting the label
                    std::string label2 = m_inst.GetOperand2();

                    // getting the location that label points to
                    int label2Loc;

                    if( !m_symtab.LookupSymbol( label2, label2Loc ) )
                    {
                        // error
                        std::string errorMessage = "Line--" + std::to_string( lineCounter );
                        errorMessage.append( "--ERROR--Undefined Label" );
                        Errors::RecordError( errorMessage );
                        // @todo retun statement here 
                    }
                    else
                    {
                        SmartFillContent( translatedContent, label2Loc );
                    }
                }
                // adding it to the  machine inst table
                m_machInstTab.AddMachineIntr( m_inst.GetOrgiInst(), loc, translatedContent );
            }

        }

        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction( loc );
    }
}

void Assembler::DisplayDeclaredConstTab()
{
    
    std::unordered_map<std::string, int>::iterator currLabIte = m_declaredConstTab.begin();
    int symIndex = 0;

    std::cout << "Declared Constant Table:" << std::endl << std::endl;

    // formating the table and printing the table headign
    std::cout << std::left;
    std::cout << std::setw( 10 + 1 ) << "Symbol# "
        << std::setw( 10 + 1 ) << "Label"
        << " Constant Stored" << std::endl;

    // printing the values
    while ( currLabIte != m_declaredConstTab.end())
    {
        std::cout << " " << std::setw( 6 ) << symIndex << "\t    "
            << std::setw( 10 + 1 ) << currLabIte->first
            << " " << currLabIte->second << std::endl;
        ++symIndex; 
        currLabIte++;
    }

    std::cout << std::setfill('_') << std::setw(50) << " " << std::endl << std::endl;
    std::cout << std::setfill( ' ' );

    std::cout << "Press Enter to Continue" << std::endl;

    std::string enter;
    std::getline( std::cin, enter );
}

bool Assembler::ValidateOperand1Lab(int a_LineCounter )
{
    // if Opcode is ORG, DC or DS and if their Operand 1
    // is a label it points to a constant number
    if( ( m_inst.GetOpCodeNum() >= 100 && m_inst.GetOpCodeNum() <= 300 ) &&
        !m_inst.IsNumOperand1() )
    {
        // checking if the label present in the declaredConstantTable
        if( m_declaredConstTab.find( m_inst.GetOperand1() ) == m_declaredConstTab.end() )
        {
            // label does not exist in declared Constant table
            // report the error
            std::string errorMessage = "Line--" + a_LineCounter;
            errorMessage.append( "--ERROR--Operand 1 does not point to a label which stores constant number" );
            Errors::RecordError( errorMessage );
            return false;
        }
    }
    return true; // no error
}

bool Assembler::ValidateOperand2Lab( int a_LineCounter )
{
    // if Opcode is ADD, SUB, MULT, or DIV then if their Operand 2 is a label
    // check if that label points to a constant
    if( ( m_inst.GetOpCodeNum() >= 1 && m_inst.GetOpCodeNum() <= 4 ) &&
        !m_inst.IsNumOperand2() )
    {
        // checking if the label present in the declaredConstantTable
        if( m_declaredConstTab.find( m_inst.GetOperand2() ) == m_declaredConstTab.end() )
        {
            // label does not exist in declared Constant table
            // report the error
            std::string errorMessage = "Line--" + a_LineCounter;
            errorMessage.append( "--ERROR--Operand 2 does not point to a label which stores constant number" );
            Errors::RecordError( errorMessage );
            return false;
        }
    }
    return true; // no error
}

void Assembler::SmartFillContent( std::string &a_TranslatedContent,int a_ToAppendNum, int a_LengthToFill)
{
    std::string toAppend = std::to_string( a_ToAppendNum );

    // apending filler '0' to the translatedContent according to length of toAppend 
    // before appending toAppend to the translatedContent
    for( int i = 0; i < ( a_LengthToFill - toAppend.size() ); ++i )
    {
        a_TranslatedContent.append( std::to_string( 0 ) );
    }

    a_TranslatedContent.append( toAppend );
}
