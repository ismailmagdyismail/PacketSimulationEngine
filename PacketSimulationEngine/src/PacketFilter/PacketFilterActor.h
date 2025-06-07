#pragma once

//! System includes
#include <memory>
#include <vector>
#include <mutex>

//! threading
#include "Actor.h"

//! Filtering
#include "PacketFilterConfigurations.h"

template <typename T>
class IChannel;
struct Packet;

class PacketFilterActor : public Actor
{
public:
    PacketFilterActor(
        std::shared_ptr<IChannel<std::shared_ptr<Packet>>> &p_pInputPacketsChannel,
        std::vector<std::shared_ptr<IChannel<std::shared_ptr<Packet>>>> &&p_vecForwardOutputChannels,
        std::vector<std::shared_ptr<IChannel<std::shared_ptr<Packet>>>> &&p_vecFilteredOutputChannels);

    void Configure(const PacketFilterConfigurations &p_oConfigurations);

private:
    void EventLoop();
    void ProcessPacket();
    bool ShouldFilter(std::shared_ptr<Packet> &p_pPacket);
    void NotifyOutputChannels(std::shared_ptr<Packet> &p_pPacket, std::vector<std::shared_ptr<IChannel<std::shared_ptr<Packet>>>> &p_vecOutputChannel);

    std::mutex m_oConfigurationsMutex;
    PacketFilterConfigurations m_oConfigurations;

    std::shared_ptr<IChannel<std::shared_ptr<Packet>>> m_pInputPacketsChannel;
    std::vector<std::shared_ptr<IChannel<std::shared_ptr<Packet>>>> m_vecForwardOutputChannels;
    std::vector<std::shared_ptr<IChannel<std::shared_ptr<Packet>>>> m_vecFilteredOutputChannels;
};