CC=clang
FLAGS=-I /opt/homebrew/include -L /opt/homebrew/lib -lSDL2
DEP=chip8.c graphics.c

main.o: $(DEP) main.c
	$(CC) $(FLAGS) main.c $(DEP) -o main.o