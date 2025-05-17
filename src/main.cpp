#include "PacketGenerationActor.h"
#include "HTTPGenerator.h"
#include "UnBufferedChannel.h"
#include "IChannel.h"

#include <memory>
#include <fstream>
#include <iostream>

int main()
{
    std::shared_ptr<IChannel<Packet>> channel = std::make_shared<UnBufferedChannel<Packet>>();
    PacketGenerationActor actor(std::make_unique<HTTPGenerator>(), channel);
    std::ofstream packetLog{"./PacketSink.log.txt"};
    actor.Start();
    int counter = 0;

    packetLog << "TimeStamp, srcAddress" << std::endl;

    while (true)
    {
        Packet oPacket{};
        bool bResult = channel->ReadValue(oPacket);
        if (!bResult)
        {
            packetLog << "Channel stopped\n";
            break;
        }
        packetLog << std::chrono::duration_cast<std::chrono::milliseconds>(oPacket.m_chronoTimestamp.time_since_epoch()).count() << ",";
        packetLog << oPacket.m_strSrcAddress << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        counter++;
        //! Generate 20 packets for testing
        if (counter == 20)
        {
            std::cerr << "Stopping Generator\n";
            actor.Pause();
            channel->Close();
            break;
        }
    }
    std::cerr << "Stopped\n";
}