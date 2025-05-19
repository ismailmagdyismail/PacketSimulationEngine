CXX := g++
CXXFLAGS := -Wall -Wextra -g -O0 -std=c++17 -MMD -MP

CONCURRENCY_LIB_PATH := $(ROOT_PATH)/Concurrency-Lib

THREAD_PATH := $(CONCURRENCY_LIB_PATH)/Thread
UnBufferedChannelPath := $(CONCURRENCY_LIB_PATH)/Channels/UnBufferedChannel
BufferedChannelPath := $(CONCURRENCY_LIB_PATH)/Channels/BufferedChannel

INCLUDES := -I$(CONCURRENCY_LIB_PATH)/Channels/ \
-I$(UnBufferedChannelPath)\
-I$(THREAD_PATH) \
-I$(BufferedChannelPath)
