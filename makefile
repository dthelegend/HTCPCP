HtcpcpStandard_build :
	gcc -Wall -ansi -pedantic-errors -c "HtcpcpStandard.c"

test1_build : HtcpcpStandard_build
	gcc -Wall -ansi -pedantic-errors -o test1.out test1.c HtcpcpStandard.o

test1_run : test1_build
	./test1.out

test1_memcheck : test1_build
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes ./test1.out