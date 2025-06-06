#pragma once

//! System includes
#include <memory>

struct Packet;

class IPacketGenerator
{
public:
    virtual std::shared_ptr<Packet> GeneratePacket() = 0;

    virtual ~IPacketGenerator() = default;
};