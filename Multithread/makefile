
CC ?= gcc
OUTPUT ?= multithread
CFLAGS := ${CFLAGS}
THREADS ?= 1

compile:
	${CC} integral.c -lm -lpthread -o ${OUTPUT}
clean:
	rm -f a.out
	rm -f ${OUTPUT}
run:
	time ./${OUTPUT} ${THREADS}

.PHONY: clean compile

%.o: %.c %.h
	$(CC) -c $< -o $@