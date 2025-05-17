#include "IPacketGenerator.h"

class HTTPGenerator : public IPacketGenerator
{
public:
    virtual Packet GeneratePacket() override;
};