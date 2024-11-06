CC := g++
CFLAGS := -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image
TARGET := Minesweeper

SRC     := $(wildcard *.cpp)

all: &(TARGET)

&(TARGET): $(SRC) 
	$(CC) $(INCLUDE1) $(LINK) $^ $(CFLAGS) -o Minesweeper

%o: %.cpp
		$(CC) $(CFLAGS) -c $^

#g++   funcs.cpp main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -o Minesweeper