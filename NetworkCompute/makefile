
CC ?= gcc
OUTPUT ?= a.out
CFLAGS := ${CFLAGS} 

.PHONY: all clean

all: clean compile_server compile_client

compile_server: structures.h server.c
	${CC} server.c -o server -std=gnu99
compile_client: structures.h client.c client_compute.c
	${CC} client.c client_compute.c -o client -lm -lpthread -std=gnu99

%.o: %.c %.h
	$(CC) -c $< -o $@

clean:
	rm -f a.out
	rm -f ${OUTPUT}
	rm -f server
	rm -f client