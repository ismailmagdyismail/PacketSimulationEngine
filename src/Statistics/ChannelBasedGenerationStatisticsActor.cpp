#include "ChannelBasedGenerationStatisticsActor.h"

ChannelBasedGenerationStatisticsActor::ChannelBasedGenerationStatisticsActor(std::shared_ptr<IChannel<std::shared_ptr<Packet>>> &p_pPacketsChannel)
    : m_pPacketsChannel{p_pPacketsChannel}
{
    m_oActorThread.StartTask(std::bind(&ChannelBasedGenerationStatisticsActor::EventLoop, this));
    m_oOperationsRepliesChannel = std::make_shared<UnBufferedChannel<StatisticsActorOperationResult>>();
    m_oOperationsChannel = std::make_shared<UnBufferedChannel<std::function<bool(std::string &)>>>();

    //! Add Operations channel to listen to through selcector, and exectue schedudled operations
    auto fOperationsHandlerCallback = std::bind(&ChannelBasedGenerationStatisticsActor::OperationsHandler, this, std::placeholders::_1);
    m_oChannelSelector.AddChannel<std::function<bool(std::string &)>>(m_oOperationsChannel, fOperationsHandlerCallback);

    auto fStatisticsCounterCallback = std::bind(&ChannelBasedGenerationStatisticsActor::StatisticsCounter, this, std::placeholders::_1);
    m_oChannelSelector.AddChannel<std::shared_ptr<Packet>>(m_pPacketsChannel, fStatisticsCounterCallback);
}

void ChannelBasedGenerationStatisticsActor::OperationsHandler(std::function<bool(std::string &)> &p_oOperation)
{
    std::string strReplyMessage;
    bool bResult = p_oOperation(strReplyMessage);
    StatisticsActorOperationResult oOperationResult;
    oOperationResult.bIsSuccess = bResult;
    oOperationResult.m_strReplyMessage = strReplyMessage;
    m_oOperationsRepliesChannel->SendValue(std::move(oOperationResult));
}

void ChannelBasedGenerationStatisticsActor::StatisticsCounter(std::shared_ptr<Packet> &p_pPacket)
{
    m_oStatistics.m_oPacketsTypeFrequency[p_pPacket->m_eProtocol]++;
    m_oStatistics.m_sPacketsCount++;

    //! TODO:
    // m_oStatistics.m_sTotalBytes += p_pPacket
}

GenerationStatistics ChannelBasedGenerationStatisticsActor::GetStatistics()
{
    GenerationStatistics oStatistics;
    m_oOperationsChannel->SendValue([this, &oStatistics](std::string &)
                                    { oStatistics = m_oStatistics; return true; });
    StatisticsActorOperationResult oOperationResult;
    m_oOperationsRepliesChannel->ReadValue(oOperationResult);
    return oStatistics;
}

bool ChannelBasedGenerationStatisticsActor::Start(std::string &p_strReplyMessage)
{
    m_oOperationsChannel->SendValue([this](std::string &p_strReplyMessage)
                                    {
        if (m_bIsPaused)
        {
            p_strReplyMessage = "Statisitcs Is Already Counting!";
            return false;
        }
        m_bIsPaused = true;
        return true; });

    StatisticsActorOperationResult oOperationReply;
    m_oOperationsRepliesChannel->ReadValue(oOperationReply);
    p_strReplyMessage = oOperationReply.m_strReplyMessage;
    return oOperationReply.bIsSuccess;
}

bool ChannelBasedGenerationStatisticsActor::Stop(std::string &p_strReplyMessage)
{
    m_oOperationsChannel->SendValue([this](std::string &p_strReplyMessage)
                                    {
        if (m_bIsTerminated)
        {
            p_strReplyMessage = "Statisitcs Is Already Stopped!";
            return false;
        }
        StopStatisticsCounting();
        m_bIsTerminated = true;
        return true; });

    StatisticsActorOperationResult oOperationReply;
    m_oOperationsRepliesChannel->ReadValue(oOperationReply);
    p_strReplyMessage = oOperationReply.m_strReplyMessage;
    return oOperationReply.bIsSuccess;
}

/*
    - Need ChannelSelector.UnRegister to pause, stop listening / pulling from Packets Channel
*/
// bool ChannelBasedGenerationStatisticsActor::Pause(std::string &p_strReplyMessage)
// {
//     m_oOperationsChannel->SendValue([this](std::string &p_strReplyMessage)
//                                     {
//         if (m_bIsPaused)
//         {
//             p_strReplyMessage = "Statisitcs Is Already Paused!";
//             return false;
//         }
//         StopStatisticsCounting();
//         m_bIsPaused = true;
//         return true; });
//     StatisticsActorOperationResult oOperationReply;
//     m_oOperationsRepliesChannel->ReadValue(oOperationReply);

//     p_strReplyMessage = oOperationReply.m_strReplyMessage;
//     return oOperationReply.bIsSuccess;
// }

void ChannelBasedGenerationStatisticsActor::StopStatisticsCounting()
{
    //! TODO: UnRegister PacketChannel from Selector to stop popping data from the buffered channel
}

void ChannelBasedGenerationStatisticsActor::EventLoop()
{
    while (!m_bIsTerminated)
    {
        m_oChannelSelector.SelectAndExecute();
    }
}

ChannelBasedGenerationStatisticsActor::~ChannelBasedGenerationStatisticsActor()
{
    std::string reply;
    Stop(reply);
    // ! NOT Thread safe btw , since this is done main thread or another thread than the actor
    // m_bIsTerminated = false;
    // m_oChannelSelector.Close();
}