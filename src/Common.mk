CXX := g++
CXXFLAGS := -Wall -Wextra -g -O0 -std=c++17 -MMD -MP

CONCURRENCY_LIB_PATH := $(ROOT_PATH)/Concurrency-Lib
PACKET_SIMULATIONS_SRC := $(ROOT_PATH)/src

THREAD_PATH := $(CONCURRENCY_LIB_PATH)/Thread
UnBufferedChannelPath := $(CONCURRENCY_LIB_PATH)/Channels/UnBufferedChannel
BufferedChannelPath := $(CONCURRENCY_LIB_PATH)/Channels/BufferedChannel
ChannelSelectorPath := $(CONCURRENCY_LIB_PATH)/Channels/ChannelSelector
ACTORS_PATH = $(CONCURRENCY_LIB_PATH)/Actors/

PACKET_GENERATION_PATH = $(PACKET_SIMULATIONS_SRC)/PacketGeneration/
STATISTICS_COUNTER_PATH = $(PACKET_SIMULATIONS_SRC)/Statistics/


INCLUDES := -I$(CONCURRENCY_LIB_PATH)/Channels/ \
-I$(UnBufferedChannelPath)\
-I$(THREAD_PATH) \
-I$(BufferedChannelPath) \
-I$(ChannelSelectorPath) \
-I$(ACTORS_PATH) \
-I$(PACKET_GENERATION_PATH) \
-I$(STATISTICS_COUNTER_PATH) \
