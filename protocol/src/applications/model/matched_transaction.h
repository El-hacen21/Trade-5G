#ifndef MATCHEDTRANSACTION_H
#define MATCHEDTRANSACTION_H

#include <memory>

#include <string>

using namespace std;

class MatchedTransaction
{
private:
    string offerTx;
    string demandTx;
    int quantity;
    int resource;
    double price;
    double offerMatchDelay;
    double demandMatchDelay;
    double demandTotalDelay;
    double offerTotalDelay;

public:
    MatchedTransaction();
    ~MatchedTransaction();

    void SetOfferTx(const string &offer);
    void SetDemandTx(const string &demand);
    void SetQuantity(int quantity);
    void SetResource(const int resource);
    void SetPrice(double price);
    void SetOfferMatchDelay(double delay);
    void SetDemandMatchDelay(double delay);
    void SetOfferTotalDelay(double delay);
    void SetDemandTotalDelay(double delay);

    string GetOfferTx() const;
    string GetDemandTx() const;
    int GetQuantity() const;
    int GetResource() const;
    double GetPrice() const;
    double GetOfferMatchDelay() const;
    double GetDemandMatchDelay() const;

    double GetOfferTotalDelay() const;
    double GetDemandTotalDelay() const;
};

#endif // MATCHEDTRANSACTION_H
