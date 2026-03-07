CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic

TARGET = main
SRC = main.cpp parsing/extract_num.cpp

run: all
	@./$(TARGET)

all: $(TARGET)

$(TARGET): $(SRC)
	@$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	@rm -f $(TARGET)

.PHONY: all clean run
