
CC ?= gcc
OUTPUT ?= a.out
CFLAGS := ${CFLAGS}
THREADS ?= 1

clean:
	rm -f a.out
compile:
	${CC} integral.c -lm -lpthread -DTHREADS=${THREADS}
run:
	time ./${OUTPUT}

%.o: %.c %.h
	$(CC) -c $< -o $@