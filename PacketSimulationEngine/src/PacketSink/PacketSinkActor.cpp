#include "PacketSinkActor.h"
#include "Packet.h"

PacketSinkActor::PacketSinkActor(std::shared_ptr<IChannel<std::shared_ptr<Packet>>> &p_pInputChannel, std::vector<std::shared_ptr<IChannel<std::shared_ptr<Packet>>>> &&p_vecOutputChannels)
    : Actor(std::bind(&PacketSinkActor::CollectPackets, this)), m_oInputChannel(p_pInputChannel), m_vecOutputChannels(std::move(p_vecOutputChannels))
{
}

void PacketSinkActor::CollectPackets()
{
    std::shared_ptr<Packet> pPacket;
    bool bResult = m_oInputChannel->ReadValue(pPacket);
    if (!bResult)
    {
        return;
    }
    NotifyOutputChannels(pPacket);
}

void PacketSinkActor::NotifyOutputChannels(std::shared_ptr<Packet> &p_pPacket)
{
    for (auto &oChannel : m_vecOutputChannels)
    {
        oChannel->SendValue(p_pPacket);
    }
}