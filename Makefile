CC = gcc
CFLAGS = -Iinclude -O2

SRC = src/main.c src/game.c src/chips.c src/gui.c
OUT = blackjack21.exe

all :
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) -lraylib -lopengl32 -lgdi32 -lwinmm

run : all
	./$(OUT)

clean :
	rm -f $(OUT)
