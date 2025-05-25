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

/*
    - a an Actor For Counting statistics in a channel based way
    - actor is responsible for all Statistics operations
        - statistics  Counting
        - Statistics operations (start,stop,pause, Configurations)
    - this is a demonstration of how a channel can be used to control whole operations without lock whatsoever
    - a lock-based solution mixed with channels may be simpler solution , this is just for Demo purposes
*/

struct StatisticsActorOperationResult
{
    std::string m_strReplyMessage;
    bool bIsSuccess;
};

class ChannelBasedGenerationStatisticsActor
{
public:
    ChannelBasedGenerationStatisticsActor(std::shared_ptr<IChannel<std::shared_ptr<Packet>>> &p_pPacketsChannel);
    /*
     */
    //! Reply happens some time in the future , reply is put on the reply channel
    //! API is blocking, but executed internally in async manner, then blocked till reply is available
    //!  Blocking till Event loop consume, puts reply to
    bool Start(std::string &p_strReplyMessage);
    // bool Pause(std::string &p_strReplyMessage);
    bool Stop(std::string &p_strReplyMessage);
    GenerationStatistics GetStatistics();

    ~ChannelBasedGenerationStatisticsActor();

private:
    void OperationsHandler(std::function<bool(std::string &)> &p_oOperation);
    void StatisticsCounter(std::shared_ptr<Packet> &p_pPacket);
    void StopStatisticsCounting();
    void EventLoop();

    //! Channel to recieve Statistics on
    std::shared_ptr<IChannel<std::shared_ptr<Packet>>> m_pPacketsChannel;
    std::shared_ptr<UnBufferedChannel<std::function<bool(std::string &)>>> m_oOperationsChannel;
    std::shared_ptr<UnBufferedChannel<StatisticsActorOperationResult>> m_oOperationsRepliesChannel;
    ChannelSelector m_oChannelSelector;

    GenerationStatistics m_oStatistics;

    Thread m_oActorThread;
    bool m_bIsPaused{true};
    bool m_bIsTerminated{false};
};