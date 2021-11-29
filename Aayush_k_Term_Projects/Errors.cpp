#include "stdafx.h"
#include "Errors.h"

// inializing the static member variables
std::vector<std::string> Errors::m_ErrorMsgs;



void Errors::RecordError( Errors::ErrorTypes a_errorType, int a_LineCounter, std::string a_OrgiInst )
{
	std::string errorMessage = "Line--" + std::to_string( a_LineCounter ) +"--\"";
	
	// Missing end does not have a instruction to show
	if( a_errorType != Errors::ErrorTypes::ERROR_MissingEnd )
	{
		errorMessage.append( a_OrgiInst );
		errorMessage += "\"";
	}
	errorMessage.append( Errors::LookUpErrorMsgs(a_errorType) );

	// adding to the vector
	m_ErrorMsgs.push_back( errorMessage );
}

void Errors::DisplayAllErrors()
{
	std::vector<std::string>::iterator currMessage = m_ErrorMsgs.begin();
	int errorNum = 1;

	// checking if error message is empty
	if( currMessage == m_ErrorMsgs.end() )
	{
		return;
	}

	std::cout << "Error List:" << std::endl << std::endl;

	// printing
	while( currMessage != m_ErrorMsgs.end() )
	{
		std::cout << errorNum << ") " << *currMessage << std::endl << std::endl;
		++currMessage;
		++errorNum;
	}
	std::cout << std::endl;

}

std::string Errors::LookUpErrorMsgs( Errors::ErrorTypes a_errorType )
{
	switch( a_errorType )
	{
		case ( Errors::ErrorTypes::ERROR_InvalidInstruction ):
		{
			return std::string( "--\n   ERROR--Invalid Instruction" );
		}
		case ( Errors::ErrorTypes::ERROR_MissingEnd ):
		{
			return std::string( "--\n   ERROR--End of File reached.Missing END statement");
		}
		case ( Errors::ErrorTypes::ERROR_EndNotLast ):
		{
			return std::string( "--\n   ERROR--END statement is not the last statement in the program" );
		}
		case ( Errors::ErrorTypes::ERROR_MachineLangInAssemLang ):
		{
			return std::string( "--\n   ERROR--Machine level code in Assembly level code" );
		}
		case ( Errors::ErrorTypes::ERROR_UndefinedLabel ):
		{
			return std::string( "--\n   ERROR--Undefined Label" );
		}
		case ( Errors::ErrorTypes::ERROR_MultipleLabel ):
		{
			return std::string( "--\n   ERROR--Multiple label defined" );
		}
		case ( Errors::ErrorTypes::ERROR_ExtraStatEle ):
		{
			return std::string( "--\n   Extra statement elements" );
		}
		case ( Errors::ErrorTypes::ERROR_OpCodePos ):
		{
			return std::string( "--\n   ERROR--Invalid Statement--Operation Code should be at the start of the statement or followed by a label" );
		}
		case ( Errors::ErrorTypes::ERROR_MissingOpCode ):
		{
			return std::string( "--\n   ERROR--Missing Operation Code" );
		}
		case ( Errors::ErrorTypes::ERROR_MissingLabel ):
		{
			return std::string( "--\n   ERROR--Missing Statement Elemnet--Operation code should have a Label" );
		}
		case ( Errors::ErrorTypes::ERROR_MissingOperand ):
		{
			return std::string( "--\n   ERROR--Missing Statement Elemnet--Operation code should have a Operand" );
		}
		case ( Errors::ErrorTypes::ERROR_ExtraOperand1 ):
		{
			return std::string( "--\n   ERROR--Extra Statement Element--Operation code does not need operand 1" );
		}
		case ( Errors::ErrorTypes::ERROR_ExtraOperand2 ):
		{
			return std::string( "--\n   ERROR--Extra Statement Element--Operation code does not need operand 2" );
		}
		case ( Errors::ErrorTypes::ERROR_NotNumOperand1 ):
		{
			return std::string( "--\n   ERROR--Invalid Syntax--Operand 1 can only have a numeric value" );
		}
		case ( Errors::ErrorTypes::ERROR_NotLabelOperand1 ):
		{
			return std::string( "--\n   ERROR--Invalid Syntax--Operand 1 can only have a Label" );
		}
		case ( Errors::ErrorTypes::ERROR_NotLabelOperand2 ):
		{
			return std::string( "--\n   ERROR--Invalid Syntax--Operand 2 can only have a Label" );
		}
		case ( Errors::ErrorTypes::ERROR_InvalidOperand1Len ):
		{
			return std::string( "--\n   ERROR--Invalid Syntax--Operand 1 is too large for memory. Its length cannot be more than 5 digit" );
		}
		case ( Errors::ErrorTypes::ERROR_NotLabelConst ):
		{
			return std::string( "--\n   ERROR--Invalid Label--Label does not point to a variable memory" );
		}
		case ( Errors::ErrorTypes::ERROR_InvalidSymSyntaxLen ):
		{
			return std::string( "--\n   ERROR--Invalid Label Naming Length--Maximum length a symbol can be is 10" );
		}
		case ( Errors::ErrorTypes::ERROR_InvalidSymSyntaxfirstChar ):
		{
			return std::string( "--\n   ERROR--Symbol Naming Error--First character of a symbol must be a alphabet" );
		}
		case ( Errors::ErrorTypes::ERROR_InvalidSymSyntaxComposition ):
		{
			return std::string( "--\n   ERROR--Symbol Naming Error--Symbol must be composed of only alphabets and digits" );
		}
		case ( Errors::ErrorTypes::ERROR_InsufficentMemory ):
		{
			return std::string( "--\n   ERROR-- Insufficent Memory" );
		}
	}
}


