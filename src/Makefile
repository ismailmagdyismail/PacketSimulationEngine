ROOT_PATH = ../
include ./Common.mk

SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)
DEPS := $(OBJS:.o=.d)

TARGET := PacketSimulation.exe

$(TARGET): $(OBJS) Packet-Generation Thread-Lib Statistics-Counter Actors-Lib
	$(CXX) $(CXXFLAGS) $(OBJS) $(PACKET_GENERATION_PATH)/*.o $(STATISTICS_COUNTER_PATH)/*.o $(THREAD_PATH)/*.o $(ACTORS_PATH)/*.o -o $@

Packet-Generation:
	make -j -C $(PACKET_GENERATION_PATH)

Thread-Lib:
	make -j -C $(THREAD_PATH)

Actors-Lib:
	make -j -C $(ACTORS_PATH)

Statistics-Counter:
	make -j -C $(STATISTICS_COUNTER_PATH)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf *.a *.o *.d $(TARGET)
	make clean -C $(PACKET_GENERATION_PATH)
	make clean -C $(THREAD_PATH)
	make clean -C $(ACTORS_PATH)
	make clean -C $(STATISTICS_COUNTER_PATH)


-include $(DEPS)
