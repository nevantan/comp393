all: main clean

main: main.o shader.o texture.o
	g++ main.o shader.o texture.o -L/usr/lib -lGL -lGLEW -lglfw -o ./bin/tme1

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

shader.o: shader.cpp
	g++ -std=c++11 -c shader.cpp

texture.o: texture.cpp
	g++ -std=c++11 -c texture.cpp

clean:
	rm -rf *.o