#ifndef SMARTCONTRACT_H
#define SMARTCONTRACT_H

#include <functional>

using namespace std;

class SmartContract {
private:
    int contractID; // Unique identifier for the contract
    function<void()> logic; // A function pointer to the contract's logic

public:
    SmartContract();
    ~SmartContract();
    // Constructor
    SmartContract(int id, function<void()> logicFunction);



    // Getter for contractID
    int GetContractID() const;

    // Setter for contractID
    void SetContractID(int id);

    // Execute the smart contract logic
    void Execute() const;

    // Optional: You can add more attributes or methods as needed, 
    // such as state variables, initialization methods, etc.
};

#endif // SMARTCONTRACT_H
