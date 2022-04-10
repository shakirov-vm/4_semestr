
CC ?= gcc
OUTPUT ?= a.out
CFLAGS := ${CFLAGS}

all: clear compile run report

.PHONY: run report

compile_no_report: main.o hashtable.o
	${CC} main.o hashtable.o -o ${OUTPUT} ${CFLAGS}
main.o_no_report: main.c ./HTLib/hashtable.h 
	${CC} -o main.o main.c -c ${CFLAGS}
hashtable.o_no_report: ./HTLib/hashtable.h 
	${CC} -o hashtable.o ./HTLib/hashtable.c -c ${CFLAGS}

compile: main.o hashtable.o
	${CC} main.o hashtable.o --coverage ${CFLAGS}
main.o: main.c ./HTLib/hashtable.h 
	${CC} -o main.o main.c -c --coverage ${CFLAGS}
hashtable.o: ./HTLib/hashtable.h 
	${CC} -o hashtable.o ./HTLib/hashtable.c -c --coverage ${CFLAGS}

run:
	./a.out
report:
	lcov -t "ht_t" -o ht.info -c -d .
	genhtml -o report ht.info

clear: clear_compile clear_report
clear_compile:
	rm -rf ${OUTPUT}
	rm -rf *.o
clear_report:
	rm -rf report
	rm -f *.gcda
	rm -f *.gcno
	rm -f ht.info
	
%.o: %.c %.h
	$(CC) -c $< -o $@