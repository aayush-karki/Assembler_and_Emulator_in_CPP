#include "stdafx.h"
#include "Errors.h"

// inializing the static member variables
std::vector<std::string> Errors::m_ErrorMsgs;

void Errors::DisplayAllErrors()
{
	std::vector<std::string>::iterator currMessage = m_ErrorMsgs.begin();

	// printing
	while( currMessage != m_ErrorMsgs.end() )
	{
		std::cout << *currMessage << std::endl;
		++currMessage;
	}

}
