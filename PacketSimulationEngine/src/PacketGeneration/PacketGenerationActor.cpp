//! Actor interface
#include "PacketGenerationActor.h"

//! Generator
#include "IPacketGenerator.h"

//! Messaging channels
#include "IChannel.h"

PacketGenerationActor::PacketGenerationActor(std::unique_ptr<IPacketGenerator> &&p_pPacketGenerator, std::shared_ptr<IChannel<Packet>> &p_pOutputChannel)
    : m_pPacketGenerator(std::move(p_pPacketGenerator)),
      m_pOutputChannel(p_pOutputChannel)
{
}

PacketGenerationActor::~PacketGenerationActor()
{
    Stop();
}

void PacketGenerationActor::Stop()
{
    std::lock_guard<std::mutex> oLock{m_oStateMutex};
    Pause();
    m_oActorThread.Stop();
}

void PacketGenerationActor::Start()
{
    m_bIsGenerating = true;
    m_oActorThread.StartTask(std::bind(&PacketGenerationActor::Generate, this));
}

void PacketGenerationActor::Pause()
{
    //! this should pause the running thread
    //! thread will pause after it finishes the current it has on hand (if any)
    //! if a packet is produced it will put it on the output channel, then stop
    //! this has importatnt implication: if there is no consumers to read the values, this thread will block forever
    //! this is basic design of channels by defnition , but important to keep in mind
    m_bIsGenerating = false;
}

void PacketGenerationActor::Generate()
{
    while (m_bIsGenerating)
    {
        Packet oPacket = m_pPacketGenerator->GeneratePacket();
        m_pOutputChannel->SendValue(std::move(oPacket));
    }
}