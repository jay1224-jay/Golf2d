# Windows path
# SFML_PATH = /home/jay/code/my_libs/SFML

# My linux path
SFML_PATH = /home/jay/code/my_libs/SFML

main: map.o main.o wall.o
	g++ main.o map.o wall.o -o main -L$(SFML_PATH)/lib -lsfml-graphics -lsfml-window -lsfml-system

main.o: main.cpp
	g++ -c main.cpp -I$(SFML_PATH)/include

wall.o: wall.cpp
	g++ -c wall.cpp -I$(SFML_PATH)/include

map.o: map.cpp
	g++ -c map.cpp -I$(SFML_PATH)/include

runl:
	export LD_LIBRARY_PATH=$(SFML_PATH)/lib && ./main

clean:
	rm *.o
	rm main

test: readmap.o
	g++ readmap.o -o main -L$(SFML_PATH)/lib -lsfml-graphics -lsfml-window -lsfml-system

readmap.o: readmap.cpp
	g++ -c readmap.cpp -I$(SFML_PATH)/include
