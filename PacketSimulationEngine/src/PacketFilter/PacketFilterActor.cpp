#include "PacketFilterActor.h"

#include <iostream>
#include <fstream>

PacketFilterActor::PacketFilterActor(
    std::shared_ptr<IChannel<std::shared_ptr<Packet>>> &p_pInputPacketsChannel,
    std::vector<std::shared_ptr<IChannel<std::shared_ptr<Packet>>>> &&p_vecForwardOutputChannels,
    std::vector<std::shared_ptr<IChannel<std::shared_ptr<Packet>>>> &&p_vecFilteredOutputChannels)
    : Actor(std::bind(&PacketFilterActor::ProcessPacket, this)),
      m_pInputPacketsChannel(p_pInputPacketsChannel),
      m_vecForwardOutputChannels(std::move(p_vecForwardOutputChannels)),
      m_vecFilteredOutputChannels(std::move(p_vecFilteredOutputChannels))
{
}

void PacketFilterActor::Configure(const PacketFilterConfigurations &p_oConfigurations)
{
    std::lock_guard<std::mutex> oLock{m_oConfigurationsMutex};
    m_oConfigurations = p_oConfigurations;
}

void PacketFilterActor::ProcessPacket()
{
    std::shared_ptr<Packet> p_pPacket;
    bool bResult = m_pInputPacketsChannel->ReadValue(p_pPacket);
    if (!bResult)
    {
        //! TODO: Stop();
        return;
    }
    if (ShouldFilter(p_pPacket))
    {
        NotifyOutputChannels(p_pPacket, m_vecFilteredOutputChannels);
        return;
    }
    NotifyOutputChannels(p_pPacket, m_vecForwardOutputChannels);
}

bool PacketFilterActor::ShouldFilter(std::shared_ptr<Packet> &p_pPacket)
{
    std::lock_guard<std::mutex> oLock{m_oConfigurationsMutex};
    return p_pPacket->m_eProtocol == m_oConfigurations.m_eProtocolToFilter;
}

void PacketFilterActor::NotifyOutputChannels(std::shared_ptr<Packet> &p_pPacket, std::vector<std::shared_ptr<IChannel<std::shared_ptr<Packet>>>> &p_vecOutputChannel)
{
    for (auto &oChannel : p_vecOutputChannel)
    {
        oChannel->SendValue(p_pPacket);
    }
}
