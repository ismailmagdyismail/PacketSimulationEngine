#pragma once

//! System includes
#include <memory>
#include <vector>

//! threading
#include "Actor.h"

#include <iostream>

struct Packet;
template <typename T>
class IChannel;

class PacketSinkActor : public Actor
{
public:
    PacketSinkActor(std::shared_ptr<IChannel<std::shared_ptr<Packet>>> &p_pInputChannel, std::vector<std::shared_ptr<IChannel<std::shared_ptr<Packet>>>> &&p_vecOutputChannels);

private:
    void CollectPackets();
    void NotifyOutputChannels(std::shared_ptr<Packet> &p_oPacket);

    std::shared_ptr<IChannel<std::shared_ptr<Packet>>> m_oInputChannel;
    std::vector<std::shared_ptr<IChannel<std::shared_ptr<Packet>>>> m_vecOutputChannels;
};
