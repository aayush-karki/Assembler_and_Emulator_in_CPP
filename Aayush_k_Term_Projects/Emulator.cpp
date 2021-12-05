//    Implementation of the Emulator class.

#include "stdafx.h"
#include "Emulator.h"
#include "Errors.h"

bool Emulator::InsertMemory( int a_location, long long a_contents )
{

    // insert only if valid location
    if( a_location >= 0 && a_location <= MEMSZ )
    {
        m_memory.at( a_location ) = a_contents;
        return true;
    }

    return false;
}

bool Emulator::RunProgram()
{
    int currAddr = m_START_ADDRESS;


    while( currAddr <= MEMSZ)
    {
        //// if content is 0 skip it
        //if( m_memory.at( currAddr ) == 0 )
        //{
        //    ++currAddr;
        //    continue;
        //}

        // extracting the instruction from current memory
        long long curVal = m_memory.at( currAddr );
        int opCode = m_memory.at( currAddr ) / 10'000'000'000;
        int operand1 = ( m_memory.at( currAddr ) % 10'000'000'000 ) / 100'000;
        int operand2 = ( m_memory.at( currAddr ) % 100'000 );

        switch( opCode )
        {
            case ( 1 ):
            {
                // add
                m_memory.at( operand1 ) += m_memory.at( operand2 );
                break;
            }
            case ( 2 ):
            {
                // subtract
                m_memory.at( operand1 ) -= m_memory.at( operand2 );
                break;
            }
            case ( 3 ):
            {
                // multiply
                m_memory.at( operand1 ) *= m_memory.at( operand2 );
                break;
            }
            case ( 4 ):
            {
                // divide
                m_memory.at( operand1 ) /= m_memory.at( operand2 );
                break;
            }
            case ( 5 ):
            {
                // copy
                m_memory.at( operand1 ) = m_memory.at( operand2 );
                break;
            }
            case ( 7 ):
            {
                // read
                std::string userInput = "0";
                if( !ReadFromUser( userInput, currAddr ) )
                {
                    return false;
                }

                m_memory.at( operand1 ) = std::stoi(userInput);

                break;
            }
            case ( 8 ):
            {
                // write
                std::cout << m_memory.at( operand1 ) << std::endl;
                break;
            }
            case ( 9 ):
            {
                // branch
                currAddr = operand1;
                continue;

                break;
            }
            case ( 10 ):
            {
                // branch minus
                if( m_memory.at( operand2 ) < 0 )
                {
                    currAddr = operand1;
                    continue;
                }
                break;
            }
            case ( 11 ):
            {
                // branch zero
                if( m_memory.at( operand2 ) == 0 )
                {
                    currAddr = operand1;
                    continue;
                }
                break;
            }
            case( 12 ) :
            {
                // branch positive
                if( m_memory.at( operand2 ) > 0 )
                {
                    currAddr = operand1;
                    continue;
                }
                break;
            }
            case( 13 ):
            {
                // halt
                return true;
                break;
            }
            default:
            {
                // not recognised opcode
                Errors::RecordError( Errors::ErrorTypes::ERROR_InvalidInstruction,"Loc", currAddr, std::to_string(m_memory.at(currAddr)));
                return false;
            }
        }
    
        ++currAddr;
    }

    //halt not found out of memory bounds
    Errors::RecordError( Errors::ErrorTypes::ERROR_MissingHalt, "Loc", currAddr, std::to_string( m_memory.at( currAddr ) ) );
    return false;
}

bool Emulator::ReadFromUser(std::string &a_userInput, int a_currAddr)
{
    
    // taking userinput until they give in int
    std::cout << "? ";
    std::cin >> a_userInput;
    

    // =================== validating the input =======================
    char sign = '0';

    // checking if the first char is - or +
    if( a_userInput.at( 0 ) == '-' || a_userInput.front() == '+' )
    {
        sign = a_userInput.at( 0 );
        a_userInput.erase( 0, 1 ); // removing the first char
    }
    
    // checking if all the char are num
    if( !std::all_of( a_userInput.begin(), a_userInput.end(), isdigit ) )
    {
        // error
        Errors::RecordError( Errors::ErrorTypes::ERROR_InvalidInputSyn, "Loc", a_currAddr, std::to_string( m_memory.at( a_currAddr ) ) );

        return false;
    }

    // chekcing if the string can successfully convert in to an int
    // valid input is (+/-)2147483647
    
    // chekcing for length of the string, max is 10 cause int can only handel (+/-)2147483647
    if( a_userInput.size() > 10 )
    {
        Errors::RecordError( Errors::ErrorTypes::ERROR_InvalidInputLen, "Loc", a_currAddr, std::to_string( m_memory.at( a_currAddr ) ) );
        return false;
    }
    else if( a_userInput.size() == 10 )
    {
        // checking if it is bigger than 2147483647
        if( a_userInput.at( 0 ) > '2' )
        {
            Errors::RecordError( Errors::ErrorTypes::ERROR_InvalidInputRange, "Loc", a_currAddr, std::to_string( m_memory.at( a_currAddr ) ) );
            return false;
        }
        else if( a_userInput.at( 0 ) == '2' && a_userInput.at( 9 ) > '7')
        {
            Errors::RecordError( Errors::ErrorTypes::ERROR_InvalidInputRange, "Loc", a_currAddr, std::to_string( m_memory.at( a_currAddr ) ) );
            return false;
        }
    }

    // input is valid

    // every thing is digit
    // adding the first char back
    // if it was not a sign, then default vaule of 0 will be added, which should not create any problem
    a_userInput = sign + a_userInput;

    return true;
}
