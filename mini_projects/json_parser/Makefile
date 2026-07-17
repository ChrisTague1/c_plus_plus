CXX = clang++
CXXFLAGS_BASE = -std=c++20 -Wall -Wextra -pedantic
CXXFLAGS_DEBUG = $(CXXFLAGS_BASE) -g -O0 -fsanitize=undefined
CXXFLAGS_RELEASE = $(CXXFLAGS_BASE) -O3 -march=native -DNDEBUG

TARGET = main
SRC = main.cpp token.cpp lexer.cpp json.cpp parser.cpp

all: debug

debug: $(SRC)
	@$(CXX) $(CXXFLAGS_DEBUG) -o $(TARGET) $(SRC)

run-debug: debug
	@./$(TARGET)

release: $(SRC)
	@$(CXX) $(CXXFLAGS_RELEASE) -o $(TARGET) $(SRC)

run-release: release
	@./$(TARGET)

clean:
	@rm -f $(TARGET)

.PHONY: all debug release run-release clean