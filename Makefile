
all: main

main: main.c
	gcc main.c -o main -lallegro -lallegro_image
