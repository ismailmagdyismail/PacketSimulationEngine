CXX := g++
CXXFLAGS := -Wall -Wextra -g -O0 -std=c++17 -MMD -MP

CONCURRENCY_LIB_PATH := $(ROOT_PATH)/Concurrency-Lib

INCLUDES := -I$(CONCURRENCY_LIB_PATH)/Channels/ \
-I$(CONCURRENCY_LIB_PATH)/Channels/UnBufferedChannel \
