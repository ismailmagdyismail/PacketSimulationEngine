#include "GenerationStatisticsActor.h"

GenerationStatisticsActor::GenerationStatisticsActor(std::shared_ptr<IChannel<std::shared_ptr<Packet>>> &p_pPacketsChannel)
    : Actor(std::bind(&GenerationStatisticsActor::ProcessPacket, this)), m_pPacketsChannel(p_pPacketsChannel)
{
}

GenerationStatistics GenerationStatisticsActor::GetStatistics()
{
    std::lock_guard<std::mutex> oLock{m_oStatisticsMutex};
    return m_oStatistics;
}

void GenerationStatisticsActor::ProcessPacket()
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

void GenerationStatisticsActor::UpdateStatistics(std::shared_ptr<Packet> &p_pPacket)
{
    std::lock_guard<std::mutex> oLock{m_oStatisticsMutex};
    m_oStatistics.m_oPacketsTypeFrequency[p_pPacket->m_eProtocol]++;
    m_oStatistics.m_sPacketsCount++;
    //! TODO:
    // m_oStatistics.m_sTotalBytes += p_pPacket
}