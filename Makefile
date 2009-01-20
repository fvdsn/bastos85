CC = gcc
FLAGS = -g -O3 -Wall -Werror -Wextra -pedantic -Wno-unused-parameter -g -Wdeclaration-after-statement -Wmissing-declarations -Wpointer-arith -Wstrict-prototypes -Wredundant-decls -Wcast-align -Wwrite-strings -Winline -Wformat-security -Wformat-nonliteral -Wswitch-enum -Wswitch-default -Winit-self
LIBS = -lglut

all : Makefile game

game: game.o
	${CC} ${FLAGS} ${LIBS} -o game world.o particle.o draw.o vector.o virtual_time.o keyboard.o factory.o game.o property.o model.o

game.o:	src/game.c world.o particle.o vector.o draw.o factory.o	property.o model.o
	${CC} ${FLAGS} -c src/game.c

factory.o: src/factory.c src/factory.h particle.o vector.o world.o
	${CC} ${FLAGS} -c src/factory.c
	
world.o: src/world.c src/world.h particle.o draw.o keyboard.o property.o
	${CC} ${FLAGS} -c src/world.c 

draw.o: src/draw.c src/draw.h particle.o
	${CC} ${FLAGS} -c src/draw.c 

particle.o: src/particle.c src/particle.h virtual_time.o vector.o keyboard.o property.o model.o
	${CC} ${FLAGS} -c src/particle.c

property.o: src/property.c src/property.h virtual_time.o
	${CC} ${FLAGS} -c src/property.c

keyboard.o: src/keyboard.c src/keyboard.h
	${CC} ${FLAGS} -c src/keyboard.c

vector.o: src/vector.c src/vector.h
	${CC} ${FLAGS} -c src/vector.c

model.o:src/model.c src/model.h
	${CC} ${FLAGS} -c src/model.c

virtual_time.o: src/virtual_time.c src/virtual_time.h
	${CC} ${FLAGS} -c src/virtual_time.c

clean:
	-rm *.o game 
doc:	Doxygen.config src/*.h src/*.c
	doxygen Doxygen.config

