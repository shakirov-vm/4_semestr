
CC ?= gcc
OUTPUT ?= a.out
CFLAGS := ${CFLAGS}
TARGET = hashtable

all: clear compile run report

.PHONY: run report

compile_no_report: test.o ${TARGET}.o
	${CC} test.o ${TARGET}.o -o ${OUTPUT} ${CFLAGS}
test.o_no_report: test.c ./HTLib/${TARGET}.h 
	${CC} -o test.o test.c -c ${CFLAGS}
hashtable.o_no_report: ./HTLib/${TARGET}.h 
	${CC} -o ${TARGET}.o ./HTLib/${TARGET}.c -c ${CFLAGS}

compile: test.o ${TARGET}.o
	${CC} test.o ${TARGET}.o --coverage ${CFLAGS}
test.o: test.c ./HTLib/${TARGET}.h 
	${CC} -o test.o test.c -c ${CFLAGS}
hashtable.o: ./HTLib/${TARGET}.h 
	${CC} -o ${TARGET}.o ./HTLib/${TARGET}.c -c --coverage ${CFLAGS}

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