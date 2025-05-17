//! Actor interface
#include "PacketGenerationActor.h"

//! Generator
#include "IPacketGenerator.h"

//! Messaging channels
#include "IChannel.h"

PacketGenerationActor::PacketGenerationActor(std::unique_ptr<IPacketGenerator> &&p_pPacketGenerator, std::shared_ptr<IChannel<Packet>> &p_pOutputChannel)
    : m_pPacketGenerator(std::move(p_pPacketGenerator)), m_pOutputChannel(p_pOutputChannel)
{
}

void PacketGenerationActor::Start()
{
    m_bIsRunning = true;
    m_oActorThread = std::thread(&PacketGenerationActor::Generate, this);

    //! TEMP till a custom RAII thread is added that joins by default in dest
    m_oActorThread.detach();
}

void PacketGenerationActor::Pause()
{
    //! this should stop the running thread
    //! thread will stop after it finishes the current it has on hand (if any)
    //! if a packet is produced it will put it on the output channel, then stop
    //! this has importatnt implication: if there is no consumers to read the values, this thread will block forever
    //! this is basic design of channels by defnition , but important to keep in mind
    m_bIsRunning = false;
}

void PacketGenerationActor::Generate()
{
    while (m_bIsRunning)
    {
        Packet oPacket = m_pPacketGenerator->GeneratePacket();
        m_pOutputChannel->SendValue(oPacket);
    }
}