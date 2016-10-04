
all: main move gac

main: main.c
	clang -Wall main.c -o main -lallegro -lallegro_image

move: move.c
	clang -Wall move.c -o move -lallegro -lallegro_image

gac: gac.cpp creature.cpp creature.h display.cpp display.h dungeon.cpp dungeon.h entity.cpp entity.h game_context.cpp game_context.h
	clang++ -Wall -std=c++11 gac.cpp creature.cpp display.cpp dungeon.cpp entity.cpp game_context.cpp -o gac -lallegro -lallegro_image

realclean:
	rm -f *.o
	rm -f main
	rm -f move
	rm -f gac
