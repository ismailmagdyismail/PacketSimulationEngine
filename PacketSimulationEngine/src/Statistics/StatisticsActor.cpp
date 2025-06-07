#include "StatisticsActor.h"

StatisticsActor::StatisticsActor(std::shared_ptr<IChannel<std::shared_ptr<Packet>>> &p_pPacketsChannel)
    : Actor(std::bind(&StatisticsActor::ProcessPacket, this)), m_pPacketsChannel(p_pPacketsChannel)
{
}

GenerationStatistics StatisticsActor::GetStatistics()
{
    std::lock_guard<std::mutex> oLock{m_oStatisticsMutex};
    return m_oStatistics;
}

void StatisticsActor::ProcessPacket()
{
    std::shared_ptr<Packet> pPacket;
    bool bResult = m_pPacketsChannel->ReadValue(pPacket);
    //! Failed to read result from channels
    //! probably got closed
    if (!bResult)
    {
        return;
    }
    UpdateStatistics(pPacket);
}

void StatisticsActor::UpdateStatistics(std::shared_ptr<Packet> &p_pPacket)
{
    std::lock_guard<std::mutex> oLock{m_oStatisticsMutex};
    m_oStatistics.m_oPacketsTypeFrequency[p_pPacket->m_eProtocol]++;
    m_oStatistics.m_sPacketsCount++;
    //! TODO:
    // m_oStatistics.m_sTotalBytes += p_pPacket
}