# HFT DSA + C++ Systems Training Plan

**Owner:** Chris Tague  
**Created:** Tuesday, May 26, 2026  
**Role start date:** Monday, August 3, 2026  
**Pre-start sprint length:** 69 days / almost 10 weeks  
**Primary goal:** Become dramatically stronger at data structures, algorithms, C++, and performance-oriented systems work before starting an entry-level HFT engineering role.  
**Secondary goal:** Build the habits, vocabulary, and technical instincts to ramp quickly on market data, packet capture, benchmarking, Linux networking, storage formats, and dev tooling once on the team.

---

## 0. How to Use This Document

Treat this as a living checklist, not a rigid syllabus.

Every week:

- [ ] Update the current week’s goals.
- [ ] Check off completed tasks.
- [ ] Move unfinished tasks forward intentionally instead of letting them disappear.
- [ ] Add links to solved problems, notes, benchmarks, snippets, and questions.
- [ ] Keep a running list of “things I heard at work but do not understand yet.”
- [ ] Review the miss log every Sunday.

Every DSA problem should produce a tiny note:

```md
### Problem Name
- Pattern:
- Brute force:
- Optimal approach:
- Key invariant:
- Complexity:
- C++ bug risk:
- Edge cases:
- Redo date:
```

Every systems topic should produce a tiny note:

```md
### Topic Name
- What it is:
- Why it matters in HFT:
- What I can explain now:
- What I still do not understand:
- One experiment I ran:
- Links / docs:
```

---

## 1. Parsed Notes From Boss Conversation

Raw notes included several relevant themes. Some phrases are probably internal names or typos, so these should be treated as hypotheses until confirmed.

### Core Role Signal

- [ ] **Primary language is C++.**
  - DSA prep should be done in modern C++, not Python.
  - Emphasize STL fluency, memory/layout awareness, move semantics, RAII, ownership, profiling, and clean low-level debugging.

- [ ] **Lots of packet capture work.**
  - Learn how packets move from NIC → kernel/userspace → application.
  - Learn why packet loss/drops happen.
  - Learn buffering, batching, polling, interrupts, ring buffers, and backpressure.

- [ ] **Networking and market data protocols matter.**
  - Learn TCP/UDP basics deeply.
  - Learn multicast basics.
  - Learn binary message parsing.
  - Learn protocol descriptors and generated parsers.

- [ ] **Minimal drops are important.**
  - This implies low latency, high throughput, bounded allocation, careful buffering, CPU affinity, NIC queues, and benchmarking.

- [ ] **Kernel bypass is relevant.**
  - Study DPDK, AF_XDP/XDP, PF_RING/netmap conceptually.
  - Do not try to become a DPDK expert before day one, but understand the mental model.

- [ ] **Distributed file system / file system distinction: HTFS vs SEF.**
  - These may be internal systems or team-specific names.
  - Learn filesystem/distributed storage concepts: write throughput, append-only storage, block sizes, compression, replication, partitioning, metadata, and failure modes.

- [ ] **Apache Spark is involved.**
  - Learn where Spark fits: batch analytics over large datasets, often reading Parquet.
  - Do not prioritize Spark over C++/packet capture, but know enough to understand pipeline architecture.

- [ ] **Parquet file format is involved.**
  - The note said “parque”; assume Apache Parquet unless corrected.
  - Learn columnar storage, row groups, compression, encoding, schema evolution, and why Parquet is useful for analytics.

- [ ] **InfluxDB is involved.**
  - Likely used for metrics/time-series observability.
  - Learn tags, fields, measurements, time-series queries, retention, and dashboards.

- [ ] **GitLab is involved.**
  - Learn GitLab CI/CD basics.
  - Be comfortable reading pipelines, artifacts, runners, merge request workflows.

- [ ] **YAML protocol descriptor → Python script → generated C++ parser/callback code.**
  - This is extremely relevant.
  - Learn code generation patterns, binary parsing, and generated C++ design.

- [ ] **Benchmarking Linux versions / new Linux stuff.**
  - Learn benchmarking methodology.
  - Learn CPU pinning, perf, flamegraphs, cache misses, context switches, syscalls, page faults.

- [ ] **10 gigabit exchange connections.**
  - Means each exchange connection can theoretically carry up to 10 Gbit/s of network traffic.
  - Rough math: 10 Gbit/s = 1.25 GB/s raw line rate before overhead.
  - Per-packet overhead matters at high packet rates.

- [ ] **FPGA NIC / SmartNIC.**
  - The note said “FPGA nik”; likely “FPGA NIC.”
  - Learn the concept: network interface hardware with programmable logic that can offload filtering, timestamping, parsing, or other latency-sensitive tasks.
  - Do not over-study FPGA development unless your team confirms you will touch it directly.

- [ ] **Cerebro / Sereibro system.**
  - This may be an internal system name.
  - First-week question: “Is Cerebro/Sereibro our packet capture/market data storage system, or something else?”

- [ ] **True Blue / Old Post Office.**
  - Probably office/location/culture related, not technical.
  - No prep needed unless it refers to an internal system.

### Likely Role Shape

The role sounds less like “write trading strategies immediately” and more like:

- [ ] Market data infrastructure.
- [ ] Packet capture and replay.
- [ ] Low-level C++ tooling.
- [ ] Protocol decoding.
- [ ] High-throughput storage.
- [ ] Benchmarking Linux/networking/storage changes.
- [ ] Developer tooling around data pipelines.
- [ ] Working with large volumes of historical market data.

### Prep Implication

The best prep stack is:

1. [ ] DSA and algorithmic problem solving.
2. [ ] Modern C++ and STL mastery.
3. [ ] Linux performance and profiling.
4. [ ] Networking fundamentals.
5. [ ] Packet capture fundamentals.
6. [ ] Storage/data formats: Parquet, compression, Spark basics.
7. [ ] Code generation and binary protocol parsing.
8. [ ] Benchmarking discipline.
9. [ ] Professional ramp-up habits: ask good questions, write notes, ship small improvements quickly.

---

## 2. North Star Outcomes by August 3

### DSA / Algorithms

- [ ] I can solve most LeetCode easy problems in 5–10 minutes.
- [ ] I can solve common medium problems in 20–35 minutes.
- [ ] I can usually identify the correct pattern within 5 minutes.
- [ ] I can explain brute force, optimized approach, invariant, and complexity clearly.
- [ ] I can implement solutions in C++ without relying on Python-like shortcuts.
- [ ] I have redone my misses instead of just reading solutions.

### C++

- [ ] I can comfortably use `vector`, `array`, `deque`, `queue`, `stack`, `priority_queue`, `set`, `multiset`, `map`, `unordered_map`, and `unordered_set`.
- [ ] I can write custom comparators correctly.
- [ ] I understand iterator invalidation rules at a practical level.
- [ ] I know when `vector` is better than linked structures due to cache locality.
- [ ] I can explain RAII, move semantics, references, const correctness, and ownership.
- [ ] I can write small benchmarks and avoid obvious benchmarking traps.

### Linux / Systems

- [ ] I can explain the rough lifecycle of a packet entering a Linux machine.
- [ ] I can explain why kernel bypass exists.
- [ ] I can explain what a NIC is, what a SmartNIC/FPGA NIC might do, and why HFT cares.
- [ ] I can use `tcpdump`/Wireshark conceptually and understand PCAP files.
- [ ] I can explain packet drops and list common causes.
- [ ] I can use basic profiling tools: `perf`, `time`, `strace`, maybe flamegraphs.

### Data / Storage

- [ ] I can explain why Parquet is columnar and why that matters.
- [ ] I can explain how Spark might be used to process market data archives.
- [ ] I can explain the difference between operational metrics storage and historical analytical storage.
- [ ] I can explain why InfluxDB is useful for time-series metrics.

### On-the-Job Readiness

- [ ] I have a prepared question list for week one.
- [ ] I have a personal glossary of team terms.
- [ ] I have a daily note-taking system.
- [ ] I can onboard without pretending to know internal systems I have never seen.
- [ ] I can contribute early by improving docs, tests, benchmarks, tooling, or small bugs.

---

## 3. Weekly Time Budget Options

### Minimum Viable Week: 6 hours

- [ ] 3 hours DSA.
- [ ] 1.5 hours C++.
- [ ] 1 hour systems/networking.
- [ ] 0.5 hour review/logging.

### Strong Week: 10 hours

- [ ] 5 hours DSA.
- [ ] 2 hours C++.
- [ ] 2 hours systems/networking/storage.
- [ ] 1 hour review/redos.

### Beast Week: 15 hours

- [ ] 7 hours DSA.
- [ ] 3 hours C++.
- [ ] 3 hours systems/networking/storage.
- [ ] 1 hour project/benchmarking.
- [ ] 1 hour review/redos.

Consistency beats occasional heroic weeks.

---

## 4. Daily Routine Templates

### Standard Weekday Session: 75–90 minutes

- [ ] 5 min: Review yesterday’s miss log.
- [ ] 10 min: Study one pattern or C++ concept.
- [ ] 45–60 min: Solve 1–2 problems in C++.
- [ ] 10 min: Write notes.
- [ ] 5 min: Schedule redo if missed.

### Systems Session: 60–90 minutes

- [ ] 15 min: Read docs/article/chapter.
- [ ] 30–45 min: Run a small experiment.
- [ ] 15 min: Write “what I understand now.”
- [ ] 5 min: Add open questions.

### Sunday Review: 60 minutes

- [ ] Count solved problems.
- [ ] Count redone problems.
- [ ] Review misses by category.
- [ ] Pick next week’s top 3 weaknesses.
- [ ] Clean up notes.
- [ ] Update confidence ratings.

---

## 5. Miss Log

```md
| Date | Problem/Topic | Miss Type | What Happened | Fix | Redo Date | Redone? |
|---|---|---|---|---|---|---|
| | | Pattern miss / invariant miss / C++ bug / complexity / rushed reading / edge case / systems gap | | | | |
```

Common miss types:

- [ ] Pattern miss.
- [ ] Wrong invariant.
- [ ] Overcomplicated solution.
- [ ] Knew approach but could not implement.
- [ ] Off-by-one.
- [ ] Iterator invalidation.
- [ ] Bad comparator.
- [ ] Integer overflow.
- [ ] Used wrong data structure.
- [ ] Did not test empty/small input.
- [ ] Did not know C++ STL function.
- [ ] Performance misconception.

---

## 6. Pre-Start Sprint Overview

### Dates

- [ ] Week 1: May 26 – June 1
- [ ] Week 2: June 2 – June 8
- [ ] Week 3: June 9 – June 15
- [ ] Week 4: June 16 – June 22
- [ ] Week 5: June 23 – June 29
- [ ] Week 6: June 30 – July 6
- [ ] Week 7: July 7 – July 13
- [ ] Week 8: July 14 – July 20
- [ ] Week 9: July 21 – July 27
- [ ] Week 10: July 28 – August 2
- [ ] Start date: August 3

### Phase Breakdown

- [ ] Weeks 1–2: Core DSA refresh + C++ STL fluency.
- [ ] Weeks 3–4: Trees, graphs, heaps, intervals, ordered structures.
- [ ] Weeks 5–6: Dynamic programming, greedy, binary search on answer, monotonic structures.
- [ ] Weeks 7–8: HFT systems track: networking, packet capture, Linux performance, Parquet/Spark.
- [ ] Weeks 9–10: Mixed timed practice + capstone mini-project + week-one readiness.

---

# Phase 1: Core DSA Refresh + C++ STL

## Week 1: Arrays, Strings, Hashing, Sorting, Two Pointers

**Dates:** May 26 – June 1  
**Theme:** Rebuild speed and confidence.

### DSA Concepts

- [ ] Arrays and strings.
- [ ] Hash maps and hash sets.
- [ ] Sorting.
- [ ] Custom comparators.
- [ ] Two pointers.
- [ ] Basic prefix sums.
- [ ] Complexity review.

### C++ Concepts

- [ ] `vector` basics and performance.
- [ ] `string` operations.
- [ ] `unordered_map` and `unordered_set`.
- [ ] `sort`, `stable_sort`, custom lambdas.
- [ ] Range-based loops vs indexed loops.
- [ ] Passing by reference vs value.
- [ ] `const` correctness basics.

### Problem Checklist

- [ ] Two Sum
- [ ] Valid Anagram
- [ ] Group Anagrams
- [ ] Top K Frequent Elements
- [ ] Product of Array Except Self
- [ ] Valid Palindrome
- [ ] 3Sum
- [ ] Container With Most Water
- [ ] Merge Intervals
- [ ] Insert Interval
- [ ] Longest Consecutive Sequence
- [ ] Sort Colors

### Systems Mini-Topic

- [ ] Learn what a NIC is.
- [ ] Learn what 10GbE means.
- [ ] Write down rough bandwidth conversions:
  - [ ] 10 Gbit/s = 1.25 GB/s raw.
  - [ ] 1 Gbit/s = 125 MB/s raw.
  - [ ] Network overhead reduces useful payload throughput.

### Deliverables

- [ ] 10+ solved problems.
- [ ] At least 3 redos scheduled.
- [ ] One note: “What does 10GbE mean and why does packet rate matter?”
- [ ] One C++ snippet file containing common STL patterns.

---

## Week 2: Sliding Window, Binary Search, Stack/Queue

**Dates:** June 2 – June 8  
**Theme:** Nail the most common interview patterns.

### DSA Concepts

- [ ] Fixed-size sliding window.
- [ ] Variable-size sliding window.
- [ ] Binary search over index.
- [ ] Binary search over answer.
- [ ] Stack patterns.
- [ ] Queue/deque basics.

### C++ Concepts

- [ ] `deque`.
- [ ] `stack` and `queue` adapters.
- [ ] `lower_bound` and `upper_bound`.
- [ ] Binary search implementation without overflow.
- [ ] `size_t` vs `int` risks.
- [ ] Avoiding signed/unsigned bugs.

### Problem Checklist

- [ ] Best Time to Buy and Sell Stock
- [ ] Longest Substring Without Repeating Characters
- [ ] Longest Repeating Character Replacement
- [ ] Minimum Window Substring
- [ ] Valid Parentheses
- [ ] Min Stack
- [ ] Evaluate Reverse Polish Notation
- [ ] Daily Temperatures
- [ ] Search in Rotated Sorted Array
- [ ] Find Minimum in Rotated Sorted Array
- [ ] Koko Eating Bananas
- [ ] Capacity To Ship Packages Within D Days

### Systems Mini-Topic

- [ ] Learn what a packet is: Ethernet/IP/TCP/UDP at a high level.
- [ ] Learn the difference between TCP and UDP.
- [ ] Learn why market data often uses UDP multicast.
- [ ] Install or review basic tools:
  - [ ] `tcpdump`
  - [ ] Wireshark
  - [ ] `ping`
  - [ ] `traceroute` / `tracepath`
  - [ ] `ss`
  - [ ] `ip addr`

### Deliverables

- [ ] 10+ solved problems.
- [ ] One written explanation of sliding window invariants.
- [ ] One written explanation of binary search on answer.
- [ ] One note: “TCP vs UDP vs multicast in market data.”

---

# Phase 2: Core Interview Mastery

## Week 3: Trees, Recursion, DFS/BFS

**Dates:** June 9 – June 15  
**Theme:** Get very good at recursive return values and tree invariants.

### Concepts

- [ ] Recursive DFS.
- [ ] Iterative DFS.
- [ ] BFS.
- [ ] Tree height/depth.
- [ ] Subtree return values.
- [ ] BST properties.
- [ ] LCA.
- [ ] Serialization.

### Problem Checklist

- [ ] Invert Binary Tree
- [ ] Maximum Depth of Binary Tree
- [ ] Same Tree
- [ ] Subtree of Another Tree
- [ ] Lowest Common Ancestor of BST
- [ ] Validate BST
- [ ] Kth Smallest Element in a BST
- [ ] Binary Tree Level Order Traversal
- [ ] Binary Tree Right Side View
- [ ] Count Good Nodes in Binary Tree
- [ ] Diameter of Binary Tree
- [ ] Balanced Binary Tree
- [ ] Binary Tree Maximum Path Sum
- [ ] Serialize and Deserialize Binary Tree

### Systems Mini-Topic

- [ ] Learn Linux process/thread concepts.
- [ ] Learn context switch basics.
- [ ] Learn syscall basics.
- [ ] Use `strace` on a simple program.
- [ ] Use `/usr/bin/time -v` on a simple program.

### Deliverables

- [ ] 10+ tree problems.
- [ ] One note: “How to design recursive return values.”
- [ ] One note: “What is a syscall and why can it matter for low-latency systems?”

---

## Week 4: Graphs, Heaps, Intervals, Ordered Structures

**Dates:** June 16 – June 22  
**Theme:** Dynamic state, event ordering, and graph discipline.

### Concepts

- [ ] Graph representation.
- [ ] DFS/BFS over graph.
- [ ] Grid BFS.
- [ ] Topological sort.
- [ ] Union-find.
- [ ] Dijkstra.
- [ ] Heap patterns.
- [ ] Ordered set/map patterns.
- [ ] Sweep line basics.

### Problem Checklist

- [ ] Number of Islands
- [ ] Clone Graph
- [ ] Rotting Oranges
- [ ] Course Schedule
- [ ] Course Schedule II
- [ ] Pacific Atlantic Water Flow
- [ ] Network Delay Time
- [ ] Redundant Connection
- [ ] Accounts Merge
- [ ] K Closest Points to Origin
- [ ] Find Median from Data Stream
- [ ] Meeting Rooms II
- [ ] Non-overlapping Intervals
- [ ] Task Scheduler
- [ ] Sliding Window Maximum

### Systems Mini-Topic

- [ ] Learn PCAP basics.
- [ ] Capture a small local packet trace with `tcpdump` if possible.
- [ ] Open a PCAP in Wireshark.
- [ ] Identify Ethernet/IP/TCP/UDP layers in one packet.
- [ ] Write down what packet capture means in your own words.

### Deliverables

- [ ] 12+ solved problems.
- [ ] One reusable C++ union-find implementation.
- [ ] One reusable C++ Dijkstra implementation.
- [ ] One note: “What is a PCAP file?”

---

# Phase 3: Harder Algorithmic Thinking

## Week 5: Dynamic Programming I + Greedy I

**Dates:** June 23 – June 29  
**Theme:** Stop fearing DP; learn to define state.

### DSA Concepts

- [ ] 1D DP.
- [ ] 2D DP.
- [ ] Memoization.
- [ ] Tabulation.
- [ ] State definition.
- [ ] Base cases.
- [ ] Memory compression.
- [ ] Greedy exchange arguments.

### Problem Checklist

- [ ] Climbing Stairs
- [ ] House Robber
- [ ] House Robber II
- [ ] Coin Change
- [ ] Longest Increasing Subsequence
- [ ] Longest Common Subsequence
- [ ] Unique Paths
- [ ] Minimum Path Sum
- [ ] Decode Ways
- [ ] Word Break
- [ ] Jump Game
- [ ] Jump Game II
- [ ] Gas Station
- [ ] Partition Labels
- [ ] Minimum Number of Arrows to Burst Balloons

### Systems Mini-Topic

- [ ] Learn what kernel bypass means conceptually.
- [ ] Learn why normal kernel networking can be too expensive at high packet rates.
- [ ] Learn the basic mental models of DPDK, XDP/AF_XDP, PF_RING, and netmap.

### Deliverables

- [ ] 10+ DP/greedy problems.
- [ ] One note: “How I define DP state.”
- [ ] One note: “What is kernel bypass?”

---

## Week 6: Monotonic Structures, Tries, Advanced Binary Search, DP II

**Dates:** June 30 – July 6  
**Theme:** Learn the patterns that feel magical until they become obvious.

### DSA Concepts

- [ ] Monotonic stack.
- [ ] Monotonic queue.
- [ ] Trie.
- [ ] Binary search on answer.
- [ ] Stock DP.
- [ ] Backtracking basics.

### Problem Checklist

- [ ] Trapping Rain Water
- [ ] Largest Rectangle in Histogram
- [ ] Sliding Window Maximum redo
- [ ] Implement Trie
- [ ] Word Search
- [ ] Word Search II
- [ ] Maximum XOR of Two Numbers in an Array
- [ ] Split Array Largest Sum
- [ ] Median of Two Sorted Arrays
- [ ] Best Time to Buy and Sell Stock II
- [ ] Best Time to Buy and Sell Stock III
- [ ] Edit Distance
- [ ] Palindromic Substrings
- [ ] Longest Palindromic Subsequence
- [ ] Combination Sum

### Systems Mini-Topic

- [ ] Learn CPU cache basics.
- [ ] Learn cache lines.
- [ ] Learn why contiguous memory is fast.
- [ ] Learn why pointer chasing can be slow.
- [ ] Write a tiny benchmark comparing vector traversal vs linked-list-like pointer chasing if possible.

### Deliverables

- [ ] 10+ problems.
- [ ] One note: “Monotonic stack/queue invariants.”
- [ ] One small benchmark: contiguous vs non-contiguous access.

---

# Phase 4: HFT Systems Ramp

## Week 7: C++ Performance, Profiling, Benchmarking

**Dates:** July 7 – July 13  
**Theme:** Build instincts for fast, measurable C++.

### C++ Concepts

- [ ] RAII.
- [ ] Move semantics.
- [ ] Copy elision basics.
- [ ] `std::span`.
- [ ] `string_view`.
- [ ] Avoiding allocations in hot paths.
- [ ] Reserving vector capacity.
- [ ] Object lifetime.
- [ ] Alignment basics.
- [ ] False sharing basics.

### Benchmarking Concepts

- [ ] Warmup.
- [ ] Repeated trials.
- [ ] Avoiding dead-code elimination.
- [ ] Measuring wall-clock vs CPU time.
- [ ] Variance.
- [ ] CPU frequency scaling.
- [ ] CPU pinning conceptually.
- [ ] Release builds only.

### Tool Checklist

- [ ] `perf stat`
- [ ] `perf record`
- [ ] `perf report`
- [ ] `strace`
- [ ] `valgrind` / `callgrind` optionally
- [ ] Compiler Explorer
- [ ] Google Benchmark optionally

### DSA Maintenance

- [ ] 3 mixed timed mediums.
- [ ] 2 redos from miss log.
- [ ] 1 hard attempt.

### Systems Mini-Topic

- [ ] Learn ring buffer basics.
- [ ] Learn producer/consumer basics.
- [ ] Learn single-producer/single-consumer queue conceptually.
- [ ] Learn why allocations and locks in hot paths are suspicious.

### Deliverables

- [ ] One microbenchmark project.
- [ ] One note: “How to benchmark without lying to myself.”
- [ ] One note: “Why allocation can hurt latency.”

---

## Week 8: Market Data, Packet Capture, Protocol Parsing, Codegen

**Dates:** July 14 – July 20  
**Theme:** Connect DSA/C++ to the team’s likely work.

### Networking Concepts

- [ ] Ethernet basics.
- [ ] IP basics.
- [ ] UDP basics.
- [ ] TCP basics.
- [ ] Multicast basics.
- [ ] Packet drops.
- [ ] NIC receive queues.
- [ ] Interrupts vs polling.
- [ ] Timestamping basics.

### Market Data Concepts

- [ ] What is market data?
- [ ] Trades vs quotes.
- [ ] Order book basics.
- [ ] Level 1 vs Level 2 data.
- [ ] Sequence numbers.
- [ ] Gap detection.
- [ ] Replay.
- [ ] Binary protocols.
- [ ] FIX conceptually, but do not over-focus unless team uses it.

### Protocol Parsing / Codegen Concepts

- [ ] YAML schema/descriptor.
- [ ] Python script reads descriptor.
- [ ] Script emits C++ structs/parsers/callbacks.
- [ ] Generated parser reads bytes from wire format.
- [ ] Parser calls typed callbacks.
- [ ] Tests validate known packet/message examples.

### Mini-Project: Binary Protocol Parser Generator

- [ ] Create a YAML descriptor for a fake market data message.
- [ ] Define fields like message type, sequence number, symbol, price, quantity.
- [ ] Write Python code that reads YAML.
- [ ] Generate a C++ header with a parser function.
- [ ] Parse a binary buffer.
- [ ] Call a callback or return a struct.
- [ ] Add tests.
- [ ] Add a benchmark.

### DSA Maintenance

- [ ] 3 mixed timed mediums.
- [ ] 2 redos.
- [ ] 1 problem involving parsing or intervals.

### Deliverables

- [ ] Toy YAML → generated C++ parser project.
- [ ] One note: “How market data packet parsing might work.”
- [ ] One note: “Packet drops: possible causes and mitigations.”

---

# Phase 5: Data Storage + Final Sprint

## Week 9: Parquet, Spark, InfluxDB, Storage Pipelines

**Dates:** July 21 – July 27  
**Theme:** Understand where captured market data goes after ingestion.

### Parquet Concepts

- [ ] Columnar vs row-oriented storage.
- [ ] Schema.
- [ ] Row groups.
- [ ] Compression.
- [ ] Encoding.
- [ ] Predicate pushdown conceptually.
- [ ] Why Parquet works well for analytics.

### Spark Concepts

- [ ] Driver vs executors conceptually.
- [ ] DataFrames.
- [ ] Reading/writing Parquet.
- [ ] Partitioning.
- [ ] Lazy execution.
- [ ] Shuffles.
- [ ] Why Spark is not low-latency packet capture.
- [ ] Where Spark fits in offline analysis.

### InfluxDB Concepts

- [ ] Time-series database.
- [ ] Measurements.
- [ ] Tags.
- [ ] Fields.
- [ ] Retention.
- [ ] Metrics dashboards.
- [ ] Example metrics: packets received, packets dropped, decode latency, queue depth, disk write throughput, CPU usage.

### Storage / Filesystem Concepts

- [ ] Sequential writes.
- [ ] Random reads/writes.
- [ ] Page cache.
- [ ] `mmap` conceptually.
- [ ] Compression tradeoffs.
- [ ] Append-only logs.
- [ ] Distributed filesystem basics.
- [ ] Replication.
- [ ] Partitioning by date/exchange/symbol.

### DSA Maintenance

- [ ] 2 timed mixed sets.
- [ ] 5 redos from miss log.
- [ ] 1 hard problem attempt.

### Deliverables

- [ ] One note: “Why packet capture and Spark solve different parts of the pipeline.”
- [ ] One note: “Why Parquet is good for historical market data.”
- [ ] One metrics schema sketch for InfluxDB-style monitoring.

---

## Week 10: Final Pre-Start Readiness

**Dates:** July 28 – August 2  
**Theme:** Consolidate, do not panic cram.

### DSA Final Review

- [ ] Redo top 10 missed problems.
- [ ] Do 2 timed mixed sets.
- [ ] Review all pattern notes.
- [ ] Review STL cheat sheet.
- [ ] Review complexity cheat sheet.

### C++ Final Review

- [ ] Re-read RAII notes.
- [ ] Re-read move semantics notes.
- [ ] Re-read STL iterator invalidation notes.
- [ ] Re-read benchmarking notes.
- [ ] Polish parser/codegen mini-project README.

### Systems Final Review

- [ ] Review packet lifecycle.
- [ ] Review kernel bypass mental model.
- [ ] Review PCAP notes.
- [ ] Review 10GbE math.
- [ ] Review Parquet/Spark/InfluxDB notes.
- [ ] Review market data glossary.

### Week-One Work Prep

- [ ] Prepare a first-week questions document.
- [ ] Prepare a personal glossary template.
- [ ] Prepare a daily notes template.
- [ ] Prepare a “how to ramp on an unfamiliar codebase” checklist.
- [ ] Clean local dev environment.
- [ ] Make sure C++ tooling is comfortable:
  - [ ] Compiler
  - [ ] CMake
  - [ ] Ninja
  - [ ] gdb/lldb
  - [ ] clangd
  - [ ] clang-format
  - [ ] perf on Linux machine if available

### Deliverables

- [ ] Final DSA miss log review.
- [ ] Final notes folder organized.
- [ ] First-week question list ready.
- [ ] Confidence ratings updated.
- [ ] Sleep and energy protected.

---

# 7. DSA Master Checklist

## Arrays / Strings

- [ ] Two Sum
- [ ] Best Time to Buy and Sell Stock
- [ ] Contains Duplicate
- [ ] Product of Array Except Self
- [ ] Maximum Subarray
- [ ] Maximum Product Subarray
- [ ] Find Minimum in Rotated Sorted Array
- [ ] Search in Rotated Sorted Array
- [ ] 3Sum
- [ ] Container With Most Water
- [ ] Valid Anagram
- [ ] Group Anagrams
- [ ] Longest Consecutive Sequence

## Two Pointers

- [ ] Valid Palindrome
- [ ] Two Sum II
- [ ] 3Sum
- [ ] Container With Most Water
- [ ] Trapping Rain Water
- [ ] Remove Duplicates from Sorted Array
- [ ] Sort Colors

## Sliding Window

- [ ] Best Time to Buy and Sell Stock
- [ ] Longest Substring Without Repeating Characters
- [ ] Longest Repeating Character Replacement
- [ ] Permutation in String
- [ ] Minimum Window Substring
- [ ] Sliding Window Maximum

## Stack / Monotonic Stack

- [ ] Valid Parentheses
- [ ] Min Stack
- [ ] Evaluate Reverse Polish Notation
- [ ] Generate Parentheses
- [ ] Daily Temperatures
- [ ] Car Fleet
- [ ] Largest Rectangle in Histogram

## Binary Search

- [ ] Binary Search
- [ ] Search a 2D Matrix
- [ ] Koko Eating Bananas
- [ ] Find Minimum in Rotated Sorted Array
- [ ] Search in Rotated Sorted Array
- [ ] Time Based Key-Value Store
- [ ] Median of Two Sorted Arrays
- [ ] Capacity To Ship Packages Within D Days
- [ ] Split Array Largest Sum

## Linked List

- [ ] Reverse Linked List
- [ ] Merge Two Sorted Lists
- [ ] Reorder List
- [ ] Remove Nth Node From End
- [ ] Copy List with Random Pointer
- [ ] Add Two Numbers
- [ ] Linked List Cycle
- [ ] Find Duplicate Number
- [ ] LRU Cache
- [ ] Merge K Sorted Lists
- [ ] Reverse Nodes in K-Group

## Trees

- [ ] Invert Binary Tree
- [ ] Maximum Depth
- [ ] Same Tree
- [ ] Subtree of Another Tree
- [ ] Lowest Common Ancestor
- [ ] Binary Tree Level Order Traversal
- [ ] Validate BST
- [ ] Kth Smallest in BST
- [ ] Construct Binary Tree from Preorder and Inorder
- [ ] Binary Tree Maximum Path Sum
- [ ] Serialize and Deserialize Binary Tree

## Graphs

- [ ] Number of Islands
- [ ] Clone Graph
- [ ] Max Area of Island
- [ ] Pacific Atlantic Water Flow
- [ ] Surrounded Regions
- [ ] Rotting Oranges
- [ ] Walls and Gates
- [ ] Course Schedule
- [ ] Course Schedule II
- [ ] Redundant Connection
- [ ] Number of Connected Components
- [ ] Graph Valid Tree
- [ ] Word Ladder
- [ ] Network Delay Time

## Heaps / Priority Queues

- [ ] Kth Largest Element in a Stream
- [ ] Last Stone Weight
- [ ] K Closest Points
- [ ] Kth Largest Element in an Array
- [ ] Task Scheduler
- [ ] Design Twitter
- [ ] Find Median from Data Stream
- [ ] Merge K Sorted Lists

## Intervals

- [ ] Insert Interval
- [ ] Merge Intervals
- [ ] Non-overlapping Intervals
- [ ] Meeting Rooms
- [ ] Meeting Rooms II
- [ ] Minimum Interval to Include Each Query

## Greedy

- [ ] Maximum Subarray
- [ ] Jump Game
- [ ] Jump Game II
- [ ] Gas Station
- [ ] Hand of Straights
- [ ] Merge Triplets to Form Target
- [ ] Partition Labels
- [ ] Valid Parenthesis String
- [ ] Candy

## Dynamic Programming

- [ ] Climbing Stairs
- [ ] Min Cost Climbing Stairs
- [ ] House Robber
- [ ] House Robber II
- [ ] Longest Palindromic Substring
- [ ] Palindromic Substrings
- [ ] Decode Ways
- [ ] Coin Change
- [ ] Maximum Product Subarray
- [ ] Word Break
- [ ] Longest Increasing Subsequence
- [ ] Partition Equal Subset Sum
- [ ] Unique Paths
- [ ] Longest Common Subsequence
- [ ] Best Time to Buy and Sell Stock with Cooldown
- [ ] Coin Change II
- [ ] Target Sum
- [ ] Interleaving String
- [ ] Edit Distance
- [ ] Burst Balloons
- [ ] Regular Expression Matching

## Tries

- [ ] Implement Trie
- [ ] Design Add and Search Words Data Structure
- [ ] Word Search II
- [ ] Maximum XOR of Two Numbers in an Array

## Backtracking

- [ ] Subsets
- [ ] Combination Sum
- [ ] Permutations
- [ ] Subsets II
- [ ] Combination Sum II
- [ ] Word Search
- [ ] Palindrome Partitioning
- [ ] Letter Combinations of a Phone Number
- [ ] N-Queens

---

# 8. C++ Master Checklist

## STL Fluency

- [ ] `vector`
- [ ] `array`
- [ ] `deque`
- [ ] `list` and why it is rarely the answer
- [ ] `stack`
- [ ] `queue`
- [ ] `priority_queue`
- [ ] `set`
- [ ] `multiset`
- [ ] `map`
- [ ] `unordered_map`
- [ ] `unordered_set`
- [ ] `tuple`
- [ ] `pair`
- [ ] `optional`
- [ ] `variant` conceptually
- [ ] `span`
- [ ] `string_view`

## Algorithms Library

- [ ] `sort`
- [ ] `stable_sort`
- [ ] `partial_sort`
- [ ] `nth_element`
- [ ] `lower_bound`
- [ ] `upper_bound`
- [ ] `binary_search`
- [ ] `find`
- [ ] `find_if`
- [ ] `accumulate`
- [ ] `min_element`
- [ ] `max_element`
- [ ] `unique`
- [ ] `remove_if`
- [ ] `partition`
- [ ] `next_permutation`

## Language Fundamentals

- [ ] Value vs reference.
- [ ] Lvalue/rvalue basics.
- [ ] Move semantics.
- [ ] RAII.
- [ ] Rule of zero.
- [ ] Rule of five.
- [ ] Constructors/destructors.
- [ ] Const correctness.
- [ ] Templates basics.
- [ ] Lambdas.
- [ ] Type deduction with `auto`.
- [ ] `constexpr` basics.
- [ ] Smart pointers.
- [ ] Undefined behavior examples.

## Performance Awareness

- [ ] Allocation cost.
- [ ] `reserve` vs `resize`.
- [ ] Copy vs move.
- [ ] Cache locality.
- [ ] Branch prediction conceptually.
- [ ] False sharing conceptually.
- [ ] Inlining conceptually.
- [ ] Compiler optimization levels.
- [ ] Benchmarking pitfalls.
- [ ] Profiling before optimizing.

## Debugging / Tooling

- [ ] `gdb` or `lldb` basics.
- [ ] AddressSanitizer.
- [ ] UndefinedBehaviorSanitizer.
- [ ] ThreadSanitizer conceptually.
- [ ] `valgrind` basics.
- [ ] `perf` basics.
- [ ] `clang-format`.
- [ ] `clang-tidy` conceptually.
- [ ] CMake basics.
- [ ] Ninja basics.

---

# 9. HFT-Relevant Systems Checklist

## Networking

- [ ] What is a NIC?
- [ ] What is Ethernet?
- [ ] What is an IP packet?
- [ ] What is UDP?
- [ ] What is TCP?
- [ ] What is multicast?
- [ ] What is a socket?
- [ ] What is a kernel network stack?
- [ ] What is a packet drop?
- [ ] What is packet loss vs application-level gap?
- [ ] What is a sequence number?
- [ ] What is packet timestamping?
- [ ] What are receive queues?
- [ ] What are interrupts?
- [ ] What is polling?
- [ ] What is busy-waiting?

## Packet Capture

- [ ] What is PCAP?
- [ ] What is `tcpdump`?
- [ ] What is Wireshark?
- [ ] What is libpcap?
- [ ] What does dropped-by-kernel mean?
- [ ] What does dropped-by-interface mean?
- [ ] How do buffer sizes affect capture?
- [ ] Why does disk write throughput matter?
- [ ] Why does batching help?
- [ ] Why do allocations hurt capture loops?
- [ ] What metrics would I track?

## Kernel Bypass

- [ ] What problem does kernel bypass solve?
- [ ] What does userspace packet processing mean?
- [ ] What is DPDK conceptually?
- [ ] What is AF_XDP/XDP conceptually?
- [ ] What is PF_RING/netmap conceptually?
- [ ] What tradeoffs come with bypassing the kernel?
- [ ] What does polling cost?
- [ ] What is CPU affinity?
- [ ] What are huge pages conceptually?

## Market Data

- [ ] What is a trade?
- [ ] What is a quote?
- [ ] What is bid/ask?
- [ ] What is spread?
- [ ] What is an order book?
- [ ] What is top-of-book?
- [ ] What is depth-of-book?
- [ ] What is a feed handler?
- [ ] What is a market data protocol?
- [ ] Why binary protocols?
- [ ] Why sequence numbers?
- [ ] What is gap detection?
- [ ] What is replay?
- [ ] What is normalization?

## Storage / Analytics

- [ ] What is Parquet?
- [ ] What is columnar storage?
- [ ] What is a row group?
- [ ] What is compression?
- [ ] What is predicate pushdown?
- [ ] What is Spark?
- [ ] What is a Spark DataFrame?
- [ ] What is partitioning?
- [ ] What is a time-series database?
- [ ] What is InfluxDB used for?
- [ ] What metrics matter for packet capture systems?

## Linux Performance

- [ ] What is a syscall?
- [ ] What is a context switch?
- [ ] What is a page fault?
- [ ] What is the page cache?
- [ ] What is `mmap`?
- [ ] What is CPU affinity?
- [ ] What is NUMA conceptually?
- [ ] What is a cache line?
- [ ] What is false sharing?
- [ ] What is a flamegraph?
- [ ] What is tail latency?
- [ ] What is p50/p95/p99 latency?

---

# 10. Mini-Projects

## Project A: STL Pattern Snippet Repo

**Purpose:** Build speed in C++ interview implementation.

- [ ] Create a small repo or notes folder.
- [ ] Add snippets for binary search, BFS, DFS, Dijkstra, union-find, trie, sliding window, monotonic stack, monotonic queue, heap comparator, ordered set lower_bound.
- [ ] Add tests or sample inputs for each.

## Project B: Packet Capture Toy

**Purpose:** Understand PCAP/networking concepts.

- [ ] Use `tcpdump` to capture local traffic.
- [ ] Use Wireshark to inspect packets.
- [ ] Write a small parser that reads a PCAP file header.
- [ ] Count packets by protocol.
- [ ] Print basic stats.
- [ ] Write notes on what was confusing.

## Project C: YAML → C++ Parser Generator

**Purpose:** Mirror the boss conversation directly.

- [ ] Define fake protocol in YAML.
- [ ] Write Python codegen.
- [ ] Generate C++ structs.
- [ ] Generate parse functions.
- [ ] Generate callback interface.
- [ ] Add unit tests.
- [ ] Add benchmark.
- [ ] Write README explaining tradeoffs.

Example fake YAML:

```yaml
messages:
  Trade:
    type_id: 1
    fields:
      - { name: sequence_number, type: u64 }
      - { name: symbol, type: char[8] }
      - { name: price, type: i64 }
      - { name: quantity, type: u32 }
```

Questions to explore:

- [ ] How do I handle endianness?
- [ ] How do I avoid unaligned reads?
- [ ] How do I avoid copies?
- [ ] How do I validate buffer length?
- [ ] What should the callback API look like?
- [ ] What should generated tests look like?

## Project D: Tiny Market Data Replay

**Purpose:** Connect parsing, sequencing, timing, and metrics.

- [ ] Generate fake binary market data messages.
- [ ] Write them to a file.
- [ ] Replay them into parser.
- [ ] Track sequence gaps.
- [ ] Track messages/sec.
- [ ] Track parsing latency.
- [ ] Output metrics as CSV.
- [ ] Optional: write metrics in line protocol style similar to time-series ingestion.

---

# 11. First Week at Work Playbook

## Mindset

- [ ] Be honest about what you know.
- [ ] Do not pretend internal systems are obvious.
- [ ] Ask precise questions.
- [ ] Take notes constantly.
- [ ] Look for small ways to reduce friction.
- [ ] Confirm assumptions before changing performance-sensitive code.
- [ ] Respect existing systems; assume there are reasons for weirdness until proven otherwise.

## Questions to Ask

### Team / System Architecture

- [ ] What are the main systems this team owns?
- [ ] Is “Cerebro/Sereibro” an internal system? What does it do?
- [ ] What are HTFS and SEF? Are they internal filesystems/storage systems?
- [ ] Where does packet capture fit in the overall market data pipeline?
- [ ] What are the main pain points right now?
- [ ] What does success look like for me in 30/60/90 days?

### Packet Capture

- [ ] What packet capture technology do we use?
- [ ] Are we using kernel networking, DPDK, AF_XDP, FPGA NICs, or something else?
- [ ] How do we measure drops?
- [ ] What are the most important metrics?
- [ ] What are the most common bottlenecks?

### Protocols / Codegen

- [ ] Which market data protocols do we decode?
- [ ] Are protocol descriptors hand-written YAML?
- [ ] How is generated C++ tested?
- [ ] How do we handle schema/protocol changes?
- [ ] How do we validate parser correctness?

### Storage / Analytics

- [ ] Where do captured packets/messages get stored?
- [ ] How is Parquet used?
- [ ] Where does Spark fit?
- [ ] What is InfluxDB used for?
- [ ] What dashboards should I learn first?

### Development Workflow

- [ ] What is the standard build system?
- [ ] What is the test command?
- [ ] What is the benchmark command?
- [ ] What is the code review style?
- [ ] What GitLab pipelines matter most?
- [ ] What docs are most important for onboarding?

## First 30 Days

- [ ] Build the main repo locally.
- [ ] Run tests.
- [ ] Run benchmarks if available.
- [ ] Read architecture docs.
- [ ] Shadow packet capture or market data pipeline debugging.
- [ ] Fix one small doc issue.
- [ ] Fix one small test/tooling issue.
- [ ] Understand one production metric dashboard.
- [ ] Trace one packet/message through the system at a high level.
- [ ] Understand how generated protocol code works.

## First 60 Days

- [ ] Own a small feature or bug fix.
- [ ] Add or improve a benchmark.
- [ ] Improve observability for a small component.
- [ ] Understand major failure modes.
- [ ] Be able to explain team architecture to a new person.
- [ ] Build trust through careful, reviewed changes.

## First 90 Days

- [ ] Own a meaningful part of a tool/component.
- [ ] Make one performance improvement or measurement improvement.
- [ ] Become useful in debugging capture/storage/protocol issues.
- [ ] Have a clear map of what to learn next.

---

# 12. Confidence Ratings

Update every Sunday.

| Area | Current Rating 1–5 | Target by Aug 3 | Notes |
|---|---:|---:|---|
| Arrays/strings | | 5 | |
| Sliding window | | 5 | |
| Binary search | | 5 | |
| Trees | | 4 | |
| Graphs | | 4 | |
| Heaps/intervals | | 5 | |
| Greedy | | 4 | |
| DP | | 4 | |
| C++ STL | | 5 | |
| C++ performance | | 4 | |
| Linux tools | | 3 | |
| Networking basics | | 3 | |
| Packet capture | | 3 | |
| Kernel bypass concepts | | 2–3 | |
| Parquet/Spark basics | | 3 | |
| Market data concepts | | 3 | |

---

# 13. Resource List

## DSA

- [ ] NeetCode 150
- [ ] Blind 75 / Grind 75
- [ ] CSES Problem Set
- [ ] Competitive Programmer’s Handbook
- [ ] LeetCode company tags if available

## C++

- [ ] cppreference
- [ ] Effective Modern C++
- [ ] C++ Core Guidelines
- [ ] Compiler Explorer
- [ ] Google Benchmark docs

## Networking / Packet Capture

- [ ] Computer Networking: A Top-Down Approach, selected chapters
- [ ] tcpdump man pages
- [ ] Wireshark docs
- [ ] DPDK docs, conceptual first
- [ ] XDP/AF_XDP docs, conceptual first

## Linux Performance

- [ ] Brendan Gregg’s Linux performance materials
- [ ] `perf` examples
- [ ] Flamegraph examples
- [ ] Systems Performance, selected chapters

## Storage / Data

- [ ] Apache Parquet docs
- [ ] Spark SQL Parquet docs
- [ ] InfluxDB docs

---

# 14. Weekly Review Template

```md
## Weekly Review: Week __

### What I completed
- [ ]

### Problems solved
- [ ]

### Problems to redo
- [ ]

### Biggest misses
- [ ]

### C++ things I learned
- [ ]

### Systems things I learned
- [ ]

### Questions for work / future me
- [ ]

### Next week priorities
1.
2.
3.
```

---

# 15. Daily Notes Template

```md
## Date

### DSA
- Problem(s):
- Pattern:
- Misses:
- Redo date:

### C++
- Concept:
- Example/snippet:

### Systems
- Topic:
- What I understand now:
- Question:

### One thing to carry forward
-
```

---

# 16. Final Reminder

The goal is not to show up already knowing everything. The goal is to show up with:

- [ ] Strong fundamentals.
- [ ] Fast learning loops.
- [ ] Good notes.
- [ ] Better questions than a normal entry-level engineer.
- [ ] Enough C++ and systems awareness that senior engineers enjoy teaching you.
- [ ] Enough DSA strength that problem solving feels exciting instead of intimidating.

Blowing away expectations will come less from pretending to be senior immediately and more from being unusually prepared, unusually curious, unusually reliable, and unusually fast at turning feedback into skill.
