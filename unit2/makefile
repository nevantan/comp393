all: main clean

main: main.o GLShader.o
	g++ main.o GLShader.o -L/usr/lib -lGLEW -lglfw -lGL -lSOIL -o ./bin/shaders

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

GLShader.o: GLShader.cpp
	g++ -std=c++11 -c GLShader.cpp

clean:
	rm *o