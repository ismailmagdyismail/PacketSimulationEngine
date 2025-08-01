#include "PacketGenerationActor.h"
#include "HTTPGenerator.h"
#include "UnBufferedChannel.h"
#include "BufferedChannel.h"
#include "IChannel.h"
#include "ChannelSelector.h"
#include "ChannelBasedGenerationStatisticsActor.h"
#include "StatisticsActor.h"
#include "PacketSinkActor.h"
#include "PacketFilterActor.h"

#include <memory>
#include <fstream>
#include <iostream>
#include <set>

int main()
{
    std::shared_ptr<IChannel<std::shared_ptr<Packet>>> oGenerationDumpChannel = std::make_shared<BufferedChannel<std::shared_ptr<Packet>>>(10000);
    PacketGenerationActor generator1(std::make_unique<HTTPGenerator>(), oGenerationDumpChannel);
    PacketGenerationActor generator2(std::make_unique<HTTPGenerator>(), oGenerationDumpChannel);

    std::shared_ptr<IChannel<std::shared_ptr<Packet>>> oGenerationStatisticsChannel = std::make_shared<BufferedChannel<std::shared_ptr<Packet>>>(10000);
    StatisticsActor oGenerationStatisticsActor(oGenerationStatisticsChannel);

    std::shared_ptr<IChannel<std::shared_ptr<Packet>>> oFilteredStatisticsChannel = std::make_shared<BufferedChannel<std::shared_ptr<Packet>>>(10000);
    StatisticsActor oFilterdStatisticsActor(oFilteredStatisticsChannel);

    std::shared_ptr<IChannel<std::shared_ptr<Packet>>> oPacketFilterChannel = std::make_shared<BufferedChannel<std::shared_ptr<Packet>>>(10000);
    std::vector<std::shared_ptr<IChannel<std::shared_ptr<Packet>>>> vecFilteredPacketsOutputChannel{oFilteredStatisticsChannel};
    std::vector<std::shared_ptr<IChannel<std::shared_ptr<Packet>>>> vecForwardPacketsOutputChannel;
    PacketFilterActor oPacketFilterActor(oPacketFilterChannel, std::move(vecForwardPacketsOutputChannel), std::move(vecFilteredPacketsOutputChannel));

    std::vector<std::shared_ptr<IChannel<std::shared_ptr<Packet>>>> vecPacketSinkOutputChannels{oGenerationStatisticsChannel, oPacketFilterChannel};
    PacketSinkActor oPacketSinkActor(oGenerationDumpChannel, std::move(vecPacketSinkOutputChannels));

    oPacketFilterActor.Configure(
        {.m_eProtocolToFilter = Protocol::HTTP});

    generator1.Start();
    generator2.Start();
    oGenerationStatisticsActor.Start();
    oFilterdStatisticsActor.Start();
    oPacketFilterActor.Start();
    oPacketSinkActor.Start();

    std::set<int> generatorsRunning{1, 2};
    int originalSize = generatorsRunning.size();
    while (true)
    {
        std::cout << "Generators Running: \n";
        for (int generatorId : generatorsRunning)
        {
            std::cout << "-Stop-Generator::" << generatorId << '\n';
        }
        std::cout << originalSize + 1 << "-GetGenerationStatistics" << std::endl;
        std::cout << originalSize + 2 << "-GetFilteredStatistics" << std::endl;

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
            GenerationStatistics oStatistics = oGenerationStatisticsActor.GetStatistics();
            std::cerr << "Generated PacketsCount: " << oStatistics.m_sPacketsCount << std::endl;
        }
        else if (input == "4")
        {
            GenerationStatistics oStatistics = oFilterdStatisticsActor.GetStatistics();
            std::cerr << "Filtered PacketsCount: " << oStatistics.m_sPacketsCount << std::endl;
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
    oGenerationDumpChannel->Close();
    oGenerationStatisticsChannel->Close();
    oFilteredStatisticsChannel->Close();
    oPacketFilterChannel->Close();

    // std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    std::cerr << "Stopped\n";
}