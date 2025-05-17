#include "PacketGenerationActor.h"
#include "HTTPGenerator.h"
#include "UnBufferedChannel.h"
#include "IChannel.h"

#include <memory>
#include <fstream>
#include <iostream>

void ProducerThread(std::shared_ptr<IChannel<Packet>> channel, PacketGenerationActor &actor)
{
    int counter = 0;
    std::ofstream packetLog{"./PacketSink.log.txt"};
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
            std::cerr << "Pausing Generator \n";
            actor.Pause();
            std::cerr << "Restarting Generator\n";
            actor.Start();
        }
        if (counter == 40)
        {
            std::cerr << "Stopping Generator \n";
            actor.Pause();
            break;
        }
    }
}

int main()
{
    std::shared_ptr<IChannel<Packet>> channel = std::make_shared<UnBufferedChannel<Packet>>();
    PacketGenerationActor actor(std::make_unique<HTTPGenerator>(), channel);
    actor.Start();
    std::thread t(ProducerThread, channel, std::ref(actor));
    t.join();
    channel->Close();

    std::cerr << "Stopped\n";
}