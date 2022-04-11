
CC ?= gcc
OUTPUT ?= a.out
CFLAGS := ${CFLAGS}

all: clear compile run report

.PHONY: run report

compile_no_report: test.o hashtable.o
	${CC} test.o hashtable.o -o ${OUTPUT} ${CFLAGS}
test.o_no_report: test.c ./HTLib/hashtable.h 
	${CC} -o test.o test.c -c ${CFLAGS}
hashtable.o_no_report: ./HTLib/hashtable.h 
	${CC} -o hashtable.o ./HTLib/hashtable.c -c ${CFLAGS}

compile: test.o hashtable.o
	${CC} test.o hashtable.o --coverage ${CFLAGS}
test.o: test.c ./HTLib/hashtable.h 
	${CC} -o test.o test.c -c --coverage ${CFLAGS}
hashtable.o: ./HTLib/hashtable.h 
	${CC} -o hashtable.o ./HTLib/hashtable.c -c --coverage ${CFLAGS}

run:
	./${OUTPUT}
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