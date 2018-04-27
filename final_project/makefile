all: main clean

main: main.o shader.o camera.o plane.o volume.o
	g++ main.o shader.o camera.o plane.o volume.o -L/usr/lib -lGL -lGLEW -lglfw -o ./bin/main

main.o: main.cpp
	g++ --std=c++11 -c main.cpp

shader.o: shader.cpp
	g++ --std=c++11 -c shader.cpp

camera.o: camera.cpp
	g++ --std=c++11 -c camera.cpp

plane.o: plane.cpp
	g++ --std=c++11 -c plane.cpp

volume.o: volume.cpp
	g++ --std=c++11 -c volume.cpp

clean:
	rm *.o