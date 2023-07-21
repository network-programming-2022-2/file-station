PROG = main
CC = gcc
CFLAGS = `pkg-config --cflags --libs gtk+-3.0`

all: ${PROG}.c 
	@rm -rf ${PROG}
	${CC} ${CFLAGS} ${PROG}.c -o ${PROG} ${CFLAGS}

clean:
	@rm -rf ${PROG} 

