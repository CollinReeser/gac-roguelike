
all: main move gac

main: main.c
	clang++ main.c -o main -lallegro -lallegro_image

move: move.c
	clang++ move.c -o move -lallegro -lallegro_image

gac: gac.cpp context.cpp context.h
	clang++ gac.cpp context.cpp -o gac -lallegro -lallegro_image

realclean:
	rm -f *.o
	rm -f main
	rm -f move
	rm -f gac
