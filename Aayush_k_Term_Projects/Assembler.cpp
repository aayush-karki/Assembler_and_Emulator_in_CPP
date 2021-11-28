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

    bool returnFound = false;

    // Successively process each line of source code.
    for( ; ; ) 
    {
        // updating the line
        ++lineCounter;
        
        // Read the next line from the source file.
        std::string line; 
        if( ! m_facc.GetNextLine( line ) ) 
        {
            // If We could have a end statement in between the file.
            // or If we are missing an end statement.
            // We will let this error be reported by Pass II.
            return;
        }

        // Parse the line and get the instruction type.
        Instruction::InstructionType st =  m_inst.ParseInstruction( line );

        // If this is an end statement, there is nothing left to do in pass I.
        // Pass II will determine if the end is the last statement and report an error if it isn't.
        if (st == Instruction::InstructionType::ST_End) 
        {
            // we will continue just in case end statement is not 
            // the last instruction in the file
            // we will let this error be reported by Pass II
            // we get out of the function when we reach the EOF
            continue;
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
            m_symtab.AddSymbol( m_inst.GetLabel(), loc, lineCounter, m_inst.GetOrgiInst() );

            // storing the number declared by DC instruction
            if( m_inst.GetOpCodeNum() == 200 )
            {
                // adding the label to declared constant table
                m_declaredConstTab.insert( { m_inst.GetLabel(),m_inst.GetNumOperand1() } );
            }
            // storing the size declared by DS instruction
            if( m_inst.GetOpCodeNum() == 300 )
            {
                // adding the label to declared constant table
                m_declaredMemVarTab.insert( { m_inst.GetLabel(),m_inst.GetNumOperand1() } );
            }
        }

        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction( loc );
    }
}

void Assembler::PassII()
{
    int loc = 0;        // Tracks the location of the instructions to be generated.
    bool insufficentMemory = false; // track memory siuation

    // rewinding back to the start of the file
    m_facc.Rewind();

    int lineCounter = 0; //  track the current lines in the file


    // Successively process each line of source code.
    for( ; ; ) {

        // updating the line
        ++lineCounter; 

        // Read the next line from the source file.
        std::string line;

        //Errors::ErrorTypes a = Errors::ErrorTypes::ERROR_MissingEnd;

        if( !m_facc.GetNextLine( line ) ) 
        {
            Errors::RecordError( Errors::ErrorTypes::ERROR_MissingEnd, lineCounter, "" );
            return;
        }

        // Parse the line and get the instruction type.
        Instruction::InstructionType st = m_inst.ParseInstruction( line );

        // record if the line is a comment and then continue to next line.
        if( st == Instruction::InstructionType::ST_Comment )
        {
            // adding the comment to the machineInstruction Table
            m_machInstTab.AddMachineIntr( m_inst.GetOrgiInst() );
            continue;
        }

        // check if there were any error while parseing the instruction
        else if( st == Instruction::InstructionType::ST_Error )
        {
            Errors::RecordError( m_inst.GetErrorMsgType(), lineCounter, m_inst.GetOrgiInst() );
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

            // END statement is not the last statement.
            Errors::RecordError( Errors::ErrorTypes::ERROR_EndNotLast, lineCounter, m_inst.GetOrgiInst() );
        }

        // record error if the line is a machine instruction
        else if( st == Instruction::InstructionType::ST_MachineLanguage )
        {
            Errors::RecordError( Errors::ErrorTypes::ERROR_MachineLangInAssemLang, lineCounter, m_inst.GetOrgiInst() );
        }

        // it is assembly code
        else
        {           
            std::string translatedContent; // translating the intruction into machine readable form
            
            int opCodeNum = m_inst.GetOpCodeNum(); // getting the opCodeNum of current instruction

            // preparing to add to the machine instruction table
             
            // for ORG, DS does not have content 
            if ( opCodeNum == 100 || opCodeNum  == 300 )
            {
                // adding it to the  machine inst table
                m_machInstTab.AddMachineIntr( m_inst.GetOrgiInst(), loc );
            }
            // for operation code DC, content is 00|00000|operand1Val
            else if( opCodeNum == 200 )
            {
                // so first 2 digit of content should be 00 
                SmartFillContent( translatedContent, 0, 2 );

                // for DC the value of operand1 is stored in operand 2
                SmartFillContent( translatedContent, m_inst.GetNumOperand1(), 10);

                // adding it to the  machine inst table
                m_machInstTab.AddMachineIntr( m_inst.GetOrgiInst(), loc,  translatedContent  );
            }
            // for halt content is 13|00000|00000
            else if( opCodeNum == 13 )
            {
                // it is halt and its content only has starting 13
                // adding it to the  machine inst table
                m_machInstTab.AddMachineIntr( m_inst.GetOrgiInst(), loc, "130000000000" );
            }
            else
            {
                // everything else has content 
                // where operand1 is label
                //       operand2 is label or empty
                // content has opCode|operand1|operand2
                
                // ============= apending the opcode to the content ============= 
                SmartFillContent( translatedContent, opCodeNum, 2 );
                
                // ============= appending operand 1 to the content ============= 
                // operand 1 can only be label
                // but operand 1 of ADD, SUB, MULT, DIV, COPY, READ, and WRITE should point to memory variable

                // getting the label stored in operand 1
                std::string label = m_inst.GetOperand1();
                
                // getting the location that label points to
                int labelLoc;

                if( !m_symtab.LookupSymbol( label, labelLoc ) )
                {
                    // label not found
                    Errors::RecordError( Errors::ErrorTypes::ERROR_UndefinedLabel, lineCounter, m_inst.GetOrgiInst() );

                    // @todo retun statement here 
                }
                else
                {
                    // for B, BM, BZ, and BP operand 1 does not have to be memory variable
                    if( opCodeNum >= 9 && opCodeNum <= 12 )
                    {
                        SmartFillContent( translatedContent, labelLoc );
                    }
                    // for ADD, SUB, MULT, DIV, COPY, READ, and WRITE
                    else
                    {
                        // checking if label points to memory variable
                        if( LookupDeclaredConst( label ) || LookupDeclaredVarMem( label ) )
                        {
                            SmartFillContent( translatedContent, labelLoc );
                        }
                        else
                        {
                            Errors::RecordError( Errors::ErrorTypes::ERROR_NotLabelConst, lineCounter, m_inst.GetOrgiInst() );
                        }
                    }

                }

                // =============  appending operand 2 to the content ============= 
                // operand 2 can be label which points to a memory variable or empty
                if( m_inst.GetOperand2() == "" )
                {
                    // empty, append 00000
                    SmartFillContent( translatedContent, 0 );
                }
                else
                {
                    // it is a label and it should point to memory variable
                     
                    // getting the label stored in operand 2
                    std::string label2 = m_inst.GetOperand2();

                    // getting the location that label points to
                    int label2Loc;

                    if( !m_symtab.LookupSymbol( label2, label2Loc ) )
                    {
                        // label not found
                        Errors::RecordError( Errors::ErrorTypes::ERROR_UndefinedLabel, lineCounter, m_inst.GetOrgiInst() );

                        // @todo retun statement here 
                    }
                    else
                    {
                        if( LookupDeclaredConst( label2 ) || LookupDeclaredVarMem( label2 ) )
                        {
                            SmartFillContent( translatedContent, label2Loc );
                        }
                        else
                        {
                            Errors::RecordError( Errors::ErrorTypes::ERROR_NotLabelConst, lineCounter, m_inst.GetOrgiInst() );
                        }
                    }

                }
                // adding it to the  machine inst table
                m_machInstTab.AddMachineIntr( m_inst.GetOrgiInst(), loc, translatedContent );
            }

        }

        // @todo: delete me
        std::cout << m_inst.GetOrgiInst() << std::endl;

        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction( loc );

        // checking for storage
        if( loc > Emulator::MEMSZ && !insufficentMemory )
        {
            // insufficent memory
            Errors::RecordError( Errors::ErrorTypes::ERROR_InsufficentMemory, lineCounter, m_inst.GetOrgiInst() );
            insufficentMemory = true;
        }
        
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

void Assembler::DisplayDeclaredMemVarTab()
{
    std::unordered_map<std::string, int>::iterator currLabIte = m_declaredMemVarTab.begin();
    int symIndex = 0;

    std::cout << "Declared Memory Variable Table:" << std::endl << std::endl;

    // formating the table and printing the table headign
    std::cout << std::left;
    std::cout << std::setw( 10 + 1 ) << "Symbol# "
        << std::setw( 10 + 1 ) << "Label"
        << " Memory size" << std::endl;

    // printing the values
    while( currLabIte != m_declaredMemVarTab.end() )
    {
        std::cout << " " << std::setw( 6 ) << symIndex << "\t    "
            << std::setw( 10 + 1 ) << currLabIte->first
            << " " << currLabIte->second << std::endl;
        ++symIndex;
        currLabIte++;
    }

    std::cout << std::setfill( '_' ) << std::setw( 50 ) << " " << std::endl << std::endl;
    std::cout << std::setfill( ' ' );

    std::cout << "Press Enter to Continue" << std::endl;

    std::string enter;
    std::getline( std::cin, enter );
}



void Assembler::SmartFillContent( std::string &a_TranslatedContent,int a_ToAppendNum, int a_LengthToFill)
{
    std::string toAppend = std::to_string( a_ToAppendNum );
    
    // checking if location is in bound
    if( a_LengthToFill < toAppend.size() )
    {
        return;
    }
    else
    {

        // apending filler '0' to the translatedContent according to length of toAppend 
        // before appending toAppend to the translatedContent
        for( int i = 0; i < ( a_LengthToFill - toAppend.size() ); ++i )
        {
            a_TranslatedContent.append( std::to_string( 0 ) );
        }
    }

    a_TranslatedContent.append( toAppend );
}

bool Assembler::LookupDeclaredConst( const std::string& a_symbol )
{
    // finding the label
    std::unordered_map<std::string, int>::iterator currSymbolIte = m_declaredConstTab.find( a_symbol );

    if( currSymbolIte == m_declaredConstTab.end() )
    {
        return false;
    }
    return true;
}

bool Assembler::LookupDeclaredVarMem( const std::string& a_symbol )
{
    // finding the label
    std::unordered_map<std::string, int>::iterator currSymbolIte = m_declaredMemVarTab.find( a_symbol );

    if( currSymbolIte == m_declaredMemVarTab.end() )
    {
        return false;
    }
    return true;
}
