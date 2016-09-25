
all: main move

main: main.c
	gcc main.c -o main -lallegro -lallegro_image

move: move.c
	gcc move.c -o move -lallegro -lallegro_image
