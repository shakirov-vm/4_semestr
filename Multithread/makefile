
CC ?= gcc
OUTPUT ?= a.out
CFLAGS := ${CFLAGS}

clean:
	rm -f a.out
compile:
	${CC} integral.c -lm -lpthread
run:
	time ./${OUTPUT}

%.o: %.c %.h
	$(CC) -c $< -o $@