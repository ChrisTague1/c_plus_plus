CXX := clang++
CXXFLAGS := -std=c++20 -Wall -Wextra

.PHONY: all clean

all: house player

house: house.o protocol.o
	@$(CXX) $(CXXFLAGS) $^ -o $@

player: player.o protocol.o
	@$(CXX) $(CXXFLAGS) $^ -o $@

house.o: house.cpp protocol.h
	@$(CXX) $(CXXFLAGS) -c house.cpp

player.o: player.cpp protocol.h
	@$(CXX) $(CXXFLAGS) -c player.cpp

protocol.o: protocol.cpp protocol.h
	@$(CXX) $(CXXFLAGS) -c protocol.cpp

clean:
	@rm -f *.o house player