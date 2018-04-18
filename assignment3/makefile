all: main clean

main: main.o shader.o texture.o
	g++ main.o shader.o texture.o -Llib -lGL -lGLEW -lglfw -o ./bin/tme3

main.o: main.cpp
	g++ -std=c++11 -Iinclude -c main.cpp

shader.o: shader.cpp
	g++ -std=c++11 -Iinclude -c shader.cpp

texture.o: texture.cpp
	g++ -std=c++11 -Iinclude -c texture.cpp

clean:
	rm -rf *.o