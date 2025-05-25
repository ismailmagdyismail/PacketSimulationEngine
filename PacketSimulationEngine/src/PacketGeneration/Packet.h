#pragma once

//! System includes
#include <string>
#include <chrono>

enum class Protocol
{
    HTTP,
    DNS,
    ICMP,
};

struct Packet
{
    std::string m_strSrcAddress;
    std::string m_strDestAddress;
    std::string m_strPayload;
    Protocol m_eProtocol;
    std::chrono::steady_clock::time_point m_chronoTimestamp;
};
