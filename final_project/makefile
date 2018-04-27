all: main clean

main: main.o shader.o
	g++ main.o shader.o -L/usr/lib -lGL -lGLEW -lglfw -o ./bin/main

main.o: main.cpp
	g++ --std=c++11 -c main.cpp

shader.o: shader.cpp
	g++ --std=c++11 -c shader.cpp

clean:
	rm *.o