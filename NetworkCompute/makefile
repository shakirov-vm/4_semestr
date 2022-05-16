
CC ?= gcc
OUTPUT ?= a.out
CFLAGS := ${CFLAGS}

clean:
	rm -f a.out
	rm -f ${OUTPUT}
	rm -f server
	rm -f client
compile_server: structures.h server.c
	${CC} server.c -o server
compile_client: structures.h client.c client_compute.c
	${CC} client.c client_compute.c -o client -lm -lpthread

%.o: %.c %.h
	$(CC) -c $< -o $@

all: compile_server compile_client