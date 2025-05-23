#pragma once

//! Threading
#include "Thread.h"
#include "ChannelSelector.h"

//! Statistics
#include "GenerationStatistics.h"

struct Packet;

template <typename T>
class BufferedChannel;
template <typename T>
class UnBufferedChannel;

class GenerationStatisticsActor
{
public:
    GenerationStatisticsActor(std::shared_ptr<IChannel<std::shared_ptr<Packet>>> &p_pPacketsChannel);
    bool Start(std::string &p_strReplyMessage);
    bool Stop(std::string &p_strReplyMessage);
    bool Pause(std::string &p_strReplyMessage);
    GenerationStatistics GetStatistics();

    ~GenerationStatisticsActor();

private:
    void Run();
    void UpdateStatistics(std::shared_ptr<Packet> &p_pPacket);

    std::shared_ptr<IChannel<std::shared_ptr<Packet>>> m_pPacketsChannel;

    GenerationStatistics m_oStatistics;

    Thread m_oActorThread;

    std::mutex m_oStatesMutex;
    bool m_bIsStarted{false};
    bool m_bIsTerminated{false};
};