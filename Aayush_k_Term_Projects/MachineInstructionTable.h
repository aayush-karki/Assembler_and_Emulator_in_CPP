#pragma once

      
 
class MachineInstructionTable
{

public:
    /// 
    /// @brief constructor
    /// 
    MachineInstructionTable() {};

    /// 
    /// @brief deconstructor
    /// 
    /// Cleans up all the dynamically allocated memory
    /// 
    ~MachineInstructionTable() {};
    
    const int DEFAULT_LOCATION = -888;
    const int DEFAULT_CONTENT  = -777;

    // ========================= Stuct_MachineInstruction and its related funcitons ================== 

    /// 
    /// @struct
    /// Stuct_MachineInstruction is a stucture to hold all the translated machine intructure
    /// 
    ///  
    struct Stuct_MachineInstruction
    {
        int m_Location;         ///> stroes the location of where the instruciton is in VC1620's memory
                                ///> is the index for m_memory
        long long m_Content;    ///> contents that is to be stored in m_memory at index m_location
        std::string m_AssembInst; ///> copy of orginal Assembly instrction that was used to translate the
        /// 
        /// @brief constructor
        /// 
        /// Initializes the member variable of the structure
        /// 
        /// @param a_AssembInst: orginal assembly intruction used to translate into machine instruction
        /// @param a_Location: location in memory of VC1620 to stored the machine instruction 
        /// @param a_Content: machine instruction for VC1620 
        /// @param a_IsError: signifies if error is present in this line or not 
        /// 
        MachineInstructionTable::Stuct_MachineInstruction( )
        {
            m_Location = 0;
            m_Content = 0;
            m_AssembInst = "";
        }
    };

    // ====================== end of Stuct_MachineInstruction and its related funcitons ====================

    /// 
    /// @brief AddMachineIntruction adds traslated intruction to the machine instruction table.
    /// 
    /// @param a_AssembInst: orginal assembly intruction used to translate into machine instruction
    /// @param a_Location: location in memory of VC1620 to stored the machine instruction 
    /// @param a_Content: machine instruction for VC1620 
    /// 
    void AddMachineIntr( std::string a_AssembInst, int a_Location = -1, long long a_Content = -1);


    /// 
    /// @brief DisplaySymbolTable Display the symbol table to the screen.
    /// 
    void DisplaySymbolTable();


private:
        std::vector<Stuct_MachineInstruction> m_machineInstTab; // stores all the translated intruction in as a pointer

};