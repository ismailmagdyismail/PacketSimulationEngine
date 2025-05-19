# Diagram

```
                                                                        +--------------------------+
 +----------------------+                                               | FilteredPacketsStatistics|
 |PacketGeneratorSource1| ----->                                        +--------------------------+
 +----------------------+                                                           ^
 +----------------------+                                                           |
 |PacketGeneratorSource2| ----->                                                    |
 +----------------------+                                                           |
 +----------------------+        +----------------------+        +----------+       |     +--------------+        +------------+
 | PacketGeneratorSource3|-----> | Classifier/PacketSink|----->  | Filter   | ----------> | LoadBalancer | -----> | OutputPort |
 +----------------------+        +----------------------+        +----------+             +--------------+        +------------+
            |                               |                       |                           |                       |
            |                               |                       |                           |                       |
            +-------------------------------+-----------------------+---------------------------+-----------------------+
                                     |                                         |
                                     v                                         v
                            +-----------------+                      +-------------------+
                            | StatsCollector  |                      | Logging Component |
                            +-----------------+                      +-------------------+

```

# Functional Requirements

## PacketGenerator

- Spawn N instances (e.g., 3) each simulating an input port.

- Generate packets with:

  - Random source/destination IPs (e.g., "192.168.1.x")

  - Random protocol from predefined set (HTTP, DNS, ICMP)

  - Random packet size between 64-1500 bytes

- Generate packets at random intervals (e.g., 10–100ms)

- Can be Pause or Stopped All together !

- Allow Changing Configuration At Run-Time for specific Generator (via File OR UI)

## Scheduler

## Classifier

- Receives packets from multiple generators.

- Classifies packets by protocol.

- Forwards packets downstream.

## Filter

- Receives packets from Classifier.

- Applies rules:

- E.g., drop all packets with protocol "ICMP".

- Passes accepted packets downstream.

- Sends dropped packets stats to StatsCollector.

## LoadBalancer

- Receives filtered packets.

- Distributes packets evenly to M output ports (e.g., 2).

- Balances load in round-robin or weighted fashion.

## OutputPort

- Receives packets from LoadBalancer.

- Simulates sending packets (e.g., logs packet info).

- Can simulate output delay (e.g., sleep 1–10ms per packet).

## StatsCollector

- Receives notifications about packets:

- Received

- Dropped

- Forwarded

- Every second, prints or logs current statistics.
