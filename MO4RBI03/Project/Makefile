CXXFLAGS = -std=c++11 -Wall -Wextra -Werror -pedantic -pedantic-errors -O3

all: main

main: main.cpp Navigator.cpp Grade.o
	g++ $(CXXFLAGS) -o main main.cpp Navigator.cpp Grade.o

clean:
	rm main.o Navigator.o