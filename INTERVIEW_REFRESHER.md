# C++ Interview Refresher

This repo shows a mix of language fundamentals, performance work, concurrency, data structures, parsing, networking, and algorithm practice. Use this as a fast last-minute scan of what you have actually built and explored.

## Fast Self-Intro

If they ask what you have been learning:

- Modern C++ fundamentals: references, move semantics, RAII, smart pointers, lambdas, ranges, iterators, templates, `constexpr`, `string_view`
- Performance: cache locality, flat vs nested layouts, padding/alignment, false sharing, avoiding copies, `std::endl` vs `"\n"`, parsing with `from_chars`
- Concurrency: `std::thread`, `std::jthread`, stop tokens, atomics, mutexes, condition variables, latches, barriers
- Systems work: TCP client/server with Asio, multithreaded filesystem searcher, Sudoku solver optimization
- Data structures and algorithms: heaps, hashmap, maps/sets, Aho-Corasick, substring search, next permutation style problems

## Core C++ Concepts You Practiced

### References, Value Categories, and Copies

- `T&` is an lvalue reference: named object, stable identity
- `T&&` is an rvalue reference: binds to temporaries and move sources
- Passing large objects by value copies them unless copy elision or move happens
- Prefer `const T&` for read-only params, or `std::string_view` for non-owning string input
- You explicitly tested vector copying by comparing `data()` pointers

```cpp
void print_vec(const std::vector<int>& v) {
    std::cout << v.size() << "\n";
}

void print_name(std::string_view name) {
    std::cout << name << "\n";
}

std::vector<int> a{1, 2, 3};
print_vec(a);
print_name("chris");
```

Good interview line:

`const` is about read-only intent. `&` avoids copies. `&&` lets you transfer resources instead of duplicating them.

### Move Semantics and RVO

You built a `Tracker` type to observe copies vs moves.

- Move constructor/assignment transfer ownership of internal resources
- Moved-from objects are valid but unspecified
- `std::move` is just a cast to an rvalue; it does not move by itself
- `push_back(x)` copies, `push_back(std::move(x))` moves, `emplace_back(...)` constructs in place
- Returning local objects by value often uses RVO/NRVO, so do not force `std::move` on return values
- Mark move operations `noexcept` so containers can prefer moves during reallocation

```cpp
std::vector<std::string> v;
std::string s = "hello";

v.push_back(s);            // copy
v.push_back(std::move(s)); // move
v.emplace_back("world");   // construct in place

std::vector<int> make_data() {
    std::vector<int> out{1, 2, 3};
    return out; // RVO/NRVO
}
```

Good interview line:

Move semantics are about stealing heap-backed state cheaply; RVO is even better because no move happens at all.

### RAII and Resource Ownership

- File streams close themselves on destruction
- `std::jthread` auto-joins on destruction
- Smart pointers encode ownership in the type system
- Manual `.close()` is usually unnecessary when scope-based cleanup is enough

```cpp
{
    std::ifstream file("data.txt");
    std::jthread worker([] {
        std::cout << "working\n";
    });
} // file closes, jthread joins
```

Good interview line:

RAII means resource lifetime is tied to object lifetime, which makes cleanup automatic and exception-safe.

### Smart Pointers

You covered this thoroughly.

- `unique_ptr`: exclusive ownership, move-only, zero refcount overhead
- `shared_ptr`: shared ownership with reference counting
- `weak_ptr`: non-owning observer, mainly to break cycles
- Use `make_unique` and `make_shared` by default
- Pass `unique_ptr` by value when transferring ownership
- Pass raw reference or `const T&` when only observing the pointee
- Pass `shared_ptr` by value only if the callee must share ownership
- You tested custom deleters and weak-pointer expiry

```cpp
auto u = std::make_unique<int>(42);
auto s = std::make_shared<int>(99);
std::weak_ptr<int> w = s;

auto u2 = std::move(u); // transfer ownership
if (auto locked = w.lock()) {
    std::cout << *locked << "\n";
}
```

Common interview distinction:

- Ownership question: who destroys the object?
- If there is exactly one owner, use `unique_ptr`
- If ownership is shared, use `shared_ptr`
- If you need observation without extending lifetime, use `weak_ptr`

### Const Correctness

- `const` member functions promise not to mutate object state
- `const T&` makes read-only APIs clear
- Iterating with `const auto&` avoids copies and prevents mutation
- You compared bad APIs like `std::string` by value vs good APIs using `const&` or `string_view`

```cpp
struct Position {
    double price;
    int qty;

    double notional() const {
        return price * qty;
    }
};

double total(const std::vector<Position>& book) {
    double out = 0.0;
    for (const auto& pos : book) out += pos.notional();
    return out;
}
```

Good interview line:

Const correctness is both documentation and a compiler-enforced safety rule.

### `constexpr` and Compile-Time Work

- `constexpr` guarantees compile-time evaluation when possible
- You wrote compile-time integer square root and lookup tables
- `static_assert` validates invariants at compile time
- `if constexpr` enables compile-time branching in templates
- Compile-time work matters because hot-path runtime work disappears

```cpp
constexpr int isqrt(int n) {
    int i = 0;
    while ((i + 1) * (i + 1) <= n) ++i;
    return i;
}

constexpr int ROOT = isqrt(9);
static_assert(ROOT == 3);
```

Good interview line:

`const` means immutable; `constexpr` means the computation itself can be baked into the binary.

### Lambdas

You practiced:

- Basic lambda syntax
- Capture by value `[=]`, by reference `[&]`, and mixed captures
- `mutable` lambdas for changing captured-by-value copies
- Lambdas with STL algorithms like `sort`, `copy_if`, `transform`, `accumulate`, `count_if`
- Generic lambdas with `auto`
- Returning lambdas from functions
- Recursive lambdas through `std::function`
- Init-captures like `[y = x * 2]` and move capture `[p = std::move(ptr)]`

Important tradeoff:

`std::function` is convenient but adds type-erasure overhead and may allocate.

```cpp
int threshold = 5;
std::vector<int> nums{1, 2, 6, 9};

auto count = std::count_if(nums.begin(), nums.end(),
    [threshold](int x) { return x > threshold; });

auto owner = [p = std::make_unique<int>(7)]() {
    std::cout << *p << "\n";
};
```

### Templates, Traits, and Tag Dispatch

You explored:

- Function templates
- Nested type aliases like `T::data_type`
- Dispatching behavior via tags like `FastTag` and `PreciseTag`
- Comparator customization for associative containers
- Transparent lookup with `std::less<>` and `is_transparent`
- Pointer-to-member use in custom sort/projection code

Good interview line:

Templates move polymorphism to compile time, so you get generic code without virtual dispatch.

```cpp
struct FastTag {};
struct PreciseTag {};

template <typename T>
void process(T value, FastTag) {
    std::cout << value * 2 << "\n";
}

template <typename T>
void process(T value, PreciseTag) {
    std::cout << value * 3.14159 << "\n";
}
```

### Iterators, Ranges, and Views

You implemented custom iterators and used C++20 ranges.

- A range needs `begin()` and `end()`
- Iterator basics: `operator*`, `operator++`, comparison
- Iterator category and associated typedefs matter for algorithm compatibility
- `std::ranges` algorithms accept whole ranges instead of begin/end pairs
- Projections let you sort/find on a member like `&Player::score`
- Views are lazy: `filter`, `transform`, `take`, `drop`, `reverse`, `iota`
- Pipelines avoid intermediate containers
- To materialize a view in C++20, construct a container from iterators

```cpp
std::vector<int> v{1, 2, 3, 4, 5, 6};

auto result = v
    | std::views::filter([](int x) { return x % 2 == 0; })
    | std::views::transform([](int x) { return x * x; });

for (int x : result) std::cout << x << " ";
```

Good interview line:

Ranges improve expressiveness, and views are lazy adapters rather than eager containers.

## Performance Topics You Explored

### Cache Locality and Flat Layouts

You compared nested containers vs contiguous memory.

- `vector<vector<T>>` means many allocations and pointer chasing
- Flat storage like `vector<T>` or `array<T, N>` is contiguous and cache-friendly
- Index flattening is `row * width + col`
- This directly influenced Sudoku optimization

```cpp
std::vector<int> grid(rows * cols);
int value = grid[r * cols + c];
```

High-value interview line:

Data layout often matters more than micro-optimizing arithmetic because cache misses dominate runtime.

### Padding and Alignment

You examined struct layout and field ordering.

- Member order changes struct size
- Reordering large-to-small often reduces padding
- `alignas(...)` can force cache-line separation
- `#pragma pack(push, 1)` removes padding but can create misaligned accesses
- Packed structs are mainly for wire formats and file layouts, not general computation

```cpp
struct Bad {
    char a;
    int b;
    char c;
};

struct Better {
    int b;
    char a;
    char c;
};
```

### False Sharing

You benchmarked two threads updating separate atomics that still lived on the same cache line.

- False sharing happens when independent variables share one cache line
- Even if threads touch different variables, cache coherence traffic can crush performance
- `alignas(cache_line_size)` can separate hot fields

```cpp
struct Hot {
    std::atomic<int> a;
    std::atomic<int> b;
};

struct Cool {
    alignas(128) std::atomic<int> a;
    alignas(128) std::atomic<int> b;
};
```

Good interview line:

Atomics can be logically correct but still slow if two hot counters bounce the same cache line between cores.

### Parsing and I/O

- `std::from_chars` parses numbers without locale overhead and without allocations
- It reports errors via `std::errc`
- `std::endl` inserts newline and flushes, while `"\n"` just inserts newline
- Repeated flushes are expensive, so prefer `"\n"` unless you need immediate visibility

```cpp
int value = 0;
auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value);

if (ec == std::errc()) {
    std::cout << value << "\n";
}
```

### Assembly and Compiler Optimization Awareness

You looked at emitted assembly with different optimization levels.

- `-O0` shows direct, unoptimized codegen
- `-O2`/`-O3` allows inlining, constant folding, dead-code elimination, and better register usage
- You used `c++filt` to demangle symbols

## Concurrency Topics You Practiced

### Threads and `jthread`

- `std::thread` requires explicit `join()` or `detach()`
- `std::jthread` auto-joins on destruction
- `std::jthread` integrates with `std::stop_token`
- You built worker loops that check `stop_requested()`

```cpp
std::jthread worker([](std::stop_token st) {
    while (!st.stop_requested()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
});

worker.request_stop();
```

### Atomics vs Mutexes

You wrote a clean comparison demo.

- Use `std::atomic<T>` for one independent value like a flag or counter
- Use a mutex when multiple values or multiple steps must stay consistent together
- `compare_exchange_strong` is the standard CAS primitive
- Integral atomics support operations like `fetch_add`
- `seq_cst` is the strongest default memory ordering

```cpp
std::atomic<int> counter = 0;
++counter;

std::mutex mtx;
std::vector<std::string> messages;

{
    std::lock_guard lock(mtx);
    messages.push_back("hello");
}
```

Good interview line:

Atomic protects one memory location atomically. Mutex protects a critical section and higher-level invariants.

### Condition Variables

You built a job queue with a worker thread.

- Protect shared state with a mutex
- Wait with a predicate to handle spurious wakeups
- Unlock while doing expensive work, relock before touching shared state again
- `condition_variable_any` can work with stop tokens; it is more flexible than `condition_variable`

```cpp
std::mutex mtx;
std::condition_variable cv;
std::queue<int> jobs;

std::unique_lock lock(mtx);
cv.wait(lock, [&] { return !jobs.empty(); });
int job = jobs.front();
jobs.pop();
```

### Latch, Barrier, and Atomic Wait/Notify

You explored modern synchronization primitives.

- `std::latch`: one-time countdown gate
- `std::barrier`: repeated phase synchronization
- Atomics can support `wait()` / `notify_one()` / `notify_all()` in C++20

```cpp
std::latch ready(3);
std::atomic<int> done = 0;

ready.count_down();
done.fetch_add(1);
done.notify_one();
```

## STL Containers and Common Patterns

### Vector and Deque

- `vector` is contiguous and best for random access plus iteration
- Reallocation can invalidate pointers/iterators/references
- `reserve()` avoids repeated reallocations
- `deque` supports cheap push/pop at both ends but is not contiguous like `vector`

### `map` vs `unordered_map`

You compared both.

- `map`: ordered tree, `O(log n)`, stable order, comparator-based
- `unordered_map`: hash table, average `O(1)`, no ordering, hash/equality based
- `try_emplace` avoids overwriting and can avoid unnecessary construction
- `contains` improves readability

```cpp
std::map<int, std::string> ordered;
std::unordered_map<int, std::string> fast;

fast.try_emplace(10, "Alice");
if (fast.contains(10)) std::cout << fast[10] << "\n";
```

### `set` and `unordered_set`

- Great for uniqueness checks
- `set` keeps sorted order
- `unordered_set` is usually faster for membership tests
- You used sets for deduplication and intersections

## Data Structures You Implemented

### Heap

You built multiple heap versions and improved them over time.

- Binary heap stored in a vector
- Parent index: `(i - 1) / 2`
- Children: `2*i + 1`, `2*i + 2`
- `percolate_up` and `percolate_down` maintain heap property
- Early version used recursive swaps
- Better version used move-based hole technique to reduce copies/swaps
- Bulk heapify from a vector is `O(n)`, not `O(n log n)`
- You wrote min-heap and max-heap variants

```cpp
size_t parent = (i - 1) / 2;
size_t left = 2 * i + 1;
size_t right = 2 * i + 2;
```

Good interview line:

Heap operations are `push` and `pop` in `O(log n)`, `peek` in `O(1)`, and heapify is `O(n)`.

### Hash Map

You built a simple hash map using separate chaining.

- Buckets are `vector<vector<Entry>>`
- Hash with `std::hash<K>`
- Bucket index is `hash(key) % bucket_count`
- Collision handling used per-bucket linear scan
- `get` returned `optional<V>`
- `set` updated existing key or inserted a new key

```cpp
size_t bucket_index = std::hash<K>{}(key) % bucket_count;
for (auto& entry : buckets[bucket_index]) {
    if (entry.key == key) return entry.value;
}
```

Important implementation note you can mention:

In your current version, `get()` copies the bucket instead of referencing it. That is correct-but-slower and easy to improve.

## Projects You Can Talk Through

### Sudoku Solver

There are two versions.

#### Naive backtracking version

- Board stored as a flat `std::array<char, 81>`
- Parser loads tokens from file and converts them with `extract_num`
- `value_works` checks row, column, and 3x3 box constraints
- `solve_sudoku` uses recursive backtracking

Main talking points:

- Recursive DFS with backtracking
- Constraint checking
- Flat storage instead of nested vectors
- Benchmarking repeated solves with `chrono`

```cpp
for (int v = 1; v <= 9; ++v) {
    if (value_works(board, v, idx)) {
        board[idx] = v;
        if (solve(board) == 0) return 0;
        board[idx] = 0;
    }
}
```

#### Optimized Sudoku version

This is one of the strongest interview stories in the repo.

- Uses bitmasks for row/column/box constraints
- Candidate mask is computed with bit operations
- Tracks empty cells separately to avoid rescanning the whole board
- Uses MRV heuristic: choose the next empty cell with the fewest candidates
- Iterates candidates via bit tricks:
  - lowest set bit: `x & -x`
  - clear lowest set bit: `x &= x - 1`
  - trailing-zero count to recover digit index

```cpp
uint16_t cands = con.candidates(r, c);
while (cands) {
    int bit = cands & -cands;
    cands &= cands - 1;
    int val = __builtin_ctz(bit) + 1;
}
```

Why it is better:

- Less repeated scanning
- Less branching
- Better cache behavior
- Better search ordering

Great interview line:

I started with straightforward backtracking, then improved it by replacing repeated row/column/box scans with bitmask constraints and an MRV heuristic.

### Poker TCP Client/Server

- Built with Asio
- TCP server listens on port `9090`
- Server accepts connections and spawns one thread per client
- Client resolves endpoint, connects, reads welcome message, then does request/response echo

What this shows:

- Basic socket lifecycle
- Accept loop
- Per-client handling
- Error handling with `asio::error_code` and exceptions
- Ownership transfer of socket into a thread lambda using move capture

```cpp
tcp::socket socket(acceptor_.get_executor());
acceptor_.accept(socket);

std::thread([s = std::move(socket)]() mutable {
    // handle client
}).detach();
```

If they ask what is missing:

- Protocol framing
- Game state machine
- Lobby/table management
- Thread-safe shared game state
- Better concurrency model than thread-per-client for scale

### Grabber: Multithreaded Filesystem Search

- Uses `std::filesystem` to walk directories
- Uses CLI11 for command-line parsing
- Work queue stored in a `std::deque`
- Multiple `std::jthread` workers pull directories and scan files
- Shared `results`, `dq`, `active_workers`, and `done` are protected by a mutex and coordinated by a condition variable
- Worker unlocks while scanning filesystem and reading files, then relocks to merge results

```cpp
cv.wait(lock, [&] {
    return done || !dq.empty();
});

std::string path = std::move(dq.back());
dq.pop_back();
lock.unlock();
// scan files
lock.lock();
```

What this demonstrates:

- Producer/consumer style work queue
- Work stealing from a shared deque
- Condition variable coordination
- Lock scoping to reduce contention
- Practical filesystem and CLI integration

If they ask about improvements:

- Separate discovery and processing queues
- Reduce debug printing inside synchronized regions
- More precise completion accounting
- Store richer result metadata such as filename and line number

## Algorithms and Challenge Work

### Aho-Corasick / DNA Health

This is your strongest algorithm topic.

- Built a trie of patterns
- Added failure links using BFS
- Added dictionary suffix links for additional matches
- Scanned input string in linear pass through the automaton
- Used it to solve the DNA health problem across multiple genes and query ranges

```cpp
for (char ch : text) {
    current = trie[current].next[ch - 'a'];

    if (trie[current].wordIndex.has_value()) {
        // direct hit
    }
}
```

Good interview line:

Aho-Corasick lets you match many patterns against one text in near-linear time after building a trie with failure links.

### Grid Search

You wrote three versions.

- Straightforward substring search plus row checks
- Cleaner version using `compare`
- Rolling-hash flavored version with verification to avoid collision errors

```cpp
for (size_t pos = G[i].find(P[0]); pos != std::string::npos;
     pos = G[i].find(P[0], pos + 1)) {
    if (check2(G, P, i, pos)) return "YES";
}
```

Talking point:

You moved from simple correctness-first code to a more optimized string-matching approach.

### Bigger Is Greater

- This is next-permutation logic
- Find pivot from the right
- Find the smallest character to the right that is larger than pivot
- Swap
- Reverse suffix

```cpp
int i = n - 2;
while (i >= 0 && w[i] >= w[i + 1]) --i;

int j = n - 1;
while (w[j] <= w[i]) --j;

std::swap(w[i], w[j]);
std::reverse(w.begin() + i + 1, w.end());
```

Very common interview pattern. You implemented it multiple ways and added extensive tests.

### Absolute Permutation

- Recognized block pattern based on `2*k`
- Returned `-1` if `n` is not divisible by `2*k`
- Built the permutation without brute force

### Simpler Practice Problems

You also practiced:

- Move zeros in-place
- Remove duplicates while preserving order
- Sorted unique intersection
- Character frequency counting
- Time-in-words formatting

One file is intentionally incomplete:

- `challenges/claude/5_two_sum.cpp` still has the TODO

## String and Parsing Work

- `std::string::find` and `npos`
- Trimming with reverse iterators and `.base()`
- Stringstream tokenization
- Manual stream-like extraction operator in `SimpleStream`
- Command-line args via `argc`, `argv`, and conversion into `vector<string>`

## Interview-Worthy Small Details

- `argv` is effectively an array of C strings; `vector<string>(argv + 1, argv + argc)` builds a convenient wrapper
- Pointer arithmetic steps by object size, not bytes
- `offsetof` and `alignof` help inspect layout
- `operator bool()` on a stream-like type should usually be `explicit`
- `try_emplace` is often better than `operator[]` when you want insert-if-absent
- `std::optional` is useful when absence is part of the domain
- `std::from_chars` is low-level, fast, and non-allocating

## Likely Interview Questions From Your Repo

### Language

1. What is the difference between `const`, `constexpr`, and `consteval`?
2. What is an lvalue vs rvalue?
3. What does `std::move` actually do?
4. What is the rule of five? When do you rely on rule of zero instead?
5. When would you use `string_view` instead of `string`?
6. What is the difference between `unique_ptr`, `shared_ptr`, and `weak_ptr`?
7. Why mark move operations `noexcept`?

### Performance

1. Why is `vector<vector<int>>` slower than flat storage?
2. What is false sharing?
3. What is padding and why does field order matter?
4. Why is `std::endl` slower?
5. Why can `from_chars` beat stream extraction?

### Concurrency

1. When should you use an atomic instead of a mutex?
2. What problem does a condition variable solve?
3. What does `std::jthread` add over `std::thread`?
4. What is a stop token?
5. What are latch and barrier for?

### Data Structures / Algorithms

1. How does a binary heap work?
2. Why is heapify `O(n)`?
3. How does a hash map resolve collisions?
4. How does Aho-Corasick work?
5. How did you optimize Sudoku?

## Best Project Stories To Use

If they ask for something interesting you built, use one of these:

### 1. Sudoku optimization

Start with:

I built a Sudoku solver twice: once as straightforward recursive backtracking, then again as an optimized version using bitmasks and MRV.

Then say:

- Flat board representation
- Constraint masks for rows, columns, boxes
- Empty-cell list to avoid rescans
- Candidate enumeration with bit tricks
- Measured runtime over repeated iterations

### 2. Multithreaded filesystem searcher

Start with:

I built a filesystem search tool using `std::filesystem`, CLI parsing, `jthread`, a shared work deque, and a condition variable.

Then say:

- Threads wait efficiently for work
- Locks are dropped during expensive I/O
- Shared results are merged safely
- It was a good exercise in coordinating work and termination conditions

### 3. Aho-Corasick / DNA health

Start with:

I implemented a trie-based multi-pattern matcher with failure links and dictionary suffix links for the DNA health problem.

Then say:

- Build trie
- BFS failure links
- Single pass over input
- Efficiently collect overlapping pattern matches

## Weak Spots To Be Honest About

These are fine to admit if asked.

- Some project code is still exploratory and not production-hardened
- The poker server is still an echo-server scaffold, not full game logic
- The grabber completion logic could be cleaned up further
- The custom hashmap is educational, not feature-complete
- `two_sum.cpp` is still unfinished

That is not a problem. Just frame it as active learning plus iteration.

## 30-Second Mental Checklist Before the Interview

- Explain ownership clearly: stack vs heap, RAII, unique/shared/weak ownership
- Explain copies vs moves clearly
- Mention cache locality and flat layout when performance comes up
- Say atomic for one value, mutex for multi-step invariants
- For optimization stories, talk about measurement and data layout
- For algorithms, prefer explaining the idea, complexity, and tradeoffs before code
- For project stories, describe the initial version, bottleneck, and improvement

## File Anchors

If you want to skim actual repo examples right before the interview:

- Sudoku naive: `sudoku/main.cpp`
- Sudoku optimized: `sudoku/fast_sudoku.cpp`
- Parsing with `from_chars`: `sudoku/parsing/extract_num.cpp`, `experiments/from_chars.cpp`
- Move semantics: `experiments/move_semantics.cpp`
- Smart pointers: `experiments/smart_pointers.cpp`, `experiments/s2.cpp`
- Atomics vs mutexes: `experiments/atomic_vs_mutex.cpp`
- Condition variables: `experiments/conditions.cpp`
- False sharing: `experiments/false_sharing.cpp`
- Padding/alignment: `experiments/padding.cpp`, `experiments/illegal_padding.cpp`
- Flat vs nested layout: `experiments/flat_vs_nested.cpp`
- Ranges: `experiments/ranges/ranges.cpp`
- Lambdas: `experiments/lambda/lambdas.cpp`
- Heap implementations: `data-structures/heap/*`
- Hash map: `data-structures/my_hashmap.cpp`
- Networking: `poker/src/server.cpp`, `poker/src/client.cpp`
- Multithreaded filesystem search: `grabber/src/main.cpp`
- Aho-Corasick: `challenges/dna_health/aho_corasick.cpp`, `challenges/dna_health/dna_health.cpp`

## Final One-Line Summary

The main pattern across this repo is: start with a correct C++ implementation, then improve ownership, data layout, synchronization, and algorithmic structure until it is cleaner and faster.
