#include "matched_transaction.h"

using namespace std;

MatchedTransaction::MatchedTransaction() {}

MatchedTransaction::~MatchedTransaction() {}

void MatchedTransaction::SetOfferTx(const string &offer)
{
    this->offerTx = offer;
}

void MatchedTransaction::SetDemandTx(const string &demand)
{
    this->demandTx = demand;
}

void MatchedTransaction::SetQuantity(int quantity)
{
    this->quantity = quantity;
}

void MatchedTransaction::SetResource(const int resource)
{
    this->resource = resource;
}

void MatchedTransaction::SetPrice(double price)
{
    this->price = price;
}

string MatchedTransaction::GetOfferTx() const
{
    return offerTx;
}

string MatchedTransaction::GetDemandTx() const
{
    return demandTx;
}

int MatchedTransaction::GetQuantity() const
{
    return quantity;
}

int MatchedTransaction::GetResource() const
{
    return resource;
}

double MatchedTransaction::GetPrice() const
{
    return price;
}

void MatchedTransaction::SetOfferMatchDelay(double delay)
{
    offerMatchDelay = delay;
}
void MatchedTransaction::SetDemandMatchDelay(double delay)
{
    demandMatchDelay = delay;
}

void MatchedTransaction::SetOfferTotalDelay(double delay)
{
    offerTotalDelay = delay;
}

void MatchedTransaction::SetDemandTotalDelay(double delay)
{
    demandTotalDelay = delay;
}

double MatchedTransaction::GetOfferMatchDelay() const
{
    return offerMatchDelay;
}
double MatchedTransaction::GetDemandMatchDelay() const
{
    return demandMatchDelay;
}

double MatchedTransaction::GetOfferTotalDelay() const
{
    return offerTotalDelay;
}

double MatchedTransaction::GetDemandTotalDelay() const
{
    return demandTotalDelay;
}