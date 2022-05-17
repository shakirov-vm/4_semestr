
CC ?= gcc
OUTPUT ?= multithread
CFLAGS := ${CFLAGS}
THREADS ?= 1

.PHONY: clean all

all: clean compile

compile:
	${CC} integral.c -lm -lpthread -o ${OUTPUT}
clean:
	rm -f a.out
	rm -f ${OUTPUT}
run:
	time ./${OUTPUT} ${THREADS}

%.o: %.c %.h
	$(CC) -c $< -o $@