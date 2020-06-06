all: build
	
build: battleship.c
	gcc battleship.c -o battleship -lncurses
run : build
	./battleship player_map
clean : battleship
	rm battleship
