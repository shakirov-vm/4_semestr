CC = gcc
OUTPUT = a.out

compile_no_report: main.o hashtable.o
	${CC} main.o hashtable.o -o ${OUTPUT} 
compile: main.o hashtable.o
	${CC} main.o hashtable.o --coverage

main.o: main.c ./HTLib/hashtable.h 
	gcc -o main.o main.c -c --coverage
hashtable.o: ./HTLib/hashtable.h 
	gcc -o hashtable.o ./HTLib/hashtable.c -c --coverage

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
