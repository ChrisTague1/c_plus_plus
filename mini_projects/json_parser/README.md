# json_parser

A hand-rolled JSON parser in C++20, written as a learning project ahead of
starting in HFT. The goal is not to be feature-complete but to use JSON
parsing as a vehicle for getting comfortable with modern C++, memory layout,
and low-latency thinking.

## Known correctness and JSON feature gaps

Correctness is not the main focus of this project, but these gaps matter when
interpreting benchmarks. A faster version should not silently do less work.

- Numbers are stored as `float`; use at least `double`, and consider separate
  `int64_t` and `double` alternatives. Prices may eventually be better modeled
  as fixed-point integers.
- Scientific notation is unsupported (`1e10`, `1E-3`). Number parsing also
  fails to enforce JSON's leading-zero and fractional-digit rules, and may
  accept inputs such as `-` or `1.`.
- The result from `std::from_chars` is ignored. Invalid and out-of-range
  numbers therefore do not produce a reliable parse error.
- A complete document is not required to consume every token. Inputs such as
  `true false` and `truex` can return a valid first value while leaving trailing
  input behind.
- An incomplete array such as `[` can call `TokenStream::peek()` past the end
  of its vector.
- Strings support only `\n`, `\t`, `\\`, and `\"`. Missing escapes include
  `\b`, `\f`, `\r`, `\/`, and Unicode `\uXXXX` (including surrogate pairs).
- Unescaped control characters and newlines inside strings are not rejected.
- JSON whitespace includes carriage return (`\r`), which the lexer currently
  rejects.
- Error positions are not consistently the start or exact failure location of
  a token, especially across multiline strings.
- There is no nesting-depth limit. Recursive parsing can overflow the C++ call
  stack on adversarial input.
- Duplicate object keys silently use a last-value-wins policy. This should be
  intentional and documented, even if it remains the chosen behavior.
- Printed strings and object keys are not escaped, so serialization can emit
  invalid JSON. Floating-point serialization also needs an explicit round-trip
  policy.
- `Json` is a non-owning reference wrapper, so it can dangle if the underlying
  `JsonValue` is destroyed or moved.
- The public API does not yet expose a structured parse result/error, input
  ownership rules, or resource limits.

## Performance-first roadmap

The immediate goal is to learn measurement, allocation behavior, locality,
and predictable latency—not to complete every item above first.

1. Establish lexer-only, parser-only, end-to-end, and field-access benchmarks.
   Track throughput, latency distribution, allocations, and bytes allocated.
2. Replace per-character `std::istream` access with a contiguous input buffer
   (`std::string_view`, pointer + end, or indexes). This is likely a larger win
   than changing the parser control flow.
3. Remove wasted parser allocations. Each call currently allocates both a
   `JsonObject` and `JsonArray`, even for scalar values; construct only the
   container actually encountered.
4. Audit hidden copies. `Token token = tokens.next()` copies the token and its
   string, and object keys are copied through `temp_key`. Measure const-reference
   and move-based alternatives while keeping ownership safe.
5. Measure the cost of materializing a complete `std::vector<Token>`. Compare
   it with a fused lexer/parser or a lightweight token containing spans into
   the source buffer.
6. Use `try_emplace`/`emplace` for object insertion and `emplace_back` where it
   improves construction, but expect this to be smaller than removing whole
   allocations and copies.
7. Compare object representations using realistic object sizes and access
   patterns: `unordered_map`, a contiguous vector with linear search, a sorted
   vector with binary search, and a flat hash map. Sorting during every insert
   may cost more than it saves; building first and sorting once is a separate
   design worth testing.
8. Make storage contiguous with an arena (`std::pmr::monotonic_buffer_resource`)
   or a flat/tape representation. Measure construction, lookup, traversal,
   destruction, and total allocated bytes—not just parse time.
9. Replace recursive parsing with an explicit stack. Treat bounded depth and
   predictable resource use as the first benefit; do not assume it is faster
   until measured.
10. Build a streaming/on-demand path that extracts selected fields without
    constructing a DOM. For exchange-style messages, avoiding unnecessary
    representation may dominate micro-optimizing the DOM parser.

Change one variable at a time and keep losing benchmark results. Benchmark
serialization separately so `operator<<` does not hide parser improvements.

---

## Improvement pointers

Pointers, not answers. Each section lists things to investigate and questions
to ask yourself, roughly ordered by expected impact for an HFT-style workload.

### 1. Memory layout & allocation (biggest lever)

- Every `JsonObject` / `JsonArray` is a separate `std::make_unique` heap
  allocation, and every `JsonValue` is a node in a graph reached through
  pointer indirections. Walk a deeply nested JSON file in a debugger and look
  at where each node lives in memory. Is the layout cache-friendly? What does
  a traversal look like in terms of cache misses?
- Investigate **arena allocators**: `std::pmr::monotonic_buffer_resource`,
  `std::pmr::polymorphic_allocator`, and what they buy you. What would a
  single bump-allocated buffer for all nodes do to your allocation count and
  your destruction cost?
- `std::unordered_map<std::string, JsonValue>` is node-based. Each insert
  allocates a node, and traversal jumps around the heap. What are the
  alternatives? Look into sorted vectors + binary search, flat hash maps
  (absl, folly), and PMR-backed maps. When is linear search over a
  `std::vector<std::pair<...>>` actually faster than a hash map? (Hint: it
  depends on N and on whether the keys are in cache.)
- `std::string` for every key and value: SSO saves you for short strings, but
  anything past SSO length is a heap alloc. What would a `std::string_view`
  that points into a single arena buffer (zero-copy) cost you, and when is it
  safe?
- Look at the **simdjson "tape"** representation: a flat array of tagged
  64-bit slots instead of a tree of pointers. Why is this so much faster to
  traverse? What does it cost you in flexibility?

### 2. Lexer hot path

- `in.get(c)` is a virtual call per byte. What happens if you read the entire
  input into a `std::string` / `std::vector<char>` first and iterate with a
  raw pointer or index? Measure the difference.
- Strings and numbers are accumulated into temporary `std::string` objects.
  Can tokens instead capture a `[start, end)` span into an owned input buffer
  and avoid copying? How will escaped strings differ from unescaped strings?
- The lexer uses `std::from_chars`, but ignores its returned pointer and error
  code. Validate both without adding work to the successful path unnecessarily.
- `in.peek()` + `in.get(c)` is two virtual calls per character. With a
  buffer + index, peek is essentially free. What does the disassembly look
  like before and after?
- For keywords (`true` / `false` / `null`), instead of a per-character state
  machine, consider matching 4 / 5 / 4 bytes at once with `std::memcmp` once
  you've buffered the input. One branch instead of N. Where does this break
  down?
- Keyword recognition does not verify that the following character is a valid
  delimiter, and the top-level parser does not reject leftover tokens. What do
  `truex`, `truefalse`, and `null0` currently do end to end?
- The state machine is a nested `switch`. Look up **jump tables** and
  computed gotos (GCC's `&&label` extension). When do they beat a switch,
  and when does the compiler already do this for you?
- SIMD: whitespace skipping and string-content scanning (finding `"` or `\`)
  are embarrassingly parallel. Look at `_mm256_cmpeq_epi8` and how simdjson
  processes 64 bytes at a time. This is a great weekend project.

### 3. Parser design

- `parser()` is recursive. For deeply nested input this risks stack
  overflow, and recursion is harder for the branch predictor than a tight
  loop. What would an **iterative parser with an explicit stack** look like?
  What does it buy you in cache behavior?
- Every call to `parser()` allocates both a `JsonObject` and a `JsonArray`
  up front, even though most calls will only use one (or neither, for
  scalars). What's the cost of this? When could you defer the allocation
  until you know which one you need?
- `map->values[temp_key] = parser(tokens)` default-constructs a `JsonValue`
  then move-assigns. Compare with `emplace` / `try_emplace`. Does it matter?
- The `ExpectingCommaOrArrayClose` switch has no `default` (the compiler
  already warned you). What happens on a malformed array? Add cases or a
  throwing default.
- Returning `JsonValue` by value from `parser()` relies on NRVO/move. Check
  the assembly to confirm no copies are happening. If you switched to an
  arena, what would the return story look like?
- For HFT "pluck one field out of a large message" workloads, building a DOM
  at all is wasteful. Investigate **SAX / streaming / on-demand** parsing
  (event callbacks: `on_key`, `on_value`, `on_array_start`, ...). What's the
  API ergonomics vs performance tradeoff? Look at `simdjson::ondemand` and
  `RapidJSON`'s SAX interface.

### 4. Number representation

- `float` is 32-bit and loses precision for things like `1.0000001`. Most
  JSON libs default to `double`. For price/qty data in HFT, fixed-point
  integers (`int64_t` with an implied scale) are often what you actually
  want. What does your input data look like?
- JSON doesn't distinguish int from float, but many HFT feeds only ever send
  integer quantities. Would a separate `int64_t` alternative in `JsonValue`
  buy you anything?
- Negative numbers have a partial path, but exponents (`1e10`), mandatory
  digits around decimal syntax, conversion errors, and leading-zero rules are
  not handled correctly. Read RFC 8259 section 6 and make a checklist.

### 5. API / type design

- `Json` holds a `JsonValue&`. References can't be reseated, can't be put in
  containers, and dangle easily (you already hit this once on line 90).
  What are the alternatives? A pointer? A value-semantic wrapper? An index
  into the arena?
- `operator[](std::string& key)` takes a non-const reference — you can't
  pass a string literal. Should this be `const std::string&` or
  `std::string_view`? Why does `string_view` matter here?
- `operator[](int index)` — `std::size_t` is more correct for indexing, and
  `at()` does bounds checking + throws. Is that what you want on the hot
  path?
- `Json::operator bool()` is **implicit**. That means `if (some_json)`
  compiles for any JSON value, including arrays. Should it be `explicit`?
  What's the cost of accidental implicit conversions in a large codebase?
- `JsonValue` is a public alias. Is that intentional? If the representation
  changes (e.g., to a tape), every external user breaks.

### 6. Error handling in an HFT context

- Exceptions are cheap on the happy path but very expensive on the throw
  path (unwinding, cache disruption). In HFT you often want failures to be
  predictable, not 10x slower than success. Look into `std::expected`
  (C++23) or an `outcome`-style result type as an alternative to throwing
  from `operator[]`.
- `std::format` in exception construction is heavy. Make sure exception
  paths are truly cold and that the `std::format` machinery isn't pulled
  into hot code by the linker.
- Annotations to investigate: `[[unlikely]]` on error branches,
  `__builtin_expect`, `noexcept` on hot read-only paths. Don't sprinkle
  them randomly — measure first.

### 7. Build, tooling, verification

- Add **AddressSanitizer** to your debug build alongside UBSan. You have
  UBSan; ASan will catch the dangling-reference class of bug you hit
  earlier and any OOB reads in the lexer.
- Consider `-Werror` in debug so warnings like the unhandled switch cases
  never sit unresolved.
- Build with **LTO** (`-flto`) and **PGO**
  (`-fprofile-generate` / `-fprofile-use`) for the release target. PGO can
  materially change branch layout in a state-machine-heavy parser.
- Add a benchmark suite (Google Benchmark, hayai, or a hand-rolled
  `std::chrono` harness with a warmup). Measure **GB/s** throughput on a
  range of inputs (small, deep, wide, lots of strings, lots of numbers).
  Without numbers you're optimizing in the dark.
- Pull in **JSONTestSuite** for conformance. It's the standard fuzzing
  corpus for JSON parsers and will find edge cases you didn't think of.
- Add **libFuzzer** targets for the lexer and parser. Fuzzing will find the
  `truex`-style bugs in minutes.
- Benchmark against `simdjson`, `RapidJSON`, and `nlohmann/json` as
  baselines. You won't beat simdjson, but the gap is instructive.

### 8. Output path

- `operator<<` for `JsonValue` uses `std::visit` + `std::ostream`. iostream
  is slow (locale, sync, virtual calls). For serialization in HFT you'd
  typically write into a char buffer with `std::to_chars`. What would a
  `to_string(const JsonValue&, std::span<char>)` API look like?
- `std::endl` flushes the stream. Use `'\n'`. (You're already doing this in
  `main.cpp` in some places — be consistent.)

### 9. Things to read / study

- **simdjson**: the papers and the blog posts. The "tape" idea alone is
  worth a week of study. Look at how they do stage 1 (structural
  detection) and stage 2 (tape building) as separate passes.
- **RapidJSON**: the SAX vs DOM distinction, and their allocator model.
- **`std::pmr`**: polymorphic memory resources. Practice porting a small
  data structure to PMR and benchmarking.
- **`std::from_chars` / `std::to_chars`**: the modern, fast, locale-free
  numeric conversions. Use them everywhere instead of `stof` /
  `to_string`.
- **Cache-oblivious data structures** and **data-oriented design** (Mike
  Acton's CppCon talk is the canonical starting point). The mindset
  transfer from OOP to DOD is the single biggest mental shift for HFT C++.
- **Agner Fog's optimization manuals** and **llvm-mca** for reading what
  your code actually compiles to.

### 10. A suggested order of attack

1. Buffer the input and replace `std::istream` with pointer/index iteration.
   Measure. This alone should be a big lexer win.
2. Replace `std::stringstream` with `std::string` + `reserve`, then with
   `string_view` spans into the buffer for string tokens. Measure.
3. Replace `stof` with `std::from_chars`. Measure.
4. Add a benchmark harness so every subsequent change has a number.
5. Move `JsonObject` / `JsonArray` storage to a PMR arena. Measure traversal
   throughput on a deep file.
6. Consider replacing the `unique_ptr`-tree DOM with a tape-style flat
   representation.
7. Vectorize the lexer's whitespace / structural-character detection.
8. Add JSONTestSuite + libFuzzer and close conformance gaps.
9. Switch to a streaming/on-demand API for the "pluck one field" workload
   and compare against the DOM path.

The earlier steps are mechanical and high-ROI. The later steps are where you
actually start learning the HFT-relevant lessons: cache behavior, branch
prediction, allocation cost, and the tradeoff between flexibility and
predictable latency.
