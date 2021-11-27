///
///		Symbol table class.
///
#pragma once

#include "Errors.h"

// This class is our symbol table.
class SymbolTable {

public:

    // Get rid of constructor and destructor later if you don't need them.
    SymbolTable( ) {};
    ~SymbolTable( ) {};
    
    const int multiplyDefinedSymbol = -999;

    /// 
    /// @brief AddSymbol adds a new symbol to the symbol table.
    ///
    /// AddSymbol function will place the symbol "a_symbol" and its location "a_loc"
    ///    in the symbol table.
    ///
    /// @param a_symbol lable name to be added in the list
    /// @param a_loc location that the label points to
    /// 
    void AddSymbol( const std::string &a_symbol, int a_loc, int a_LineCounter );

    /// 
    /// @brief DisplaySymbolTable Display the symbol table to the screen.
    /// 
    void DisplaySymbolTable();

    /// 
    /// @brief Lookup a symbol in the symbol table.
    /// 
    /// 
    /// @param a_symbol address of symbol to lookup
    /// @param a_loc stores VC1960 memory location of symbol if found
    /// 
    /// @returns LookupSymbol returns true is symbol is found in table else returns false 
    /// 
    bool LookupSymbol(const std::string &a_symbol, int& a_loc);

private:

    // This is the actual symbol table.  The symbol is the key to the map.  The value is the location.
    std::map<std::string, int> m_symbolTable;
    const int MAX_SYMBOL_LENGTH = 10;
};
