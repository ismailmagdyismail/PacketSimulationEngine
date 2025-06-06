ROOT_PATH = ../../
include $(ROOT_PATH)/PacketSimulationEngine/src/Common.mk

SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)
DEPS := $(OBJS:.o=.d)

TARGET := PacketSimulation.exe

all: $(TARGET)

$(TARGET): $(OBJS) Packet-Generation Statistics-Counter Concurrency-Lib Packet-Sink
	$(CXX) $(CXXFLAGS) $(OBJS) $(PACKET_GENERATION_PATH)/*.o $(STATISTICS_COUNTER_PATH)/*.o $(THREAD_PATH)/*.o $(ACTORS_PATH)/*.o $(PACKET_SINK_PATH)/*.o -o $@

Packet-Generation:
	make -j -C $(PACKET_GENERATION_PATH)

Concurrency-Lib:
	make -j -C $(CONCURRENCY_LIB_PATH)

Statistics-Counter:
	make -j -C $(STATISTICS_COUNTER_PATH)

Packet-Sink:
	make -j -C $(PACKET_SINK_PATH)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf *.a *.o *.d $(TARGET)
	make clean -C $(CONCURRENCY_LIB_PATH)
	make clean -C $(PACKET_GENERATION_PATH)
	make clean -C $(STATISTICS_COUNTER_PATH)


-include $(DEPS)
