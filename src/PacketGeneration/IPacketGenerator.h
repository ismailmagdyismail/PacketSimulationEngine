#pragma once

struct Packet;

class IPacketGenerator
{
public:
    virtual Packet GeneratePacket() = 0;

    virtual ~IPacketGenerator() = default;
};