
all: main move gac

main: main.c
	clang -Wall main.c -o main -lallegro -lallegro_image

move: move.c
	clang -Wall move.c -o move -lallegro -lallegro_image

gac: gac.cpp creature.cpp creature.h display.cpp display.h dungeon.cpp dungeon.h entity.cpp entity.h game_context.cpp game_context.h config.o
	clang++ -Wall -std=c++11 gac.cpp config.o creature.cpp display.cpp dungeon.cpp entity.cpp game_context.cpp -o gac -lallegro -lallegro_image

config.o: config.cpp config.h json/src/json.hpp
	clang++ -Wall -std=c++11 -c config.cpp -o config.o

realclean:
	rm -f *.o
	rm -f main
	rm -f move
	rm -f gac
