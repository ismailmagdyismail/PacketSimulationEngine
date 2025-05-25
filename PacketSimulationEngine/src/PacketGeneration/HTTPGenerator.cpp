#include "HTTPGenerator.h"
#include "Packet.h"

Packet HTTPGenerator::GeneratePacket()
{
    //! Dummy temp place holder
    return Packet{
        .m_eProtocol = Protocol::HTTP,
        .m_strDestAddress = "12345678",
        .m_strSrcAddress = "12345678",
        .m_strPayload = "123456789101112131415161718192021222324252627282930313233343536373839404142434546",
        .m_chronoTimestamp = std::chrono::steady_clock::now(),
    };
}