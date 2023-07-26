PROG = main
CC = gcc
CFLAGS = `pkg-config --cflags --libs gtk+-3.0`

${PROG}: ${PROG}.c 
	${CC} ${CFLAGS} -o ${PROG} ${PROG}.c -lpq -pthread -lssl -lcrypto
