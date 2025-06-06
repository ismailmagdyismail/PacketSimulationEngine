#pragma once

//! Threading
#include "Thread.h"
#include "Actor.h"

//! Statistics
#include "GenerationStatistics.h"

struct Packet;

template <typename T>
class BufferedChannel;
template <typename T>
class UnBufferedChannel;

class GenerationStatisticsActor : public Actor
{
public:
    GenerationStatisticsActor(std::shared_ptr<IChannel<std::shared_ptr<Packet>>> &p_pPacketsChannel);

    GenerationStatistics GetStatistics();

private:
    void ProcessPacket();
    void UpdateStatistics(std::shared_ptr<Packet> &p_pPacket);

    std::shared_ptr<IChannel<std::shared_ptr<Packet>>> m_pPacketsChannel;

    std::mutex m_oStatisticsMutex;
    GenerationStatistics m_oStatistics;

    Thread m_oActorThread;
};