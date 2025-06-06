#include "IPacketGenerator.h"

class HTTPGenerator : public IPacketGenerator
{
public:
    virtual std::shared_ptr<Packet> GeneratePacket() override;
};