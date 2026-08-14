CXX = clang++
CXXFLAGS_BASE = -std=c++20 -Wall -Wextra -pedantic
CXXFLAGS_DEBUG = $(CXXFLAGS_BASE) -g -O0 -fsanitize=undefined
CXXFLAGS_RELEASE = $(CXXFLAGS_BASE) -O3 -march=native -DNDEBUG

TARGET = main
SRC = main.cpp token.cpp lexer.cpp json.cpp parser.cpp
LIB_SRC = token.cpp lexer.cpp json.cpp parser.cpp
TEST_TARGET = parser_tests
TEST_SRC = tests/parser_tests.cpp $(LIB_SRC)

all: debug

debug: $(SRC)
	@$(CXX) $(CXXFLAGS_DEBUG) -o $(TARGET) $(SRC)

run-debug: debug
	@./$(TARGET)

release: $(SRC)
	@$(CXX) $(CXXFLAGS_RELEASE) -o $(TARGET) $(SRC)

run-release: release
	@./$(TARGET)

test: $(TEST_SRC)
	@$(CXX) $(CXXFLAGS_DEBUG) -I. -o $(TEST_TARGET) $(TEST_SRC)
	@./$(TEST_TARGET)

clean:
	@rm -f $(TARGET) $(TEST_TARGET)

.PHONY: all debug release run-debug run-release test clean
