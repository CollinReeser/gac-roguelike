all: main gac

CC = clang -Wall
CPPC = clang++ -Wall -std=c++11
ALLEGRO_LIBS = -lallegro -lallegro_image

main: main.c
	$(CC) main.c -o main $(ALLEGRO_LIBS)

gac: gac.cpp config.o creature.o display.o dungeon.o entity.o game_context.o
	$(CPPC) \
	    gac.cpp \
	    config.o creature.o display.o dungeon.o entity.o \
	    game_context.o \
	    $(ALLEGRO_LIBS) \
	    -o gac

config.o: config.cpp config.h json/src/json.hpp
	$(CPPC) -c config.cpp -o config.o

creature.o: creature.cpp creature.h
	$(CPPC) -c creature.cpp -o creature.o

display.o: display.cpp display.h
	$(CPPC) -c display.cpp -o display.o

dungeon.o: dungeon.cpp dungeon.h
	$(CPPC) -c dungeon.cpp -o dungeon.o

entity.o: entity.cpp entity.h
	$(CPPC) -c entity.cpp -o entity.o

game_context.o: game_context.cpp game_context.h
	$(CPPC) -c game_context.cpp -o game_context.o

realclean:
	rm -f *.o
	rm -f main
	rm -f move
	rm -f gac
