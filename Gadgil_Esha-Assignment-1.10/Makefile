# Makefile for compiling the Battleship game

CXX = g++                      
CXXFLAGS = -Wall -g -std=c++11  
LDFLAGS = -lncurses             


all: battleships


battleships: battleship.cpp
	$(CXX) $(CXXFLAGS) battleship.cpp -o battleships $(LDFLAGS)


clean:
	rm -f battleships
