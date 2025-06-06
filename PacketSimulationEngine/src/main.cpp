#include "PacketGenerationActor.h"
#include "HTTPGenerator.h"
#include "UnBufferedChannel.h"
#include "BufferedChannel.h"
#include "IChannel.h"
#include "ChannelSelector.h"
#include "ChannelBasedGenerationStatisticsActor.h"
#include "GenerationStatisticsActor.h"
#include "PacketSinkActor.h"

#include <memory>
#include <fstream>
#include <iostream>
#include <set>

int main()
{
    std::shared_ptr<IChannel<std::shared_ptr<Packet>>> channel = std::make_shared<BufferedChannel<std::shared_ptr<Packet>>>(10000);
    PacketGenerationActor generator1(std::make_unique<HTTPGenerator>(), channel);
    PacketGenerationActor generator2(std::make_unique<HTTPGenerator>(), channel);

    std::shared_ptr<IChannel<std::shared_ptr<Packet>>> statisticsChannel = std::make_shared<BufferedChannel<std::shared_ptr<Packet>>>(10000);
    GenerationStatisticsActor statisticsActor(statisticsChannel);

    std::vector<std::shared_ptr<IChannel<std::shared_ptr<Packet>>>> vecOutputChannels{statisticsChannel};
    PacketSinkActor oPacketSinkActor(channel, std::move(vecOutputChannels));

    generator1.Start();
    generator2.Start();
    statisticsActor.Start();
    oPacketSinkActor.Start();

    std::set<int> generatorsRunning{1, 2};

    while (true)
    {
        std::cout << "Generators Running: \n";
        for (int generatorId : generatorsRunning)
        {
            std::cout << "-Stop-Generator::" << generatorId << '\n';
        }
        std::cout << "-GetStatistics" << std::endl;

        std::string input;
        std::cout << "Enter OperationId:";
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
        else if (input == "3")
        {
            GenerationStatistics oStatistics = statisticsActor.GetStatistics();
            std::cerr << "PacketsCount: " << oStatistics.m_sPacketsCount << std::endl;
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
    statisticsChannel->Close();

    // std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    std::cerr << "Stopped\n";
}