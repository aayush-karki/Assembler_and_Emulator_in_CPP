/// /*************************************************************************************/
/// 
/// @file SymTab.cpp 
/// 
/// @brief  This file is a source file for SymbolTable class.
/// 
/// It contains all of the defination  of the member funciton.
///
/// @author Aayush Karki
/// 
/// @date  November 12, 2021 
///
/*************************************************************************************/

#include "stdafx.h"
#include "SymTab.h"

bool SymbolTable::AddSymbol( const std::string &a_symbol, int a_loc, int a_LineCounter, std::string a_orgiInst )
{
    // If the symbol is already in the symbol table, record it as multiply defined.
    std::map<std::string, int>::iterator st = m_symbolTable.find( a_symbol );
    if( st != m_symbolTable.end() ) {

        // report the error
        Errors::RecordError( Errors::ErrorTypes::ERROR_MultipleLabel, "Line", a_LineCounter, a_orgiInst );
        return false;
    }
    // Record a the  location in the symbol table.
    m_symbolTable[a_symbol] = a_loc;

    return true;
}

void SymbolTable::DisplaySymbolTable()
{
    std::map<std::string, int>::iterator currSymbolIte = m_symbolTable.begin();
    int symIndex = 0;

    std::cout << "Symbol Table:" << std::endl << std::endl;

    // formating the table and printing the table headign
    std::cout << std::left;
	std::cout << std::setw( m_MAX_SYMBOL_LENGTH + 1 ) << "Symbol# "
		<< std::setw( m_MAX_SYMBOL_LENGTH + 1 ) << "Symbol"
		<< " Location" << std::endl;

    // printing the values
    while (currSymbolIte != m_symbolTable.end())
    {
		std::cout << " " << std::setw( 6 ) << symIndex << "\t    "
			<< std::setw( m_MAX_SYMBOL_LENGTH + 1 ) << currSymbolIte->first
			<< " " << currSymbolIte->second << std::endl;
        ++symIndex; 
        currSymbolIte++;
    }

    std::cout << std::setfill('_') << std::setw(90) << " " << std::endl << std::endl;
    std::cout << std::setfill( ' ' );

    std::cout << "Press Enter to Continue" << std::endl;

    std::string enter;
    std::getline( std::cin, enter );

}

bool SymbolTable::LookupSymbol( const std::string& a_symbol, int& a_loc )
{
    // finding the symbol
    std::map<std::string, int>::iterator currSymbolIte = m_symbolTable.find(a_symbol);

    if( currSymbolIte == m_symbolTable.end() )
    {
        return false;
    }

    a_loc = currSymbolIte->second;

    return true;
}

