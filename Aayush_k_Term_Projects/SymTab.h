/*************************************************************************************/
/// 
/// @file SymTab.h 
/// 
/// @brief  This file is a header file for SymbolTable class.
/// 
/// It contains all of the declaration of the member funciton of SymbolTable class.
///
/// @author Aayush Karki
/// 
/// @date  November 12, 2021 
///  
/*************************************************************************************/

#pragma once

#include "Errors.h"

///
/// @class SymbolTable "SymTab.h"
/// 
/// @brief This is a class for SymbolTable.
///	
/// SymbolTable holds all of the symbol that we scan in Pass I
/// 
/// @author Aayush Karki
/// 
/// @date  November 12, 2021 
/// 
class SymbolTable
{

public:

	/// 
	/// @brief AddSymbol adds a new symbol to the symbol table.
	///
	/// AddSymbol function will place the symbol "a_symbol" and its location "a_loc"
	///    in the symbol table.
	///
	/// @param a_symbol lable name to be added in the list
	/// @param a_loc location that the label points to
	/// @param a_LineCounter current line in the file, 
	///         needed for error reporting
	/// @param a_orgiInst orginal Assembly instruction in current line, 
	///         needed for error reporting 
	/// 
	/// @author Aayush Karki
	/// @author Victor Miller
	/// 
	/// @date  November 12, 2021 
	/// 
	bool AddSymbol( const std::string& a_symbol, int a_loc, int a_LineCounter, std::string a_orgiInst );

	/// 
	/// @brief DisplaySymbolTable Display the symbol table to the screen.
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 12, 2021 
	/// 
	void DisplaySymbolTable();

	/// 
	/// @brief Lookup a symbol in the symbol table.
	/// 
	/// @param a_symbol address of symbol to lookup
	/// @param a_loc stores VC1960 memory location of symbol if found
	/// 
	/// @returns LookupSymbol returns true is symbol is found in table else returns false 
	/// 
	/// @author Aayush Karki
	/// 
	/// @date  November 12, 2021
	/// 
	bool LookupSymbol( const std::string& a_symbol, int& a_loc );


private:

	// This is the actual symbol table.  The symbol is the key to the map.  The value is the location.
	std::map<std::string, int> m_symbolTable;

	const int m_MAX_SYMBOL_LENGTH = 10; // maximum char a symbol name can be 
	
	const int multiplyDefinedSymbol = -999; // indicator of multiply defined symbol
};
