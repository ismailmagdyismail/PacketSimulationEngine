#include "PacketGenerationActor.h"
#include "HTTPGenerator.h"
#include "UnBufferedChannel.h"
#include "BufferedChannel.h"
#include "IChannel.h"

#include <memory>
#include <fstream>
#include <iostream>
#include <set>

void ConsumerThread(std::shared_ptr<IChannel<Packet>> channel)
{
    std::ofstream packetLog{"./PacketSink.log.txt"};
    packetLog << "TimeStamp, srcAddress" << std::endl;

    while (true)
    {
        Packet oPacket{};
        bool bResult = channel->ReadValue(oPacket);
        if (!bResult)
        {
            break;
        }
        packetLog << std::chrono::duration_cast<std::chrono::milliseconds>(oPacket.m_chronoTimestamp.time_since_epoch()).count() << ",";
        packetLog << oPacket.m_strSrcAddress << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main()
{
    std::shared_ptr<IChannel<Packet>> channel = std::make_shared<BufferedChannel<Packet>>(10000);
    PacketGenerationActor generator1(std::make_unique<HTTPGenerator>(), channel);
    PacketGenerationActor generator2(std::make_unique<HTTPGenerator>(), channel);

    generator1.Start();
    generator2.Start();

    std::thread t(ConsumerThread, channel);

    std::set<int> generatorsRunning{1, 2};

    while (true)
    {
        std::cout << "Generators Running: \n";
        for (int generatorId : generatorsRunning)
        {
            std::cout << "GeneratorID::" << generatorId << '\n';
        }
        std::string input;
        std::cout << "Enter ID:";
        std::cin >> input;
        if (input == "1")
        {
            std::cerr << "Stopping Generator" << input << std::endl;
            generator1.Pause();
            generatorsRunning.erase(1);
        }
        else if (input == "2")
        {
            std::cerr << "Stopping Generator" << input << std::endl;
            generator2.Pause();
            generatorsRunning.erase(2);
        }
        else
        {
            std::cerr << "Invalid ID\n";
        }
        if (generatorsRunning.empty())
        {
            std::cerr << "All Generators Stopped\n";
            break;
        }
    }

    // std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    // generator1.Pause();
    // generator2.Pause();
    // generator1.Stop();
    // generator2.Stop();

    std::cerr << "All Generators Terminated\n";
    std::cerr << "Closing Chanels...\n";
    channel->Close();
    t.join();

    std::cerr << "Stopped\n";
}