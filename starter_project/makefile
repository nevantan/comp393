all: main clean

main: main.o
	g++ main.o -L/usr/lib -lGL -lGLEW -lglfw -o ./bin/main

main.o: main.cpp
	g++ --std=c++11 -c main.cpp

clean:
	rm *.o