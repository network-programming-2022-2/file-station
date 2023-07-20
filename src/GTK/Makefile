PROG = main
CC = gcc
CFLAGS = `pkg-config --cflags --libs gtk+-3.0`

all: ${PROG}.c 
	@rm -rf ${PROG}
	${CC} ${CFLAGS} -o ${PROG} ${PROG}.c 

clean:
	@rm -rf ${PROG} 

