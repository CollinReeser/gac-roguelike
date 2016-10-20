all: gac

CPPC = clang++ -Wall -std=c++11
ALLEGRO_LIBS = -lallegro -lallegro_image

gac: gac.cpp animation.o config.o creature.o display.o dungeon.o entity.o \
	game_context.o item.o
	$(CPPC) \
	    gac.cpp \
	    animation.o config.o creature.o display.o dungeon.o entity.o \
	    game_context.o item.o \
	    $(ALLEGRO_LIBS) \
	    -o gac

animation.o: animation.cpp animation.h
	$(CPPC) -c animation.cpp -o animation.o

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

item.o: item.cpp item.h
	$(CPPC) -c item.cpp -o item.o

clean:
	rm -f *.o

realclean: clean
	rm -f gac
