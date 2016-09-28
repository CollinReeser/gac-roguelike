
all: main move gac

main: main.c
	clang++ -std=c++11 main.c -o main -lallegro -lallegro_image

move: move.c
	clang++ -std=c++11 move.c -o move -lallegro -lallegro_image

gac: gac.cpp creature.cpp creature.h display.cpp display.h dungeon.cpp dungeon.h entity.cpp entity.h
	clang++ -std=c++11 gac.cpp creature.cpp display.cpp dungeon.cpp entity.cpp -o gac -lallegro -lallegro_image

realclean:
	rm -f *.o
	rm -f main
	rm -f move
	rm -f gac
