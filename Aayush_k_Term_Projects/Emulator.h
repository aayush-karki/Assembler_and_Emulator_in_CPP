//
//		Emulator class - supports the emulation of VC1620 programs
//
#ifndef _EMULATOR_H      // UNIX way of preventing multiple inclusions.
#define _EMULATOR_H

class Emulator {

public:

    const static int MEMSZ = 100000;	// The size of the memory of the VC1620.
    
    /// 
    /// @brief constructor
    /// 
    Emulator() {

         m_memory.resize(MEMSZ, 0);
    }

    /// 
    /// @brief InsertMemory Records instructions and data into VC1620.
    /// 
    /// @param a_location index of VC1620 memory when the intruction and data should be stored
    /// @param a_contents intruction and data that needs to be stored in the memory 
    /// 
    /// @returns true if successfull
    /// 
    bool InsertMemory( int a_location, long long a_contents );
    
    /// 
    /// @brief RunProgram Runs the VC5000 program recorded in memory.
    /// 
    /// returns true if recorded program run successfully
    /// 
    bool RunProgram( );

private:
    // ==================================== private function =====================================


    bool ReadFromUser( std::string& a_userInput, int a_currAddr );

private:
    // ==================================== private variables =====================================

    std::vector<long long> m_memory;  	// Memory for the VC1620
    const int m_START_ADDRESS = 100;
};

#endif

