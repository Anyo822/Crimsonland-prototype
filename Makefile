CC=D:/Projects/Programming/mingw64/bin/g++
sfml_include=D:\Projects\Programming\Praktica\SFML-2.4.2\include
sfml_lib=-L D:\Projects\Programming\Praktica\SFML-2.4.2\lib -lsfml-graphics -lsfml-window -lsfml-system
game: main.o game.o objects.o
	$(CC) main.o game.o objects.o -o game $(sfml_lib)
main.o: main.cpp game.o objects.o
	$(CC) -o main.o -c main.cpp -I $(sfml_include)
game.o: game.cpp objects.o
	$(CC) -o game.o -c game.cpp -I $(sfml_include)
objects.o: objects.cpp 
	$(CC) -o objects.o -c objects.cpp -I $(sfml_include)
clean:
	del *.o *.exe 