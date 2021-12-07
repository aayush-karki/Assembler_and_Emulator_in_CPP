#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"

Assembler::Assembler( int argc, char *argv[] ):
	m_facc( argc, argv )
{
	m_noError = true;
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

        // If the instruction has a label, record it and its location in the symbol tabel
        // also add all the constant declared by dc opCode to the declared constant table 
        // also add all the memory declared by ds opCode to the declared memory variable table 
        if( m_inst.IsLabel( ) ) 
        {
            if (!m_symtab.AddSymbol(m_inst.GetLabel(), loc, lineCounter, m_inst.GetOrgiInst()))
            {
                m_noError = false;
            }

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
            Errors::RecordError( Errors::ErrorTypes::ERROR_MissingEnd, "Line", lineCounter, "");
            m_noError = false;

            return;
        }

        // Parse the line and get the instruction type.
        Instruction::InstructionType st = m_inst.ParseInstruction( line );

        // check if there were any error while parseing the instruction
        if( st == Instruction::InstructionType::ST_Error )
        {
			m_noError = false;
            std::vector<Errors::ErrorTypes>::iterator currErrorPtr = m_inst.GetErrorMsgTypeBegin();

            while(currErrorPtr != m_inst.GetErrorMsgTypeEnd() )
            {
                Errors::RecordError(*currErrorPtr, "Line", lineCounter, m_inst.GetOrgiInst() );
                ++currErrorPtr;
            }

            // checking if error was found in fundamental variable to append it ot the instruction table
            if( m_inst.IsErrorFundVar() )
            {
                if( m_inst.IsErrorOpCode() )
                {
                    m_machInstTab.AddMachineIntr( m_inst.GetOrgiInst(), loc, "????????????" );
                }
                else
                {
                    std::string content;

                    // insering op code into the content
                    SmartFillContent( content, m_inst.GetOpCodeNum() % 100, 2 );

                    // inserting error value of operand1 and operand2
                    content.append( "??????????" );

                    m_machInstTab.AddMachineIntr( m_inst.GetOrgiInst(), loc, content );
                }

                // computing next loc
                ComputeNextLoc( loc, lineCounter, insufficentMemory );
                continue;
            }
        }

        // record if the line is a comment and then continue to next line.
        if( st == Instruction::InstructionType::ST_Comment )
        {
            // adding the comment to the machineInstruction Table
            m_machInstTab.AddMachineIntr( m_inst.GetOrgiInst() );
            continue;
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
            Errors::RecordError( Errors::ErrorTypes::ERROR_EndNotLast, "Line", lineCounter, m_inst.GetOrgiInst() );
			m_noError = false;
        }
        // record error if the line is a machine instruction
        else if( st == Instruction::InstructionType::ST_MachineLanguage )
        {
			m_noError = false;
            Errors::RecordError( Errors::ErrorTypes::ERROR_MachineLangInAssemLang, "Line", lineCounter, m_inst.GetOrgiInst() );
        }
        // it is assembly code
        else
        {
			TranslateInstruction(loc, lineCounter);
        }
        
        ComputeNextLoc( loc, lineCounter, insufficentMemory );
    }

    return;
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

void Assembler::RunProgramInEmulator()
{
    // Insert all the translated instruction form  machine Instruction table into emulator
    InsertInstToEmulator();

    if( !m_emul.RunProgram() )
    {
        std::cout << std::endl << "Program Execution Stoped!" << std::endl << std::endl;

        Errors::DisplayAllErrors();

        m_machInstTab.DisplayMachineInstTable();
    }

}

void Assembler::InsertInstToEmulator()
{
    std::vector<MachineInstructionTable::Stuct_MachineInstruction>::iterator currInst = m_machInstTab.GetMachineInstTabBegin();
    
    while( currInst != m_machInstTab.GetMachineInstTabEnd() )
    {
        // getting the content and loc
        long long currNumContent = std::stoll( currInst->m_Content );
        int currLoc = currInst->m_Location;

        // checking if default location, == true then skip this instruction, 
        // its a comment
        if( currLoc == m_machInstTab.GetDefaultLocation() )
        {
            ++currInst;
            continue;
        }

        // if content not present set that to 0
        if( currNumContent == std::stoll(m_machInstTab.GetDefaultContent()) )
        {
            currNumContent = 0;
        }

        // error if location is less than 0 or more than MEMSZ
        if( !m_emul.InsertMemory( currInst->m_Location, currNumContent ) )
        {
            Errors::RecordError( Errors::ErrorTypes::ERROR_InvalidLoc, "Line", currInst->m_Location, currInst->m_Content );
			m_noError = false;
        }

        ++currInst;
    }

    return;
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

void Assembler::TranslateInstruction( int a_Loc, int a_LineCounter )
{
    std::string translatedContent;  // stores translating the intruction into machine readable form

    int opCodeNum = m_inst.GetOpCodeNum(); // getting the opCodeNum of current instruction

    // preparing to add to the machine instruction table
    
    // there is always an op code
    SmartFillContent( translatedContent, opCodeNum % 100, 2 );

    // checking if both operand 1 and  operand 2 have error
    if( m_inst.IsErrorOperand1() && m_inst.IsErrorOperand2() )
    {
        translatedContent.append( "??????????" );
        m_machInstTab.AddMachineIntr( m_inst.GetOrgiInst(), a_Loc, translatedContent );

        return;
    }
   
    // for ORG, DS does not have content 
    if( opCodeNum == 100 || opCodeNum == 300 )
    {
        // checking for error
        if( m_inst.IsErrorOperand1() )
        {
            m_machInstTab.AddMachineIntr( m_inst.GetOrgiInst(), a_Loc, "00?????00000" );
        }
        else
        {
            m_machInstTab.AddMachineIntr( m_inst.GetOrgiInst(), a_Loc );
        }
    }
    // for operation code DC, content is 00|00000|operand1Val
    else if( opCodeNum == 200 )
    {
        // checking error
        if( m_inst.IsErrorOperand1() )
        {
			m_noError = false;
            translatedContent.append ("?????00000") ;
        }
        else
        {
            //no error
            // for DC the value of operand1 is stored in operand 2
            SmartFillContent( translatedContent, m_inst.GetNumOperand1(), 10 );
        }

        // adding it to the  machine inst table
        m_machInstTab.AddMachineIntr( m_inst.GetOrgiInst(), a_Loc, translatedContent );
        return;
    }
    // for halt content is 13|00000|00000
    else if( opCodeNum == 13 )
    {
        // checking error
        if( m_inst.IsErrorOperand1() )
        {
			m_noError = false;
            m_machInstTab.AddMachineIntr( m_inst.GetOrgiInst(), a_Loc, "13?????00000" );
        }
        else
        {
            // it is halt and its content only has starting 13
            // adding it to the  machine inst table
            m_machInstTab.AddMachineIntr( m_inst.GetOrgiInst(), a_Loc, "130000000000" );
        }

        return;
    }
    else
    {
        // everything else has content -> opCode|operand1|operand2
        // where operand1 is label 
        //       operand2 is label that points to memory variable i.e. DC or DS or empty

        // ============= appending operand 1 to the content ============= 
        // operand 1 can only be label
        // but operand 1 of ADD, SUB, MULT, DIV, COPY, READ, and WRITE should point to memory variable

        // getting the label stored in operand 1
        std::string label = m_inst.GetOperand1();

        // getting the location that label points to
        int labelLoc;
        // checking if error
        if( m_inst.IsErrorOperand1() )
        {
			m_noError = false;
            translatedContent.append( "?????" );
        }
        else if( !m_symtab.LookupSymbol( label, labelLoc ) )
        {
            // label not found
			m_noError = false;
            Errors::RecordError( Errors::ErrorTypes::ERROR_UndefinedLabel, "Line", a_LineCounter, m_inst.GetOrgiInst() );
            translatedContent.append( "?????" );
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
					m_noError = false;
                    Errors::RecordError( Errors::ErrorTypes::ERROR_NotLabelConst, "Line", a_LineCounter, m_inst.GetOrgiInst() );
                    translatedContent.append( "?????" );
                }
            }
        }

        // =============  appending operand 2 to the content ============= 
        // operand 2 can be label which points to a memory variable or empty
        if( m_inst.IsErrorOperand2() )
        {
			m_noError = false;
            translatedContent.append( "?????" );
        }
        else if( m_inst.GetOperand2() == "" )
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
				m_noError = false;
                Errors::RecordError( Errors::ErrorTypes::ERROR_UndefinedLabel, "Line", a_LineCounter, m_inst.GetOrgiInst() );
                translatedContent.append( "?????" );
            }
            else
            {
                if( LookupDeclaredConst( label2 ) || LookupDeclaredVarMem( label2 ) )
                {
                    SmartFillContent( translatedContent, label2Loc );
                }
                else
                {
					m_noError = false;
                    Errors::RecordError( Errors::ErrorTypes::ERROR_NotLabelConst, "Line", a_LineCounter, m_inst.GetOrgiInst() );
                    translatedContent.append( "?????" );
                }
            }
        }
        // adding it to the  machine inst table
        m_machInstTab.AddMachineIntr( m_inst.GetOrgiInst(), a_Loc, translatedContent);
    }

	return;
}

void Assembler::ComputeNextLoc( int& a_loc, int& a_LineCounter, bool& a_insufficentMemory )
{
    // Compute the location of the next instruction.
    a_loc = m_inst.LocationNextInstruction( a_loc );

    // checking for storage
    if( a_loc > Emulator::MEMSZ && !a_insufficentMemory)
    {
        // insufficent memory
        Errors::RecordError( Errors::ErrorTypes::ERROR_InsufficentMemory, "Line", a_LineCounter, m_inst.GetOrgiInst() );
        a_insufficentMemory = true;
    }
}
