#ifndef PEDERSEN_COMMITMENT_H
#define PEDERSEN_COMMITMENT_H

#include <cryptopp/eccrypto.h>
#include <cryptopp/oids.h>
#include <cryptopp/osrng.h>



class PedersenCommitment {
public:
    PedersenCommitment(const CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey& privateKey,
                       const CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey& publicKey);

    PedersenCommitment();
    ~PedersenCommitment();


    static void InitializeStaticMembers();

   
    std::string Commit(const CryptoPP::Integer& x, const CryptoPP::Integer& r) const;
    bool Verify(const std::string& commitmentStr, const CryptoPP::Integer& x, const CryptoPP::Integer& r) const;

    bool VerifyPoint(const CryptoPP::ECP::Point& C, const CryptoPP::Integer& x, const CryptoPP::Integer& r) const;


private:
    static CryptoPP::ECP::Point G, H;
    CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey m_privateKey;
    CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey m_publicKey;

    static std::string PointToString(const CryptoPP::ECP::Point& point);
    static CryptoPP::ECP::Point StringToPoint(const std::string& str);
};

#endif // PEDERSEN_COMMITMENT_H
