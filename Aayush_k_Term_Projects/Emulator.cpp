/*************************************************************************************/
/// 
/// @file Emulator.cpp 
/// 
/// @brief  This file is a source file for Emulator class.
/// 
/// It contains all of the defination  of the member funciton.
///
/// @author Aayush Karki
/// 
/// @date  December 03, 2021
///
/*************************************************************************************/

#include "stdafx.h"
#include "Emulator.h"
#include "Errors.h"

bool Emulator::InsertMemory( int a_location, long long a_contents )
{
	// insert only if  location is valid
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


	while( currAddr <= MEMSZ )
	{
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

				m_memory.at( operand1 ) = std::stoll( userInput );

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

				break; // just for my sanity
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

				break; // just for my sanity
			}
			case( 12 ):
			{
				// branch positive
				if( m_memory.at( operand2 ) > 0 )
				{
					currAddr = operand1;
					continue;
				}

				break; // just for my sanity
			}
			case( 13 ):
			{
				// halt
				return true;

				break; // just for my sanity
			}
			default:
			{
				// not recognised opcode
				Errors::RecordError( Errors::ErrorTypes::ERROR_InvalidInstruction, "Loc",
									 currAddr, std::to_string( m_memory.at( currAddr ) ) );
				return false;

				break; // just for my sanity
			}
		}

		++currAddr;
	}

	// halt not found when memory went out bounds
	Errors::RecordError( Errors::ErrorTypes::ERROR_MissingHalt, "Loc",
						 currAddr, std::to_string( m_memory.at( currAddr ) ) );

	return false;
}

bool Emulator::ReadFromUser( std::string& a_userInput, int a_currAddr )
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
		Errors::RecordError( Errors::ErrorTypes::ERROR_InvalidInputSyn, "Loc",
							 a_currAddr, std::to_string( m_memory.at( a_currAddr ) ) );

		return false;
	}

	// checking for length of the string, max is 12 cause word of VC1600 
	// memory can only handel 12 digits; which is be (+/-)99,999,999,999
	if( a_userInput.size() > 11 )
	{
		Errors::RecordError( Errors::ErrorTypes::ERROR_InvalidInputRange, "Loc",
							 a_currAddr, std::to_string( m_memory.at( a_currAddr ) ) );
		return false;
	}

	// input is valid

	// every thing is digit, so, adding the first char back
	// if it was not a sign, then default vaule of 0 will be added, which should not create any problem
	a_userInput = sign + a_userInput;

	return true;
}
