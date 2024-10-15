#include "smart_contract.h"
#include <iostream>

using namespace std;

// Constructor implementation
SmartContract::SmartContract(int id, std::function<void()> logicFunction)
    : contractID(id), logic(logicFunction) {}

SmartContract::SmartContract()
{
}

SmartContract::~SmartContract()
{
}

// Getter for contractID
int SmartContract::GetContractID() const
{
    return contractID;
}

// Setter for contractID
void SmartContract::SetContractID(int id)
{
    contractID = id;
}

// Execute the smart contract logic
void SmartContract::Execute() const
{
    logic();
}
