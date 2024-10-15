#include "pedersen_commitment.h"
#include <cryptopp/nbtheory.h>
#include <cryptopp/hex.h>

// Static member initialization
CryptoPP::ECP::Point PedersenCommitment::G, PedersenCommitment::H;

PedersenCommitment::PedersenCommitment(const CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey &privateKey,
                                       const CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey &publicKey)
    : m_privateKey(privateKey), m_publicKey(publicKey) {}

PedersenCommitment::PedersenCommitment() {}
PedersenCommitment::~PedersenCommitment() {}

void PedersenCommitment::InitializeStaticMembers()
{
    CryptoPP::AutoSeededRandomPool prng;

    // Initialize using a standard curve
    CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey tempPrivateKey;
    tempPrivateKey.Initialize(prng, CryptoPP::ASN1::secp256k1());
    CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey tempPublicKey;
    tempPrivateKey.MakePublicKey(tempPublicKey);

    // Set G as the standard generator point of the curve
    G = tempPublicKey.GetPublicElement();

    // Generate H, another point on the curve, ensuring its discrete log relation to G is unknown
    // This is a simplified example. The actual implementation should ensure the security of H
    CryptoPP::Integer xCoordH = CryptoPP::Integer(prng, CryptoPP::Integer::One(), tempPrivateKey.GetGroupParameters().GetMaxExponent());
    H = tempPrivateKey.GetGroupParameters().GetCurve().Multiply(xCoordH, G);
}

std::string PedersenCommitment::Commit(const CryptoPP::Integer &x, const CryptoPP::Integer &r) const
{
    // C = xG + rH
    CryptoPP::ECP::Point C = m_privateKey.GetGroupParameters().GetCurve().Multiply(x, G);
    C = m_privateKey.GetGroupParameters().GetCurve().Add(C, m_privateKey.GetGroupParameters().GetCurve().Multiply(r, H));
    return PointToString(C);
}

bool PedersenCommitment::VerifyPoint(const CryptoPP::ECP::Point &C, const CryptoPP::Integer &x, const CryptoPP::Integer &r) const
{
    // Recalculate C using x and r and compare with the given C
    CryptoPP::ECP::Point calculatedC = m_privateKey.GetGroupParameters().GetCurve().Multiply(x, G);
    calculatedC = m_privateKey.GetGroupParameters().GetCurve().Add(calculatedC, m_privateKey.GetGroupParameters().GetCurve().Multiply(r, H));

    return (C.x == calculatedC.x) && (C.y == calculatedC.y);
}

bool PedersenCommitment::Verify(const std::string &commitmentStr, const CryptoPP::Integer &x, const CryptoPP::Integer &r) const
{
    CryptoPP::ECP::Point C = StringToPoint(commitmentStr);
    return VerifyPoint(C, x, r);
}

std::string PedersenCommitment::PointToString(const CryptoPP::ECP::Point& point) {
    CryptoPP::Integer x = point.x, y = point.y;

    std::string xStr, yStr;
    x.Encode(CryptoPP::StringSink(xStr).Ref(), x.ByteCount()); // Use ByteCount for full precision
    y.Encode(CryptoPP::StringSink(yStr).Ref(), y.ByteCount()); 

    std::string combinedStr = xStr + yStr;
    std::string hexStr;
    CryptoPP::StringSource(combinedStr, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(hexStr)));

    return hexStr;
}


CryptoPP::ECP::Point PedersenCommitment::StringToPoint(const std::string& str) {
    std::string decodedStr;
    CryptoPP::StringSource(str, true, new CryptoPP::HexDecoder(new CryptoPP::StringSink(decodedStr)));

    size_t len = decodedStr.size() / 2; // Assuming x and y have the same size
    CryptoPP::Integer x((const CryptoPP::byte*)decodedStr.substr(0, len).data(), len);
    CryptoPP::Integer y((const CryptoPP::byte*)decodedStr.substr(len).data(), len);

    return CryptoPP::ECP::Point(x, y);
}

