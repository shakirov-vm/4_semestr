compile:
	gcc main.c ./HTLib/hashtable.c
run:
	./a.out
clear_compile:
	rm ./a.out
report:
	lcov -t "ht_t" -o ht_o -c -d .
	genhtml -o report ht_o
clear_report:
	rm -r report
	rm *.gcda
	rm *.gcno