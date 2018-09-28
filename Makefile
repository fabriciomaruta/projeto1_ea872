FLAGS=-lncurses -lpthread -lportaudio -std=c++11

all: model

model: main.cpp model.cpp model.hpp playback.cpp 
	g++ -o main main.cpp model.cpp playback.cpp $(FLAGS)
	./main
clean:
	rm -f main
