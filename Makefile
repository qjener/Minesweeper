CC      := g++
CFLAGS := -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image
TARGET := Minesweeper

SRC     := $(wildcard *.cpp)
INCLUDE1 := -ISDL\SDL\include #-ISDL\TTF\include
LINK := -LSDL\SDL\lib #-LSDL\TTF\lib

all: &(TARGET)

&(TARGET): $(SRC) 
	$(CC) $(INCLUDE1) $(LINK) $^ $(CFLAGS) -o Minesweeper

%o: %.cpp
		$(CC) $(CFLAGS) -c $^

#g++ funcs.cpp main.cpp -Wall -lmingw32 -lSDL2main -lSDL2 -ISDL\SDL\include\SDL2 -ISDL\TTF\include\SDL2 -LSDL\SDL\lib -LSDL\TTF\lib  test.cpp