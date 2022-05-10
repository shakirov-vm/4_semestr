
CC ?= gcc
OUTPUT ?= a.out
CFLAGS := ${CFLAGS}

clean:
	rm -f a.out
	rm -f ${OUTPUT}
compile:
	${CC} server.c
run:
	time ./${OUTPUT}

%.o: %.c %.h
	$(CC) -c $< -o $@