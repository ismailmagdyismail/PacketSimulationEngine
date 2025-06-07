#pragma once

#include <map>

#include "Packet.h"

struct GenerationStatistics
{
    std::size_t m_sPacketsCount;
    std::map<Protocol, std::size_t> m_oPacketsTypeFrequency;
    std::size_t m_sTotalBytes;
};