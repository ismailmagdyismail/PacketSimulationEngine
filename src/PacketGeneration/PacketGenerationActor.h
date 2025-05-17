#pragma once

//! system includes
#include <memory>
#include <thread>
#include <atomic>

//! Packet
#include "Packet.h"

class IPacketGenerator;

template <typename T>
class IChannel;

class PacketGenerationActor
{
public:
    PacketGenerationActor(std::unique_ptr<IPacketGenerator> &&p_pPacketGenerator, std::shared_ptr<IChannel<Packet>> &p_pOutputChannel);

    void Start();
    void Pause();

private:
    void Generate();

    //! PIMPL
    std::unique_ptr<IPacketGenerator> m_pPacketGenerator; //! Packet Generator
    std::shared_ptr<IChannel<Packet>> m_pOutputChannel;   //! Channel to put generated packets on

    //! threading for actor
    std::thread m_oActorThread;
    std::atomic<bool> m_bIsRunning{false};
};