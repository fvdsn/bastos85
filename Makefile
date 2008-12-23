CC = gcc
FLAGS = -Wall -g
LIBS = -lglut

all : engine

engine: world.o
	${CC} ${FLAGS} ${LIBS} -o engine world.o particle.o draw.o vector.o virtual_time.o keyboard.o

world.o: src/world.c src/world.h particle.o draw.o keyboard.o
	${CC} ${FLAGS} -c src/world.c 

draw.o: src/draw.c src/draw.h particle.o
	${CC} ${FLAGS} -c src/draw.c 

particle.o: src/particle.c src/particle.h virtual_time.o vector.o keyboard.o
	${CC} ${FLAGS} -c src/particle.c

keyboard.o: src/keyboard.c src/keyboard.h
	${CC} ${FLAGS} -c src/keyboard.c

vector.o: src/vector.c src/vector.h
	${CC} ${FLAGS} -c src/vector.c

virtual_time.o: src/virtual_time.c src/virtual_time.h
	${CC} ${FLAGS} -c src/virtual_time.c

clean:
	-rm *.o engine 
doc:	Doxygen.config src/*.h src/*.c
	doxygen Doxygen.config

