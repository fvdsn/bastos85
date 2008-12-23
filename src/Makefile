CC = gcc
FLAGS = -Wall -g
LIBS = -lglut

all : engine

engine: world.o
	${CC} ${FLAGS} ${LIBS} -o engine world.o particle.o draw.o vector.o virtual_time.o

world.o: world.c world.h particle.o draw.o
	${CC} ${FLAGS} -c world.c 

draw.o: draw.c draw.h particle.o
	${CC} ${FLAGS} -c draw.c 

particle.o: particle.c particle.h virtual_time.o vector.o 
	${CC} ${FLAGS} -c particle.c 

vector.o: vector.c vector.h
	${CC} ${FLAGS} -c vector.c

virtual_time.o: virtual_time.c virtual_time.h
	${CC} ${FLAGS} -c virtual_time.c

clean:
	-rm *.o 

