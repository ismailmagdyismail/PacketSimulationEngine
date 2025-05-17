ROOT_PATH = ../
include ./Common.mk

SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)
DEPS := $(OBJS:.o=.d)

TARGET := PacketSimulation.exe

PACKET_GENERATION_PATH = ./PacketGeneration/

INCLUDES += -I$(PACKET_GENERATION_PATH) \

$(TARGET): $(OBJS) Packet-Generation Thread-Lib
	$(CXX) $(CXXFLAGS) $(OBJS) $(PACKET_GENERATION_PATH)/*.o $(THREAD_PATH)/*.o -o $@

Packet-Generation:
	make -j -C $(PACKET_GENERATION_PATH)

Thread-Lib:
	make -j -C $(THREAD_PATH)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf *.a *.o *.d $(TARGET)
	make clean -C $(PACKET_GENERATION_PATH)
	make clean -C $(THREAD_PATH)

-include $(DEPS)
