#pragma once

//! system includes
#include <memory>
#include <thread>
#include <atomic>

//! Packet
#include "Packet.h"

//! Concurrency
#include "Thread.h"

template <typename T>
class IChannel;

//! Generation
class IPacketGenerator;

class PacketGenerationActor
{
public:
    PacketGenerationActor(std::unique_ptr<IPacketGenerator> &&p_pPacketGenerator, std::shared_ptr<IChannel<std::shared_ptr<Packet>>> &p_pOutputChannel);
    ~PacketGenerationActor();

    void Start();
    void Stop();
    void Pause();

private:
    void Generate();

    //! PIMPL
    std::unique_ptr<IPacketGenerator> m_pPacketGenerator;                //! Packet Generator
    std::shared_ptr<IChannel<std::shared_ptr<Packet>>> m_pOutputChannel; //! Channel to put generated packets on

    //! threading for actor
    Thread m_oActorThread;
    std::mutex m_oStateMutex;
    bool m_bIsGenerating{false};
};