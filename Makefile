CXX = clang++
CXXFLAG_BASE = -std=c++17 -Wall -Wextra -pedantic

TARGET = main
SRC = main.cpp

all: build

build: $(SRC)
	@$(CXX) $(CXXFLAG_BASE) -o $(TARGET) $(SRC)

run: build
	@./$(TARGET)