#include "StatisticsActor.h"

GenerationStatisticsActor::GenerationStatisticsActor(std::shared_ptr<IChannel<std::shared_ptr<Packet>>> &p_pPacketsChannel)
    : m_pPacketsChannel(p_pPacketsChannel)
{
}

bool GenerationStatisticsActor::Start(std::string &p_strReplyMessage)
{
    std::lock_guard<std::mutex> oLock{m_oStatesMutex};
    if (m_bIsTerminated)
    {
        p_strReplyMessage = "Statistics Counter has been Terminated, Cannot be Restarted";
        return false;
    }
    if (m_bIsStarted)
    {
        p_strReplyMessage = "Statistics Counter Is Already Started";
        return false;
    }
    m_bIsStarted = true;
    m_oActorThread.StartTask(std::bind(&GenerationStatisticsActor::Run, this));
    return true;
}

bool GenerationStatisticsActor::Stop(std::string &p_strReplyMessage)
{
    std::lock_guard<std::mutex> oLock{m_oStatesMutex};
    if (m_bIsTerminated)
    {
        p_strReplyMessage = "Statistics Counter Is Already Terminated";
        return false;
    }
    m_bIsStarted = false;
    m_bIsTerminated = true;
    return true;
}

bool GenerationStatisticsActor::Pause(std::string &p_strReplyMessage)
{
    std::lock_guard<std::mutex> oLock{m_oStatesMutex};
    if (m_bIsTerminated)
    {
        p_strReplyMessage = "Statistics Counter has been Terminated, Cannot be Paused";
        return false;
    }
    if (!m_bIsStarted)
    {
        p_strReplyMessage = "Statistics Counter Is Already Paused";
        return false;
    }
    m_bIsStarted = true;
    return true;
}

GenerationStatistics GenerationStatisticsActor::GetStatistics()
{
    std::lock_guard<std::mutex> oLock{m_oStatisticsMutex};
    return m_oStatistics;
}

void GenerationStatisticsActor::Run()
{
    while (m_bIsStarted)
    {
        std::shared_ptr<Packet> pPacket;
        bool bResult = m_pPacketsChannel->ReadValue(pPacket);
        //! Failed to read result from channels
        //! probably got closed
        if (!bResult)
        {
            continue;
        }
        UpdateStatistics(pPacket);
    }
}

void GenerationStatisticsActor::UpdateStatistics(std::shared_ptr<Packet> &p_pPacket)
{
    std::lock_guard<std::mutex> oLock{m_oStatisticsMutex};
    m_oStatistics.m_oPacketsTypeFrequency[p_pPacket->m_eProtocol]++;
    m_oStatistics.m_sPacketsCount++;
    //! TODO:
    // m_oStatistics.m_sTotalBytes += p_pPacket
}

GenerationStatisticsActor::~GenerationStatisticsActor()
{
    std::string strReply;
    Stop(strReply);
}