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
        std::string m_Content;    ///> contents that is to be stored in m_memory at index m_location, 
                                ///         needs to be converted to long long
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
            m_Content = "";
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
    void AddMachineIntr( std::string a_AssembInst, int a_Location = -1, std::string a_Content = "-5");


    /// 
    /// @brief DisplaySymbolTable Display the symbol table to the screen.
    /// 
    void DisplayMachineInstTable();

    ///
    /// @brief getter function to access the Default location
    /// 
    /// @return GetDefaultLocation returns value of Default location
    ///
    inline const int GetDefaultLocation(){ return DEFAULT_LOCATION; }

    ///
    /// @brief getter function to access the Default content
    /// 
    /// @return GetDefaultContent returns value of Default content
    ///
    inline const std::string GetDefaultContent(){ return DEFAULT_CONTENT; }

    ///
    /// @brief getter function to access the first element in machine instruction table
    /// 
    /// @return GetDefaultContent returns a pointer to begining of the table
    ///
    inline const std::vector<Stuct_MachineInstruction>::iterator GetMachineInstTabBegin(){ return m_machineInstTab.begin(); }

    ///
    /// @brief getter function to access the end of in machine instruction table
    /// 
    /// @return GetDefaultContent returns a pointer to end of the table
    ///
    inline const std::vector<Stuct_MachineInstruction>::iterator GetMachineInstTabEnd(){ return m_machineInstTab.end(); }

private:
        
    std::vector<Stuct_MachineInstruction> m_machineInstTab; // stores all the translated intruction in as a pointer
    const int DEFAULT_LOCATION = -888;
    const std::string DEFAULT_CONTENT = "-777";
};