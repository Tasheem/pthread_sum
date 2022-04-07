CC = gcc
FILE = pthread_sum.c
OUT = output
VER = -std=c11

all:
	${CC} -Wall ${VER} -o ${OUT} ${FILE}
exec:
	./${OUT} 4 5
clean:
	rm -rf ${OUT}