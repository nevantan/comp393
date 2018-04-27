all: main clean

main: main.o shader.o camera.o
	g++ main.o shader.o camera.o -L/usr/lib -lGL -lGLEW -lglfw -o ./bin/main

main.o: main.cpp
	g++ --std=c++11 -c main.cpp

shader.o: shader.cpp
	g++ --std=c++11 -c shader.cpp

camera.o: camera.cpp
	g++ --std=c++11 -c camera.cpp

clean:
	rm *.o