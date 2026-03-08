CXX = clang++
CXXFLAGS_BASE = -std=c++17 -Wall -Wextra -pedantic
CXXFLAGS_DEBUG = $(CXXFLAGS_BASE) -g -O0 -fsanitize=address,undefined
CXXFLAGS_RELEASE = $(CXXFLAGS_BASE) -O3 -march=native -DNDEBUG

TARGET = main
SRC = main.cpp parsing/extract_num.cpp

# Default build is debug
all: debug

debug: $(SRC)
	@$(CXX) $(CXXFLAGS_DEBUG) -o $(TARGET) $(SRC)

release: $(SRC)
	@$(CXX) $(CXXFLAGS_RELEASE) -o $(TARGET) $(SRC)

run: debug
	@./$(TARGET)

run-release: release
	@./$(TARGET)

clean:
	@rm -f $(TARGET)

.PHONY: all debug release run run-release clean
