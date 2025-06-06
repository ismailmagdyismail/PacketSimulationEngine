#include "HTTPGenerator.h"
#include "Packet.h"

std::shared_ptr<Packet> HTTPGenerator::GeneratePacket()
{
    //! Dummy temp place holder
    return std::shared_ptr<Packet>(
        new Packet{
            "12345678",
            "12345678",
            "123456789101112131415161718192021222324252627282930313233343536373839404142434546",
            Protocol::HTTP,
            std::chrono::steady_clock::now()});
}